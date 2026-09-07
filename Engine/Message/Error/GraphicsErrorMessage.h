/*********************************************************************
 * @file   GraphicsErrorMessage.h
 * @brief  그래픽스 에러 메시지 모음
 * @date   2024-11-04
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace Message
	{
		namespace Error
		{
			// DirectX 기반 팩토리 생성 실패 에러 메시지
			constexpr const char* CREATE_FACTORY_FAILURE = "Failed To Create Factory.";
			// DirectX 기반 어댑터 설정 실패 에러 메시지
			constexpr const char* SET_ADAPTERS_FAILURE = "Failed To Set Adapters.";
			// 모니터 출력 설정 실패 에러 메시지
			constexpr const char* SET_OUTPUT_FAILURE = "Failed To Set Outputs.";
			// 모니터 모드 획득 실패 에러 메시지
			constexpr const char* GET_DISPLAY_MODE_FAILURE = "Failed To Get Display Mode.";
			// 모니터 모드 개수 획득 실패 에러 메시지
			constexpr const char* GET_DISPLAY_MODE_COUNT_FAILURE = "Failed To Get Display Mode Count.";
			// 모니터 모드 리스트 획득 실패 에러 메시지
			constexpr const char* GET_DISPLAY_MODE_LIST_FAILURE = "Failed To Get Display Mode List.";
			// 그래픽 카드 속성 설정 실패 에러 메시지
			constexpr const char* SET_GRAPHICS_CARD_DESCRIPTION_FAILURE = "Failed to Set Graphics Card Description.";
			// DirectX 장치 및 스왑 체인 생성 실패 에러 메시지
			constexpr const char* CREATE_DEVICE_AND_SWAP_CHAIN_FAILURE = "Failed To Create Device And Swap Chain.";
			// 백 버퍼 설정 실패 에러 메시지
			constexpr const char* SET_BACK_BUFFER_FAILURE = "Failed To Set Back Buffer.";
			// 렌더 타겟 뷰 생성 실패 에러 메시지
			constexpr const char* CREATE_RENDER_TARGET_VIEW_FAILURE = "Failed To Create Render Target View.";
			// 래스터라이저 상태 생성 실패 에러 메시지
			constexpr const char* CREATE_RASTERIZER_STATE_FAILURE = "Failed To Create Rasterizer State";
			// 블렌드 상태 생성 실패 에러 메시지
			constexpr const char* CREATE_BLEND_STATE_FAILURE = "Failed To Create Blend State.";
		} // Error
	} // Message
} // Engine