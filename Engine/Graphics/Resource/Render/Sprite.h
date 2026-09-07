#pragma once

#include "TextureRenderResource.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			// 비트 플래그 자료형
			using BitFlag = unsigned int;

			// 스프라이트 피벗 비트
			enum SpritePivotBit : BitFlag
			{
				CENTER = 0u,		// 중앙
				LEFT = 1u << 1u,	// 좌
				RIGHT = 1u << 2u,	// 우
				TOP = 1u << 3u,		// 상
				BOTTOM = 1u << 4u	// 하
			};

			/**
			 * @brief 스프라이트 클래스 : 텍스처 렌더 리소스 상속
			 * @date 2024-10-16
			 * @class
			 * 스프라이트 리소스의 정보를 지님
			 */
			class Sprite : public TextureRenderResource
			{
			private: // ==== [스프라이트 정보] ====
				DirectX::XMFLOAT2 pivot;	// 렌더 피벗(렌더될 스프라이트 중심점 위치 비율)
				DirectX::XMFLOAT2 offset;	// 렌더될 상대 위치(피벗을 중심으로)
				DirectX::XMFLOAT2 size;		// 렌더될 상대 크기
				int order;					// 렌더될 순서(낮을수록 빠름)

			public: // ==== [생성자 및 소멸자] ====
				/**
				 * @param vertexBuffer : 정점 버퍼 참조용 포인터
				 * @param indexBuffer : 인덱스 버퍼 참조용 포인터
				 * @param texture : 텍스처 참조용 포인터
				 */
				Sprite(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Texture* texture);
				/** @param moveSprite : 이동 생성할 스프라이트 */
				Sprite(Sprite&& moveSprite) noexcept;
				virtual ~Sprite();

			private: // ==== [생성자] ====
				/** @deprecated */
				Sprite() = delete;
				Sprite(const Sprite& copySprite) = delete;

			public: // ==== [연산자 재정의] ====
				Sprite& XM_CALLCONV operator=(Sprite&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				Sprite& operator=(const Sprite& rhs) = delete;

			public: // ==== [Set] ====
				/**
				 * @param pivotBitFlag : 피벗 비트 플래그
				 * @example
				 * (SpritePivotBit::LEFT | SpritePivotBit::TOP)
				 */
				void SetPivot(BitFlag pivotBitFlag);

				/** @param offsetValue : 렌더될 상대 위치 값 */
				void SetOffset(const DirectX::XMFLOAT2& offsetValue);
				/** @param offsetVector : 렌더될 상대 위치 벡터 */
				void XM_CALLCONV SetOffset(DirectX::FXMVECTOR offsetVector);

				/** @param order : 렌더될 순서 */
				void SetOrder(int order);

			public: // ==== [Get] ====
				/** @return 렌더될 스프라이트 중심점 위치 비율 값 */
				DirectX::XMFLOAT2 GetPivot() const;
				/** @return 렌더될 스프라이트 중심점 위치 비율 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetPivotVector() const;

				/** @return 렌더될 상대 위치 값 */
				DirectX::XMFLOAT2 GetOffset() const;
				/** @return 렌더될 상대 위치 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetOffsetVector() const;

				/** @return 렌더될 상대 크기 값 */
				DirectX::XMFLOAT2 GetSize() const;
				/** @return 렌더될 상대 크기 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetSizeVector() const;

				/** @return 렌더될 순서 */
				int GetOrder() const;

			private: // ==== [행렬 갱신] ====
				/**
				 * @brief 변환 행렬 갱신
				 * @date 2024-10-16
				 * @details
				 * 상대 위치, 상대 크기, 피벗을 계산하여
				 * 렌더링될 변환 행렬 갱신
				 */
				void XM_CALLCONV UpdateTransformMatrix();
			};
		} // Resource
	} // Graphics
} // Engine
