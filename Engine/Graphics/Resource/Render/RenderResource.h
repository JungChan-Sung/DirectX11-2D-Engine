#pragma once

#include "Engine/Graphics/Resource/Buffer/VertexBuffer.h"
#include "Engine/Graphics/Resource/Buffer/IndexBuffer.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 렌더 리소스 추상화 클래스
			 * @date 2024-10-14
			 * @class
			 * 렌더링에 필요한 리소스들을 지닌 렌더 리소스 기반 클래스
			 */
			class RenderResource abstract
			{
			protected: // ==== [리소스] ====
				VertexBuffer* vertexBuffer;	// 정점 버퍼 참조용 포인터
				IndexBuffer* indexBuffer;	// 인덱스 참조용 버퍼

			protected: // ==== [렌더 정보] ====
				DirectX::XMFLOAT4X4 transformMatrix;	// 변환 행렬

			public: // ==== [생성자와 소멸자] ====
				/**
				 * @param vertexBuffer : 정점 버퍼 참조용 포인터
				 * @param indexBuffer : 인덱스 버퍼 참조용 포인터
				 */
				RenderResource(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
				/** @para moveRenderResource : 이동 생성할 렌더 리소스 */
				RenderResource(RenderResource&& moveRenderResource) noexcept;
				virtual ~RenderResource();

			private: // ==== [생성자] ====
				/** @deprecated */
				RenderResource() = delete;
				RenderResource(const RenderResource& copyRenderResource) = delete;

			public: // ==== [연산자 재정의] ====
				RenderResource& XM_CALLCONV operator=(RenderResource&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				RenderResource& operator=(const RenderResource& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 렌더 리소스 자원 해제
				 * @details
				 * 참조한 버퍼들 참조 해제
				 */
				virtual void Release();

			public: // ==== [Set] ====
				/** @param transformMatrix : 변환 행렬 */
				void XM_CALLCONV SetTransformMatrix(DirectX::FXMMATRIX transformMatrix);

			public: // ==== [Get] ====
				/** @return 정점 버퍼 참조용 포인터 */
				VertexBuffer* GetVertexBuffer() const;
				/** @return 인덱스 버퍼 참조용 포인터 */
				IndexBuffer* GetIndexBuffer() const;

				/** @return 변환 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetTransformMatrix() const;
			};
		} // Resource
	} // Graphics
} // Engine
