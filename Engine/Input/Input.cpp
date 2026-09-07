#include "Input.h"
/*
 * HID 매크로를 사용하기위해 추가, 반드시 <Windows.h>밑에 정의
 * 윈도우의 입력 디바이스 설정
 */
#include <hidusage.h> 
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/InputErrorMessage.h"

namespace Engine
{
	namespace Input
	{
		Input::Input() : keyboard(nullptr), mouse(nullptr)
		{
		}

		Input::~Input()
		{
			Release();
		}

		bool Input::Initialize(UINT screenWidth, UINT screenHeight)
		{
			// Setup Raw Input
			// WM_INPUT을 호출을 하려면 반드시 세팅해야함
			RAWINPUTDEVICE ridev; // RAWINPUTDEVICE : https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinputdevice
			std::memset(&ridev, 0, sizeof(RAWINPUTDEVICE));
			ridev.usUsagePage = HID_USAGE_PAGE_GENERIC;	// 일반 데스크탑 컨트롤, https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/hid-usages#usage-page
			ridev.usUsage = HID_USAGE_GENERIC_MOUSE; // 마우스,  https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/hid-usages#usage-id
			ridev.dwFlags = 0;
			ridev.hwndTarget = nullptr; // NULL이면 키보드 포커스에 따름

			// RegisterRawInputDevices : https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerrawinputdevices
			if (!RegisterRawInputDevices(&ridev, 1, sizeof(ridev)))
			{
				CommonUtility::Log::ErrorMessage(GetLastError(), Message::Error::RAW_INPUT_DEVICE_REGISTER_FAILURE);
				exit(-1);
			}

			keyboard = new Keyboard::KeyboardInput;
			if (!keyboard) // keyboard == nullptr
				return false;

			mouse = new Mouse::MouseInput(screenWidth, screenHeight);
			if (!mouse) // mouse == nullptr
				return false;

			return true;
		}

		void Input::Release()
		{
			CommonUtility::SafeDelete(mouse);
			CommonUtility::SafeDelete(keyboard);
		}

		void Input::Reset()
		{
			keyboard->Reset();
			mouse->Reset();
		}

		void Input::UpdateState()
		{
			keyboard->UpdateState();
			mouse->UpdateState();
		}

		void Input::InputMessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
		{
			keyboard->KeyboardMessageHandler(hWnd, uMessage, wParam, lParam);
			mouse->MouseMessageHandler(hWnd, uMessage, wParam, lParam);
		}

		Keyboard::KeyboardInput* Input::GetKeyboard() const
		{
			return keyboard;
		}

		Mouse::MouseInput* Input::GetMouse() const
		{
			return mouse;
		}
	} // Input
} // Engine