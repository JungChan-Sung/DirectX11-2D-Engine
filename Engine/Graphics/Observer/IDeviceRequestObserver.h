/*********************************************************************
 * @file   IDeviceRequestObserver.h
 * @brief  DirectX11 장치 요청 관찰자 패턴
 * @date   2024-10-20
 *********************************************************************/

#pragma once

#include <d3d11.h>

namespace Engine
{
	namespace Graphics
	{
		namespace Observer
		{
			/**
			 * @brief DirectX11 장치 요청 관찰자 인터페이스
			 * @date 2024-10-20
			 * @interface
			 * 그래픽스 클래스
			 */
			__interface IDeviceRequestObserver
			{
			public: // ==== [DirectX11 장치 요청 관찰자] ====
				ID3D11Device* DeviceRequest();
			};

			/**
			 * @brief DirectX11 장치 요청 관찰대상 인터페이스
			 * @date 2024-10-20
			 * @interface
			 * 리소스 매니저 클래스
			 */
			__interface IDeviceRequestSubject
			{
			public: // ==== [DirectX11 장치 요청 관찰대상] ====
				void AttachDeviceRequestObserver(IDeviceRequestObserver* deviceRequestObserver);
				void DetachDeviceRequestObserver();
				ID3D11Device* DeviceRequestNotify();
			};
		} // Observer
	} // Graphics
} // Engine
