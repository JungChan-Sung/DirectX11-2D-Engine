#pragma once

#include <comdef.h>
#include "CommonUtility/Utility/StringUtility.h"
#include "CommonUtility/Constant/CommonConstants.h"
#include "CommonUtility/Message/Error/ExceptionErrorMessage.h"

namespace CommonUtility
{
	/**
	 * @brief COM(Component Object Model) 예외처리 클래스
	 * @date 2024-10-03
	 * @class
	 * 마이크로 소프트 COM 객체 예외처리용
	 */
	class COMException
	{
	private: // ==== [구성 요소] ====
		std::wstring errorMessage; // 에러 메시지 와이드 문자열

	public: // ==== [생성자] ====
		/**
		 * @details
		 * 오류난 메시지, 파일, 함수, 코드 라인을 메시지 박스로 띄워줌
		 * @param hr : 윈도우 오류 및 경고 결과
		 * @param message : 메시지 내용
		 * @param file : 오류난 파일 이름
		 * @param function : 오류난 함수 이름
		 * @param line : 오류난 코드 라인
		 */
		COMException(HRESULT hr, const std::string& message, const std::string& file, const std::string& function, int line)
		{
			_com_error error(hr);
			errorMessage = Message::Error::EXCEPTION_MESSAGE + StringUtility::ConvertToWideString(message) + Constant::RETURN_WIDE_STRING;
			errorMessage += error.ErrorMessage();
			errorMessage += Message::Error::EXCEPTION_FILE + StringUtility::ConvertToWideString(file);
			errorMessage += Message::Error::EXCEPTION_FUNCTION + StringUtility::ConvertToWideString(function);
			errorMessage += Message::Error::EXCEPTION_LINE + std::to_wstring(line);
		}
		COMException(HRESULT hr, const std::wstring& message, const std::string& file, const std::string& function, int line)
		{
			_com_error error(hr);
			errorMessage = Message::Error::EXCEPTION_MESSAGE + message + Constant::RETURN_WIDE_STRING;
			errorMessage += error.ErrorMessage();
			errorMessage += Message::Error::EXCEPTION_FILE + StringUtility::ConvertToWideString(file);
			errorMessage += Message::Error::EXCEPTION_FUNCTION + StringUtility::ConvertToWideString(function);
			errorMessage += Message::Error::EXCEPTION_LINE + std::to_wstring(line);
		}

	public: // ==== [What] ====
		/** @return 에러 메시지를 C언어 문자열로 변환된 문자열 */
		LPCWSTR What() const
		{
			return errorMessage.c_str();
		}
	};

	/**
	 * 매크로로 정의해야 알맞게 동작함
	 * __FILE과 __LINE__ 동적 매크로가 함수로 되어있으면 작동하지 않음
	 * @brief COM 에러 실패시 예외처리 및 메시지 출력 매크로
	 * @param hr : 윈도우 오류 및 경고 결과
	 * @param message : 메시지 내용
	 */
	inline void ComErrorFailed(HRESULT hr, const std::string& message)
	{
		if (FAILED(hr))
			throw COMException(hr, message, __FILE__, __FUNCTION__, __LINE__);
	}
	inline void ComErrorFailed(HRESULT hr, const std::wstring& message)
	{
		if (FAILED(hr))
			throw COMException(hr, message, __FILE__, __FUNCTION__, __LINE__);
	}

#define COM_ERROR_IF_FAILED(hr, msg) if(FAILED(hr)) throw COMException(hr, msg, __FILE__, __FUNCTION__, __LINE__)
} // CommonUtility