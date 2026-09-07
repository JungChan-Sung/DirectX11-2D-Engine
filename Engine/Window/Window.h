#pragma once

// 자주 사용되지 않는 WinAPI 코드를 제외
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Engine
{
	namespace Window
	{
		/**
		 * @brief 윈도우(창) 속성 구조체
		 * @date 2024-10-04
		 * @struct
		 * 윈도우(창)에 대한 데이터를 지님
		 */
		struct WindowDescription
		{
		public: // ==== [윈도우 속성 구성 요소] ====
			LPCWSTR title;			// 윈도우 제목
			int x, y;				// 윈도우 초기 위치
			UINT width, height;		// 윈도우 크기(테두리 포함된 크기)
			DWORD dwStyle;			// 윈도우 스타일
		};

		// 윈도우 컨테이너 전방 선언
		class WindowContainer;
		/**
		 * @brief 윈도우(창) 클래스
		 * @date 2024-11-06
		 * @class
		 * 윈도우 창 생성 및 화면에 띄워주며 윈도우의 데이터를 보유
		 */
		class Window
		{
		private: // ==== [구성 요소] ====
			HINSTANCE hInstance;					// Handle + Instance : 윈도우 인스턴스 값, 프로그램 자체의 실체화된 주소 값
			HWND hWnd;								// Handle + Window : 윈도우 식별 값, 화면에 띄워진 윈도우 번호 값
			WindowDescription windowDescription;	// 윈도우 속성

		public: // ==== [생성자 및 소멸자] ====
			Window();
			~Window();

		public: // ==== [Setup] ====
			/**
			 * @brief 윈도우(창) 초기화
			 * @details
			 * 윈도우(창)을 생성 및 화면에 띄워줌
			 * @param hInstance : 윈도우 핸들값
			 * @param title : 윈도우 제목
			 * @param width : 윈도우 너비
			 * @param height : 윈도우 높이
			 * @param isFullSceen : 전체화면 여부
			 * @param windowContainer : 윈도우 컨테이너 참조용 포인터
			 * @return 윈도우 생성 성공 여부
			 */
			bool Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height, bool isFullSceen, WindowContainer* windowContainer);
			/**
			 * @brief 윈도우 자원 해제
			 * @date 2024-11-06
			 * @details
			 * 윈도우(창)을 파괴 및 윈도우 인스턴스 값 할당 해제
			 */
			void Release();

		public: // ==== [윈도우 동작] ====
			/** @brief 기본 화면 설정 복원 */
			void RestoreDefaultDisplaySettings();

		public: // ==== [Get] ====
			/** @return 윈도우 식별 값 */
			HWND GetHWND() const;
			/** @return 윈도우 너비 */
			UINT GetWindowWidth() const;
			/** @return 윈도우 높이 */
			UINT GetWindowHeight() const;

		public: // ==== [Static] ====
			/**
			 * @brief 윈도우 프로시저
			 * @date 2024-11-05
			 * @details
			 * 제일 처음 윈도우(창)을 생성될 때 메시지 처리할 프로시저
			 * @param hWnd : 윈도우 식별 값
			 * @param uMessage : 메시지 코드
			 * @param wParam : 추가 메시지 정보(실제 데이터 값 : 핸들 및 정수)
			 * @param lParam : 추가 메시지 정보(포인터 값 : 위치 및 주소)
			 * @return 메시지 처리 값
			 */
			static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

		private: // ==== [윈도우 생성 및 등록] ====
			/**
			 * @brief 윈도우 속성 설정
			 * @date 2024-11-05
			 * @details
			 * 전체화면 여부에 따라 윈도우 크기와 화면 크기를 결정함
			 * @param title : 설정할 윈도우 타이틀
			 * @param width : 설정할 윈도우 너비
			 * @param height : 설정할 윈도우 높이
			 * @param isFullScreen : 전체화면 여부
			 */
			void DescriptionWindow(LPCWSTR title, UINT width, UINT height, bool isFullScreen);
			/** @brief 윈도우 등록 */
			void RegisterWindowClass();
			/**
			 * @brief 윈도우 생성 및 화면 띄우기
			 * @details
			 * 윈도우 식별 값 생성 및 윈도우(창)을 화면에 띄워줌
			 * @param windowContainer : 윈도우 컨테이너 참조용 포인터
			 * @return 윈도우 생성 성공 여부
			 */
			bool CreateAndShowWindow(WindowContainer* windowContainer);
		};
	} // Window
} // Engine