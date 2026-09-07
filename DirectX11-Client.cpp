#include "CommonUtility/Utility/SafeDelete.h"
// 메모리 누수 탐지 정의 사용
#include "Engine/Config/EngineConfig.h"
#include "Engine/GameEngine.h"

/**
 * @brief DirectX11 클라이언트 메인 함수
 * @date 2024-10-03
 * @details
 * wWinMain <- 유니코드 버전의 WinMain
 * @param hInstance : 자신의 인스턴스 값(주소). 코드의 시작주소가 전달됨
 * @param hPrevInstance : 현재 사용 안하는 매개변수(nullptr 전달)
 * @param lpCmdLine : 명령행(프로그램 시작시 전달됨)
 * @param nCmdShow : 프로그램 시작시 전달되는 Window 초기 모양
 * @return 함수 종료 값
 */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// 메인 클라이언트
	Engine::GameEngine* gameEngine = new Engine::GameEngine;
	if (gameEngine->Initialize(hInstance)) // gameEngine->Initialize == true
		gameEngine->Run();
	
	CommonUtility::SafeDelete(gameEngine);

#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}