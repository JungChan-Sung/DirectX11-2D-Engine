#pragma once

#include "VertexShaderFactory.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 스프라이트 정점 셰이더 팩토리 클래스 : 정점 셰이더 팩토리 상속
			 * @date 2024-10-18
			 * @class
			 * 스프라이트용 정점 셰이더 생성하는 팩토리 클래스
			 */
			class SpriteVertexShaderFactory : public VertexShaderFactory
			{
			public: // ==== [정점 셰이더 생성(정점 셰이더 팩토리) 재정의] ====
				/**
				 * @brief 정점 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param bufferManager : 버퍼 매니저 참조용 포인터
				 * @param shaderData : 셰이더 데이터 Json
				 * @return 생성된 정점 셰이더 포인터
				 */
				virtual Resource::VertexShader* Create(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup) override;
			};
		} // Factory
	} // Graphics
} // Engine
