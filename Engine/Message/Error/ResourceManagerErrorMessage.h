/*********************************************************************
 * @file   ResourceManagerErrorMessage.h
 * @brief  리소스 매니저 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// 정점 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_VERTEX_BUFFERS_FAILURE = "Failed To Create Vertex Buffers.";
			// 인덱스 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_INDEX_BUFFERS_FAILURE = "Failed To Create Index Buffers.";
			// 정점 셰이더 상수 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_VERTEX_SHADER_CONSTANT_BUFFERS_FAILURE = "Failed To Create Vertex Shader Constant Buffers.";
			// 픽셀 셰이더 상수 버퍼 생성 실패 에러 메시지
			constexpr const char* CREATE_PIXEL_SHADER_CONSTANT_BUFFERS_FAILURE = "Failed To Create Pixel Shader Constant Buffers.";
			// 셰이더 데이터 파일 열기 실패 에러 메시지
			constexpr const char* SHADER_DATA_FILE_OPEN_FAILURE = "Failed To Open Shader Data File.";
			// 셰이더 생성 실패 에러 메시지
			constexpr const char* CREATE_SHADERS_FAILURE = "Failed To Create Shaders.";
			// 텍스처 데이터 파일 열기 실패 에러 메시지
			constexpr const char* TEXTURE_DATA_FILE_OPEN_FAILURE = "Failed To Open Texture Data File.";
			// 샘플러 상태 생성 실패 에러 메시지
			constexpr const char* CREATE_SAMPLER_STATES_FAILURE = "Failed To Create Sampler States.";
			// 스프라이트 데이터 파일 열기 실패 에러 메시지
			constexpr const char* SPRITE_DATA_FILE_OPEN_FAILURE = "Failed To Open Sprite Data File.";
		} // Error
	} // Message
} // Engine