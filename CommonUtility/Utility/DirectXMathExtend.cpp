#include "DirectXMathExtend.h"
#include <cmath>

namespace CommonUtility
{
    namespace DirectXMathExtend
    {
        DirectX::XMFLOAT3 ToEulerAngle(const DirectX::XMFLOAT4& quaternionValue)
        {
            // 중복 연산 캐싱
            float squareQx = quaternionValue.x * quaternionValue.x;

            float sinPitch = 2.0f * (quaternionValue.w * quaternionValue.x - quaternionValue.z * quaternionValue.y);
            float pitch;
            // 짐벌락 발생 특수 상황 처리
            if (std::fabsf(sinPitch) >= 1.0f)
                pitch = std::copysignf(DirectX::XM_PIDIV2, sinPitch);
            else
                pitch = std::asinf(sinPitch);

            return DirectX::XMFLOAT3
            (
                pitch,
                std::atan2f(2.0f * (quaternionValue.w * quaternionValue.y + quaternionValue.x * quaternionValue.z), 1.0f - 2.0f * (squareQx + quaternionValue.y * quaternionValue.y)),
                std::atan2f(2.0f * (quaternionValue.w * quaternionValue.z + quaternionValue.y * quaternionValue.x), 1.0f - 2.0f * (squareQx + quaternionValue.z * quaternionValue.z))
            );
        }

        DirectX::XMVECTOR XM_CALLCONV ToEulerAngleVector(const DirectX::XMFLOAT4& quaternionValue)
        {
            // 중복 연산 캐싱
            float squareQx = quaternionValue.x * quaternionValue.x;

            float sinPitch = 2.0f * (quaternionValue.w * quaternionValue.x - quaternionValue.z * quaternionValue.y);
            float pitch;
            // 짐벌락 발생 특수 상황 처리
            if (std::fabsf(sinPitch) >= 1.0f)
                pitch = std::copysignf(DirectX::XM_PIDIV2, sinPitch);
            else
                pitch = std::asinf(sinPitch);

            return DirectX::XMVectorSet
            (
                pitch,
                std::atan2f(2.0f * (quaternionValue.w * quaternionValue.y + quaternionValue.x * quaternionValue.z), 1.0f - 2.0f * (squareQx + quaternionValue.y * quaternionValue.y)),
                std::atan2f(2.0f * (quaternionValue.w * quaternionValue.z + quaternionValue.y * quaternionValue.x), 1.0f - 2.0f * (squareQx + quaternionValue.z * quaternionValue.z)),
                0.0f
            );
        }

        DirectX::XMFLOAT3 XM_CALLCONV ToEulerAngle(DirectX::FXMVECTOR quaternionVector)
        {
            float qx = DirectX::XMVectorGetX(quaternionVector);
            float qy = DirectX::XMVectorGetY(quaternionVector);
            float qz = DirectX::XMVectorGetZ(quaternionVector);
            float qw = DirectX::XMVectorGetW(quaternionVector);

            // 중복 연산 캐싱
            float squareQx = qx * qx;

            float sinPitch = 2.0f * (qw * qx - qz * qy);
            float pitch;
            // 짐벌락 발생 특수 상황 처리
            if (std::fabsf(sinPitch) >= 1.0f)
                pitch = std::copysignf(DirectX::XM_PIDIV2, sinPitch);
            else
                pitch = std::asinf(sinPitch);

            return DirectX::XMFLOAT3
            (
                pitch,
                std::atan2f(2.0f * (qw * qy + qx * qz), 1.0f - 2.0f * (squareQx + qy * qy)),
                std::atan2f(2.0f * (qw * qz + qy * qx), 1.0f - 2.0f * (squareQx + qz * qz))
            );
        }

        DirectX::XMVECTOR XM_CALLCONV ToEulerAngleVector(DirectX::FXMVECTOR quaternionVector)
        {
            float qx = DirectX::XMVectorGetX(quaternionVector);
            float qy = DirectX::XMVectorGetY(quaternionVector);
            float qz = DirectX::XMVectorGetZ(quaternionVector);
            float qw = DirectX::XMVectorGetW(quaternionVector);

            // 중복 연산 캐싱
            float squareQx = qx * qx;

            float sinPitch = 2.0f * (qw * qx - qz * qy);
            float pitch;
            // 짐벌락 발생 특수 상황 처리
            if (std::fabsf(sinPitch) >= 1.0f)
                pitch = std::copysignf(DirectX::XM_PIDIV2, sinPitch);
            else
                pitch = std::asinf(sinPitch);

            return DirectX::XMVectorSet
            (
                pitch,
                std::atan2f(2.0f * (qw * qy + qx * qz), 1.0f - 2.0f * (squareQx + qy * qy)),
                std::atan2f(2.0f * (qw * qz + qy * qx), 1.0f - 2.0f * (squareQx + qz * qz)),
                0.0f
            );
        }

