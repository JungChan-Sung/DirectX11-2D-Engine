#include "StringUtility.h"
#include "CommonUtility/Constant/CommonConstants.h"

namespace CommonUtility
{
	std::wstring StringUtility::ConvertToWideString(const std::string& str)
	{
		// C++ 17 부터 동작하지 않음
		// std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
		// return converter.from_bytes(str);
		return std::wstring(str.begin(), str.end());
	}

	std::string StringUtility::ConvertToString(const std::wstring& wstr)
	{
		// C++ 17 부터 동작하지 않음
		// std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
		// return converter.to_bytes(wstr);
		// C4244 경고 발생
		return std::string(wstr.begin(), wstr.end());
	}

	std::string StringUtility::GetDirectoryFromPath(const std::string& filePath)
	{
		std::size_t offset1 = filePath.find_last_of(Constant::BACK_SLASH);	// find_last_of() : 해당 문자의 위치를 찾지 못하면 std::string::npos를 반환
		std::size_t offset2 = filePath.find_last_of(Constant::SLASH);		// 문자를 찾으면 배열상의 인덱스를 반환

		if ((offset1 == std::string::npos) && (offset2 == std::string::npos))
			return Constant::EMPTY_STRING;

		if (offset1 == std::string::npos)
			return std::string(filePath.substr(0u, offset2));

		if (offset2 == std::string::npos)
			return std::string(filePath.substr(0u, offset1));

		// 두 offset값이 다 존재할 경우, 더 큰 값을 사용
		return std::string(filePath.substr(0u, std::max(offset1, offset2)));
	}

	std::wstring StringUtility::GetDirectoryFromPath(const std::wstring& filePath)
	{
		std::size_t offset1 = filePath.find_last_of(Constant::BACK_SLASH);	// find_last_of() : 해당 문자의 위치를 찾지 못하면 std::string::npos를 반환
		std::size_t offset2 = filePath.find_last_of(Constant::SLASH);	// 문자를 찾으면 배열상의 인덱스를 반환

		if ((offset1 == std::string::npos) && (offset2 == std::string::npos))
			return Constant::EMPTY_WIDE_STRING;

		if (offset1 == std::string::npos)
			return std::wstring(filePath.substr(0u, offset2));

		if (offset2 == std::string::npos)
			return std::wstring(filePath.substr(0u, offset1));

		// 두 offset값이 다 존재할 경우, 더 큰 값을 사용
		return std::wstring(filePath.substr(0u, std::max(offset1, offset2)));
	}

	std::string StringUtility::GetFileExtension(const std::string& fileName)
	{
		std::size_t offset = fileName.find_last_of(Constant::PERIOD);

		if (offset == std::string::npos)
			return Constant::EMPTY_STRING;

		return std::string(fileName.substr(offset + 1u));
	}

	std::string StringUtility::GetFileExtension(const std::wstring& fileName)
	{
		return GetFileExtension(ConvertToString(fileName));
	}
} // CommonUtility