#pragma once

#include "RenderResource.h"
#include "Engine/Graphics/Resource/Texture/Texture.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 텍스처 렌더 리소스 클래스 : 렌더 리소스 상속
			 * @date 2024-10-14
			 * @class
			 * 텍스처 기반 렌더 리소스 클래스
			 */
			class TextureRenderResource : public RenderResource
			{
			protected: // ==== [리소스] ====
				Texture* texture; // 텍스처 참조용 포인터

			public: // ==== [생성자 및 소멸자] ====
				/**
				 * @param vertexBuffer : 정점 버퍼 참조용 포인터
				 * @param indexBuffer : 인덱스 버퍼 참조용 포인터
				 * @param texture : 텍스처 참조용 포인터
				 */
				TextureRenderResource(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Texture* texture);
				/** @param moveTextureRenderResource : 이동 생성할 텍스처 렌더 리소스 */
				TextureRenderResource(TextureRenderResource&& moveTextureRenderResource) noexcept;
				virtual ~TextureRenderResource();

			private: // ==== [생성자] ====
				/** @deprecated */
				TextureRenderResource() = delete;
				TextureRenderResource(const TextureRenderResource& copyTextureRenderResource) = delete;

			public: // ==== [연산자 재정의] ====
				TextureRenderResource& XM_CALLCONV operator=(TextureRenderResource&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				TextureRenderResource& operator=(const TextureRenderResource& rhs) = delete;

			public: // ==== [Setup(렌더 리소스) 재정의] ====
				/**
				 * @brief 텍스처 렌더 리소스 자원 해제
				 * @details
				 * 텍스처 참조 해제
				 */
				virtual void Release() override;

			public: // ==== [Get] ====
				/** @return 텍스처 포인터 */
				Texture* GetTexture() const;
			};
		} // Resource
	} // Graphics
} // Engine