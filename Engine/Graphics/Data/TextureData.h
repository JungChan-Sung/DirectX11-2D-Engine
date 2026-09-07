/*********************************************************************
 * @file   TextureData.h
 * @brief  텍스처 데이터 구조체 정의
 * @date   2024-10-20
 *********************************************************************/

#pragma once

#include <dxgiformat.h>
#include <string>
#include <unordered_map>
#include "Engine/Graphics/Type/SamplerStateType.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Data
		{
			/**
			 * @brief 텍스처 데이터
			 * @date 2024-10-20
			 */
			struct TextureData
			{
			public: // ==== [구성 요소] ====
				std::string path;							// 텍스처 경로
				DXGI_FORMAT format;							// 텍스처 포맷
				Type::SamplerStateType samplerStateType;	// 텍스처 샘플러 상태 타입

			public: // ==== [생성자] ====
				TextureData() : format(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM), samplerStateType(Type::SamplerStateType::TEXTURE_2D)
				{
				}
			};

			// <텍스처 이름, 텍스처 데이터 포인터>
			using TextureDataTable = std::unordered_map<std::string, Data::TextureData*>;
			// <텍스처 그룹 이름, 텍스처 데이터 테이블>
			using TextureDataGroupTable = std::unordered_map<std::string, TextureDataTable>;
		} // Data
	} // Graphics
} // Engine