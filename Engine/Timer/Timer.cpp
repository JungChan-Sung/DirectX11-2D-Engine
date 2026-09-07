#include "Timer.h"

namespace Engine
{
	namespace Timer
	{
		Timer::Timer() : timeScale(1.0), isRunning(false)
		{
			start = Clock::now();
			stop = Clock::now();
			deltaTime = TimeUnit(0);
		}

		Timer::~Timer()
		{
		}

		bool Timer::Start()
		{
			if (isRunning) // isRunning == true
				return false;

			start = Clock::now();
			isRunning = true;

			return true;
		}

		bool Timer::Stop()
		{
			if (!isRunning) // isRunning == false
				return false;

			stop = Clock::now();
			isRunning = false;

			return true;
		}

		void Timer::Restart()
		{
			start = Clock::now();
			isRunning = true;
		}

		void Timer::Update()
		{
			start = Clock::now();
		}

		double Timer::GetDeltaTime()
		{
			// 타이머가 동작중인 경우
			if (isRunning)
			{
				// 현재 시간 - 시작 시간
				deltaTime = Duration(Clock::now() - start);
				return deltaTime.count();
			}

			// 타이머가 멈춘 경우 멈춘 시간 - 시작 시간
			deltaTime = Duration(stop - start);
			return deltaTime.count();
		}
	} // Timer
} // Engine