#include "Vertex.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			Vertex::Vertex() : position(0.0f, 0.0f)
			{
			}

			Vertex::Vertex(float x, float y) : position(x, y)
			{
			}

			VertexColor::VertexColor() : color(1.0f, 1.0f, 1.0f, 1.0f)
			{
			}

			VertexColor::VertexColor(float x, float y, float r, float g, float b, float a) : Vertex(x, y), color(r, g, b, a)
			{
			}

			VertexColor::VertexColor(float x, float y, float r, float g, float b) : VertexColor(x, y, r, g, b, 1.0f)
			{
			}

			VertexTexture::VertexTexture() : textureCoordinate(0.0f, 0.0f)
			{
			}

			VertexTexture::VertexTexture(float x, float y, float r, float g, float b, float a, float u, float v) : VertexColor(x, y, r, g, b, a), textureCoordinate(u, v)
			{
			}

			VertexTexture::VertexTexture(float x, float y, float r, float g, float b, float u, float v) : VertexTexture(x, y, r, g, b, 1.0f, u, v)
			{
			}

			VertexTexture::VertexTexture(float x, float y, float u, float v) : VertexTexture(x, y, 1.0f, 1.0f, 1.0f, 1.0f, u, v)
			{
			}
		} // Common
	} // Graphics
} // Engine
