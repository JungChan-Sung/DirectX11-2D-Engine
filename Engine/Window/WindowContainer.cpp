#include "WindowContainer.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "Engine/Config/EngineConfig.h"

namespace Engine
{
	namespace Window
	{
		WindowContainer::WindowContainer() : screenOption(nullptr),
			window(nullptr), input(nullptr), graphics(nullptr)
		{
		}

		WindowContainer::~WindowContainer()
		{
			Release();
		}

		bool WindowContainer::Initialize(HINSTANCE hInstance)
		{
			// ==== 화면 설정 생성 및 로드 ====
			screenOption = new Option::ScreenOption;
			if (!screenOption) // screenOption == nullptr
				return false;

			if (!screenOption->LoadScreenOption()) // screenOption->LoadScreenOption == false
				return false;
			// ========================

			UINT resolutionWidth = screenOption->GetResolutionWidth();
			UINT resolutionHeight = screenOption->GetResolutionHeight();
			bool isFullScreen = screenOption->IsFullScreen();

			// ==== 윈도우 창 생성 및 초기화 ====
			window = new Window;
			if (!window) // window == nullptr
				return false;

			if (!window->Initialize(hInstance, EngineConfig::Instance()->WindowTitle().c_str(),
				resolutionWidth, resolutionHeight, isFullScreen, this)) // window->Initialize == false
				return false;
			// ==================================

			// ==== 입력 시스템 생성 및 초기화 ====
			input = new Input::Input;
			if (!input) // input == nullptr
				return false;

			if (!input->Initialize(resolutionWidth, resolutionHeight)) // input->Initialize == false
				return false;
			// ====================================

			HWND hWnd = window->GetHWND();

			// ==== 그래픽스 생성 및 초기화 ====
			graphics = new Graphics::Graphics;
			if (!graphics) // graphics == nullptr
				return false;

			if (!graphics->Initialize(hWnd, resolutionWidth, resolutionHeight, isFullScreen, screenOption->IsVsync())) // graphics->Initialize == false
				return false;
			// ===============================

			// 윈도우 프로시저 재설정
			// GWLP_WNDPROC : 윈도우 프로시저를 새로 설정
			SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowContainer::HandleMessageRedirect));

			return true;
		}

		void WindowContainer::Release()
		{
			// 전체 화면인 경우 세팅을 원래대로 되돌림
			if (screenOption->IsFullScreen()) // screenOption->IsFullScreen == true
				window->RestoreDefaultDisplaySettings();

			// 구성 요소
			CommonUtility::SafeDelete(graphics);
			CommonUtility::SafeDelete(input);
			CommonUtility::SafeDelete(window);

			// 설정
			CommonUtility::SafeDelete(screenOption);
		}

		LRESULT WindowContainer::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
		{
			switch (uMessage)
			{
			case WM_CLOSE: // 닫기 버튼을 눌렀을 경우 호출
				PostQuitMessage(0);
				return 0;
			
			case WM_SIZE: // 화면 크기 조정이 끝난후 호출
				
				return 0;

			case WM_MOVE: // 화면 이동 종료후 호출

				return 0;

			default:
				input->InputMessageHandler(hWnd, uMessage, wParam, lParam);
				break;
			}

			return DefWindowProcW(hWnd, uMessage, wParam, lParam);
		}

		Option::ScreenOption* WindowContainer::GetScreenOption() const
		{
			return screenOption;
		}

		Window* WindowContainer::GetWindow() const
		{
			return window;
		}

		Input::Input* WindowContainer::GetInput() const
		{
			return input;
		}

		Graphics::Graphics* WindowContainer::GetGraphics() const
		{
			return graphics;
		}

		LRESULT WindowContainer::HandleMessageRedirect(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
		{
			// GWLP_USERDATA를 WindowContainer*로 Set 했기 때문에 해당 클래스로 Get 할 수 있다
			WindowContainer* windowContainer = reinterpret_cast<WindowContainer*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)));
			return windowContainer->MessageHandler(hWnd, uMessage, wParam, lParam);
		}
	} // Window
} // Engine