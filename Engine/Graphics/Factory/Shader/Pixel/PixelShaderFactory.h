#pragma once

#include "Engine/Graphics/Data/ShaderData.h"
#include "Engine/Graphics/Resource/Shader/PixelShader.h"
#include "Engine/Graphics/Manager/BufferManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 픽셀 셰이더 팩토리 추상화 클래스
			 * @date 2024-10-18
			 * @class
			 * 픽셀 셰이더를 생성하는 클래스
			 */
			class PixelShaderFactory abstract
			{
			public: // ==== [픽셀 셰이더 생성] ====
				/**
				 * @brief 픽셀 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param bufferManager : 버퍼 매니저 참조용 포인터
				 * @param shaderData : 셰이더 데이터 Json
				 * @return 생성된 픽셀 셰이더 포인터
				 */
				virtual Resource::PixelShader* Create(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup) abstract;
			};
		} // Factory
	} // Graphics
} // Engine
