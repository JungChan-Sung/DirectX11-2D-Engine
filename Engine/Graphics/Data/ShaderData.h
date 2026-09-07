/*********************************************************************
 * @file   ShaderData.h
 * @brief  셰이더 데이터 구조체 정의
 * @date   2024-10-20
 *********************************************************************/

#pragma once

#include <string>
#include <unordered_map>

namespace Engine
{
	namespace Graphics
	{
		namespace Data
		{
			/**
			 * @brief 셰이더 데이터
			 * @date 2024-10-20
			 */
			struct ShaderData
			{
			public: // ==== [구성 요소] ====
				std::string path;	// 경로
			};

			// <셰이더 이름, 셰이더 데이터 포인터>
			using ShaderDataTable = std::unordered_map<std::string, Data::ShaderData*>;
			// <셰이더 그룹 이름, 셰이더 데이터 테이블>
			using ShaderDataGroupTable = std::unordered_map<std::string, ShaderDataTable>;
		} // Data
	} // Graphics
} // Engine
