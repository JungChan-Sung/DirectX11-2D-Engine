#pragma once

#include <list>
#include <vector>
#include "Engine/GameObject/GameObject.h"
#include "Observer/ICreateGameObjectObserver.h"

namespace Engine
{
	namespace Manager
	{
		/**
		 * @brief 게임 오브젝트 매니저 클래스 : 게임 오브젝트 생성 관찰대상 인터페이스 상속
		 * @date 2024-10-21
		 * @class
		 * 게임 오브젝트를 생성 및 파괴를 관리하는 클래스
		 */
		class GameObjectManager : public Observer::ICreateGameObjectSubject
		{
		private: // ==== [선언] ====
			// <게임 오브젝트 포인터>
			using GameObjectList = std::list<GameObject::GameObject*>;
			// <게임 오브젝트 생성 관찰자 참조용 포인터>
			using CreateGameObjectObservers = std::vector<Observer::ICreateGameObjectObserver*>;

		private: // ==== [구성 요소] ====
			GameObjectList gameObjects; // 게임 오브젝트 리스트

		private: // ==== [관찰자 구성 요소] ====
			CreateGameObjectObservers createGameObjectObservers; // 게임 오브젝트 생성 관찰자 배열

		public: // ==== [생성자 및 소멸자] ====
			GameObjectManager();
			~GameObjectManager();

		public: // ==== [Setup] ====
			/**
			 * @brief 게임 오브젝트 매니저 자원 해제
			 * @details
			 * 게임 오브젝트 전체 자원 해제
			 */
			void Release();

		public: // ==== [게임 오브젝트] ====
			/**
			 * @brief 게임 오브젝트 생성
			 * @param gameObjectName : 게임 오브젝트 이름
			 * @details
			 * 게임 오브젝트 생성 및 이름 설정
			 * @return 생성된 게임 오브젝트 포인터
			 */
			GameObject::GameObject* CreateGameObject(const std::wstring& gameObjectName);
			/**
			 * @brief 게임 오브젝트 파괴
			 * @details
			 * std::find 함수로 파괴할 게임 오브젝트 포인터 탐색 후
			 * 탐색에 성공 시 해당 게임 오브젝트 파괴
			 * @param destroyGameObject : 파괴할 게임 오브젝트 포인터
			 */
			void DestroyGameObject(GameObject::GameObject* destroyGameObject);
			/**
			 * @brief 게임 오브젝트 탐색
			 * @details
			 * 게임 오브젝트 리스트를 순회하면서 찾을 이름과 비교 후
			 * 탐색에 성공 시 해당 게임 오브젝트 참조용 포인터 반환
			 * @param name : 탐색할 게임 오브젝트 이름
			 * @return 탐색된 게임 오브젝트 참조용 포인터(nullptr : 탐색 실패)
			 */
			GameObject::GameObject* FindGameObject(const std::wstring& name);

		public: // ==== [게임 오브젝트 트랜스폼] ====
			/** @brief 게임 오브젝트 트랜스폼 전체 갱신 */
			void UpdateTransform();
			/** @brief 게임 오브젝트 트랜스폼 전체 갱신 되었는지 여부 리셋 */
			void ResetUpdatedTransform();

		public: // ==== [게임 오브젝트 생성 관찰대상] ====
			/**
			 * @brief 게임 오브젝트 생성 관찰자 연결
			 * @param createGameObjectObserver : 게임 오브젝트 생성 관찰자 참조용 포인터
			 */
			void AttachCreateGameObjectObserver(Observer::ICreateGameObjectObserver* createGameObjectObserver);
			/**
			 * @brief 게임 오브젝트 생성 관찰자 연결 해제
			 * @param createGameObjectObserver : 게임 오브젝트 생성 관찰자 참조용 포인터
			 */
			void DetachCreateGameObjectObserver(Observer::ICreateGameObjectObserver* createGameObjectObserver);
			/**
			 * @brief 게임 오브젝트 생성 알림
			 * @param createGameObject : 생성된 게임 오브젝트 참조용 포인터
			 */
			void CreateGameObjectNotify(GameObject::GameObject* createGameObject);
		};
	}
} // Engine

