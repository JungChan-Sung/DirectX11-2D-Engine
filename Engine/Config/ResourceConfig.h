/*********************************************************************
 * @file   ResourceConfig.h
 * @brief  리소스 설정 값
 * @date   2024-11-06
 *********************************************************************/

#pragma once

#include "CommonUtility/Utility/StringUtility.h"
#include "Engine/Config/EngineConfig.h"

namespace Engine
{
	namespace ResourceConfigSection // ==== [리소스 섹션 값] ====
	{
		constexpr static const wchar_t* RESOURCE_DATA_FILE_PATH = L"ResourceDataFilePath";
		constexpr static const wchar_t* SHADER_COMPILE_SECTION = L"ShaderCompile";
	} // ResourceConfigSection

	namespace ResourceConfigKeyValue // ==== [리소스 키-값] ====
	{
		constexpr static const wchar_t* SHADER_DATA_FILE_PATH_KEY = L"shaderDataFilePath";
		constexpr static const wchar_t* TEXTURE_DATA_FILE_PATH_KEY = L"textureDataFilePath";
		constexpr static const wchar_t* SPRITE_DATA_FILE_PATH_KEY = L"spriteDataFilePath";
		constexpr static const wchar_t* VERTEX_SHADER_VERSION_KEY = L"vertexShaderVersion";
		constexpr static const wchar_t* PIXEL_SHADER_VERSION_KEY = L"pixelShaderVersion";
		constexpr static const wchar_t* SHADER_MAIN_FUNCTION_KEY = L"shaderMainFunction";
	} // ResourceConfigKeyValue

	/**
	 * @brief 리소스 설정 값 클래스
	 * @date 2024-11-06
	 * @class
	 * Resource Config 파일을 불러와 설정 값 저장하는 클래스
	 */
	class ResourceConfig : public CommonUtility::Singleton<ResourceConfig>
	{
	private: // ==== [리소스 데이터 파일 경로 섹션 값] ====
		std::string shaderDataFilePath;		// 셰이더 데이터 파일 경로
		std::string textureDataFilePath;	// 텍스처 데이터 파일 경로
		std::string spriteDataFilePath;		// 스프라이트 데이터 파일 경로

	private: // ==== [셰이더 컴파일 섹션 값] ====
		std::string vertexShaderVersion;	// 정점 셰이더 버전
		std::string pixelShaderVersion;		// 픽셀 셰이더 버전
		std::string shaderMainFunction;		// 셰이더 메인 함수

	public: // ==== [Setup] ====
		/**
		 * @brief 리소스 설정 값 초기화
		 * @date 2024-11-06
		 * @details
		 * 리소스 Config 파일을 불러와 설정에 사용할 값들을 불러옴
		 * @return 리소스 설정 값 초기화 성공 여부(true : 파일 읽기 성공, false : 파일 읽기 실패)
		 */
		bool Initialize()
		{
			CommonUtility::ConfigParser resourceConfig;

			if (resourceConfig.Parse(EngineConfig::Instance()->ResourceConfigPath())) // resourceConfig.Parse == true
			{
				// 리소스 데이터 파일 경로 섹션 값
				shaderDataFilePath = CommonUtility::StringUtility::ConvertToString(resourceConfig.GetValue(ResourceConfigSection::RESOURCE_DATA_FILE_PATH,
					ResourceConfigKeyValue::SHADER_DATA_FILE_PATH_KEY));
				textureDataFilePath = CommonUtility::StringUtility::ConvertToString(resourceConfig.GetValue(ResourceConfigSection::RESOURCE_DATA_FILE_PATH,
					ResourceConfigKeyValue::TEXTURE_DATA_FILE_PATH_KEY));
				spriteDataFilePath = CommonUtility::StringUtility::ConvertToString(resourceConfig.GetValue(ResourceConfigSection::RESOURCE_DATA_FILE_PATH,
					ResourceConfigKeyValue::SPRITE_DATA_FILE_PATH_KEY));

				// 셰이더 컴파일 섹션
				vertexShaderVersion = CommonUtility::StringUtility::ConvertToString(resourceConfig.GetValue(ResourceConfigSection::SHADER_COMPILE_SECTION,
					ResourceConfigKeyValue::VERTEX_SHADER_VERSION_KEY));
				pixelShaderVersion = CommonUtility::StringUtility::ConvertToString(resourceConfig.GetValue(ResourceConfigSection::SHADER_COMPILE_SECTION,
					ResourceConfigKeyValue::PIXEL_SHADER_VERSION_KEY));
				shaderMainFunction = CommonUtility::StringUtility::ConvertToString(resourceConfig.GetValue(ResourceConfigSection::SHADER_COMPILE_SECTION,
					ResourceConfigKeyValue::SHADER_MAIN_FUNCTION_KEY));

				return true;
			}

			return false;
		}

	public: // ==== [Get] ====
		/** @return 셰이더 데이터 파일 경로 문자열 */
		inline const std::string& ShaderDataFilePath() const
		{
			return shaderDataFilePath;
		}
		/** @return 텍스처 데이터 파일 경로 문자열 */
		inline const std::string& TextureDataFilePath() const
		{
			return textureDataFilePath;
		}
		/** @return 스프라이트 데이터 파일 경로 문자열 */
		inline const std::string& SpriteDataFilePath() const
		{
			return spriteDataFilePath;
		}

		/** @return 정점 셰이더 버전 문자열 */
		inline const std::string& VertexShaderVersion() const
		{
			return vertexShaderVersion;
		}
		/** @return 픽셀 셰이더 버전 문자열 */
		inline const std::string& PixelShaderVersion() const
		{
			return pixelShaderVersion;
		}
		/** @return 셰이더 메인 함수 문자열 */
		inline const std::string& ShaderMainFunction() const
		{
			return shaderMainFunction;
		}
	};
} // Engine
