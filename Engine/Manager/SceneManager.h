#pragma once

#include <unordered_map>
#include "Engine/Scene/Scene.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"

namespace Engine
{
	namespace Manager
	{
		/**
		 * @brief 씬 매니저 클래스
		 * @date 2024-10-21
		 * @class
		 * 씬들을 등록 및 등록 해제하는 클래스
		 */
		class SceneManager
		{
		private: // ==== [선언] ====
			// <씬 이름, 씬 포인터>
			using SceneTable = std::unordered_map<std::wstring, Scene::Scene*>;

		private: // ==== [구성 요소] ====
			SceneTable scenes;			// 씬 테이블
			Scene::Scene* currentScene;	// 현재 로드된 씬 참조용 포인터

		private: // ==== [참조 매니저] ====
			GameObjectManager* gameObjectManager;	// 게임 오브젝트 매니저 참조용 포인터
			ResourceManager* resourceManager;		// 리소스 매니저 참조용 포인터

		public: // ==== [생성자 및 소멸자] ====
			SceneManager();
			~SceneManager();

		public: // ==== [Setup] ====
			/**
			 * @brief 씬 매니저 초기화
			 * @date 2024-10-21
			 * @param gameObjectManager : 게임 오브젝트 매니저 참조용 포인터
			 * @param resourceManager : 리소스 매니저 참조용 포인터
			 * @return 씬 매니저 초기화 성공 여부
			 */
			bool Initialize(GameObjectManager* gameObjectManager, ResourceManager* resourceManager);
			/**
			 * @brief 씬 매니저 자원 해제
			 * @details
			 * 씬 전체 자원 해제
			 */
			void Release();

		public: // ==== [씬 등록 및 등록 해제] ====
			/**
			 * @brief 씬 등록
			 * @param registerScene : 등록할 씬 포인터
			 */
			void RegisterScene(Scene::Scene* registerScene);
			/**
			 * @brief 씬 등록 해제
			 * @param sceneName : 등록 해제할 씬
			 */
			void UnRegisterScene(const std::wstring& sceneName);
			void UnRegisterScene(const std::string& sceneName);

		public: // ==== [씬 로드] ====
			/**
			 * @brief 씬 로드
			 * @param sceneName : 로드할 씬 이름
			 * @return 로드 성공 여부
			 */
			bool LoadScene(const std::wstring& sceneName);
			bool LoadScene(const std::string& sceneName);
			
		public: // ==== [메인 씬 로드] ====
			/** @brief 메인 씬 로드 */
			bool LoadMainScene();

		public: // ==== [Get] ====
			/** @return 현재 로드된 씬 참조용 포인터 */
			Scene::Scene* GetCurrentScene() const;
		};
	} // Manager
} // Engine
