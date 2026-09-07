/*********************************************************************
 * @file   FontColor.h
 * @brief  상수 버퍼용 폰트 색상
 * @date   2024-10-18
 *********************************************************************/
#pragma once

#include <DirectXMath.h>
// std::move
#include <utility>

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			struct FontColor
			{
			public: // ==== [구성 요소] ====
				DirectX::XMFLOAT4 color; // 색상

			public: // ==== [연산자 재정의] ====
				FontColor& operator=(const FontColor& rhs)
				{
					if (this == &rhs)
						return *this;

					color = rhs.color;

					return *this;
				}
				FontColor& operator=(FontColor&& rhs) noexcept
				{
					if (this != &rhs)
						color = std::exchange(rhs.color, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

					return *this;
				}

			public: // ==== [생성자] ====
				FontColor() : color(1.0f, 1.0f, 1.0f, 1.0f)
				{
				}
				/** @param copyFontColor : 복사 생성할 폰트 색상 */
				FontColor(const FontColor& copyFontColor) : color(copyFontColor.color)
				{
				}
				/** @param moveFontColor : 이동 생성할 폰트 색상 */
				FontColor(FontColor&& moveFontColor) noexcept : color(std::exchange(moveFontColor.color, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)))
				{
				}
			};
		} // Common
	} // Graphics
} // Engine