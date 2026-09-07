/*********************************************************************
 * @file   ManagerErrorMessage.h
 * @brief  매니저 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// 현재 씬 로드 실패 에러 메시지
			constexpr const char* CURRENT_SCENE_LOAD_FAILURE = "Failed To Current Scene Load.";
			// 씬 로드 실패 에러 메시지
			constexpr const char* SCENE_LOAD_FAILURE = "Failed To Scene Load.";
		} // Error
	} // Message
} // Engine