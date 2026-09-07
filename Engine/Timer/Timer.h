#pragma once

/*
 * C++11에서 추가된 시간 관련 라이브러리
 * C의 time 보다 다양한 기능을 제공하며 사용이 쉽다
 * 나노(nano) 밀리(milli) 초(seconds) 단위까지 측정 가능
 * OS와 독립적으로 정밀도 높은 시간 측정이 가능
 */
#include <chrono>

namespace Engine
{
	namespace Timer
	{
		/**
		 * @brief 엔진 타이머 클래스
		 * @date 2024-10-03
		 * @class
		 * 엔진의 동작 시간을 관리 및 제어
		 */
		class Timer
		{
		private: // ==== [선언] ====
			// 시계 타입
			using Clock = std::chrono::steady_clock;
			// 시간 타입
			using Time = Clock::time_point;
			// 시간 간격
			using Duration = Clock::duration;
			// 시간 단위
			using TimeUnit = std::chrono::nanoseconds;

		private: // ==== [구성 요소] ====
			Time start;	// 시작 시간
			Time stop;	// 정지 시간

			Duration deltaTime;	// 현재 프레임과 이전 프레임 사이의 시간 간격
			double timeScale;	// 시간 배율
			bool isRunning;		// 타이머 실행 여부

		public: // ==== [생성자 및 소멸자] ====
			Timer();
			~Timer();

		public: // ==== [타이머 기능] ====
			/**
			 * @brief 타이머 시작
			 * @return 타이머 시작 성공 여부
			 */
			bool Start();
			/**
			 * @brief 타이머 정지
			 * @return 타이머 정지 성공 여부
			 */
			bool Stop();
			/**
			 * @brief 타이머 재시작
			 */
			void Restart();
			/**
			 * @brief 타이머 갱신
			 */
			void Update();

		public: // ==== [Get] ====
			/** @return 현재 프레임과 이전 프레임 사이의 시간 간격 */
			double GetDeltaTime();
		};
	} // Timer
} // Engine

