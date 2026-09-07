#pragma once

// SafeDelete
#include "CommonUtility/Utility/SafeDelete.h"

namespace CommonUtility
{
	/**
	 * @brief 싱글톤 패턴 템플릿 정적 클래스
	 * @date 2024-10-06
	 * @class
	 * 해당 클래스를 상속받아 사용하는 템플릿 클래스
	 * 최대한 짧게 쓰기위해 Get 함수에 Get단어 제외하기로함
	 * @code
	 * class ClassName : public Singleton<ClassName>
	 */
	template<typename T>
	class Singleton
	{
	private: // ==== [접근] ====
		static T* instance; // 생성되는 인스턴스

	protected: // ==== [생성자] ====
		Singleton() 
		{ 
		}

	public: // ==== [Setup] ====
		/** @return 생성된 인스턴스 포인터 */
		static T* Instance()
		{
			if (!instance) // instance == nullptr
				instance = new T;

			return instance;
		}

		/** @brief 싱글톤 패턴 클래스 파괴 */
		static void Destroy()
		{
			SafeDelete(instance);
		}
	};

	// 템플릿 정적 instance 변수 초기화
	template<typename T> T* Singleton<T>::instance = nullptr;
} // CommonUtility
