#include "SpriteRenderer.h"
#include "Engine/Graphics/Resource/Buffer/ConstantBuffer.h"
#include "Engine/Graphics/Common/RenderMatrix.h"
#include "Engine/GameObject/GameObject.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			SpriteRenderer::SpriteRenderer(GameObject* owner) : Renderer(owner),
				sprite(nullptr), shader(nullptr)
			{
				owner->AddComponentNotify(this);
			}

			SpriteRenderer::~SpriteRenderer()
			{
				Release();
			}

			void SpriteRenderer::Release()
			{
				owner->RemoveComponentNotify(this);

				shader = nullptr;
				sprite = nullptr;
			}

			void SpriteRenderer::SetSprite(Graphics::Resource::Sprite* sprite)
			{
				this->sprite = sprite;
			}

			void SpriteRenderer::SetShader(Graphics::Resource::Shader* shader)
			{
				this->shader = shader;
			}

			Graphics::Resource::Sprite* SpriteRenderer::GetSprite() const
			{
				return sprite;
			}

			Graphics::Resource::Shader* SpriteRenderer::GetShader() const
			{
				return shader;
			}

			void XM_CALLCONV SpriteRenderer::Operate(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix)
			{
				// ==== [입력 어셈블러] ====
				// 입력 어셈블러 정점 버퍼 설정
				UINT offset = 0u;
				Graphics::Resource::VertexBuffer* vertexBuffer = sprite->GetVertexBuffer();
				deviceContext->IASetVertexBuffers(0u, 1u, vertexBuffer->GetAddressOf(), vertexBuffer->GetStridePtr(), &offset);
				// 입력 어셈블러 인덱스 버퍼 설정(포맷은 인덱스 자료형이 32비트 부호없는 정수 값으로 설정)
				Graphics::Resource::IndexBuffer* indexBuffer = sprite->GetIndexBuffer();
				deviceContext->IASetIndexBuffer(indexBuffer->Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0u);
				// 입력 어셈블러 입력 레이아웃 설정
				Graphics::Resource::VertexShader* vertexShader = shader->GetVertexShader();
				deviceContext->IASetInputLayout(vertexShader->GetInputLayout());
				// 입력 어셈블러 프리미티브 유형 설정
				deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				// =========================

				// ==== [셰이더] ====
				// 정점 셰이더 바인딩
				deviceContext->VSSetShader(vertexShader->Get(), nullptr, 0u);
				// 픽셀 셰이더 바인딩
				deviceContext->PSSetShader(shader->GetPixelShader()->Get(), nullptr, 0u);
				// 정점 셰이더 상수 버퍼 설정
				Graphics::Resource::ConstantBuffer<Graphics::Common::RenderMatrix>* vertexShaderConstantBuffer =
					static_cast<Graphics::Resource::ConstantBuffer<Graphics::Common::RenderMatrix>*>(vertexShader->GetVertexShaderConstantBuffer());
				Graphics::Common::RenderMatrix renderMarix;
				DirectX::XMStoreFloat4x4(&renderMarix.world, DirectX::XMMatrixTranspose(sprite->GetTransformMatrix() * owner->GetTransform()->GetWorldMatrix()));
				DirectX::XMStoreFloat4x4(&renderMarix.viewProjection, DirectX::XMMatrixTranspose(viewProjectionMatrix));
				vertexShaderConstantBuffer->SetData(renderMarix);
				vertexShaderConstantBuffer->ApplyChanges(deviceContext);
				deviceContext->VSSetConstantBuffers(0u, 1u, vertexShaderConstantBuffer->GetAddressOf());
				// 픽셀 셰이더 텍스처 리소스 뷰 설정
				Graphics::Resource::Texture* texture = sprite->GetTexture();
				deviceContext->PSSetShaderResources(0u, 1u, texture->GetTextureResourceViewAddress());
				// 샘플러 설정
				deviceContext->PSSetSamplers(0u, 1u, texture->GetSamplerState());
				// ==================

				// ==== [드로우 콜] ====
				// 인덱스 버퍼의 정보로 그리기 수행
				deviceContext->DrawIndexed(indexBuffer->GetIndexCount(), 0u, 0);
				// =====================
			}
		} // Component
	} // GameObject
} // Engine