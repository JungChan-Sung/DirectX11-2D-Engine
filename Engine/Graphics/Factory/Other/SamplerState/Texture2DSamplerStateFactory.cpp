#include "Texture2DSamplerStateFactory.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			ID3D11SamplerState* Texture2DSamplerStateFactory::Create(ID3D11Device* device)
			{
				CD3D11_SAMPLER_DESC texture2DSamplerDescription(D3D11_DEFAULT);
				texture2DSamplerDescription.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
				texture2DSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
				texture2DSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
				texture2DSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

				// 샘플러 상태 생성
				ID3D11SamplerState* texture2DSamplerState;
				HRESULT hr = device->CreateSamplerState(&texture2DSamplerDescription, &texture2DSamplerState);
				if (FAILED(hr))
				{
					CommonUtility::SafeDelete(texture2DSamplerState);
					CommonUtility::Log::ErrorMessage(hr, Message::Error::CREATE_TEXTURE2D_SAMPLER_STATE_FAILURE);
					return nullptr;
				}

				return texture2DSamplerState;
			}
		} // Factory
	} // Graphics
} // Engine