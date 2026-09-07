/*********************************************************************
 * @file   InputErrorMessage.h
 * @brief  입력 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// Raw 입력 디바이스 등록 실패 에러 메시지
			constexpr const char* RAW_INPUT_DEVICE_REGISTER_FAILURE = "Failed To Register Raw Input Devices.";
		} // Error
	} // Message
} // Engine