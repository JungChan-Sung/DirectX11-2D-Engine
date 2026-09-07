#pragma once

#include <unordered_map>
#include "Engine/Graphics/Type/BufferType.h"
#include "Engine/Graphics/Resource/Buffer/VertexBuffer.h"
#include "Engine/Graphics/Resource/Buffer/IndexBuffer.h"
#include "Engine/Graphics/Resource/Buffer/BaseConstantBuffer.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			/**
			 * @brief 버퍼 매니저 클래스
			 * @date 2024-10-18
			 * @class
			 * GPU에서 사용되는 정점, 인덱스, 상수 버퍼들을 관리하는 클래스
			 */
			class BufferManager
			{
			private: // ==== [선언] ====
				// <렌더 리소스 버퍼 타입, 정점 버퍼 포인터>
				using VertexBufferTable = std::unordered_map<Type::RenderResourceBufferType, Resource::VertexBuffer*>;
				// <렌더 리소스 버퍼 타입, 인덱스 버퍼 포인터>
				using IndexBufferTable = std::unordered_map<Type::RenderResourceBufferType, Resource::IndexBuffer*>;
				// <정점 셰이더 상수 버퍼 타입, 상수 버퍼 기반 포인터>
				using VertexShaderConstantBufferTable = std::unordered_map<Type::VertexShaderConstantBufferType, Resource::BaseConstantBuffer*>;
				// <픽셀 셰이더 상수 버퍼 타입, 상수 버퍼 기반 포인터>
				using PixelShaderConstantBufferTable = std::unordered_map<Type::PixelShaderConstantBufferType, Resource::BaseConstantBuffer*>;

			private: // ==== [버퍼 리소스] ====
				VertexBufferTable vertexBuffers;	// 정점 버퍼 테이블
				IndexBufferTable indexBuffers;		// 인덱스 버퍼 테이블

			private: // ==== [셰이더 상수 버퍼 리소스] ====
				VertexShaderConstantBufferTable vertexShaderConstantBuffers;	// 정점 셰이더 상수 버퍼 테이블
				PixelShaderConstantBufferTable pixelShaderConstantBuffers;		// 픽셀 셰이더 상수 버퍼 테이블

			public: // ==== [생성자 및 소멸자] ====
				BufferManager();
				~BufferManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 버퍼 매니저 초기화
				 * @details
				 * 정점 버퍼, 인덱스 버퍼, 상수 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 버퍼 매니저 초기화 성공 여부
				 */
				bool Initialize(ID3D11Device* device);
				/**
				 * @brief 버퍼 매니저 자원 해제
				 * @details
				 * 정점 버퍼, 인덱스 버퍼, 상수 버퍼 자원 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/**
				 * @param renderResourceBufferType : 렌더 리소스 버퍼 타입
				 * @return 정점 버퍼 포인터
				 */
				Resource::VertexBuffer* GetVertexBuffer(Type::RenderResourceBufferType renderResourceBufferType) const;
				/**
				 * @param renderResourceBufferType : 렌더 리소스 버퍼 타입
				 * @return 인덱스 버퍼 포인터
				 */
				Resource::IndexBuffer* GetIndexBuffer(Type::RenderResourceBufferType renderResourceBufferType) const;
				/**
				 * @param vertexShaderConstantBufferType : 정점 셰이더 상수 버퍼 타입
				 * @return 상수 버퍼 포인터
				 */
				Resource::BaseConstantBuffer* GetVertexShaderConstantBuffer(Type::VertexShaderConstantBufferType vertexShaderConstantBufferType) const;
				/**
				 * @param pixelShaderConstantBufferType : 픽셀 셰이더 상수 버퍼 타입
				 * @return 상수 버퍼 포인터
				 */
				Resource::BaseConstantBuffer* GetPixelShaderConstantBuffer(Type::PixelShaderConstantBufferType pixelShaderConstantBufferType) const;

			private: // ==== [버퍼 생성] ====
				/**
				 * @brief 정점 버퍼 생성
				 * @date 2024-10-18
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 메시지 처리 값
				 */
				HRESULT CreateVertexBuffers(ID3D11Device* device);
				/**
				 * @brief 인덱스 버퍼 생성
				 * @date 2024-10-18
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 메시지 처리 값
				 */
				HRESULT CreateIndexBuffers(ID3D11Device* device);
				/**
				 * @brief 정점 셰이더 상수 버퍼 생성
				 * @date 2024-10-18
				 * @param device : DirectX 장치11 참조용 포인터
				 * @return 메시지 처리 값
				 */
				HRESULT CreateVertexShaderConstantBuffers(ID3D11Device* device);
				/**
				 * @brief 픽셀 셰이더 상수 버퍼 생성
				 * @date 2024-10-18
				 * @param device : DirectX 장치11 참조용 포인터
				 * @return 메시지 처리 값
				 */
				HRESULT CreatePixelShaderConstantBuffers(ID3D11Device* device);
			};
		} // Manager
	} // Graphics
} // Engine
