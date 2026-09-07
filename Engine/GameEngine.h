#pragma once

#include "Window/WindowContainer.h"
#include "Manager/SceneManager.h"
#include "Manager/Component/CameraManager.h"
#include "Manager/Component/SpriteRendererManager.h"
#include "Timer/Timer.h"
#include "Game/Game.h"

namespace Engine
{
	/**
	 * @brief 게임 엔진 클래스
	 * @date 2024-11-06
	 * @class
	 * 게임 로직과 여러 시스템 및 매니저를 이용하여 게임 작동
	 */
	class GameEngine
	{
	private: // ==== [컨테이너] ====
		Window::WindowContainer* windowContainer;	// 윈도우 컨테이너 포인터

	private: // ==== [매니저] ====
		Manager::ResourceManager* resourceManager;		// 리소스 관리 매니저 포인터
		Manager::GameObjectManager* gameObjectManager;	// 게임 오브젝트 매니저 포인터
		Manager::SceneManager* sceneManager;			// 씬 관리 매니저 포인터

	private: // ==== [컴포넌트] ====
		Manager::Component::CameraManager* cameraManager;					// 카메라 매니저 포인터
		Manager::Component::SpriteRendererManager* spriteRendererManager;	// 스프라이트 렌더러 매니저 포인터

	private: // ==== [구성 요소] ====
		Timer::Timer* timer;	// 엔진 타이머 포인터
		Game::Game* game;		// 구동할 게임 포인터

	public: // ==== [생성자 및 소멸자] ====
		GameEngine();
		~GameEngine();

	public: // ==== [Setup] ==== 
		/**
		 * @brief 게임 엔진 초기화
		 * @date 2024-11-04
		 * @details
		 * 윈도우 컨테이너, 여러 매니저, 게임 클래스 초기화
		 * @param hInstance : 윈도우 핸들값
		 * @return 게임 엔진 초기화 성공 여부
		 */
		bool Initialize(HINSTANCE hInstance);
		/**
		 * @brief 게임 엔진 자원 해제
		 * @date 2024-11-04
		 * @details
		 * 게임 클래스 자원 해제
		 */
		void Release();

	public: // ==== [게임 엔진 동작] ====
		/**
		 * @brief 게임 엔진 실행
		 * @date 2024-11-04
		 * @details
		 * 논블로킹 메시지 처리 Game Loop 패턴이 존재
		 */
		void Run();

	private: // ==== [게임 엔진 동작] ====
		/**
		 * @brief 게임 엔진 프레임 갱신
		 * @date 2024-10-04
		 * @details
		 * 게임 엔진의 게임 클래스, 입력, 타이머 등을 한 프레임 갱신
		 */
		void UpdateFrame();
		/**
		 * @brief 게임 엔진 프레임 렌더
		 * @date 2024-10-20
		 * @details
		 * 게임의 갱신된 현재 프레임을 DirectX11로 렌더
		 */
		void RenderFrame();
		/**
		 * @brief 게임 엔진 후처리 갱신
		 * @date 2024-11-04
		 * @details
		 * 게임 엔진의 렌더 이후 후처리 갱신 작업
		 */
		void PostFrameUpdate();

	private: // ==== [전역 설정 값 초기화] ====
		/**
		 * @brief 전역 설정 값 초기화
		 * @date 2024-11-04
		 * @return 전역 설정 값 초기화 성공 여부
		 */
		bool InitializeConfig();
		/**
		 * @brief 윈도우 컨테이너 초기화
		 * @date 2024-11-04
		 * @param hInstance : 윈도우 핸들값
		 * @return 윈도우 컨테이너 초기화 성공 여부
		 */
		bool InitializeWindowContainer(HINSTANCE hInstance);
		/**
		 * @brief 매니저 초기화
		 * @date 2024-11-04
		 * @return 매니저 초기화 성공 여부
		 */
		bool InitializeManager();
		/**
		 * @brief 컴포넌트 매니저 초기화
		 * @date 2024-11-04
		 * @return 컴포넌트 매니저 초기화 성공 여부
		 */
		bool InitializeComponentManager();
		/**
		 * @brief 구성 요소 초기화
		 * @date 2024-11-04
		 * @return 구성 요소 초기화 성공 여부
		 */
		bool InitializeComponents();
	};
} // Engine