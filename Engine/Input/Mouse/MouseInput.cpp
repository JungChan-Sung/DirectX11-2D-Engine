#include "MouseInput.h"
#include <memory>

namespace Engine
{
	namespace Input
	{
		namespace Mouse
		{
			MouseInput::MouseInput(UINT screenWidth, UINT screenHeigh)
			{
				Initialize(screenWidth, screenHeigh);
			}

			MouseInput::~MouseInput()
			{
				Release();
			}

			void MouseInput::Initialize(UINT screenWidth, UINT screenHeight = 0u)
			{
				ResetMouseInputPoint();
				ResetMouseStates();
				ScreenResize(screenWidth, screenHeight);
			}

			void MouseInput::Release()
			{
				ClearMouseEventBuffer();
				EndCapture();
			}

			void MouseInput::Reset()
			{
				ResetMouseInputPoint();
				ResetMouseStates();
				ClearMouseEventBuffer();
				EndCapture();
			}

			void MouseInput::UpdateState()
			{
				for (UINT i = 0; i < MOUSE_COUNT; ++i)
				{
					switch (mouseStates[i])
					{
					case MouseState::DOWN:
						mouseStates[i] = MouseState::PRESSED;
						break;

					case MouseState::UP:
						mouseStates[i] = MouseState::NONE;
						break;
					}
				}

				ClearMouseEventBuffer();
			}

			bool MouseInput::MouseMessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
			{
				switch (uMessage)
				{
				case WM_MOUSEMOVE:
					OnMove(LOWORD(lParam), HIWORD(lParam));
					return true;

					// 마우스 왼쪽 클릭
				case WM_LBUTTONDOWN:
					OnDown(hWnd, MouseButtonType::LEFT, LOWORD(lParam), HIWORD(lParam));
					return true;
				case WM_LBUTTONUP:
					OnUp(hWnd, MouseButtonType::LEFT, LOWORD(lParam), HIWORD(lParam));
					return true;

					// 마우스 오른쪽
				case WM_RBUTTONDOWN:
					OnDown(hWnd, MouseButtonType::RIGHT, LOWORD(lParam), HIWORD(lParam));
					return true;
				case WM_RBUTTONUP:
					OnUp(hWnd, MouseButtonType::RIGHT, LOWORD(lParam), HIWORD(lParam));
					return true;

					// 마우스 휠
				case WM_MBUTTONDOWN:
					OnDown(hWnd, MouseButtonType::MIDDLE, LOWORD(lParam), HIWORD(lParam));
					return true;
				case WM_MBUTTONUP:
					OnUp(hWnd, MouseButtonType::MIDDLE, LOWORD(lParam), HIWORD(lParam));
					return true;

					// 마우스 기능성 버튼
				case WM_XBUTTONDOWN:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);

					switch (GET_XBUTTON_WPARAM(wParam))
					{
					case XBUTTON1:
						OnDown(hWnd, MouseButtonType::X1, LOWORD(lParam), HIWORD(lParam));
						break;

					case XBUTTON2:
						OnDown(hWnd, MouseButtonType::X2, LOWORD(lParam), HIWORD(lParam));
						break;
					}

					return true;
				}
				case WM_XBUTTONUP:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);

					switch (GET_XBUTTON_WPARAM(wParam))
					{
					case XBUTTON1:
						OnUp(hWnd, MouseButtonType::X1, LOWORD(lParam), HIWORD(lParam));
						break;

					case XBUTTON2:
						OnUp(hWnd, MouseButtonType::X2, LOWORD(lParam), HIWORD(lParam));
						break;
					}

					return true;
				}

				// 마우스 휠
				case WM_MOUSEWHEEL:
				{
					short delta = GET_WHEEL_DELTA_WPARAM(wParam);
					if (delta > 0)
						OnWheelVertical(1);
					else
						OnWheelVertical(-1);

					return true;
				}
				case WM_MOUSEHWHEEL:
				{
					short delta = GET_WHEEL_DELTA_WPARAM(wParam);
					if (delta > 0)
						OnWheelHorizontal(1);
					else
						OnWheelHorizontal(-1);

					return true;
				}

