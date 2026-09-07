/*********************************************************************
 * @file   BufferType.h
 * @brief  버퍼 타입 모음
 * @date   2024-10-18
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Graphics
	{
		namespace Type
		{
			// 렌더 리소스 버퍼 타입
			enum class RenderResourceBufferType
			{
				SPRITE = 0,	// 스프라이트 리소스
				END
			};

			// 정점 셰이더 상수 버퍼 타입
			enum class VertexShaderConstantBufferType
			{
				RENDER_MATRIX = 0,	// 렌더 행렬
				END
			};

			// 픽셀 셰이더 상수 버퍼 타입
			enum class PixelShaderConstantBufferType
			{
				FONT_COLOR = 0,	// 폰트 색상
				END
			};
		} // Type
	} // Graphics
} // Engine
