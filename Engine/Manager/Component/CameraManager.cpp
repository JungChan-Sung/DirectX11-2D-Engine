#include "CameraManager.h"
#include "Engine/Config/EngineConfig.h"

namespace Engine
{
	namespace Manager
	{
		namespace Component
		{
			CameraManager::CameraManager() : mainCamera(nullptr)
			{
			}

			CameraManager::~CameraManager()
			{
				Release();
			}

			bool CameraManager::Initialize(GameObjectManager* gameObjectManager)
			{
				CreateMainCamera(gameObjectManager);

				return true;
			}

			void CameraManager::Release()
			{
				cameras.clear();

				mainCamera = nullptr;
			}

			void CameraManager::UpdateCamera()
			{
				for (GameObject::Component::Camera* camera : cameras)
					camera->UpdateViewMatrix();
			}

			GameObject::Component::Camera* CameraManager::GetMainCamera() const
			{
				return mainCamera;
			}

			void CameraManager::CreateMainCamera(GameObjectManager* gameObjectManager)
			{
				GameObject::GameObject* mainCameraGameObject = gameObjectManager->CreateGameObject(EngineConfig::Instance()->MainCameraGameObjectName());
				mainCamera = mainCameraGameObject->AddComponent<GameObject::Component::Camera>();
			}
		} // Component
	} // Manager
} // Engine