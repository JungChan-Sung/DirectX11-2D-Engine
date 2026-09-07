#include "Color.h"
#include "Engine/Config/EngineConfig.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			Color::Color() : color(0.0f, 0.0f, 0.0f, 0.0f)
			{
			}

			Color::Color(DirectX::FXMVECTOR colorVector)
			{
				DirectX::XMStoreFloat4(&color, colorVector);
			}

			Color::Color(float r, float g, float b) : Color(r, g, b, 1.0f)
			{
			}

			Color::Color(float r, float g, float b, float a) : color(r, g, b, a)
			{
			}

			Color::Color(Byte r, Byte g, Byte b) : Color(r, g, b, (Byte)255)
			{
			}

			Color::Color(Byte r, Byte g, Byte b, Byte a)
			{
				SetColor(r, g, b, a);
			}

			Color::Color(const Color32Bit& color32Bit)
			{
				SetColor(color32Bit);
			}

			Color::Color(const Color& copyColor) : color(copyColor.color)
			{
			}

			Color& Color::operator=(const Color& rhs)
			{
				if (this == &rhs)
					return *this;

				color = rhs.color;

				return *this;
			}

			bool XM_CALLCONV Color::operator==(const Color& rhs)
			{
				return DirectX::XMColorEqual(DirectX::XMLoadFloat4(&color), DirectX::XMLoadFloat4(&rhs.color));
			}

			bool Color::operator!=(const Color& rhs)
			{
				return !(*this == rhs);
			}

			void Color::SetColor(const DirectX::XMFLOAT4& colorValue)
			{
				color = colorValue;
			}

			void XM_CALLCONV Color::SetColor(DirectX::FXMVECTOR colorVector)
			{
				DirectX::XMStoreFloat4(&color, colorVector);
			}

			void Color::SetColor(const Color32Bit& color32Bit)
			{
				SetColor(color32Bit.GetR(), color32Bit.GetG(), color32Bit.GetB(), color32Bit.GetA());
			}

			void Color::SetColor(float r, float g, float b)
			{
				SetColor(r, g, b, 1.0f);
			}

			void XM_CALLCONV Color::SetColor(float r, float g, float b, float a)
			{
				DirectX::XMStoreFloat4(&color, DirectX::XMVectorSet(r, g, b, a));
			}

			void Color::SetColor(Byte r, Byte g, Byte b)
			{
				SetColor(r, g, b, (Byte)255);
			}

			void Color::SetColor(Byte r, Byte g, Byte b, Byte a)
			{
				float inverseByte = EngineConfig::Instance()->InverseByte();
				SetColor(DirectX::XMVectorSet(r * inverseByte, g * inverseByte, b * inverseByte, a * inverseByte));
			}

			void Color::SetR(float r)
			{
				color.x = r;
			}

			void Color::SetG(float g)
			{
				color.y = g;
			}

			void Color::SetB(float b)
			{
				color.z = b;
			}

			void Color::SetA(float a)
			{
				color.w = a;
			}

			DirectX::XMFLOAT4 Color::GetColor() const
			{
				return color;
			}

			DirectX::XMVECTOR XM_CALLCONV Color::GetColorVector() const
			{
				return DirectX::XMLoadFloat4(&color);
			}

			float Color::GetR() const
			{
				return color.x;
			}

			float Color::GetG() const
			{
				return color.y;
			}

			float Color::GetB() const
			{
				return color.z;
			}

			float Color::GetA() const
			{
				return color.w;
			}
		} // Common
	} // Graphics
} // Engine
