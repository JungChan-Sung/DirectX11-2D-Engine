/*********************************************************************
 * @file   MathExtend.h
 * @brief  cmath 확장 버전
 * @date   2024-10-06
 *********************************************************************/

#pragma once

#include <type_traits>
 // std::signbit
#include <cmath>

namespace CommonUtiliy
{
	/**
	 * @brief 부호 확인 함수
	 * @date 2024-10-05
	 * @details
	 * value 값이 양수, 음수, 0인지 확인하는 함수
	 * @warning
	 * NaN, Inf 값을 처리하지 못할 수 있음
	 * @tparam value : 부호 확인할 값
	 * @return value 값이 양수이면 1, 음수이면 -1, 0이면 0을 반환
	 */
	template<typename T>
	inline constexpr int Sign(T value)
	{
		//return (value > 0) - (value < 0); // C++ 17 이전 버전 방법
		return (value <=> 0); // C++ 20 이후 우주선 연산자를 사용한 방법
		// 우주선 연산자는 >, <, == 값비교를 한번에 해줌
	}
	/**
	 * @brief 개선된 부호 확인 함수
	 * @date 2024-10-05
	 * @details
	 * Sign 함수에서 특수한 부동 소수점 값(NaN, Inf)의 부호를 확인하기위해 개선된 버전
	 * @tparam value : 부호 확인할 값
	 * @return value 값이 양수이면 1, 음수이면 -1, 0이면 0을 반환
	 */
	template<typename T>
	inline constexpr int AdvancedSign(T value)
	{
		// constexpr은 컴파일 단계에서 확정을 지어줌
		// 부동 소수점인지 확인
		if constexpr (std::is_floating_point_v<T>)
		{
			return std::signbit(value) ? -1 : (value == 0 ? 0 : 1);
		}
		// 부동 소수점이 아니면 정수형 함수로 확인
		else
		{
			return Sign(value);
		}
	}
} // CommonUtility
