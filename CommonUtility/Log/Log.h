#pragma once

#include <Windows.h>
#include "CommonUtility/Exception/COMException.h"

namespace CommonUtility
{
	/**
	 * @brief 디버깅용 로그 Static 클래스
	 * @date 2024-10-03
	 * @class
	 * 에러 메시지 박스 출력 및 디버그창 로그 출력
	 */
	class Log
	{
	public: // ==== [에러 메시지] ====
		/**
		 * @brief 에러 메시지 박스 표시
		 * @param message : 에러 메시지 내용
		 */
		static void ErrorMessage(std::string message);
		/**
		 * @brief 에러 메시지 박스 표시
		 * @param hr : 윈도우 오류 및 경고 결과
		 * @param message : 에러 메시지 내용
		 */
		static void ErrorMessage(HRESULT hr, std::string message);
		/**
		 * @brief 에러 메시지 박스 표시
		 * @param hr : 윈도우 오류 및 경고 결과
		 * @param message : 에러 메시지 내용
		 */
		static void ErrorMessage(HRESULT hr, std::wstring message);
		/**
		 * @brief 에러 메시지 박스 표시
		 * @param exception : COM 예외처리 객체
		 */
		static void ErrorMessage(COMException& exception);

	public: // ==== [디버그창 출력] ====
		/**
		 * @brief 디버그 내용 출력
		 * @param str : 디버그 내용 문자열
		 */
		static void DebugOutput(LPCSTR str);
		/**
		 * @brief 디버그 내용 출력
		 * @param str : 디버그 내용 와이드 문자열
		 */
		static void DebugOutput(LPCWSTR str);
	};
} // CommonUtility