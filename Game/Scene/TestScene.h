#pragma once

#include "Engine/Scene/Scene.h"

namespace Game
{
	namespace Scene
	{
		using namespace Engine;

		class TestScene : public Engine::Scene::Scene
		{
		public: // ==== [생성자] ====
			/** @param sceneName : 씬 이름 */
			TestScene(const std::wstring& sceneName);
			TestScene(const std::string& sceneName);

		public: // ==== [씬 로드 및 로드 해제] ====
			/**
			 * @brief 씬 로드
			 * @param resourceManager : 리소스 로드할 매니저
			 * @param gameObjectManager : 게임 오브젝트 생성할 매니저
			 * @return 씬 로드 성공 여부
			 */
			virtual bool LoadScene(Manager::ResourceManager* resourceManager, Manager::GameObjectManager* gameObjectManager) override;
			/**
			 * @brief 씬 로드 해제
			 * @param resourceManager : 리소스 로드 해제할 매니저
			 * @param gameObjectManager : 게임 오브젝트 제거할 매니저
			 * @return 씬 로드 해제 성공 여부
			 */
			virtual bool UnLoadScene(Manager::ResourceManager* resourceManager, Manager::GameObjectManager* gameObjectManager) override;
		};
	} // Scene
} // Game
