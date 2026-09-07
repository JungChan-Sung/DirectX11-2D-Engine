/*********************************************************************
 * @file   SpriteData.h
 * @brief  스프라이트 데이터 구조체 정의
 * @date   2024-10-20
 *********************************************************************/

#pragma once

#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "Engine/Graphics/Type/BufferType.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Data
		{
			/**
			 * @brief 스프라이트 데이터
			 * @date 2024-10-20
			 */
			struct SpriteData
			{
			public: // ==== [구성 요소] ====
				Type::RenderResourceBufferType vertexBufferType;	// 정점 버퍼 타입
				Type::RenderResourceBufferType indexBufferType;		// 인덱스 버퍼 타입
				std::string textureGroupName;						// 텍스처 그룹 이름
				std::string textureName;							// 텍스처 이름
				UINT pivot;											// 스프라이트 피벗
				DirectX::XMFLOAT2 offset;							// 스프라이트 상대 위치
				int order;											// 스프라이트 렌더 순서

			public: // ==== [생성자] ====
				SpriteData() : vertexBufferType(Type::RenderResourceBufferType::SPRITE), indexBufferType(Type::RenderResourceBufferType::SPRITE),
					pivot(0u), offset(0.0f, 0.0f), order(0)
				{
				}
			};

			// <스프라이트 이름, 스프라이트 데이터 포인터>
			using SpriteDataTable = std::unordered_map<std::string, Data::SpriteData*>;
			// <스프라이트 그룹 이름, 스프라이트 데이터 테이블>
			using SpriteDataGroupTable = std::unordered_map<std::string, SpriteDataTable>;
		} // Data
	} // Graphics
} // Engine