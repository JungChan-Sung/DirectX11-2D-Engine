#include "Game.h"

namespace Game
{
	Game::Game()
	{
	}

	Game::~Game()
	{
		Release();
	}

	bool Game::Initialize()
	{
		return true;
	}

	void Game::Release()
	{
	}

	void Game::Update(const float& deltaTime)
	{
	}
} // Game