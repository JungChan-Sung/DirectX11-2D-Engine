#pragma once

#include <Windows.h>
#include <queue>

namespace Engine
{
	namespace Input
	{
		namespace Keyboard
		{
			/**
			 * @brief 키보드 입력 클래스
			 * @date 2024-10-03
			 * @class
			 * 키보드의 입력을 받아 이벤트를 발생 시킴
			 */
			class KeyboardInput
			{
			private: // ==== [선언] ====
				// 키보드 입력 상수
				enum KeyboardConstant : UINT
				{
					KEY_COUNT = 256u,		// 키 개수
				};

				// 키 상태
				enum class KeyState
				{
					NONE,
					DOWN,
					PRESSED,
					UP
				};

			private: // ==== [구성 요소] ====
				KeyState keyStates[KEY_COUNT];	// 키 상태 배열
				std::queue<UCHAR> charBuffer;	// 문자 버퍼
				bool isPressedAnyKey;			// 아무키 입력 여부

			public: // ==== [생성자 및 소멸자] ====
				KeyboardInput();
				~KeyboardInput();

				/* ==== [Setup] ==== */
				/**
				 * @brief 키보드 입력 초기화
				 * @details
				 * 키보드 상태 초기화
				 */
				void Initialize();
				/**
				 * @brief 키보드 입력 자원 해제
				 * @details
				 * 문자 버퍼에 들어있는 문자들을 전부 비움
				 */
				void Release();

			public: // ==== [키보드 입력 동작] ====
				/**
				 * @brief 키보드 입력 재설정
				 * @details
				 * 키보드 상태 배열과 아무키 입력 여부를 초기 상태로 되돌리며
				 * 문자 버퍼를 전부 비움
				 */
				void Reset();
				/**
				 * @brief 키보드 입력 상태 갱신
				 * @details
				 * 키보드 상태를 변경하며 아무키 입력 여부를 초기 상태로 되돌리고
				 * 문자 버퍼를 전부 비움
				 */
				void UpdateState();
				/**
				 * @brief 윈도우 메시지의 키보드 입력 처리
				 * @details
				 * 윈도우 메시지의 모든 키보드 입력에 관한 처리
				 * @param hWnd : 윈도우 식별 값
				 * @param uMessage : 메시지 코드
				 * @param wParam : 추가 메시지 정보(실제 데이터 값 : 핸들 및 정수)
				 * @param lParam : 추가 메시지 정보(포인터 값 : 위치 및 주소)
				 * @return 키보드 입력 여부
				 */
				bool KeyboardMessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

			public: // ==== [키보드 입력 상태 반환] ====
				/**
				 * @details
				 * 주어진 키 값에 해당하는 키가 현재 눌려 있는 상태인지 확인
				 * @param keyValue : 확인할 키 값
				 * @return 해당 키가 눌려 있는지 여부
				 */
				bool IsDown(const UCHAR keyValue) const;
				/**
				 * @details
				 * 주어진 키 값에 해당하는 키가 현재 눌려 졌었는지 확인
				 * @param keyValue : 확인할 키 값
				 * @return 해당 키가 눌려 졌었는지 여부
				 */
				bool IsPressed(const UCHAR keyValue) const;
				/**
				 * @details
				 * 주어진 키 값에 해당하는 키가 현재 눌려 있는 않은 상태인지 확인
				 * @param keyValue : 확인할 키 값
				 * @return 해당 키가 눌려 있는 않은지 여부
				 */
				bool IsUp(const UCHAR keyValue) const;
				/** @return 아무키가 눌러 졌었는지 여부 */
				bool IsPressedAnyKey() const;
				// 입력된 문자 버퍼에서 문자 하나 반환
				/**
				 * @brief 문자 읽기
				 * @details
				 * 입력된 문자 버퍼에서 문자 하나 반환
				 * @return 버퍼에 제일 먼저 입력된 문자
				 */
				UCHAR ReadChar();

			private: // ==== [키보드 입력 리셋] ====
				/**
				 * @brief 키 상태 배열 리셋
				 * @details
				 * 키 상태 배열을 초기값으로 되돌림
				 */
				void ResetKeyStates();
				/**
				 * @brief 아무키 누름 여부 리셋
				 * @details
				 * 아무키 누름 여부 값을 초기값으로 되돌림
				 */
				void ResetPressedAnyKey();
				/**
				 * @brief 문자 버퍼 비우기
				 */
				void ClearCharBuffer();

			private: // ==== [키보드 입력 이벤트] ====
				/** @param keyValue : 누른 키 값 */
				void OnDown(const UCHAR keyValue);
				/** @param keyValue : 뗀 키 값 */
				void OnUp(const UCHAR keyValue);
				/** @param character : 입력된 문자 */
				void OnChar(const UCHAR character);
			};
		} // Keyboard
	} // Input
} // Engine