/*********************************************************************
 * @file   DirectXMathExtend.h
 * @brief  DirectXMath.h 확장된 함수를 제공함
 * @date   2024-10-06
 *********************************************************************/
#pragma once

#include <DirectXMath.h>

namespace CommonUtility
{
	namespace DirectXMathExtend
	{
		// ==== [수치(DirectXMath 내부에 존재하지만 더 쓰기 편하게 하기 위함)] ====
		/** @brief 라디안 -> 각도로 변환하기 위한 상수 (180 / PI) */
		constexpr static float XM_RAD2DEG = 57.2957795130f;
		/** @brief 각도 -> 라디안으로 변환하기 위한 상수 (PI / 180) */
		constexpr static float XM_DEG2RAD = 0.0174532925f;
		/** @brief 부동소수점 비교 시 허용되는 최대 오차 */
		constexpr static float XM_EPSILON = 0.001f;

		// ==== [2차원 방향 벡터] ====
		// 상수 벡터(const XMVECTOR) 인스턴스에는 XMVECTORF32 형식을 사용
		// 16바이트 경계에 정합되는 구조체
		// 정수형은 XMVECTORU32를 사용
		/**
		 * @brief 2차원 공간 위쪽 방향을 나타내는 단위 벡터
		 * @return DirectX::XMVECTOR(0.0f, 1.0f, 0.0f, 0.0f)
		 */
		const static DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
		/**
		 * @brief 2차원 공간 아래쪽 방향을 나타내는 단위 벡터
		 * @return DirectX::XMVECTOR(0.0f, -1.0f, 0.0f, 0.0f)
		 */
		const static DirectX::XMVECTORF32 down = { 0.0f, -1.0f, 0.0f, 0.0f };
		/**
		 * @brief 2차원 공간 왼쪽 방향을 나타내는 단위 벡터
		 * @return DirectX::XMVECTOR(1.0f, 0.0f, 0.0f, 0.0f)
		 */
		const static DirectX::XMVECTORF32 right = { 1.0f, 0.0f, 0.0f, 0.0f };
		/**
		 * @brief 2차원 공간 오른쪽 방향을 나타내는 단위 벡터
		 * @return DirectX::XMVECTOR(-1.0f, 0.0f, 0.0f, 0.0f)
		 */
		const static DirectX::XMVECTORF32 left = { -1.0f, 0.0f, 0.0f, 0.0f };

		// ==== [회전] ====
		// DirectXMath의 오일러 각도를 이용한 쿼터니언 변환은 Z(Roll)-X(Pitch)-Y(Yaw) 순서로 진행
		// 따라서 쿼터니언을 오일러 각도 YXZ로 변환해야 오일러 각도가 나옴
		/**
		 * @brief 쿼터니언 -> 오일러 각도(YXZ) 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 쿼터니언 값
		 * @return 변환된 오일러 각도 값
		 */
		DirectX::XMFLOAT3 ToEulerAngle(const DirectX::XMFLOAT4& quaternionValue);
		/**
		 * @brief 쿼터니언 -> 오일러 각도(YXZ) 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 쿼터니언 값
		 * @return 변환된 오일러 각도 벡터
		 */
		DirectX::XMVECTOR XM_CALLCONV ToEulerAngleVector(const DirectX::XMFLOAT4& quaternionValue);
		/**
		 * @brief 쿼터니언 -> 오일러 각도(YXZ) 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 쿼터니언 벡터
		 * @return 변환된 오일러 각도 값
		 */
		DirectX::XMFLOAT3 XM_CALLCONV ToEulerAngle(DirectX::FXMVECTOR quaternionVector);
		/**
		 * @brief 쿼터니언 -> 오일러 각도(YXZ) 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 쿼터니언 벡터
		 * @return 변환된 오일러 각도 벡터
		 */
		DirectX::XMVECTOR XM_CALLCONV ToEulerAngleVector(DirectX::FXMVECTOR quaternionVector);

		/**
		 * @brief 오일러 각도 -> 쿼터니언 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 오일러 각도 값
		 * @return 변환된 쿼터니언 값
		 */
		DirectX::XMFLOAT4 ToQuarternion(const DirectX::XMFLOAT3& eulerValue);
		/**
		 * @brief 오일러 각도 -> 쿼터니언 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 오일러 각도 값
		 * @return 변환된 쿼터니언 벡터
		 */
		DirectX::XMVECTOR XM_CALLCONV ToQuarternionVector(const DirectX::XMFLOAT3& eulerValue);
		/**
		 * @brief 오일러 각도 -> 쿼터니언 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 오일러 각도 벡터
		 * @return 변환된 쿼터니언 값
		 */
		DirectX::XMFLOAT4 XM_CALLCONV ToQuarternion(DirectX::FXMVECTOR eulerVector);
		/**
		 * @brief 오일러 각도 -> 쿼터니언 변환
		 * @date 2024-10-05
		 * @param quaternionValue : 오일러 각도 벡터
		 * @return 변환된 쿼터니언 벡터
		 */
		DirectX::XMVECTOR XM_CALLCONV ToQuarternionVector(DirectX::FXMVECTOR eulerVector);
	} // DirectXMathExtend
} // Engine