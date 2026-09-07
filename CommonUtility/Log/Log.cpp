#include "Log.h"
#include "CommonUtility/Message/Error/LogErrorMessage.h"
#include "CommonUtility/Constant/CommonConstants.h"

namespace CommonUtility
{
	void Log::ErrorMessage(std::string message)
	{
		MessageBoxA(nullptr, message.c_str(), Message::Error::LOG_ERROR_TITLE, MB_ICONERROR);
	}

	void Log::ErrorMessage(HRESULT hr, std::string message)
	{
		ErrorMessage(hr, StringUtility::ConvertToWideString(message));
	}

	void Log::ErrorMessage(HRESULT hr, std::wstring message)
	{
		_com_error error(hr);
		std::wstring log = message + Constant::RETURN_WIDE_STRING + error.ErrorMessage();
		MessageBoxW(nullptr, log.c_str(), Message::Error::LOG_ERROR_WIDE_TITLE, MB_ICONERROR);
	}

	void Log::ErrorMessage(COMException& exception)
	{
		MessageBoxW(nullptr, exception.What(), Message::Error::LOG_ERROR_WIDE_TITLE, MB_ICONERROR);
	}

	void Log::DebugOutput(LPCSTR str)
	{
		// OutputDebugString() : 문자열을 [출력]창에 출력한다
		OutputDebugStringA(str);
	}

	void Log::DebugOutput(LPCWSTR str)
	{
		OutputDebugStringW(str);
	}
} // CommonUtility