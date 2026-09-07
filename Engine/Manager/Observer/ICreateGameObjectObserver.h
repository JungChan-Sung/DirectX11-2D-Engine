/*********************************************************************
 * @file   ICreateGameObjectObserver.h
 * @brief  게임 오브젝트 생성 관찰자 패턴
 * @date   2024-10-21
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace GameObject
	{
		// 게임 오브젝트 전방 선언
		class GameObject;
	} // GameObject

	namespace Manager
	{
		namespace Observer
		{
			/**
			 * @brief 게임 오브젝트 생성 관찰자 인터페이스
			 * @date 2024-10-21
			 * @interface
			 * 스프라이트 렌더러 매니저 클래스
			 */
			__interface ICreateGameObjectObserver
			{
			public: // ==== [게임 오브젝트 생성 관찰자] ====
				void CreateGameObjectUpdate(GameObject::GameObject* createGameObject);
			};

			/**
			 * @brief 게임 오브젝트 생성 관찰대상 인터페이스
			 * @date 2024-10-21
			 * @interface
			 * 게임 오브젝트 매니저 클래스
			 */
			__interface ICreateGameObjectSubject
			{
			public: // ==== [게임 오브젝트 생성 관찰대상] ====
				void AttachCreateGameObjectObserver(ICreateGameObjectObserver* createGameObjectObserver);
				void DetachCreateGameObjectObserver(ICreateGameObjectObserver* createGameObjectObserver);
				void CreateGameObjectNotify(GameObject::GameObject* createGameObject);
			};
		} // Observer
	} // Manager
} // Engine
