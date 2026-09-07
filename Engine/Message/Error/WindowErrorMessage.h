/*********************************************************************
 * @file   WindowErrorMessage.h
 * @brief  윈도우(창) 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// 윈도우 컨테이너 Null 포인터 에러 메시지
			constexpr const char* WINDOW_CONTAINER_NULL_POINTER = "Critical Error : Pointer To Window Container Is Null.";
			// 윈도우 생성 함수 실패 에러 메시지
			constexpr const wchar_t* CREATE_WINDOW_EX_W_FAILURE = L"Failed To CreateWindowExW : ";
		} // Error
	} // Message
} // Engine