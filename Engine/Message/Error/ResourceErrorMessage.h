/*********************************************************************
 * @file   ResourceErrorMessage.h
 * @brief  리소스 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// 상수 버퍼 메모리 잠금 실패 에러 메시지
			constexpr const char* CONSTANT_BUFFER_MAP_FAILURE = "Failed To Map(Memory Lock) Constant Buffer.";
			// 정점 셰이더 컴파일 실패 에러 메시지
			constexpr const wchar_t* VERTEX_SHADER_COMPILE_FAILURE = L"Failed To Compile Vertex Shader : ";
			// 정점 셰이더 생성 실패 에러 메시지
			constexpr const wchar_t* CREATE_VERTEX_SHADER_FAILURE = L"Failed To Create Vertex Shader : ";
			// 입력 레이아웃 생성 실패 에러 메시지
			constexpr const char* CREATE_INPUT_LAYOUT_FAILURE = "Failed To Create Input Layout.";
			// 픽셀 셰이더 컴파일 실패 에러 메시지
			constexpr const wchar_t* PIXEL_SHADER_COMPILE_FAILURE = L"Failed To Compile Pixel Shader : ";
			// 픽셀 셰이더 생성 실패 에러 메시지
			constexpr const wchar_t* CREATE_PIXEL_SHADER_FAILURE = L"Failed To Create Pixel Shader : ";
			// 셰이더 리소스 뷰 생성 실패 에러 메시지
			constexpr const char* CREATE_SHADER_RESOURCE_VIEW_FAILURE = "Failed To Create Shader Resource View.";
			// 색상 텍스처 초기화 실패 에러 메시지
			constexpr const char* INITIALIZE_TEXTURE_COLOR_DATA_FAILURE = "Failed To Initialize Texture From Color Data.";
			// 색상 텍스처 셰이더 리소스 뷰 생성 실패 에러 메시지
			constexpr const char* CREATE_SHADER_RESOURCE_VIEW_TEXTURE_COLOR_DATA_FAILURE = "Failed To Create Resource View From Texture Generated From Color Data.";
		} // Error
	} // Message
} // Engine