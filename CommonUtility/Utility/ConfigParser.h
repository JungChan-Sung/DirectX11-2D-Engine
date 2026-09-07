#pragma once

#include <string>
#include <unordered_map>

namespace CommonUtility
{
	/**
	 * @brief Config 파일 파싱 클래스
	 * @date 2024-10-05
	 * @class
	 * .cfg 파일을 읽어와 변수명과 값 파싱작업을 해줌
	 * string으로만 저장해두고 이후 사용할시 따로 클래스에서 변환 수행 바람
	 */
	class ConfigParser
	{
	private: // ==== [선언] ====
		// <변수명, 값>
		using Section = std::unordered_map<std::wstring, std::wstring>;
		// <섹션명, 섹션값>
		using Config = std::unordered_map<std::wstring, Section>;

	private: // ==== [구성 요소] ====
		Config config;	// 섹션을 저장할 Key-Value 맵

	public: // ==== [생성자 및 소멸자] ====
		ConfigParser();
		~ConfigParser();

	public: // ==== [Config 파서 동작] ====
		/**
		 * @brief Config 파일 파싱 작업 수행
		 * @details
		 * 파일 입출력으로 Config 파일을 불러온후 파싱 작업을 수행함
		 * @param path : Config 파일 경로
		 * @return 파싱 성공 여부
		 */
		bool Parse(const std::wstring& path);
		bool Parse(const std::string& path);

	public: // ==== [Get] ====
		/**
		 * @param sectionName : 값을 가져올 섹션 이름
		 * @param key : 값을 가져올 변수명
		 * @return 키에 해당하는 값(empty : 값을 찾지 못함)
		 */
		std::wstring GetValue(const std::wstring& sectionName, const std::wstring& key);

	private: // ==== [Config 파서 보조] ====
		/**
		 * @brief 라인 양끝 공백 제거
		 * @param line : 공백 제거할 라인
		 * @return 공백 제거된 라인
		 */
		std::wstring LineTrim(const std::wstring& line);
	};
} // CommonUtility
