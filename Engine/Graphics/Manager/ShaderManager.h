#pragma once

#include "Engine/Graphics/Type/ShaderType.h"
#include "Engine/Graphics/Resource/Shader/Shader.h"
#include "Engine/Graphics/Data/ShaderData.h"
#include "BufferManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			/**
			 * @brief 셰이더 매니저
			 * @date 2024-10-20
			 * @class
			 * 셰이더 파일들을 관리하는 클래스
			 */
			class ShaderManager
			{
			private: // ==== [Setup] ====
				// <셰이더 타입, 정점 셰이더 포인터>
				using VertexShaderTable = std::unordered_map<Type::ShaderType, Resource::VertexShader*>;
				// <셰이더 타입, 픽셀 셰이더 포인터>
				using PixelShaderTable = std::unordered_map<Type::ShaderType, Resource::PixelShader*>;
				// <셰이더 타입, 셰이더 포인터>
				using ShaderTable = std::unordered_map<Type::ShaderType, Resource::Shader*>;

			private: // ==== [셰이더 데이터] ====
				Data::ShaderDataGroupTable shaderDataGroupTable;	// 셰이더 데이터 그룹 테이블

			private: // ==== [셰이더] ====
				VertexShaderTable vertexShaders;	// 정점 셰이더 테이블
				PixelShaderTable pixelShaders;		// 픽셀 셰이더 테이블
				ShaderTable shaders;				// 셰이더 테이블

			public: // ==== [생성자 및 소멸자] ====
				ShaderManager();
				~ShaderManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 셰이더 매니저 초기화
				 * @date 2024-10-20
				 * @details
				 * 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param bufferManager : 버퍼 매니저 참조용 포인터
				 * @return 셰이더 매니저 초기화 성공 여부
				 */
				bool Initialize(ID3D11Device* device, BufferManager* bufferManager);
				/**
				 * @brief 셰이더 매니저 자원 해제
				 * @date 2024-10-20
				 * @details
				 * 셰이더 테이블 자원 해제
				 * 셰이더 데이터 그룹 테이블 자원 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/**
				 * @param shaderType : 셰이더 타입
				 * @return 셰이더 포인터
				 */
				Resource::Shader* GetShader(Type::ShaderType shaderType) const;

			private: // ==== [셰이더 생성] ====
				/**
				 * @brief 셰이더 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param bufferManager : 버퍼 매니저 참조용 포인터
				 * @return 메시지 처리 값
				 */
				HRESULT CreateShaders(ID3D11Device* device, BufferManager* bufferManager);
			};
		} // Manager
	} // Graphics
} // Engine