				case WM_INPUT:
				{
					UINT dataSize = 0u;
					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getrawinputdata

					GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));

					if (dataSize > 0)
					{
						std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
						if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
						{
							// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinput
							RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
							if (raw->header.dwType == RIM_TYPEMOUSE) // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinputheader
							{
								// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse
								OnRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
							}
						}
					}

					return true;
				}

				default:
					return false;
				}
			}

			void MouseInput::ScreenResize(UINT screenWidth, UINT screenHeight)
			{
				screenHalfSize.x = screenWidth / 2u;
				screenHalfSize.y = (screenHeight > 0u) ? screenHeight / 2u : screenHalfSize.x;
			}

			bool MouseInput::IsDown(MouseButtonType mouseButtonType) const
			{
				switch (mouseStates[static_cast<int>(mouseButtonType)])
				{
				case MouseState::DOWN:
				case MouseState::PRESSED:
					return true;
				}

				return false;
			}

			bool MouseInput::IsUp(MouseButtonType mouseButtonType) const
			{
				return (mouseStates[static_cast<int>(mouseButtonType)] == MouseState::UP);
			}

			MouseEvent MouseInput::ReadEvent()
			{
				// 마우스 이벤트 버퍼가 비어있으면
				// 유효하지 않은 마우스 이벤트 반환
				if (mouseEventBuffer.empty())
					return MouseEvent();

				// 마우스 이벤트 하나 반환
				MouseEvent currentEvent = mouseEventBuffer.front();
				mouseEventBuffer.pop();
				return currentEvent;
			}

			int MouseInput::GetX() const
			{
				return x;
			}

			int MouseInput::GetY() const
			{
				return y;
			}

			void MouseInput::ResetMouseStates()
			{
				for (UINT i = 0; i < MOUSE_COUNT; ++i)
					mouseStates[i] = MouseState::NONE;
			}

			void MouseInput::ResetMouseInputPoint()
			{
				x = y = 0;
			}

			void MouseInput::ClearMouseEventBuffer()
			{
				while (!mouseEventBuffer.empty())
					mouseEventBuffer.pop();
			}

			void MouseInput::OnDown(HWND hWnd, MouseButtonType mouseButtonType, int x, int y)
			{
				mouseStates[static_cast<int>(mouseButtonType)] = MouseState::DOWN;
				mouseEventBuffer.push(MouseEvent(MouseEventType::BUTTON_DOWN, x, y, mouseButtonType));
				this->x = x;
				this->y = y;

				StartCapture(hWnd);
			}

			void MouseInput::OnUp(HWND hWnd, MouseButtonType mouseButtonType, int x, int y)
			{
				mouseStates[static_cast<int>(mouseButtonType)] = MouseState::UP;
				mouseEventBuffer.push(MouseEvent(MouseEventType::BUTTON_UP, x, y, mouseButtonType));
				this->x = x;
				this->y = y;

				EndCapture();
			}

			void MouseInput::OnWheelVertical(int value)
			{
				mouseEventBuffer.push(MouseEvent(MouseEventType::WHEEL_VERTICAL, 0, value));
			}

			void MouseInput::OnWheelHorizontal(int value)
			{
				mouseEventBuffer.push(MouseEvent(MouseEventType::WHEEL_HORIZONTAL, 0, value));
			}

			void MouseInput::OnMove(int x, int y)
			{
				mouseEventBuffer.push(MouseEvent(MouseEventType::MOVE, x, y));
				this->x = x;
				this->y = y;
			}

			void MouseInput::OnRawMove(int x, int y)
			{
				mouseEventBuffer.push(MouseEvent(MouseEventType::RAW_MOVE, x, y));
				this->x = x;
				this->y = y;
			}

			void MouseInput::StartCapture(HWND hWnd)
			{
				RECT window;
				GetWindowRect(hWnd, &window);

				// 윈도우(창)이 창모드 일때 옮겨진 경우
				// 전체화면일때 해상도가 다른 경우가 있으므로 스크린 좌표 계산이 필요함
				int centerX = window.left + (window.right - window.left) / 2;
				int centerY = window.top + (window.bottom - window.top) / 2;
				POINT leftTop = { centerX - screenHalfSize.x, centerY - screenHalfSize.y };
				POINT rightBottom = { centerX + screenHalfSize.x, centerY + screenHalfSize.y };
				RECT rectClip = { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };

				SetCapture(hWnd);
				ClientToScreen(hWnd, &leftTop);
				ClientToScreen(hWnd, &rightBottom);
				ClipCursor(&rectClip);
			}

			void MouseInput::EndCapture()
			{
				ClipCursor(nullptr);
				ReleaseCapture();
			}
		} // Mouse
	} // Input
} // Engine