/*********************************************************************
 * @file   RenderMatrix.h
 * @brief  상수 버퍼용 렌더 행렬
 * @date   2024-10-09
 *********************************************************************/

#pragma once

#include <DirectXMath.h>
// std::move
#include <utility>

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			struct RenderMatrix
			{
			public: // ==== [구성 요소] ====
				DirectX::XMFLOAT4X4 world;			// 월드 좌표계 행렬
				DirectX::XMFLOAT4X4 viewProjection; // 카메라 뷰 * 투영 행렬

			public: // ==== [연산자 재정의] ====
				RenderMatrix& operator=(const RenderMatrix& rhs)
				{
					if (this == &rhs)
						return *this;

					world = rhs.world;
					viewProjection = rhs.viewProjection;

					return *this;
				}
				RenderMatrix& XM_CALLCONV operator=(RenderMatrix&& rhs) noexcept
				{
					if (this != &rhs)
					{
						DirectX::XMFLOAT4X4 identityMatrix;
						DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
						world = std::exchange(rhs.world, identityMatrix);
						viewProjection = std::exchange(rhs.viewProjection, identityMatrix);
					}

					return *this;
				}

			public: // ==== [생성자] ====
				RenderMatrix()
				{
					DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
					DirectX::XMStoreFloat4x4(&viewProjection, DirectX::XMMatrixIdentity());
				}
				/** @param copyRenderMatrix : 복사 생성할 렌더 행렬 */
				RenderMatrix(const RenderMatrix& copyRenderMatrix) : world(copyRenderMatrix.world), viewProjection(copyRenderMatrix.viewProjection)
				{
				}
				/** @param moveRenderMatrix : 이동 생성할 렌더 행렬 */
				RenderMatrix(RenderMatrix&& moveRenderMatrix) noexcept 
					: world(std::move(moveRenderMatrix.world)),
					viewProjection(std::move(moveRenderMatrix.viewProjection))
				{
					DirectX::XMStoreFloat4x4(&moveRenderMatrix.world, DirectX::XMMatrixIdentity());
					DirectX::XMStoreFloat4x4(&moveRenderMatrix.viewProjection, DirectX::XMMatrixIdentity());
				}
			};
		} // Common
	} // Graphics
} // Engine