#pragma once

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			// 부호없는 1바이트 자료형
			using Byte = unsigned char;
			// 부호없는 4바이트 자료형
			using Byte4 = unsigned int;

			/**
			 * @brief 32비트(4바이트) 색상 클래스
			 * @date 2024-10-13
			 * @class
			 * 32비트 색상을 나타내며 계산 및 저장을 하는 클래스
			 */
			class Color32Bit
			{
			private: // ==== [선언] ====
				// 32비트 색상 상수
				enum Color32BitConstant : Byte4
				{
					COLOR_TYPE_COUNT = 4u,	// 색상 종류 개수(RGBA)
					RED = 0u,				// Red Index
					GREEN = 1u,				// Green Index
					BLUE = 2u,				// Blue Index
					ALPHA = 3u,				// Alpha Index
				};

			private: // ==== [구성 요소] ====
				// 4바이트 배열과 unsigned int(4바이트)형을 메모리 공유
				union
				{
					Byte rgba[COLOR_TYPE_COUNT]; // 각 항목별 (0 ~ 255)
					Byte4 color;
				};

			public: // ==== [생성자] ====
				Color32Bit();
				/**
				 * @param colorValue : 4바이트 색상 값(RRGGBBAA)
				 * @example
				 * Color32Bit(0xFFFFFFFF); // (255, 255, 255, 255)
				 */
				Color32Bit(Byte4 colorValue);
				/**
				 * @param r : Red 색상 값(0 ~ 255)
				 * @param g : Green 색상 값(0 ~ 255)
				 * @param b : Blue 색상 값(0 ~ 255)
				 */
				Color32Bit(Byte r, Byte g, Byte b);
				/**
				 * @param r : Red 색상 값(0 ~ 255)
				 * @param g : Green 색상 값(0 ~ 255)
				 * @param b : Blue 색상 값(0 ~ 255)
				 * @param a : Alpha 투명도 값(0 ~ 255)
				 */
				Color32Bit(Byte r, Byte g, Byte b, Byte a);
				/** @param copyColor32Bit : 복사 생성할 32비트 색상 */
				Color32Bit(const Color32Bit& copyColor32Bit);

			public: // ==== [연산자 재정의] ====
				Color32Bit& operator=(const Color32Bit& rhs);
				bool operator==(const Color32Bit& rhs);
				bool operator!=(const Color32Bit& rhs);

			public: // ==== [Set] ====
				/**
				 * @param colorValue : 4바이트 색상 값(RRGGBBAA)
				 * @example
				 * SetColor(0xFFFFFFFF); // (255, 255, 255, 255)
				 */
				void SetColor(Byte4 colorValue);
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
				void SetColor(Byte r, Byte g, Byte b, Byte a);
				/** @param r : Red 색상 값(0 ~ 255) */
				void SetR(Byte r);
				/** @param g : Green 색상 값(0 ~ 255) */
				void SetG(Byte g);
				/** @param b : Blue 색상 값(0 ~ 255) */
				void SetB(Byte b);
				/** @param a : Alpha 투명도 값(0 ~ 255) */
				void SetA(Byte a);

			public: // ==== [Get] ====
				/** @return 색상 비트 값 */
				Byte4 GetColorBit() const;
				/** @return Red 색상 값(0 ~ 255) */
				Byte GetR() const;
				/** @return Green 색상 값(0 ~ 255) */
				Byte GetG() const;
				/** @return Blue 색상 값(0 ~ 255) */
				Byte GetB() const;
				/** @return Alpha 투명도 값(0 ~ 255) */
				Byte GetA() const;
			};
		} // Common
	} // Graphics
} // Engine
