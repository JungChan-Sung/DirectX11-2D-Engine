#pragma once

#include "Engine/Graphics/Data/ShaderData.h"
#include "Engine/Graphics/Resource/Shader/VertexShader.h"
#include "Engine/Graphics/Manager/BufferManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 정점 셰이더 팩토리 추상화 클래스
			 * @date 2024-10-18
			 * @class
			 * 정점 셰이더를 생성하는 클래스
			 */
			class VertexShaderFactory abstract
			{
			public: // ==== [정점 셰이더 생성] ====
				/**
				 * @brief 정점 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param bufferManager : 버퍼 매니저 참조용 포인터
				 * @param shaderData : 셰이더 데이터 Json
				 * @return 생성된 정점 셰이더 포인터
				 */
				virtual Resource::VertexShader* Create(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup) abstract;
			};
		} // Factory
	} // Graphics
} // Engine