        DirectX::XMFLOAT4 ToQuarternion(const DirectX::XMFLOAT3& eulerValue)
        {
            // Roll(X-Axis Rotation)
            float sinr = std::sinf(eulerValue.x * 0.5f);
            float cosr = std::cosf(eulerValue.x * 0.5f);

            // Pitch(Y-Axis Rotation)
            float sinp = std::sinf(eulerValue.y * 0.5f);
            float cosp = std::cosf(eulerValue.y * 0.5f);

            // Yaw(Z-Axis Rotation)
            float siny = std::sinf(eulerValue.z * 0.5f);
            float cosy = std::cosf(eulerValue.z * 0.5f);

            return DirectX::XMFLOAT4
            (
                sinr * cosp * cosy - cosr * sinp * siny,
                cosr * sinp * cosy + sinr * cosp * siny,
                cosr * cosp * siny - sinr * sinp * cosy,
                cosr * cosp * cosy - sinr * sinp * siny
            );
        }

        DirectX::XMVECTOR XM_CALLCONV ToQuarternionVector(const DirectX::XMFLOAT3& eulerValue)
        {
            // Roll(X-Axis Rotation)
            float sinr = std::sinf(eulerValue.x * 0.5f);
            float cosr = std::cosf(eulerValue.x * 0.5f);

            // Pitch(Y-Axis Rotation)
            float sinp = std::sinf(eulerValue.y * 0.5f);
            float cosp = std::cosf(eulerValue.y * 0.5f);

            // Yaw(Z-Axis Rotation)
            float siny = std::sinf(eulerValue.z * 0.5f);
            float cosy = std::cosf(eulerValue.z * 0.5f);

            return DirectX::XMVectorSet
            (
                sinr * cosp * cosy - cosr * sinp * siny,
                cosr * sinp * cosy + sinr * cosp * siny,
                cosr * cosp * siny - sinr * sinp * cosy,
                cosr * cosp * cosy - sinr * sinp * siny
            );
        }

        DirectX::XMFLOAT4 XM_CALLCONV ToQuarternion(DirectX::FXMVECTOR eulerVector)
        {
            float roll = DirectX::XMVectorGetX(eulerVector);
            float pitch = DirectX::XMVectorGetY(eulerVector);
            float yaw = DirectX::XMVectorGetZ(eulerVector);

            // Roll(X-Axis Rotation)
            float sinr = std::sinf(roll * 0.5f);
            float cosr = std::cosf(roll * 0.5f);

            // Pitch(Y-Axis Rotation)
            float sinp = std::sinf(pitch * 0.5f);
            float cosp = std::cosf(pitch * 0.5f);

            // Yaw(Z-Axis Rotation)
            float siny = std::sinf(yaw * 0.5f);
            float cosy = std::cosf(yaw * 0.5f);

            return DirectX::XMFLOAT4
            (
                sinr * cosp * cosy - cosr * sinp * siny,
                cosr * sinp * cosy + sinr * cosp * siny,
                cosr * cosp * siny - sinr * sinp * cosy,
                cosr * cosp * cosy - sinr * sinp * siny
            );
        }

        DirectX::XMVECTOR XM_CALLCONV ToQuarternionVector(DirectX::FXMVECTOR eulerVector)
        {
            float roll = DirectX::XMVectorGetX(eulerVector);
            float pitch = DirectX::XMVectorGetY(eulerVector);
            float yaw = DirectX::XMVectorGetZ(eulerVector);

            // Roll(X-Axis Rotation)
            float sinr = std::sinf(roll * 0.5f);
            float cosr = std::cosf(roll * 0.5f);

            // Pitch(Y-Axis Rotation)
            float sinp = std::sinf(pitch * 0.5f);
            float cosp = std::cosf(pitch * 0.5f);

            // Yaw(Z-Axis Rotation)
            float siny = std::sinf(yaw * 0.5f);
            float cosy = std::cosf(yaw * 0.5f);

            return DirectX::XMVectorSet
            (
                sinr * cosp * cosy - cosr * sinp * siny,
                cosr * sinp * cosy + sinr * cosp * siny,
                cosr * cosp * siny - sinr * sinp * cosy,
                cosr * cosp * cosy - sinr * sinp * siny
            );
        }
    } // DirectXMathExtend
} // CommonUtility