#include "Color32Bit.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			Color32Bit::Color32Bit() : color(0x00000000)
			{
			}

			Color32Bit::Color32Bit(Byte4 colorValue) : color(colorValue)
			{
			}

			Color32Bit::Color32Bit(Byte r, Byte g, Byte b) : Color32Bit(r, g, b, (Byte)255)
			{
			}

			Color32Bit::Color32Bit(Byte r, Byte g, Byte b, Byte a)
			{
				rgba[RED] = r;
				rgba[GREEN] = g;
				rgba[BLUE] = b;
				rgba[ALPHA] = a;
			}

			Color32Bit::Color32Bit(const Color32Bit& copyColor32Bit) : color(copyColor32Bit.color)
			{
			}

			Color32Bit& Color32Bit::operator=(const Color32Bit& rhs)
			{
				if (this == &rhs)
					return *this;

				color = rhs.color;

				return *this;
			}

			bool Color32Bit::operator==(const Color32Bit& rhs)
			{
				return (color == rhs.color);
			}

			bool Color32Bit::operator!=(const Color32Bit& rhs)
			{
				return !((*this) == rhs);
			}

			void Color32Bit::SetColor(Byte4 colorValue)
			{
				color = colorValue;
			}

			void Color32Bit::SetColor(Byte r, Byte g, Byte b)
			{
				SetColor(r, g, b, (Byte)255);
			}

			void Color32Bit::SetColor(Byte r, Byte g, Byte b, Byte a)
			{
				rgba[RED] = r;
				rgba[GREEN] = g;
				rgba[BLUE] = b;
				rgba[ALPHA] = a;
			}

			void Color32Bit::SetR(Byte r)
			{
				rgba[RED] = r;
			}

			void Color32Bit::SetG(Byte g)
			{
				rgba[GREEN] = g;
			}

			void Color32Bit::SetB(Byte b)
			{
				rgba[BLUE] = b;
			}

			void Color32Bit::SetA(Byte a)
			{
				rgba[ALPHA] = a;
			}

			Byte4 Color32Bit::GetColorBit() const
			{
				return color;
			}

			Byte Color32Bit::GetR() const
			{
				return rgba[RED];
			}

			Byte Color32Bit::GetG() const
			{
				return rgba[GREEN];
			}

			Byte Color32Bit::GetB() const
			{
				return rgba[BLUE];
			}

			Byte Color32Bit::GetA() const
			{
				return rgba[ALPHA];
			}
		} // Common
	} // Graphics
} // Engine
