#pragma once

#include <vector>
#include "Engine/GameObject/Component/Render/SpriteRenderer.h"
#include "Engine/GameObject/Observer/IComponentObserver.h"
#include "Engine/Manager/Observer/ICreateGameObjectObserver.h"

namespace Engine
{
	namespace Manager
	{
		namespace Component
		{
			/**
			 * @brief 스프라이트 렌더러 매니저 : 게임 오브젝트 생성 관찰자, 컴포넌트 관찰자 상속
			 * @date 2024-10-21
			 * @class
			 * 스프라이트 렌더러들을 모아 한번에 렌더 작업 수행
			 */
			class SpriteRendererManager : public Observer::ICreateGameObjectObserver, public GameObject::Observer::IComponentObserver
			{
			private: // ==== [선언] ====
				// <스프라이트 렌더러 참조용 포인터>
				using SpriteRenderers = std::vector<GameObject::Component::SpriteRenderer*>;

			private: // ==== [구성 요소] ====
				SpriteRenderers spriteRenderers; // 스프라이트 렌더러 배열

			public: // ==== [생성자 및 소멸자] ====
				SpriteRendererManager();
				~SpriteRendererManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 스프라이트 렌더러 초기화
				 * @date 2024-10-21
				 * @param createGameObjectSubject : 게임 오브젝트 생성 관찰대상
				 * @return 스프라이트 렌더러 초기화 여부
				 */
				bool Initialize(Observer::ICreateGameObjectSubject* createGameObjectSubject);
				/**
				 * @brief 렌더러 매니저 자원 해제
				 * 스프라이트 렌더러 참조용 포인터 배열 요소 전부 제거
				 */
				void Release();

			public: // ==== [렌더] ====
				/**
				 * @brief 스프라이트 렌더
				 * @details
				 * 스프라이트 렌더러들을 정렬 후 전체 렌더링 렌더
				 * @param deviceContext : DirectX11 장치 내용 참조용 포인터
				 * @param viewProjectionMatrix : 뷰 * 투영 행렬
				 */
				void Render(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix);

			public: // ==== [게임 오브젝트 생성 관찰자] ====
				/**
				 * @brief 게임 오브젝트 생성 갱신
				 * @date 2024-10-21
				 * @param createGameObject : 생성된 게임 오브젝트 참조용 포인터
				 */
				void CreateGameObjectUpdate(GameObject::GameObject* createGameObject);

			public: // ==== [컴포넌트 관찰자] ====
				/**
				 * @brief 컴포넌트 추가 갱신
				 * @date 2024-10-21
				 * @param addComponent : 추가된 컴포넌트
				 */
				void AddComponentUpdate(GameObject::Component::Component* addComponent);
				/**
				 * @brief 컴포넌트 제거 갱신
				 * @date 2024-10-21
				 * @param removeComponent : 제거된 컴포넌트
				 */
				void RemoveComponentUpdate(GameObject::Component::Component* removeComponent);

			private: // ==== [Static] ====
				/**
				 * @brief 스프라이트 렌더러 정렬 함수
				 * @param lhs : 스프라이트 렌더러 참조용 포인터
				 * @param rhs : 스프라이트 렌더러 참조용 포인터
				 * @return 스왑 여부
				 */
				static bool SpriteCompareTo(GameObject::Component::SpriteRenderer* lhs, GameObject::Component::SpriteRenderer* rhs);
			};
		} // Component
	} // Manager
} // Engine
