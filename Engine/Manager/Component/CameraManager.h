#pragma once

#include <vector>
#include "Engine/GameObject/Component/Render/Camera.h"
#include "Engine/Manager/GameObjectManager.h"

namespace Engine
{
	namespace Manager
	{
		namespace Component
		{
			/**
			 * @brief 카메라 매니저 클래스
			 * @date 2024-11-01
			 * @class
			 * 카메라들을 관리하는 클래스
			 */
			class CameraManager
			{
			private: // ==== [구성 요소] ====
				GameObject::Component::Camera* mainCamera;				// 메인 카메라 참조용 포인터
				std::vector<GameObject::Component::Camera*> cameras;	// 카메라 참조용 포인터 배열

			public: // ==== [생성자 및 소멸자] ====
				CameraManager();
				~CameraManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 카메라 매니저 초기화
				 * @date 2024-11-01
				 * @details
				 * 메인 카메라 생성
				 * gameObjectManager : 게임 오브젝트 매니저 참조용 포인터
				 * @return 카메라 매니저 초기화 성공 여부
				 */
				bool Initialize(GameObjectManager* gameObjectManager);
				/**
				 * @brief 카메라 매니저 자원 해제
				 * @details
				 * 카메라 참조용 포인터 배열 클리어
				 * 메인 카메라 참조 해제
				 */
				void Release();

			public: // ==== [카메라 갱신] ====
				/**
				 * @brief 카메라 갱신
				 * @details
				 * 카메라들의 뷰 행렬 갱신
				 */
				void UpdateCamera();

			public: // ==== [Get] ====
				/** @return 메인 카메라 참조용 포인터 */
				GameObject::Component::Camera* GetMainCamera() const;

			private: // ==== [카메라 생성] ====
				/**
				 * @brief 카메라 메인 생성
				 * @date 2024-10-21
				 * @details
				 * 메인 카메라 게임 오브젝트 생성 및 카메라 컴포넌트 추가
				 * @param gameObjectManager : 게임 오브젝트 매니저 참조용 포인터
				 */
				void CreateMainCamera(GameObjectManager* gameObjectManager);
			};
		} // Component
	} // Manager
} // Engine
