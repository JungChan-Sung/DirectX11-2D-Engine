/*********************************************************************
 * @file   ResourceFactoryErrorMessage.h
 * @brief  리소스 팩토리 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// 스프라이트 정점 셰이더 생성 실패 에러 메시지
			constexpr const char* CREATE_SPRITE_VERTEX_SHADER_FAILURE = "Failed To Create Sprite Vertex Shader.";
			// 스프라이트 정점 셰이더 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_SPRITE_VERTEX_SHADER_FAILURE = "Failed To Initialize Sprite Vertex Shader.";

			// 스프라이트 픽셀 셰이더 생성 실패 에러 메시지
			constexpr const char* CREATE_SPRITE_PIXEL_SHADER_FAILURE = "Failed To Create Sprite Pixel Shader.";
			// 스프라이트 픽셀 셰이더 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_SPRITE_PIXEL_SHADER_FAILURE = "Failed To Initialize Sprite Pixel Shader.";

			// 2D 텍스처 샘플러 상태 생성 실패 에러 메시지
			constexpr const char* CREATE_TEXTURE2D_SAMPLER_STATE_FAILURE = "Failed To Create Texture2D Sampler State.";

			// 스프라이트 정점 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_SPRITE_VERTEX_BUFFER_FAILURE = "Failed To Create Sprite Vertex Buffer.";
			// 스프라이트 정점 버퍼 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_SPRITE_VERTEX_BUFFER_FAILURE = "Failed To Initialize Sprite Vertex Buffer.";

			// 스프라이트 인덱스 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_SPRITE_INDEX_BUFFER_FAILURE = "Failed To Create Sprite Index Buffer.";
			// 스프라이트 인덱스 버퍼 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_SPRITE_INDEX_BUFFER_FAILURE = "Failed To Initialize Sprite Index Buffer.";

			// 렌더 행렬 상수 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_RENDER_MATRIX_CONSTANT_BUFFER_FAILURE = "Failed To Create Render Matrix Constant Buffer.";
			// 렌더 행렬 상수 버퍼 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_RENDER_MATRIX_CONSTANT_BUFFER_FAILURE = "Failed To Initialize Render Matrix Constant Buffer.";
			// 폰트 색상 상수 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_FONT_COLOR_CONSTANT_BUFFER_FAILURE = "Failed To Create Font Color Constant Buffer.";
			// 폰트 색상 상수 버퍼 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_FONT_COLOR_CONSTANT_BUFFER_FAILURE = "Failed To Initialize Font Color Constant Buffer.";
		} // Error
	} // Message
} // Engine