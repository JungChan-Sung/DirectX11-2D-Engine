#pragma once

#include "Engine/Option/ScreenOption.h"
#include "Engine/Window/Window.h"
#include "Engine/Input/Input.h"
#include "Engine/Graphics/Graphics.h"

namespace Engine
{
	namespace Window
	{
		/**
		 * @brief 윈도우 컨테이너 클래스
		 * @date 2024-11-06
		 * @class
		 * 윈도우OS의 시스템 윈도우(창), 입력, 그래픽스 생성 및 관리
		 */
		class WindowContainer
		{
		private: // ==== [설정] ====
			Option::ScreenOption* screenOption;	// 화면 설정 포인터

		private: // ==== [구성 요소] ====
			Window* window;					// 윈도우(창) 포인터
			Input::Input* input;			// 입력 포인터
			Graphics::Graphics* graphics;	// 그래픽스 포인터

		public: // ==== [생성자 및 소멸자] ====
			WindowContainer();
			~WindowContainer();

		public: // ==== [Setup] ====
			/**
			 * @brief 윈도우 컨테이너 초기화
			 * @date 2024-11-06
			 * @details
			 * 윈도우, 입력, 그래픽스 생성 및 초기화
			 * @param hInstance : 윈도우 핸들값
			 * @return 윈도우 컨테이너 초기화 성공 여부
			 */
			bool Initialize(HINSTANCE hInstance);
			/**
			 * @brief 윈도우 컨테이너 자원 해제
			 * @date 2024-11-06
			 * @details
			 * 윈도우(창), 입력, 그래픽스 자원 해제
			 */
			void Release();

		public: // ==== [윈도우 컨테이너 동작] ====
			/**
			 * @brief 윈도우 메시지 처리
			 * @date 2024-11-03
			 * @details
			 * 윈도우 입력 메시지를 처리
			 * @param hWnd : 윈도우 식별 값
			 * @param uMessage : 메시지 코드
			 * @param wParam : 추가 메시지 정보(실제 데이터 값 : 핸들 및 정수)
			 * @param lParam : 추가 메시지 정보(포인터 값 : 위치 및 주소)
			 * @return 메시지 처리 값
			 */
			LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

		public: // ==== [Get] ====
			/** @return 화면 설정 참조용 포인터 */
			Option::ScreenOption* GetScreenOption() const;

			/** @return 윈도우 참조용 포인터 */
			Window* GetWindow() const;
			/** @return 입력 참조용 포인터 */
			Input::Input* GetInput() const;
			/** @return 그래픽스 참조용 포인터 */
			Graphics::Graphics* GetGraphics() const;

		public: // ==== [Static] ====
			/**
			 * @brief 변경될 프로시저
			 * @date 2024-11-03
			 * @details
			 * SetWindowLongPtrW 함수를 통해 윈도우 프로시저를 변경함
			 * 파생 클래스에서 초기화할 때 변경할 것
			 * @throw
			 * 윈도우(창)을 자원 해제할 때 오류가 발생할 수 있으므로
			 * 윈도우가 자원 해제할 때 다시 SetWindowLongPtrW 함수를 사용하여 원래 프로시저로 되돌릴 것
			 * @param hWnd : 윈도우 식별 값
			 * @param uMessage : 메시지 코드
			 * @param wParam : 추가 메시지 정보(실제 데이터 값 : 핸들 및 정수)
			 * @param lParam : 추가 메시지 정보(포인터 값 : 위치 및 주소)
			 * @return 메시지 처리 값
			 */
			static LRESULT CALLBACK HandleMessageRedirect(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
		};
	} // Window
} // Engine