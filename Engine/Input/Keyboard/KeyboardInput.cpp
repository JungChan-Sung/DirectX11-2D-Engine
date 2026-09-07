#include "KeyboardInput.h"

namespace Engine
{
	namespace Input
	{
		namespace Keyboard
		{
			KeyboardInput::KeyboardInput() : isPressedAnyKey(false)
			{
				Initialize();
			}

			KeyboardInput::~KeyboardInput()
			{
				Release();
			}

			void KeyboardInput::Initialize()
			{
				ResetKeyStates();
			}

			void KeyboardInput::Release()
			{
				ClearCharBuffer();
			}

			void KeyboardInput::Reset()
			{
				ResetKeyStates();
				ResetPressedAnyKey();
				ClearCharBuffer();
			}

			void KeyboardInput::UpdateState()
			{
				for (UINT i = 0; i < KEY_COUNT; ++i)
				{
					switch (keyStates[i])
					{
					case KeyState::DOWN:
						keyStates[i] = KeyState::PRESSED;
						break;

					case KeyState::UP:
						keyStates[i] = KeyState::NONE;
						break;
					}
				}

				ResetPressedAnyKey();
				ClearCharBuffer();
			}

			bool KeyboardInput::KeyboardMessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
			{
				switch (uMessage)
				{
				case WM_KEYDOWN:
				{
					UCHAR keyCode = static_cast<UCHAR>(wParam);
					// lParam : https://learn.microsoft.com/ko-kr/windows/win32/inputdev/wm-char
					if (!(lParam & 0x40000000/*30 : Repeat Check Flag*/))
					{
						OnDown(keyCode);
						isPressedAnyKey = true;
					}
					return true;
				}

				case WM_KEYUP:
				{
					UCHAR keyCode = static_cast<UCHAR>(wParam);
					OnUp(keyCode);

					return true;
				}

				case WM_CHAR:
				{
					UCHAR ch = static_cast<UCHAR>(wParam);
					OnChar(ch);

					return true;
				}

				default:
					return false;
				}
			}

			bool KeyboardInput::IsDown(const UCHAR keyValue) const
			{
				return (keyStates[keyValue] == KeyState::DOWN);
			}

			bool KeyboardInput::IsPressed(const UCHAR keyValue) const
			{
				switch (keyStates[keyValue])
				{
				case KeyState::DOWN:
				case KeyState::PRESSED:
					return true;
				}

				return false;
			}

			bool KeyboardInput::IsUp(const UCHAR keyValue) const
			{
				return (keyStates[keyValue] == KeyState::UP);
			}

			bool KeyboardInput::IsPressedAnyKey() const
			{
				return isPressedAnyKey;
			}

			UCHAR KeyboardInput::ReadChar()
			{
				// 버퍼가 비어있는 경우
				// 널문자 반환
				if (charBuffer.empty())
					return '\0';

				// 버퍼 문자 하나 반환
				UCHAR uChar = charBuffer.front();
				charBuffer.pop();
				return uChar;
			}

			void KeyboardInput::ResetKeyStates()
			{
				for (UINT i = 0; i < KEY_COUNT; ++i)
					keyStates[i] = KeyState::NONE;
			}

			void KeyboardInput::ResetPressedAnyKey()
			{
				isPressedAnyKey = false;
			}

			void KeyboardInput::ClearCharBuffer()
			{
				while (!charBuffer.empty())
					charBuffer.pop();
			}

			void KeyboardInput::OnDown(const UCHAR keyValue)
			{
				keyStates[keyValue] = KeyState::DOWN;
			}

			void KeyboardInput::OnUp(const UCHAR keyValue)
			{
				keyStates[keyValue] = KeyState::UP;
			}

			void KeyboardInput::OnChar(const UCHAR uChar)
			{
				charBuffer.push(uChar);
			}
		} // Keyboard
	} // Input
} // Engine