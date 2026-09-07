/*********************************************************************
 * @file   EngineConfig.h
 * @brief  엔진 설정 값
 * @date   2024-11-06
 *********************************************************************/

#pragma once

#include <crtdbg.h>
#include "CommonUtility/Pattern/Singleton.h"
#include "CommonUtility/Utility/ConfigParser.h"

 // 엔진 디버그 모드(0 : Off, 1 : On)
#define ENGINE_DEBUG_MODE 1

// 디버그 모드 동작시 정의할 define
#if ENGINE_DEBUG_MODE
// 메모리 누수 탐지
#define _CRTDBG_MAP_ALLOC
// 충돌체 그리기 여부
#define COLLIDER_DRAW
#endif // ENGINE_DEBUG_MODE

// 비균형 변형(비등방성 스케일) 허용 여부(0 : 균형 변형만 허용, 1 : 비균형 변형 허용)
#define NON_UNIFORM_SCALING 1
// 화면 좌표계 여부(0 : 데카르트 좌표계, 1 : 화면 좌표계)
#define SCREEN_CROODINATE 1

namespace Engine
{
	namespace EngineConfigSection // ==== [엔진 섹션 값] ====
	{
		constexpr static const wchar_t* ENGINE_CONFIG_PATH = L"Config/EngineConfig.cfg";
		constexpr static const wchar_t* WINDOW_SECTION = L"Window";
		constexpr static const wchar_t* ENGINE_SECTION = L"Engine";
		constexpr static const wchar_t* GRAPHICS_SECTION = L"Graphics";
		constexpr static const wchar_t* GAME_OBJECT_SECTION = L"GameObject";
		constexpr static const wchar_t* CONFIG_PATH_SECTION = L"ConfigPath";
	} // EngineConfigSection

	namespace EngineConfigKeyValue // ==== [엔진 키-값] ====
	{
		constexpr static const wchar_t* TITLE_KEY = L"title";
		constexpr static const wchar_t* WORLD_RATIO_KEY = L"worldRatio";
		constexpr static const wchar_t* INVERSE_WORLD_RATIO_KEY = L"inverseWorldRatio";
		constexpr static const wchar_t* INVERSE_BYTE_KEY = L"inverseByte";
		constexpr static const wchar_t* GAME_OBJECT_NAME_KEY = L"gameObjectName";
		constexpr static const wchar_t* MAIN_CAMERA_GAME_OBJECT_NAME_KEY = L"mainCameraGameObjectName";
		constexpr static const wchar_t* REOUSRCE_CONFIG_PATH_KEY = L"resourceConfigPath";
		constexpr static const wchar_t* OPTION_CONFIG_PATH_KEY = L"optionConfigPath";
	} // EngineConfigKeyValue

	/**
	 * @brief 엔진 설정 값 클래스
	 * @date 2024-11-06
	 * @class
	 * Config 파일을 불러와 싱글톤 패턴으로 전역 값처럼 사용함
	 * constexpr을 사용한 상수 값 참조를 줄이기 위해 사용
	 */
	class EngineConfig : public CommonUtility::Singleton<EngineConfig>
	{
	private: // ==== [윈도우 섹션 값] ====
		std::wstring title;	// 윈도우 타이틀

	private: // ==== [엔진 섹션 값] ====
		float worldRatio;			// 월드 비율
		float inverseWorldRatio;	// 역 월드 비율

	private: // ==== [그래픽스 섹션 값] ====
		float inverseByte;	// 역 바이트 값(1 / 255)

	private: // ==== [게임 오브젝트 섹션 값] ====
		std::wstring defaultGameObjectName;		// 기본 게임 오브젝트 이름
		std::wstring mainCameraGameObjectName;	// 메인 카메라 게임 오브젝트 이름

	private: // ==== [Config 경로 섹션 값] ====
		std::wstring resourceConfigPath;	// 리소스 Config 파일 경로
		std::wstring optionConfigPath;		// 설정 Config 파일 경로

	public: // ==== [생성자] ====
		EngineConfig() : worldRatio(0.0f), inverseWorldRatio(0.0f), inverseByte(0.0f)
		{
		}

	public: // ==== [Setup] ====
		/**
		 * @brief 엔진 설정 값 초기화
		 * @date 2024-11-06
		 * @details
		 * 엔진 Config 파일을 불러와 설정에 사용할 값들을 불러옴
		 * @return 엔진 설정 값 초기화 성공 여부(true : 파일 읽기 성공, false : 파일 읽기 실패)
		 */
		bool Initialize()
		{
			CommonUtility::ConfigParser engineConfig;
			
			if (engineConfig.Parse(EngineConfigSection::ENGINE_CONFIG_PATH)) // engineConfig.Parse == true
			{
				// 윈도우 섹션
				title = engineConfig.GetValue(EngineConfigSection::WINDOW_SECTION, EngineConfigKeyValue::TITLE_KEY);

				// 엔진 섹션
				worldRatio = std::stof(engineConfig.GetValue(EngineConfigSection::ENGINE_SECTION, EngineConfigKeyValue::WORLD_RATIO_KEY));
				inverseWorldRatio = std::stof(engineConfig.GetValue(EngineConfigSection::ENGINE_SECTION, EngineConfigKeyValue::INVERSE_WORLD_RATIO_KEY));

				// 그래픽스 섹션
				inverseByte = std::stof(engineConfig.GetValue(EngineConfigSection::GRAPHICS_SECTION, EngineConfigKeyValue::INVERSE_BYTE_KEY));

				// 게임 오브젝트 섹션
				defaultGameObjectName = engineConfig.GetValue(EngineConfigSection::GAME_OBJECT_SECTION, EngineConfigKeyValue::GAME_OBJECT_NAME_KEY);
				mainCameraGameObjectName = engineConfig.GetValue(EngineConfigSection::GAME_OBJECT_SECTION, EngineConfigKeyValue::MAIN_CAMERA_GAME_OBJECT_NAME_KEY);

				// Config 경로 섹션
				resourceConfigPath = engineConfig.GetValue(EngineConfigSection::CONFIG_PATH_SECTION, EngineConfigKeyValue::REOUSRCE_CONFIG_PATH_KEY);
				optionConfigPath = engineConfig.GetValue(EngineConfigSection::CONFIG_PATH_SECTION, EngineConfigKeyValue::OPTION_CONFIG_PATH_KEY);

				return true;
			}

			return false;
		}

	public: // ==== [Get] ====
		/** @return 윈도우 타이틀 문자열 */
		inline const std::wstring& WindowTitle() const
		{
			return title;
		}

		/** @return 월드 비율 */
		inline float WorldRatio() const
		{
			return worldRatio;
		}
		/** @return 월드 역 비율 */
		inline float InverseWorldRatio() const
		{
			return inverseWorldRatio;
		}

		/** @return 역 바이트 값(1 / 255) */
		inline float InverseByte() const
		{
			return inverseByte;
		}

		/** @return 기본 게임 오브젝트 이름 */
		inline const std::wstring& DefaultGameObjectName() const
		{
			return defaultGameObjectName;
		}
		/** @return 메인 카메라 게임 오브젝트 이름 */
		inline const std::wstring& MainCameraGameObjectName() const
		{
			return mainCameraGameObjectName;
		}

		/** @return 리소스 Config 파일 경로 */
		inline const std::wstring& ResourceConfigPath() const
		{
			return resourceConfigPath;
		}
		/** @return 옵션 Config 파일 경로 */
		inline const std::wstring& OptionConfigPath() const
		{
			return optionConfigPath;
		}
	};
} // Engine
