#include "GameEngine.h"
#include "Config/EngineConfig.h"
#include "Config/ResourceConfig.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	GameEngine::GameEngine() : windowContainer(nullptr),
		resourceManager(nullptr), gameObjectManager(nullptr), sceneManager(nullptr),
		cameraManager(nullptr), spriteRendererManager(nullptr),
		timer(nullptr), game(nullptr)
	{
	}

	GameEngine::~GameEngine()
	{
		Release();
	}

	bool GameEngine::Initialize(HINSTANCE hInstance)
	{
		// 전역 설정 값 초기화
		if (!InitializeConfig()) // InitializeConfig == false
			return false;

		// 윈도우 컨테이너 초기화
		if (!InitializeWindowContainer(hInstance)) // InitializeWindowContainer == false
			return false;

		// 매니저 초기화
		if (!InitializeManager()) // InitializeManager == false
			return false;

		// 컴포넌트 매니저 초기화
		if (!InitializeComponentManager()) // InitializeComponentManager == false
			return false;

		// 구성 요소 초기화
		if (!InitializeComponents()) // InitializeComponents == false
			return false;

		// 메인 카메라 세팅
		Option::ScreenOption* screenOption = windowContainer->GetScreenOption();
		cameraManager->GetMainCamera()->SetProjection(static_cast<float>(screenOption->GetResolutionWidth()), static_cast<float>(screenOption->GetResolutionHeight()), -10.0f, 10.0f);

		// 메인 씬 로드
		if (!sceneManager->LoadMainScene()) // sceneManager->LoadMainScene == false
			return false;

		// 타이머 시작
		timer->Start();

		return true;
	}

	void GameEngine::Release()
	{
		// 구성 요소
		CommonUtility::SafeDelete(game);
		CommonUtility::SafeDelete(timer);

		// 컴포넌트 매니저
		CommonUtility::SafeDelete(spriteRendererManager);
		CommonUtility::SafeDelete(cameraManager);

		// 매니저
		CommonUtility::SafeDelete(sceneManager);
		CommonUtility::SafeDelete(gameObjectManager);
		CommonUtility::SafeDelete(resourceManager);

		// 컨테이너
		CommonUtility::SafeDelete(windowContainer);

		// 전역 설정 값
		ResourceConfig::Destroy();
		EngineConfig::Destroy();
	}

	void GameEngine::Run()
	{
		// 윈도우 메시지 초기화
		MSG message;
		std::memset(&message, 0, sizeof(MSG));

		Option::ScreenOption* screenOption = windowContainer->GetScreenOption();
		double inverseFps;
		double acculmulateDeltaTime = 0.0; // 시간 간격 총합

		// 게임 루프
		while (true)
		{
			if (PeekMessageW(&message, nullptr, 0u, 0u, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
					break;

				TranslateMessage(&message);
				DispatchMessageW(&message);
			}

			inverseFps = screenOption->GetInverseFps();
			acculmulateDeltaTime += timer->GetDeltaTime();
			if (acculmulateDeltaTime >= inverseFps)
			{
				UpdateFrame();
				RenderFrame();

				PostFrameUpdate();

				acculmulateDeltaTime -= inverseFps;
			}
		}
	}

	void GameEngine::UpdateFrame()
	{
		// 게임 로직 갱신
		game->Update(static_cast<float>(timer->GetDeltaTime()));

		// 입력 상태 갱신
		windowContainer->GetInput()->UpdateState();
		// 타이머 재시작
		timer->Restart();
	}

	void GameEngine::RenderFrame()
	{
		Graphics::Graphics* graphics = windowContainer->GetGraphics();
		graphics->BeginRender();

		// 트랜스폼 갱신
		gameObjectManager->UpdateTransform();
		// 카메라 갱신
		cameraManager->UpdateCamera();
		// 렌더링
		spriteRendererManager->Render(graphics->GetDeviceContext(), cameraManager->GetMainCamera()->GetViewProjectionMatrix());

		graphics->EndRender();
	}

	void GameEngine::PostFrameUpdate()
	{
		// 트랜스폼 갱신 여부 리셋
		gameObjectManager->ResetUpdatedTransform();
	}

	bool GameEngine::InitializeConfig()
	{
		// ==== 게임 엔진 설정 값 초기화 ====
		if (!EngineConfig::Instance()->Initialize()) // EngineConfig->Initialize == false
			return false;
		// ==================================

		// ==== 리소스 설정 값 초기화 ====
		if (!ResourceConfig::Instance()->Initialize()) // ResourceConfig->Initialize == false
			return false;
		// ===============================

		return true;
	}

	bool GameEngine::InitializeWindowContainer(HINSTANCE hInstance)
	{
		// ==== 윈도우 컨테이너 생성 및 초기화 ====
		windowContainer = new Window::WindowContainer;
		if (!windowContainer) // windowContainer == nullptr
			return false;

		if (!windowContainer->Initialize(hInstance)) // windowContainer->Initialize == false
			return false;
		// ========================================

		return true;
	}

	bool GameEngine::InitializeManager()
	{
		// ==== 리소스 매니저 생성 및 초기화 ====
		resourceManager = new Manager::ResourceManager;
		if (!resourceManager) // resourceManager == nullptr
			return false;

		if (!resourceManager->Initialize(windowContainer->GetGraphics())) // resourceManager->Initialize == false
			return false;
		// ======================================

		// ==== 게임 오브젝트 매니저 생성 ======
		gameObjectManager = new Manager::GameObjectManager;
		if (!gameObjectManager) // gameObjectManager == nullptr
			return false;
		// =====================================

		// ==== 씬 매니저 생성 및 초기화 ======
		sceneManager = new Manager::SceneManager;
		if (!sceneManager) // sceneManager == nullptr
			return false;

		if (!sceneManager->Initialize(gameObjectManager, resourceManager)) // sceneManager->Initialize == false
			return false;
		// ====================================

		return true;
	}

	bool GameEngine::InitializeComponentManager()
	{
		// ==== 카메라 매니저 생성 및 초기화 ====
		cameraManager = new Manager::Component::CameraManager;
		if (!cameraManager) // cameraManager == nullptr
			return false;

		if (!cameraManager->Initialize(gameObjectManager)) // cameraManager->Initialize == false
			return false;
		// ======================================

		// ==== 스프라이트 렌더러 매니저 생성 및 초기화 ====
		spriteRendererManager = new Manager::Component::SpriteRendererManager;
		if (!spriteRendererManager) // spriteRendererManager == nullptr
			return false;

		if (!spriteRendererManager->Initialize(gameObjectManager)) // spriteRendererManager->Initialize == false
			return false;
		// =================================================

		return true;
	}

	bool GameEngine::InitializeComponents()
	{
		// ==== 타이머 생성 ====
		timer = new Timer::Timer;
		if (!timer) // timer == nullptr
			return false;
		// =====================

		// ==== 게임 생성 및 초기화 ====
		game = new Game::Game;
		if (!game) // game == nullptr;
			return false;

		if (!game->Initialize()) // game->Initialize == false
			return false;
		// =============================

		return true;
	}
} // Engine