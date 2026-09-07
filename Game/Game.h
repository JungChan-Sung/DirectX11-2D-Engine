#pragma once

namespace Game
{
	/**
	 * @brief 게임 클래스
	 * @date 2024-10-09
	 * @class
	 * 게임의 전체적인 로직을 담당
	 */
	class Game
	{
	public: // ==== [생성자 및 소멸자] ====
		Game();
		~Game();

	public: // ==== [Setup] ====
		/**
		 * @brief 게임 초기화
		 * @details
		 * 
		 * @return 게임 초기화 성공 여부
		 */
		bool Initialize();
		/**
		 * @brief 게임 자원 해제
		 * @details
		 * 
		 */
		void Release();

	public: // ==== [게임 동작] ====
		/**
		 * @brief 게임 갱신
		 * @details
		 * 
		 * @param deltaTime : 현재 프레임과 이전 프레임 사이의 시간 간격
		 */
		void Update(const float& deltaTime);
	};
} // Game
