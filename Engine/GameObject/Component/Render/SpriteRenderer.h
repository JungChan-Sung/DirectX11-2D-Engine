#pragma once

#include "Engine/GameObject/Component/Render/Renderer.h"
#include "Engine/Graphics/Resource/Render/Sprite.h"
#include "Engine/Graphics/Resource/Shader/Shader.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			/**
			 * @brief 스프라이트 렌더러 클래스 : 렌더러 상속
			 * @date 2024-10-21
			 * @class
			 * 스프라이트 리소스로 렌더링을 수행해줌
			 */
			class SpriteRenderer : public Renderer
			{
			private: // ==== [리소스] ====
				Graphics::Resource::Sprite* sprite;	// 스프라이트 참조용 포인터
				Graphics::Resource::Shader* shader;	// 셰이더 참조용 포인터

			public: // ==== [생성자 및 소멸자] ====
				/** @param owner : 컴포넌트 소유자의 게임 오브젝트 참조용 포인터 */
				SpriteRenderer(GameObject* owner);
				virtual ~SpriteRenderer();

			public: // ==== [Setup(컴포넌트) 재정의] ====
				/**
				 * @brief 스프라이트 렌더러 자원 해제
				 * @date 2024-10-16
				 * @details
				 * 스프라이트 및 셰이더 참조 해제
				 */
				virtual void Release() override;

			public: // ==== [Set] ====
				/** @param sprite : 스프라이트 참조용 포인터 */
				void SetSprite(Graphics::Resource::Sprite* sprite);
				/** @param shader : 셰이더 참조용 포인터 */
				void SetShader(Graphics::Resource::Shader* shader);

			public: // ==== [Get] ====
				/** @return 스프라이트 참조용 포인터 */
				Graphics::Resource::Sprite* GetSprite() const;
				/** @return 셰이더 참조용 포인터 */
				Graphics::Resource::Shader* GetShader() const;

			private: // ==== [작업(렌더러) 재정의] ====
				/**
				 * @brief 렌더러 작업 수행
				 * @param deviceContext : DirectX11 장치 내용 참조용 포인터
				 * @param viewProjectionMatrix : 렌더링될 뷰 * 투영 행렬
				 */
				virtual void XM_CALLCONV Operate(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix) override;
			};
		} // Component
	} // GameObject
} // Engine
