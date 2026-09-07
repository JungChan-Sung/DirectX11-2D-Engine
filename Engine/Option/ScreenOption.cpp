#include "ScreenOption.h"
#include "Engine/Config/EngineConfig.h"

namespace Engine
{
	namespace Option
	{
		ScreenOption::ScreenOption() : resolutionWidth(0u), resolutionHeight(0u),
			fps(0u), inverseFps(0.0),
			isFullScreen(false), isVsync(false),
			minResolutionWidth(0u), minResolutionHeight(0u),
			maxFps(0u)
		{
		}

		bool ScreenOption::SaveScreenOption()
		{
			// todo : 저장 제작
			return false;
		}

		bool ScreenOption::LoadScreenOption()
		{
			CommonUtility::ConfigParser optionConfig;

			if (optionConfig.Parse(EngineConfig::Instance()->OptionConfigPath())) // optionConfig.Parse == true
			{
				// 화면 섹션
				resolutionWidth = std::stoul(optionConfig.GetValue(ScreenOptionSection::SCREEN_SECTION, ScreenOptionKeyValue::RESOLUTION_WIDTH_KEY));
				resolutionHeight = std::stoul(optionConfig.GetValue(ScreenOptionSection::SCREEN_SECTION, ScreenOptionKeyValue::RESOLUTION_HEIGHT_KEY));
				fps = std::stoul(optionConfig.GetValue(ScreenOptionSection::SCREEN_SECTION, ScreenOptionKeyValue::FPS_KEY));
				inverseFps = 1.0 / static_cast<double>(fps);
				isFullScreen = std::stoi(optionConfig.GetValue(ScreenOptionSection::SCREEN_SECTION, ScreenOptionKeyValue::FULL_SCREEN_KEY));
				isVsync = std::stoi(optionConfig.GetValue(ScreenOptionSection::SCREEN_SECTION, ScreenOptionKeyValue::VSYNC_KEY));
				
				// 제한 섹션
				minResolutionWidth = std::stoul(optionConfig.GetValue(ScreenOptionSection::LIMIT_SECTION, ScreenOptionKeyValue::MIN_RESOLUTION_WIDTH_KEY));
				minResolutionHeight = std::stoul(optionConfig.GetValue(ScreenOptionSection::LIMIT_SECTION, ScreenOptionKeyValue::MIN_RESOLUTION_HEIGHT_KEY));
				maxFps = std::stoul(optionConfig.GetValue(ScreenOptionSection::LIMIT_SECTION, ScreenOptionKeyValue::MAX_FPS_KEY));

				return true;
			}

			return false;
		}

		void ScreenOption::SetResolution(UInt resolutionWidth, UInt resolutionHeight)
		{
			// 최소 해상도 너비
			if (resolutionWidth < minResolutionWidth)
				resolutionWidth = minResolutionWidth;

			// 최소 해상도 높이
			if (resolutionHeight < minResolutionHeight)
				resolutionHeight = minResolutionHeight;

			this->resolutionWidth = resolutionWidth;
			this->resolutionHeight = resolutionHeight;

			// todo : 해상도 변경 이벤트 추가(그래픽스, 입력, 카메라) 윈도우는 따로 조정
		}

		void ScreenOption::SetFps(UInt fps)
		{
			// 최대 초당 프레임 수
			if (fps > maxFps)
				fps = maxFps;

			this->fps = fps;
			this->inverseFps = 1.0 / static_cast<double>(fps);
		}

		void ScreenOption::SetFullScreen(bool isFullScreen)
		{
			this->isFullScreen = isFullScreen;

			// todo : 전체화면 변경 이벤트 추가(윈도우, 그래픽스)
		}

		void ScreenOption::SetVsync(bool isVsync)
		{
			this->isVsync = isVsync;

			// todo : 수직 동기화 변경 이벤트 추가(그래픽스)
		}
	} // Option
} // Engine