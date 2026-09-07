#pragma once

#include <Windows.h>
#include <queue>

namespace Engine
{
	namespace Input
	{
		namespace Mouse
		{
			// 마우스 버튼 종류
			enum class MouseButtonType
			{
				LEFT,	// 왼쪽
				RIGHT,	// 오른쪽
				MIDDLE,	// 휠
				X1,		// 마우스3
				X2,		// 마우스4
				INVALID	// 유효하지 않음
			};

			// 마우스 이벤트 종류
			enum class MouseEventType
			{
				BUTTON_DOWN,		// 마우스 다운
				BUTTON_UP,			// 마우스 업
				WHEEL_VERTICAL,		// 수직 휠
				WHEEL_HORIZONTAL,	// 수평 휠
				MOVE,				// 마우스 이동
				RAW_MOVE,			// 마우스 이동(DPI 더 높은 감지)
				INVALID				// 유효하지 않음
			};

			/**
			 * @brief 마우스 이벤트 구조체
			 * @date 2024-10-03
			 * @struct
			 * 마우스의 입력 이벤트 데이터를 저장할 구조체
			 */
			struct MouseEvent
			{
			public: // ==== [마우스 이벤트 구성 요소] ====
				MouseButtonType button;		// 마우스 버튼 종류
				MouseEventType eventType;	// 이벤트 종류
				int x, y;					// 이벤트 발생 위치

			public: // ==== [생성자] ====
				MouseEvent() : button(MouseButtonType::INVALID), eventType(MouseEventType::INVALID), x(0), y(0)
				{
				}
				/**
				 * @param eventType : 이벤트 종류
				 * @param x : 마우스 이벤트 발생 X축 위치
				 * @param y : 마우스 이벤트 발생 Y축 위치
				 * @param button : 버튼 종류
				 */
				MouseEvent(MouseEventType eventType, int x, int y, MouseButtonType button = MouseButtonType::INVALID) : button(button), eventType(eventType), x(x), y(y)
				{
				}
			};

			/**
			 * @brief 마우스 입력 클래스
			 * @date 2024-10-03
			 * @class
			 * 마우스의 입력을 받아 이벤트를 발생 시킴
			 */
			class MouseInput
			{
			private: // ==== [선언] ====
				// 마우스 입력 상수
				enum MouseConstant : UINT
				{
					MOUSE_COUNT = static_cast<UINT>(MouseButtonType::INVALID),	// 마우스 버튼 개수
				};

				// 마우스 상태
				enum class MouseState
				{
					NONE,
					DOWN,
					PRESSED,
					UP
				};

			private: // ==== [구성 요소] ====
				MouseState mouseStates[MOUSE_COUNT];		// 마우스 상태 배열
				std::queue<MouseEvent> mouseEventBuffer;	// 마우스 이벤트 버퍼

			private: // ==== [마우스 정보] ====
				int x, y;				// 마우스 입력 위치
				POINT screenHalfSize;	// 화면 절반 크기

			public: // ==== [생성자 및 소멸자] ====
				/**
				 * @details
				 * 마우스 입력을 받을 영역을 매개변수로 받음
				 * 화면 높이를 0으로 설정하면 화면 너비와 같은 값으로 자동 생성됨
				 * @param screenWidth : 화면 너비
				 * @param screenHeight : 화면 높이
				 */
				MouseInput(UINT screenWidth, UINT screenHeight = 0u);
				~MouseInput();

			public: // ==== [Setup] ====
				/**
				 * @brief 마우스 입력 초기화
				 * @details
				 * 마우스 입력 위치와 상태를 초기화하며
				 * 마우스가 입력이 가능한 화면 크기를 설정
				 * @param screenWidth : 화면 너비
				 * @param screenHeight : 화면 높이
				 */
				void Initialize(UINT screenWidth, UINT screenHeight);
				/**
				 * @brief 마우스 입력 자원 해제
				 * @details
				 * 마우스 이벤트 버퍼를 비우면서 마우스 이벤트 입력 캡쳐를 종료함
				 */
				void Release();

