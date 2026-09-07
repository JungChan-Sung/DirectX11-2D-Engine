#include "SceneManager.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Utility/StringUtility.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ManagerErrorMessage.h"
#include "Game/Scene/TestScene.h"

namespace Engine
{
	namespace Manager
	{
		SceneManager::SceneManager() : currentScene(nullptr),
			gameObjectManager(nullptr), resourceManager(nullptr)
		{
		}

		SceneManager::~SceneManager()
		{
			Release();
		}

		bool SceneManager::Initialize(GameObjectManager* gameObjectManager, ResourceManager* resourceManager)
		{
			this->gameObjectManager = gameObjectManager;
			this->resourceManager = resourceManager;

			return true;
		}

		void SceneManager::Release()
		{
			CommonUtility::SafeMapContainerRelease(scenes);

			resourceManager = nullptr;
			gameObjectManager = nullptr;
		}

		void SceneManager::RegisterScene(Scene::Scene* registerScene)
		{
			const std::wstring& sceneName = registerScene->GetSceneName();
			if (scenes.find(sceneName) == scenes.end())
			{
				// 씬 등록
				scenes.insert(std::make_pair(sceneName, registerScene));
			}
		}

		void SceneManager::UnRegisterScene(const std::wstring& sceneName)
		{
			SceneTable::iterator iterator = scenes.find(sceneName);
			if (iterator != scenes.end())
			{
				// 씬 자원 해제 및 등록 해제
				CommonUtility::SafeDelete(iterator->second);
				scenes.erase(iterator);
			}
		}

		void SceneManager::UnRegisterScene(const std::string& sceneName)
		{
			UnRegisterScene(CommonUtility::StringUtility::ConvertToWideString(sceneName));
		}

		bool SceneManager::LoadScene(const std::wstring& sceneName)
		{
			SceneTable::const_iterator constIterator = scenes.find(sceneName);
			if (constIterator != scenes.end() && currentScene != constIterator->second)
			{
				// 이전 씬 로드 해제
				if (currentScene) // currentScene != nullptr
					currentScene->UnLoadScene(resourceManager, gameObjectManager);

				// 현재 씬 변경 및 로드
				currentScene = constIterator->second;
				if (!currentScene->LoadScene(resourceManager, gameObjectManager)) // currentScene->LoadScene == false
				{
					currentScene = nullptr;
					
					CommonUtility::Log::ErrorMessage(Message::Error::CURRENT_SCENE_LOAD_FAILURE);

					return false;
				}
			}
			else
			{
				CommonUtility::Log::ErrorMessage(Message::Error::SCENE_LOAD_FAILURE);
				return false;
			}

			return true;
		}

		bool SceneManager::LoadScene(const std::string& sceneName)
		{
			return LoadScene(CommonUtility::StringUtility::ConvertToWideString(sceneName));
		}

		bool SceneManager::LoadMainScene()
		{
			Game::Scene::TestScene* test = new Game::Scene::TestScene(L"TestScene");

			RegisterScene(test);
			LoadScene(L"TestScene");

			return true;
		}

		Scene::Scene* SceneManager::GetCurrentScene() const
		{
			return currentScene;
		}
	} // Manager
} // Engine
