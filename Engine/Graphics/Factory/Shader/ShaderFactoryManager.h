#pragma once

#include <unordered_map>
#include "Engine/Graphics/Type/ShaderType.h"
#include "ShaderFactoryHeaders.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 셰이더 팩토리 매니저
			 * @date 2024-10-18
			 * @class
			 * 셰이더 팩토리들을 관리하는 클래스
			 */
			class ShaderFactoryManager
			{
			private: // ==== [선언] ====
				// <셰이더 타입, 정점 셰이더 포인터>
				using VertexShaderFactoryTable = std::unordered_map<Type::ShaderType, VertexShaderFactory*>;
				// <셰이더 타입, 픽셀 셰이더 포인터>
				using PixelShaderFactoryTable = std::unordered_map<Type::ShaderType, PixelShaderFactory*>;

			private: // ==== [구성 요소] ====
				VertexShaderFactoryTable vertexShaderFactories; // 정점 셰이더 팩토리 테이블
				PixelShaderFactoryTable pixelShaderFactories;	// 픽셀 셰이더 팩토리 테이블

			public: // ==== [생성자 및 소멸자] ====
				ShaderFactoryManager();
				~ShaderFactoryManager();

			public: // ==== [Setup] ====
				void Release();

			public: // ==== [셰이더 생성] ====
				/**
				 * @brief 정점 셰이더 생성
				 * @details
				 * ShaderType에 맞는 정점 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param fileName : 정점 셰이더 경로
				 * @param vertexShaderConstantBuffer : 정점 셰이더 상수 버퍼 참조용 포인터
				 * @param shaderType : 셰이더 타입
				 * @return 생성된 정점 셰이더 포인터
				 */
				Resource::VertexShader* CreateVertexShader(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup, Type::ShaderType shaderType);
				/**
				 * @brief 픽셀 셰이더 생성
				 * @details
				 * ShaderType에 맞는 픽셀 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param fileName : 픽셀 셰이더 경로
				 * @param pixelShaderConstantBuffer : 픽셀 셰이더 상수 버퍼 참조용 포인터
				 * @param shaderType : 셰이더 타입
				 * @return 생성된 픽셀 셰이더 포인터
				 */
				Resource::PixelShader* CreatePixelShader(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup, Type::ShaderType shaderType);
			};
		} // Factory
	} // Graphics
} // Engine
