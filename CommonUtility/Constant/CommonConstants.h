/*********************************************************************
 * @file   CommonConstants.h
 * @brief  공용 상수 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace CommonUtility
{
	namespace Constant
	{
		constexpr char PERIOD = '.';
		constexpr char SLASH = '/';
		constexpr char BACK_SLASH = '\\';
		constexpr char HASH = '#';
		constexpr char SEMI_COLON = ';';
		constexpr char LEFT_SQUARE_BRACKECT = '[';
		constexpr char RIGHT_SQUARE_BRACKECT = ']';
		constexpr char EQUAL_SIGN = '=';
		constexpr char SINGLE_QUOTE = '\'';
		constexpr char DOUBLE_QUOTE = '\"';

		constexpr const char* EMPTY_STRING = "";
		constexpr const wchar_t* EMPTY_WIDE_STRING = L"";
		constexpr const wchar_t* SPACE_WIDE_STRING = L" \t\n\r";
		constexpr const wchar_t* LOWER_CASE_TRUE_WIDE_STRING = L"true";
		constexpr const wchar_t* LETTER_CASE_TRUE_WIDE_STRING = L"True";
		constexpr const wchar_t* UPPER_CASE_TRUE_WIDE_STRING = L"TRUE";
		constexpr const wchar_t* LOWER_CASE_FALSE_WIDE_STRING = L"false";
		constexpr const wchar_t* LETTER_CASE_FALSE_WIDE_STRING = L"False";
		constexpr const wchar_t* UPPER_CASE_FALSE_WIDE_STRING = L"FALSE";
		constexpr const wchar_t* TRUE_NUMBER_WIDE_STRING = L"1";
		constexpr const wchar_t* FALSE_NUMBER_WIDE_STRING = L"0";
		constexpr const wchar_t* RETURN_WIDE_STRING = L"\n";
	} // Constant
} // CommonUtility
