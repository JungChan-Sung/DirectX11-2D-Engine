#pragma once

#include <string>

namespace CommonUtility
{
	/**
	 * @brief 문자열 유틸리티 클래스
	 * @date 2024-10-13
	 * @class
	 * 파일 경로 및 파일 확장자 반환
	 */
	class StringUtility
	{
	public: // ==== [문자열] ====
		/**
		 * @brief 문자열을 와이드 문자열로 변환
		 * @param str : 변환할 문자열
		 * @return 변한된 와이드 문자열
		 */
		static std::wstring ConvertToWideString(const std::string& str);
		/**
		 * @brief 문자열을 와이드 문자열로 변환
		 * @date 2024-10-13
		 * @param wstr : 변환할 와이드 문자열
		 * @return 변한된 문자열
		 */
		static std::string ConvertToString(const std::wstring& wstr);

	public: // ==== [파일] ====
		/**
		 * @brief 현재 파일 폴더의 경로 반환
		 * @details
		 * 현재 파일 이름을 제외한 폴더의 경로를 반환
		 * @param filePath : 현재 파일 경로
		 * @return 폴더 경로
		 */
		static std::string GetDirectoryFromPath(const std::string& filePath);
		static std::wstring GetDirectoryFromPath(const std::wstring& filePath);
		// 파일 확장자 반환
		/**
		 * @brief  파일 확장자 반환
		 * @details
		 * 현재 파일 이름에서 파일의 확장자만 반환
		 * @param fileName : 파일 이름
		 * @return 파일 확장자
		 */
		static std::string GetFileExtension(const std::string& fileName);
		static std::string GetFileExtension(const std::wstring& fileName);
	};
} // CommonUtility