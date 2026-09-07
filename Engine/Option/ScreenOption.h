#pragma once

namespace Engine
{
	namespace Option
	{
		using UInt = unsigned int;

		namespace ScreenOptionSection // ==== [화면 설정 섹션 값] ====
		{
			constexpr static const wchar_t* SCREEN_SECTION = L"Screen";
			constexpr static const wchar_t* LIMIT_SECTION = L"Limit";
		} // ScreenOptionSection

		namespace ScreenOptionKeyValue // ==== [화면 설정 키-값] ====
		{
			constexpr static const wchar_t* RESOLUTION_WIDTH_KEY = L"resolutionWidth";
			constexpr static const wchar_t* RESOLUTION_HEIGHT_KEY = L"resolutionHeight";
			constexpr static const wchar_t* FPS_KEY = L"fps";
			constexpr static const wchar_t* FULL_SCREEN_KEY = L"fullScreen";
			constexpr static const wchar_t* VSYNC_KEY = L"vsync";
			constexpr static const wchar_t* MIN_RESOLUTION_WIDTH_KEY = L"minResolutionWidth";
			constexpr static const wchar_t* MIN_RESOLUTION_HEIGHT_KEY = L"minResolutionHeight";
			constexpr static const wchar_t* MAX_FPS_KEY = L"maxFps";
		} // ScreenOptionKeyValue

		/**
		 * @brief 화면 설정 클래스
		 * @date 2024-11-06
		 * @class
		 * 화면 설정 값을 지니고 있는 클래스
		 */
		class ScreenOption
		{
		private: // ==== [구성 요소] ====
			UInt resolutionWidth;	// 해상도 너비
			UInt resolutionHeight;	// 해상도 높이
			UInt fps;				// 초당 프레임 수
			double inverseFps;		// 역 초당 프레임 수(프레임당 시간 간격)
			bool isFullScreen;		// 전체화면 여부
			bool isVsync;			// 수직 동기화 여부

		private: // ==== [한계 수치] ====
			UInt minResolutionWidth;	// 해상도 최소 너비
			UInt minResolutionHeight;	// 해상도 최소 높이
			UInt maxFps;				// 최대 초당 프레임 수

		public: // ==== [생성자] ====
			ScreenOption();

		public: // ==== [설정 값 저장 및 불러오기] ====
			/**
			 * @brief 화면 설정 값 저장
			 * @return 저장 성공 여부
			 */
			bool SaveScreenOption();
			/**
			 * @brief 화면 설정 값 불러오기
			 * @return 불러오기 성공 여부
			 */
			bool LoadScreenOption();

		public: // ==== [Set] ====
			/**
			 * @param resolutionWidth : 해상도 너비
			 * @param resolutionHeight : 해상도 높이
			 */
			void SetResolution(UInt resolutionWidth, UInt resolutionHeight);
			/** @param fps : 초당 프레임 수 */
			void SetFps(UInt fps);
			/** @param isFullScreen : 전체화면 여부 */
			void SetFullScreen(bool isFullScreen);
			/** @param isVsync : 수직동기화 여부 */
			void SetVsync(bool isVsync);

		public: // ==== [Get] ====
			/** @return 해상도 너비 */
			inline UInt GetResolutionWidth() const 
			{
				return resolutionWidth;
			}
			/** @return 해상도 높이 */
			inline UInt GetResolutionHeight() const 
			{
				return resolutionHeight;
			}
			/** @return 초당 프레임 수 */
			inline UInt GetFps() const
			{
				return fps;
			}
			/** @return 역 초당 프레임 수 */
			inline double GetInverseFps() const
			{
				return inverseFps;
			}
			/** @return 전체화면 여부 */
			inline bool IsFullScreen() const
			{
				return isFullScreen;
			}
			/** @return 수직 동기화 여부 */
			inline bool IsVsync() const
			{
				return isVsync;
			}
		};
	} // Option
} // Engine
