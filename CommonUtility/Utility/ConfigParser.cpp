#include "ConfigParser.h"
#include <fstream>
#include "CommonUtility/Utility/StringUtility.h"
#include "CommonUtility/Log/Log.h"
#include "CommonUtility/Message/Error/ConfigParserErrorMessage.h"
#include "CommonUtility/Constant/CommonConstants.h"

namespace CommonUtility
{
	ConfigParser::ConfigParser()
	{
	}

	ConfigParser::~ConfigParser()
	{
		for (Config::iterator iterator = config.begin(); iterator != config.end(); ++iterator)
			iterator->second.clear();
		config.clear();
	}

	bool ConfigParser::Parse(const std::wstring& path)
	{
		std::wifstream file;
		file.open(path);
		if (!file.is_open()) // file.is_open == false
		{
			Log::ErrorMessage(Message::Error::CONFIG_FILE_OPEN_FAILURE);
			return false;
		}

		std::wstring line, section, key, value;
		std::size_t equalPosition;
		while (std::getline(file, line))
		{
			// 라인 공백 제거
			line = LineTrim(line);

			// 빈 라인이거나 주석(#, ;)이면 무시
			if (line.empty() || line[0] == Constant::HASH || line[0] == Constant::SEMI_COLON)
				continue;

			// 섹션 처리
			if (line.front() == Constant::LEFT_SQUARE_BRACKECT && line.back() == Constant::RIGHT_SQUARE_BRACKECT)
			{
				// [, ] 양끝을 제외한 부분 문자열
				section = line.substr(1u, line.length() - 2u);
				// 섹션 추가
				config.insert(std::make_pair(section, Section()));
			}
			// Key-Value 처리
			else
			{
				equalPosition = line.find(Constant::EQUAL_SIGN);
				if (equalPosition != std::string::npos)
				{
					// ' = '의 공백도 처리하여 Key, Value 값 삽입됨
					key = LineTrim(line.substr(0u, equalPosition));
					value = LineTrim(line.substr(equalPosition + 1u));

					// 문자열 값 따옴표 처리
					if ((value.front() == Constant::DOUBLE_QUOTE && value.back() == Constant::DOUBLE_QUOTE)
						|| (value.front() == Constant::SINGLE_QUOTE && value.back() == Constant::SINGLE_QUOTE))
					{
						value = value.substr(1u, value.length() - 2u);
					}
					// 참 여부 값
					else if (value == Constant::LOWER_CASE_TRUE_WIDE_STRING || value == Constant::LETTER_CASE_TRUE_WIDE_STRING)
					{
						value = Constant::TRUE_NUMBER_WIDE_STRING;
					}
					// 거짓 여부 값
					else if (value == Constant::LOWER_CASE_FALSE_WIDE_STRING || value == Constant::LETTER_CASE_FALSE_WIDE_STRING)
					{
						value = Constant::FALSE_NUMBER_WIDE_STRING;
					}

					// Key-Value 추가
					config[section][key] = value;
				}
			}
		}

		file.close();

		return true;
	}

	bool ConfigParser::Parse(const std::string& path)
	{
		return Parse(StringUtility::ConvertToWideString(path));
	}

	std::wstring ConfigParser::GetValue(const std::wstring& sectionName, const std::wstring& key)
	{
		if (config.find(sectionName) != config.end())
		{
			Section& section = config[sectionName];

			if (section.find(key) != section.end())
				return section[key];
		}

		return Constant::EMPTY_WIDE_STRING;
	}

	std::wstring ConfigParser::LineTrim(const std::wstring& line)
	{
		// 첫 부분과 마지막 부분에서 공백, 탭, 엔터가 아닌 문자 위치 찾기
		std::size_t first = line.find_first_not_of(Constant::SPACE_WIDE_STRING);
		std::size_t last = line.find_last_not_of(Constant::SPACE_WIDE_STRING);

		// 공백, 탭, 엔터 문자만 있는 경우
		if (first == std::wstring::npos || last == std::wstring::npos)
			return Constant::EMPTY_WIDE_STRING;

		// 양끝 공백을 제외한 문자열 반환
		return line.substr(first, last - first + 1u);
	}
} // CommonUtility
