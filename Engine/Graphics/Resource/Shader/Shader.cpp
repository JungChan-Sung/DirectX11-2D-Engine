#include "Shader.h"
// std::move
#include <utility>

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			Shader::Shader(VertexShader* vertexShader, PixelShader* pixelShader)
				: vertexShader(vertexShader), pixelShader(pixelShader)
			{
			}

			Shader::Shader(Shader&& moveShader) noexcept
				: vertexShader(std::exchange(moveShader.vertexShader, nullptr)),
				pixelShader(std::exchange(moveShader.pixelShader, nullptr))
			{
			}

			Shader::~Shader()
			{
				Release();
			}

			Shader& Shader::operator=(Shader&& rhs) noexcept
			{
				if (this != &rhs)
				{
					vertexShader = std::exchange(rhs.vertexShader, nullptr);
					pixelShader = std::exchange(rhs.pixelShader, nullptr);
				}

				return *this;
			}

			void Shader::Release()
			{
				pixelShader = nullptr;
				vertexShader = nullptr;
			}

			VertexShader* Shader::GetVertexShader() const
			{
				return vertexShader;
			}

			PixelShader* Shader::GetPixelShader() const
			{
				return pixelShader;
			}
		} // Resource
	} // Graphics
} // Engine
