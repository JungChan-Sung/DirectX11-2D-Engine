#include "Sprite.h"
#include "Engine/Config/EngineConfig.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			Sprite::Sprite(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Texture* texture)
				: TextureRenderResource(vertexBuffer, indexBuffer, texture),
				pivot(0.5f, 0.5f), offset(0.0f, 0.0f), order(0)
			{
				float inverseWorldRatio = EngineConfig::Instance()->InverseWorldRatio();
				// 텍스처의 픽셀 값을 상대 크기 값으로 변환
				size = DirectX::XMFLOAT2(texture->GetWidth() * inverseWorldRatio, texture->GetHeight() * inverseWorldRatio);

				// 변환 행렬 갱신
				UpdateTransformMatrix();
			}

			Sprite::Sprite(Sprite&& moveSprite) noexcept
				: TextureRenderResource(std::move(moveSprite)),
				pivot(std::exchange(moveSprite.pivot, DirectX::XMFLOAT2(0.5f, 0.5f))),
				offset(std::exchange(moveSprite.offset, DirectX::XMFLOAT2(0.0f, 0.0f))),
				size(std::exchange(moveSprite.size, DirectX::XMFLOAT2(0.0f, 0.0f))),
				order(std::exchange(moveSprite.order, 0))
			{
			}

			Sprite::~Sprite()
			{
			}

			Sprite& XM_CALLCONV Sprite::operator=(Sprite&& rhs) noexcept
			{
				if (this != &rhs)
				{
					// 렌더 리소스
					vertexBuffer = std::exchange(rhs.vertexBuffer, nullptr);
					indexBuffer = std::exchange(rhs.indexBuffer, nullptr);
					DirectX::XMFLOAT4X4 identityMatrix;
					DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
					transformMatrix = std::exchange(rhs.transformMatrix, identityMatrix);

					// 텍스처 렌더 리소스
					texture = std::exchange(rhs.texture, nullptr);

					// 스프라이트
					pivot = std::exchange(rhs.pivot, DirectX::XMFLOAT2(0.5f, 0.5f));
					offset = std::exchange(rhs.offset, DirectX::XMFLOAT2(0.0f, 0.0f));
					size = std::exchange(rhs.size, DirectX::XMFLOAT2(0.0f, 0.0f));
					order = std::exchange(rhs.order, 0);
				}

				return *this;
			}

			void Sprite::SetPivot(BitFlag pivotBitFlag)
			{
				float pivotX = 0.5f;
				float pivotY = 0.5f;

				if (pivotBitFlag & SpritePivotBit::LEFT)
					pivotX = 0.0f;
				else if (pivotBitFlag & SpritePivotBit::RIGHT)
					pivotX = 1.0f;

#if SCREEN_CROODINATE // 화면 좌표계
				if (pivotBitFlag & SpritePivotBit::BOTTOM)
					pivotY = 1.0f;
				else if (pivotBitFlag & SpritePivotBit::TOP)
					pivotY = 0.0f;
#else // 데카르트 좌표계
				if (pivotBitFlag & SpritePivotBit::BOTTOM)
					pivotY = 0.0f;
				else if (pivotBitFlag & SpritePivotBit::TOP)
					pivotY = 1.0f;
#endif

				pivot = DirectX::XMFLOAT2(pivotX, pivotY);

				// 변환 행렬 갱신
				UpdateTransformMatrix();
			}

			void Sprite::SetOffset(const DirectX::XMFLOAT2& offsetValue)
			{
				offset = offsetValue;

				// 변환 행렬 갱신
				UpdateTransformMatrix();
			}

			void XM_CALLCONV Sprite::SetOffset(DirectX::FXMVECTOR offsetVector)
			{
				DirectX::XMStoreFloat2(&offset, offsetVector);

				// 변환 행렬 갱신
				UpdateTransformMatrix();
			}

			void Sprite::SetOrder(int order)
			{
				this->order = order;
			}

			DirectX::XMFLOAT2 Sprite::GetPivot() const
			{
				return pivot;
			}

			DirectX::XMVECTOR XM_CALLCONV Sprite::GetPivotVector() const
			{
				return DirectX::XMLoadFloat2(&pivot);
			}

			DirectX::XMFLOAT2 Sprite::GetOffset() const
			{
				return offset;
			}

			DirectX::XMVECTOR XM_CALLCONV Sprite::GetOffsetVector() const
			{
				return DirectX::XMLoadFloat2(&offset);
			}

			DirectX::XMFLOAT2 Sprite::GetSize() const
			{
				return size;
			}

			DirectX::XMVECTOR XM_CALLCONV Sprite::GetSizeVector() const
			{
				return DirectX::XMLoadFloat2(&size);
			}

			int Sprite::GetOrder() const
			{
				return order;
			}

			void XM_CALLCONV Sprite::UpdateTransformMatrix()
			{
				// 피벗과 크기로 상대 위치 설정정
				DirectX::XMMATRIX offsetMatrix = DirectX::XMMatrixTranslation(offset.x - (pivot.x * size.x), offset.y - (pivot.y * size.y), 0.0f);
				DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(size.x, size.y, 0.0f);

				// 변환 행렬 갱신
				DirectX::XMStoreFloat4x4(&transformMatrix, scaleMatrix * offsetMatrix);
			}
		} // Resource
	} // Graphics
} // Engine
