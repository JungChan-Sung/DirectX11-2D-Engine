#pragma once

#include "Engine/Manager/GameObjectManager.h"
#include "Engine/Manager/ResourceManager.h"

namespace Engine
{
	namespace Scene
	{
		/**
		 * @brief 씬 추상 클래스
		 * @date 2024-10-20
		 * @class
		 * 게임 오브젝트 및 리소스 로드 및 로드 해제하는 씬
		 */
		class Scene abstract
		{
		protected: // ==== [구성 요소] ====
			std::wstring name; // 씬 이름

		public: // ==== [생성자] ====
			/** @param sceneName : 씬 이름 */
			Scene(const std::wstring& sceneName);
			Scene(const std::string& sceneName);

		public: // ==== [씬 로드 및 로드 해제] ====
			/**
			 * @brief 씬 로드
			 * @param resourceManager : 리소스 로드할 매니저
			 * @param gameObjectManager : 게임 오브젝트 생성할 매니저
			 * @return 씬 로드 성공 여부
			 */
			virtual bool LoadScene(Manager::ResourceManager* resourceManager, Manager::GameObjectManager* gameObjectManager) abstract;
			/**
			 * @brief 씬 로드 해제
			 * @param resourceManager : 리소스 로드 해제할 매니저
			 * @param gameObjectManager : 게임 오브젝트 제거할 매니저
			 * @return 씬 로드 해제 성공 여부
			 */
			virtual bool UnLoadScene(Manager::ResourceManager* resourceManager, Manager::GameObjectManager* gameObjectManager) abstract;

		public: // ==== [Get] ====
			/** @return 씬 이름 */
			const std::wstring& GetSceneName() const;
		};
	} // Scene
} // Engine
