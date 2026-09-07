/*********************************************************************
 * @file   SafeDelete.h
 * @brief  안전한 메모리 할당 해제 매크로 파일
 * @date   2024-10-20
 *********************************************************************/

#pragma once

namespace CommonUtility
{
	/**
	 * @brief 안전한 메모리 할당 해제
	 * @tparam pointer : 메모리 할당 해제할 참조값
	 */
	template<typename T>
	inline void SafeDelete(T& pointer)
	{
		if (pointer) // pointer != nullptr
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	 /**
	  * @brief 안전한 배열 메모리 할당 해제
	  * @tparam pointer : 메모리 할당 해제할 배열 참조값
	  */
	template<typename T>
	inline void SafeDeleteArray(T& pointer)
	{
		if (pointer) // pointer != nullptr
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}

	 /**
	  * @brief 안전한 COM 자원 해제
	  * @tparam pointer : 자원 해제할 COM 참조값
	  */
	template<typename T>
	inline void SafeComPtrRelease(T& pointer)
	{
		if (pointer) // pointer != nullptr
		{
			pointer.Reset();
			pointer = nullptr;
		}
	}

	/**
	 * @brief Standard Template Library 컨테이너 자료형 자원 해제
	 * @date 2024-10-19
	 * @tparam container : STL 컨테이너 자료형 참조값
	 */
	template<typename T>
	inline void SafeContainerRelease(T& container)
	{
		for (typename T::iterator iterator = container.begin(); iterator != container.end();)
		{
			if (*iterator) // *iterator != nullptr
			{
				delete *iterator;
				*iterator = nullptr;
				iterator = container.erase(iterator);
			}
			else
			{
				++iterator;
			}
		}
	}

	/**
	 * @brief Standard Template Library 컨테이너 자료형 자원 해제
	 * @date 2024-10-19
	 * @tparam container : STL 컨테이너 자료형 참조값
	 */
	template<typename T>
	inline void SafeContainerComPtrRelease(T& container)
	{
		for (typename T::iterator iterator = container.begin(); iterator != container.end();)
		{
			if (*iterator) // *iterator != nullptr
			{
				iterator->Reset();
				*iterator = nullptr;
				iterator = container.erase(iterator);
			}
			else
			{
				++iterator;
			}
		}
	}

	/**
	 * @brief Standard Template Library Key-Value 컨테이너 자료형 자원 해제
	 * @date 2024-10-19
	 * @tparam container : STL Key-Value 컨테이너 자료형 참조값 
	 */
	template<typename T>
	inline void SafeMapContainerRelease(T& container)
	{
		for (typename T::iterator iterator = container.begin(); iterator != container.end();)
		{
			if (iterator->second) // iterator->second != nullptr
			{
				delete iterator->second;
				iterator->second = nullptr;
				iterator = container.erase(iterator);
			}
			else
			{
				++iterator;
			}
		}
	}

	/**
	 * @brief Standard Template Library Key-Value 컨테이너 자료형 자원 해제
	 * @date 2024-10-19
	 * @tparam container : STL Key-Value 컨테이너 자료형 참조값
	 */
	template<typename T>
	inline void SafeMapContainerComPtrRelease(T& container)
	{
		for (typename T::iterator iterator = container.begin(); iterator != container.end();)
		{
			if (iterator->second) // iterator->second != nullptr
			{
				iterator->second.Reset();
				iterator->second = nullptr;
				iterator = container.erase(iterator);
			}
			else
			{
				++iterator;
			}
		}
	}

	/**
	 * @brief Standard Template Library 중첩된 Key-Value 컨테이너 자료형 자원 해제
	 * @date 2024-10-20
	 * @tparam container : STL 중첩된 Key-Value 컨테이너 자료형 참조값
	 */
	template<typename T>
	inline void SafeNestedMapContainerRelease(T& container)
	{
		for (typename T::iterator iterator = container.begin(); iterator != container.end(); ++iterator)
		{
			CommonUtility::SafeMapContainerRelease(iterator->second);
			iterator->second.clear();
		}
		container.clear();
	}
} // CommonUtility