			public: // ==== [마우스 입력 동작] ====
				/**
				 * @brief 마우스 입력 재설정
				 * @details
				 * 마우스 입력 위치, 상태를 초기상태로 되돌리며
				 * 마우스 이벤트 버퍼를 비우고 마우스 이벤트 입력 캡쳐를 종료함
				 */
				void Reset();
				/**
				 * @brief 마우스 입력 상태 갱신
				 * @details
				 * 마우스 상태를 변경하며 마우스 이벤트 버퍼를 비움
				 */
				void UpdateState();
				/**
				 * @brief 윈도우 메시지의 마우스 입력 처리
				 * @details
				 * 윈도우 메시지의 모든 마우스 입력에 관한 처리
				 * @param hWnd : 윈도우 식별 값
				 * @param uMessage : 메시지 코드
				 * @param wParam : 추가 메시지 정보(실제 데이터 값 : 핸들 및 정수)
				 * @param lParam : 추가 메시지 정보(포인터 값 : 위치 및 주소)
				 * @return 마우스 입력 여부
				 */
				bool MouseMessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

			public: // ==== [화면 크기] ====
				/**
				 * @brief 화면 크기 재조정
				 * @details
				 * 화면 높이를 0으로 설정하면 화면 너비와 같은 값으로 자동 생성됨
				 * @param screenWidth : 재조정할 화면 너비
				 * @param screenHeight : 재조정할 화면 높이
				 */
				void ScreenResize(UINT screenWidth, UINT screenHeight = 0u);

			public: // ==== [마우스 입력 상태 반환] ====
				/**
				 * @details
				 * 지정된 마우스 버튼이 현재 눌려 있는 상태인지 확인
				 * @param mouseButtonType : 확인할 마우스 버튼 종류
				 * @return 해당 마우스 버튼이 눌려 있는지 여부
				 */
				bool IsDown(MouseButtonType mouseButtonType) const;
				/**
				 * @details
				 * 지정된 마우스 버튼이 현재 눌려 있는 않은 상태인지 확인
				 * @param mouseButtonType : 확인할 마우스 버튼 종류
				 * @return 해당 마우스 버튼이 눌려 있는 않은지 여부
				 */
				bool IsUp(MouseButtonType mouseButtonType) const;
				/**
				 * @brief 이벤트 읽기
				 * @details
				 * 입력된 이벤트 버퍼에서 이벤트 하나 반환
				 * @return 버퍼에 제일 먼저 입력된 마우스 이벤트
				 */
				MouseEvent ReadEvent();

			public: // ==== [Get] ====
				/** @return 마우스 X축 위치 */
				int GetX() const;
				/** @return 마우스 Y축 위치 */
				int GetY() const;

			private: // ==== [마우스 입력 리셋] ====
				/**
				 * @brief 마우스 상태 리셋
				 * @details
				 * 마우스 상태 배열을 초기값으로 되돌림
				 */
				void ResetMouseStates();
				/**
				 * @brief 마우스 입력 위치 리셋
				 * @details
				 * 마우스 입력 위치를 초기값으로 되돌림
				 */
				void ResetMouseInputPoint();
				/**
				 * @brief 이벤트 버퍼 비우기
				 */
				void ClearMouseEventBuffer();

			private: // ==== [마우스 입력 이벤트] ====
				/**
				 * @param hWnd : 윈도우 식별 값
				 * @param mouseButtonType : 마우스 버튼 종류
				 * @param x : 누른 X축 위치
				 * @param y : 누른 Y축 위치
				 */
				void OnDown(HWND hWnd, MouseButtonType mouseButtonType, int x, int y);
				/**
				 * @param hWnd : 윈도우 식별 값
				 * @param mouseButtonType : 마우스 버튼 종류
				 * @param x : 뗀 X축 위치
				 * @param y : 뗀 Y축 위치
				 */
				void OnUp(HWND hWnd, MouseButtonType mouseButtonType, int x, int y);
				/** @param value : 휠 입력 수직 방향(양수 : 위, 음수 : 아래) */
				void OnWheelVertical(int value);
				/** @param value : 휠 입력 수평 방향(양수 : 우, 음수 : 좌) */
				void OnWheelHorizontal(int value);
				/**
				 * @param x : 움직인 마우스 X축 위치
				 * @param y : 움직인 마우스 Y축 위치
				 */
				void OnMove(int x, int y);
				/**
				 * @param x : 움직인 마우스 X축 위치
				 * @param y : 움직인 마우스 Y축 위치
				 */
				void OnRawMove(int x, int y);

			private: // ==== [마우스 입력 캡쳐] ====
				/**
				 * @brief 마우스 이벤트 입력 캡쳐 시작
				 * @param hWnd : 윈도우 식별 값
				 */
				void StartCapture(HWND hWnd);
				/**
				 * @brief 마우스 이벤트 입력 캡쳐 종료
				 */
				void EndCapture();
			};
		} // Mouse
	} // Input
} // Engine

