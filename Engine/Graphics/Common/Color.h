#pragma once

#include <DirectXMath.h>
#include "Color32Bit.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			/**
			 * @brief 색상 클래스
			 * @date 2024-10-13
			 * @class
			 * 부동 소수점 색상 데이터를 계산 및 저장하는 클래스
			 */
			class Color
			{
			private: // ==== [구성 요소] ====
				// 각 항목마다 (0.0f ~ 1.0f)
				DirectX::XMFLOAT4 color; // x : r, y : g, z : b, w : a
				
			public: // ==== [생성자] ====
				Color();
				/** @param colorVector : 색상 벡터 */
				Color(DirectX::FXMVECTOR colorVector);
				/**
				 * @param r : Red 색상 값(0.0f ~ 1.0f)
				 * @param g : Green 색상 값(0.0f ~ 1.0f)
				 * @param b : Blue 색상 값(0.0f ~ 1.0f)
				 */
				Color(float r, float g, float b);
				/**
				 * @param r : Red 색상 값(0.0f ~ 1.0f)
				 * @param g : Green 색상 값(0.0f ~ 1.0f)
				 * @param b : Blue 색상 값(0.0f ~ 1.0f)
				 * @param a : Alpha 투명도 값(0.0f ~ 1.0f)
				 */
				Color(float r, float g, float b, float a);
				/**
				 * @param r : Red 색상 값(0 ~ 255)
				 * @param g : Green 색상 값(0 ~ 255)
				 * @param b : Blue 색상 값(0 ~ 255)
				 */
				Color(Byte r, Byte g, Byte b);
				/**
				 * @param r : Red 색상 값(0 ~ 255)
				 * @param g : Green 색상 값(0 ~ 255)
				 * @param b : Blue 색상 값(0 ~ 255)
				 * @param a : Alpha 투명도 값(0 ~ 255)
				 */
				Color(Byte r, Byte g, Byte b, Byte a);
				/** @param color32Bit : 32비트 색상 */
				Color(const Color32Bit& color32Bit);
				/** @param copyColor : 복사 생성할 색상 */
				Color(const Color& copyColor);

			public: // ==== [연산자 재정의] ====
				Color& operator=(const Color& rhs);
				bool XM_CALLCONV operator==(const Color& rhs);
				bool operator!=(const Color& rhs);

			public: // ==== [Set] ====
				/** @param colorValue : 색상 값 */
				void SetColor(const DirectX::XMFLOAT4& colorValue);
				/** @param colorVector : 색상 벡터 */
				void XM_CALLCONV SetColor(DirectX::FXMVECTOR colorVector);
				/** @param color32Bit : 32비트 색상 */
				void SetColor(const Color32Bit& color32Bit);
				/**
				 * @param r : Red 색상 값(0.0f ~ 1.0f)
				 * @param g : Green 색상 값(0.0f ~ 1.0f)
				 * @param b : Blue 색상 값(0.0f ~ 1.0f)
				 */
				void SetColor(float r, float g, float b);
				/**
				 * @param r : Red 색상 값(0.0f ~ 1.0f)
				 * @param g : Green 색상 값(0.0f ~ 1.0f)
				 * @param b : Blue 색상 값(0.0f ~ 1.0f)
				 * @param a : Alpha 투명도 값(0.0f ~ 1.0f)
				 */
				void XM_CALLCONV SetColor(float r, float g, float b, float a);
				/**
				 * @param r : Red 색상 값(0 ~ 255)
				 * @param g : Green 색상 값(0 ~ 255)
				 * @param b : Blue 색상 값(0 ~ 255)
				 */
				void SetColor(Byte r, Byte g, Byte b);
				/**
				 * @param r : Red 색상 값(0 ~ 255)
				 * @param g : Green 색상 값(0 ~ 255)
				 * @param b : Blue 색상 값(0 ~ 255)
				 * @param a : Alpha 투명도 값(0 ~ 255)
				 */
				void XM_CALLCONV SetColor(Byte r, Byte g, Byte b, Byte a);
				/** @param r : Red 색상 값 */
				void SetR(float r);
				/** @param g : Green 색상 값 */
				void SetG(float g);
				/** @param b : Blue 색상 값 */
				void SetB(float b);
				/** @param a : Alpha 투명도 값 */
				void SetA(float a);

			public: // ==== [Get] ====
				/** @return 색상 값 */
				DirectX::XMFLOAT4 GetColor() const;
				/** @return 색상 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetColorVector() const;
				/** @return Red 색상 값 */
				float GetR() const;
				/** @return Green 색상 값 */
				float GetG() const;
				/** @return Blue 색상 값 */
				float GetB() const;
				/** @return Alpha 투명도 값 */
				float GetA() const;
			};
		} // Common
	} // Graphics
} // Engine
