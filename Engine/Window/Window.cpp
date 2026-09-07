#include "Window.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/WindowErrorMessage.h"

namespace Engine
{
	namespace Window
	{
		Window::Window() : hInstance(nullptr), hWnd(nullptr)
		{
			std::memset(&windowDescription, 0, sizeof(WindowDescription));
		}

		Window::~Window()
		{
			Release();
		}

		bool Window::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height, bool isFullSceen, WindowContainer* windowContainer)
		{
			this->hInstance = hInstance;

			DescriptionWindow(title, width, height, isFullSceen);
			RegisterWindowClass();
			return CreateAndShowWindow(windowContainer);
		}

		void Window::Release()
		{
			// 윈도우 창 파괴 및 할당 해제
			if (hWnd) // hWnd != nullptr
			{
				// 윈도우 창 파괴전 함수 변경
				SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::WindowProcedure));
				// 윈도우 창 파괴
				DestroyWindow(hWnd);
				hWnd = nullptr;
			}

			// 인스턴스 핸들 할당 해제
			if (hInstance) // hInstance != nullptr
			{
				UnregisterClassW(windowDescription.title, hInstance);
				hInstance = nullptr;
			}
		}

		void Window::RestoreDefaultDisplaySettings()
		{
			ChangeDisplaySettingsW(nullptr, 0ul);
		}

		HWND Window::GetHWND() const
		{
			return hWnd;
		}

		UINT Window::GetWindowWidth() const
		{
			return windowDescription.width;
		}

		UINT Window::GetWindowHeight() const
		{
			return windowDescription.height;
		}

		LRESULT Window::WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
		{
			switch (uMessage)
			{
			case WM_NCCREATE: // WM_CREATE 이전에 발생하는 메시지
			{
				const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
				WindowContainer* windowContainer = reinterpret_cast<WindowContainer*>(create->lpCreateParams);

				if (!windowContainer) // windowContainer == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::WINDOW_CONTAINER_NULL_POINTER);
					exit(-1); // 프로그램 종료
				}

				// SetWindowLongPtr : 지정된 창의 속성을 변경
				// GWLP_USERDATA : 사용자 데이터를 설정, 창을 만든 응용 프로그램에서 사용하기 위한 것
				SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowContainer));
			}
			}

			return DefWindowProcW(hWnd, uMessage, wParam, lParam);
		}

		void Window::DescriptionWindow(LPCWSTR title, UINT width, UINT height, bool isFullScreen)
		{
			windowDescription.title = title;
			// WS_CLIPSIBLINGS : 특정 자식 창이 WM_PAINT 메시지를 받으면 업데이트할 자식 창의 영역에서 겹치는 다른 모든 자식 창을 클립합니다.
			// WS_CLIPCHILDREN : 부모 창 내에서 그리기를 수행할 때 자식 창이 차지하는 영역을 제외. 이 스타일은 부모 창을 만들 때 사용됩니다.
			// WS_POPUP : 팝업 창 스타일, WS_CHILD 스타일과 함께 사용할 수 없다. 반드시 WNDCLASSEXW.hbrBackground를 설정해야 한다.
			windowDescription.dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

			if (isFullScreen) // isFullScreen == true
			{
				windowDescription.width = width;
				windowDescription.height = height;

				// 전체화면 모드를 위한 세팅
				DEVMODEW dmScreenSettings;
				std::memset(&dmScreenSettings, 0, sizeof(DEVMODEW));
				// 전체화면일 경우 디스플레이 장치의 색상 해상도 최대 크기를 픽셀당 32비트 설정
				// ex) 1: 흑백, 4: 16색, 8: 2^15색, 24,31: 2^242색
				// DM_BITSPERPEL : dmBitsPerPel
				// DM_PELSWIDTH : dmPelsWidth
				// DM_PELSHEIGHT : dmPelsHeight
				dmScreenSettings.dmSize = sizeof(DEVMODEW);
				dmScreenSettings.dmPelsWidth = static_cast<DWORD>(windowDescription.width);
				dmScreenSettings.dmPelsHeight = static_cast<DWORD>(windowDescription.height);
				dmScreenSettings.dmBitsPerPel = 32;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				// Full Screen에 맞는 디스플레이 설정으로 변경
				// CDS : Change Display Settings
				ChangeDisplaySettingsW(&dmScreenSettings, CDS_FULLSCREEN);
			}
			else
			{
				// 창 모드일 경우 Caption과 System Menu를 추가
				windowDescription.dwStyle |= WS_CAPTION | WS_SYSMENU;

				RECT rectClient;
				SetRect(&rectClient, 0, 0, width, height);
				// client 영역을 설정한 크기로 만들기 위해 windows 크기를 Resize
				AdjustWindowRectEx(&rectClient, windowDescription.dwStyle, false, WS_EX_APPWINDOW);
				windowDescription.width = rectClient.right - rectClient.left;
				windowDescription.height = rectClient.bottom - rectClient.top;
				// windows를 모니터 중앙에 배치
				windowDescription.x = (GetSystemMetrics(SM_CXSCREEN) - windowDescription.width) / 2;
				windowDescription.y = (GetSystemMetrics(SM_CYSCREEN) - windowDescription.height) / 2;
			}
		}

		void Window::RegisterWindowClass()
		{
			WNDCLASSEXW windowClassExtended;
			std::memset(&windowClassExtended, 0, sizeof(WNDCLASSEXW));
			windowClassExtended.cbSize = sizeof(WNDCLASSEXW);										// 구조체 크기
			windowClassExtended.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;							// CS_(H or V)REDRAW : 크기가 변할시 새로 그림, CS_OWNDC : 윈도우 클래스의 각 창에 대해 고유한 디바이스 컨텍스트를 할당
			windowClassExtended.lpfnWndProc = Window::WindowProcedure;								// 윈도우 프로시저 함수 포인터
			windowClassExtended.cbClsExtra = 0;														// 클래스 여분 메모리
			windowClassExtended.cbWndExtra = 0;														// 윈도우 여분 메모리
			windowClassExtended.hInstance = hInstance;												// 인스턴트 핸들 값
			windowClassExtended.hIcon = LoadIconW(nullptr, IDI_WINLOGO);							// 기본 윈도우 아이콘
			windowClassExtended.hCursor = LoadCursorW(nullptr, IDC_ARROW);							// 기본 커서
			windowClassExtended.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));	// 배경 색
			windowClassExtended.lpszMenuName = nullptr;												// 메뉴 리소스 이름
			windowClassExtended.lpszClassName = windowDescription.title;							// 윈도우 클래스 이름(타이틀 이름)
			windowClassExtended.hIconSm = LoadIconW(nullptr, IDI_WINLOGO);							// 작은 아이콘

			// 윈도우 클래스 등록
			RegisterClassExW(&windowClassExtended);
		}

		bool Window::CreateAndShowWindow(WindowContainer* windowContainer)
		{
			hWnd = CreateWindowExW(WS_EX_APPWINDOW, windowDescription.title, windowDescription.title, windowDescription.dwStyle,
				windowDescription.x, windowDescription.y, windowDescription.width, windowDescription.height,
				nullptr, nullptr, hInstance, windowContainer);

			if (!hWnd) // hWnd == nullptr
			{
				CommonUtility::Log::ErrorMessage(GetLastError(), Message::Error::CREATE_WINDOW_EX_W_FAILURE + std::wstring(windowDescription.title));
				return false;
			}

			ShowWindow(hWnd, SW_SHOW);	// 창(Window) 띄우기
			SetForegroundWindow(hWnd);	// 이 창(Window)을 가장 앞으로 위치
			SetFocus(hWnd);				// 이 창(Window)에 포커스

			return true;
		}
	} // Window
} // Engine