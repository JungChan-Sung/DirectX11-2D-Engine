#pragma once

#include "Keyboard/KeyboardInput.h"
#include "Mouse/MouseInput.h"

namespace Engine
{
	namespace Input
	{
		/**
		 * @brief 엔진 입력 클래스
		 * @date 2024-11-01
		 * @class
		 * 윈도우 시스템의 키보드 및 마우스 입력 윈도우 메시지 처리
		 */
		class Input
		{
		private: // ==== [구성 요소] ====
			Keyboard::KeyboardInput* keyboard;	// 키보드 입력
			Mouse::MouseInput* mouse;			// 마우스 입력

		public: // ==== [생성자 및 소멸자] ====
			Input();
			~Input();

		public: // ==== [Setup] ====
			/**
			 * @brief 입력 초기화
			 * @details
			 * 윈도우의 WM_INPUT 메시지를 처리하기 위해 RawInput 설정
			 * 윈도우 키보드 입력과 마우스 입력을 생성
			 * @param screenWidth : 화면 너비
			 * @param screenHeight : 화면 높이
			 * @return 입력 초기화 성공 여부
			 */
			bool Initialize(UINT screenWidth, UINT screenHeight = 0u);
			/**
			 * @brief 입력 자원 해제
			 * @details
			 * 키보드 입력과 마우스 입력 자원 해제
			 */
			void Release();

		public: // ==== [입력 동작] ====
			/**
			 * @brief 입력 재설정
			 * @details
			 * 키보드 입력과 마우스 입력 재설정
			 */
			void Reset();
			/**
			 * @brief 입력 상태 갱신
			 * @date 2024-11-01
			 * @details
			 * 키보드 입력과 마우스 입력 상태 갱신
			 */
			void UpdateState();
			/**
			 * @brief 윈도우 프로시저 메시지 입력 처리
			 * @details
			 * 윈도우 메시지 중 입력을 처리함
			 * @param hWnd : 윈도우 식별 값
			 * @param uMsg : 메시지 코드
			 * @param wParam : 추가 메시지 정보(실제 데이터 값 : 핸들 및 정수)
			 * @param lParam : 추가 메시지 정보(포인터 값 : 위치 및 주소)
			 */
			void InputMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		public: // ==== [Get] ====
			/** @return 키보드 입력 참조용 포인터 */
			Keyboard::KeyboardInput* GetKeyboard() const;
			/** @return 마우스 입력 참조용 포인터 */
			Mouse::MouseInput* GetMouse() const;
		};
	} // Input
} // Engine

