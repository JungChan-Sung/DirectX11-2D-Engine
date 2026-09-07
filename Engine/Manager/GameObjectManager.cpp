#include "GameObjectManager.h"
#include <algorithm>
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Manager
	{
		GameObjectManager::GameObjectManager()
		{
		}

		GameObjectManager::~GameObjectManager()
		{
			Release();
		}

		void GameObjectManager::Release()
		{
			createGameObjectObservers.clear();

			CommonUtility::SafeContainerRelease(gameObjects);
		}

		GameObject::GameObject* GameObjectManager::CreateGameObject(const std::wstring& gameObjectName)
		{
			GameObject::GameObject* gameObject = new GameObject::GameObject;
			if (!gameObject) // gameObject == nullptr
				return nullptr;

			gameObject->SetName(gameObjectName);
			gameObjects.push_back(gameObject);

			// 게임 오브젝트 생성 알림
			CreateGameObjectNotify(gameObject);

			return gameObject;
		}

		void GameObjectManager::DestroyGameObject(GameObject::GameObject* destroyGameObject)
		{
			GameObjectList::iterator iterator = std::find(gameObjects.begin(), gameObjects.end(), destroyGameObject);
			if (iterator != gameObjects.end())
			{
				CommonUtility::SafeDelete(*iterator);
				gameObjects.erase(iterator);
			}
		}

		GameObject::GameObject* GameObjectManager::FindGameObject(const std::wstring& name)
		{
			for (GameObject::GameObject* gameObject : gameObjects)
			{
				if (gameObject->GetName() == name)
					return gameObject;
			}

			return nullptr;
		}

		void GameObjectManager::UpdateTransform()
		{
			for (GameObject::GameObject* gameObject : gameObjects)
				gameObject->GetTransform()->Update();
		}

		void GameObjectManager::ResetUpdatedTransform()
		{
			for (GameObject::GameObject* gameObject : gameObjects)
				gameObject->GetTransform()->ResetUpdated();
		}

		void GameObjectManager::AttachCreateGameObjectObserver(Observer::ICreateGameObjectObserver* createGameObjectObserver)
		{
			if (std::find(createGameObjectObservers.begin(), createGameObjectObservers.end(), createGameObjectObserver) == createGameObjectObservers.end())
				createGameObjectObservers.push_back(createGameObjectObserver);
		}

		void GameObjectManager::DetachCreateGameObjectObserver(Observer::ICreateGameObjectObserver* createGameObjectObserver)
		{
			CreateGameObjectObservers::const_iterator constIterator = std::find(createGameObjectObservers.cbegin(), createGameObjectObservers.cend(), createGameObjectObserver);
			if (constIterator != createGameObjectObservers.end())
				createGameObjectObservers.erase(constIterator);
		}

		void GameObjectManager::CreateGameObjectNotify(GameObject::GameObject* createGameObject)
		{
			for (Observer::ICreateGameObjectObserver* observer : createGameObjectObservers)
				observer->CreateGameObjectUpdate(createGameObject);
		}
	}
} // Game
