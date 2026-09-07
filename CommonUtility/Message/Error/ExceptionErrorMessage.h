/*********************************************************************
 * @file   ExceptionErrorMessage.h
 * @brief  예외처리 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace CommonUtility
{
	namespace Message
	{
		namespace Error
		{
			constexpr const wchar_t* EXCEPTION_MESSAGE = L"Message : ";
			constexpr const wchar_t* EXCEPTION_FILE = L"\nFile : ";
			constexpr const wchar_t* EXCEPTION_FUNCTION = L"\nFunction : ";
			constexpr const wchar_t* EXCEPTION_LINE = L"\nLine : ";
		} // Error
	} // Message
} // CommonUtlity
