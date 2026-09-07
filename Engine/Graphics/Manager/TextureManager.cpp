#include "TextureManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Config/ResourceConfig.h"
#include "Engine/Message/Error/ResourceManagerErrorMessage.h"
#include "Engine/Graphics/Constant/TextureJsonConstants.h"
#include "Engine/Graphics/Factory/Other/SamplerState/SamplerStateFactoryManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			TextureManager::TextureManager()
			{
			}

			TextureManager::~TextureManager()
			{
				Release();
			}

			bool TextureManager::Initialize(ID3D11Device* device)
			{
				std::ifstream textureDataFile(ResourceConfig::Instance()->TextureDataFilePath());
				if (!textureDataFile.is_open()) // textureDataFile.is_open == false
				{
					CommonUtility::Log::ErrorMessage(Message::Error::TEXTURE_DATA_FILE_OPEN_FAILURE);
					return false;
				}

				// 텍스처 데이터 파일 Json 파싱
				nlohmann::json textureDataJson;
				textureDataJson = nlohmann::json::parse(textureDataFile);

				Data::TextureData* textureData;
				for (const auto& [textureGroupName, textureGroup] : textureDataJson.items())
				{
					for (const auto& [textureName, texture] : textureGroup.items())
					{
						// 텍스처 데이터 생성
						textureData = new Data::TextureData;

						// 텍스처 데이터 구조화
						textureData->path = texture[Constant::TEXTURE_PATH].get<std::string>();
						textureData->format = texture[Constant::TEXTURE_FORMAT].get<DXGI_FORMAT>();
						textureData->samplerStateType = texture[Constant::TEXTURE_SAMPLER_STATE_TYPE].get<Type::SamplerStateType>();

						// 텍스처 데이터 삽입
						textureDataGroupTable[textureGroupName][textureName] = textureData;
					}
				}

				textureDataJson.clear();
				textureDataFile.close();

				try
				{
					// ==== 샘플러 상태 생성 ====
					HRESULT hr = CreateSamplerState(device);
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_SAMPLER_STATES_FAILURE);
					// ==========================
				}
				catch (CommonUtility::COMException& exception)
				{
					CommonUtility::Log::ErrorMessage(exception);
					return false;
				}

				return true;
			}

			void TextureManager::Release()
			{
				CommonUtility::SafeNestedMapContainerRelease(textureGroupTable);

				CommonUtility::SafeMapContainerComPtrRelease(samplerStates);

				CommonUtility::SafeNestedMapContainerRelease(textureDataGroupTable);
			}

			bool TextureManager::LoadTexture(ID3D11Device* device, const std::wstring& textureGroupName)
			{
				return LoadTexture(device, CommonUtility::StringUtility::ConvertToString(textureGroupName));
			}

			bool TextureManager::LoadTexture(ID3D11Device* device, const std::string& textureGroupName)
			{
				// 이미 텍스처 그룹 로드됨
				if (textureGroupTable.find(textureGroupName) != textureGroupTable.end())
					return false;

				// 텍스처 그룹 이름으로 텍스처 테이블 생성
				textureGroupTable.insert(std::make_pair(textureGroupName, TextureTable()));

				// 텍스처 데이터에 텍스처 그룹 이름으로 된 텍스처 데이터 테이블을 읽어 텍스처 생성
				Data::TextureData* textureData;
				Resource::Texture* texture;
				for (const std::pair<std::string, Data::TextureData*>& textureDataTable : textureDataGroupTable[textureGroupName])
				{
					textureData = textureDataTable.second;
					texture = new Resource::Texture(device, textureData->path,
						textureData->format, samplerStates[textureData->samplerStateType].Get());

					textureGroupTable[textureGroupName].insert(std::make_pair(textureDataTable.first, texture));
				}

				return true;
			}

			bool TextureManager::UnLoadTexture(const std::wstring& textureGroupName)
			{
				return UnLoadTexture(CommonUtility::StringUtility::ConvertToString(textureGroupName));
			}

			bool TextureManager::UnLoadTexture(const std::string& textureGroupName)
			{
				// 이미 텍스처 그룹 로드 해제됨
				if (textureGroupTable.find(textureGroupName) == textureGroupTable.end())
					return false;

				// 텍스처 그룹 자원 해제 및 제거
				CommonUtility::SafeMapContainerRelease(textureGroupTable[textureGroupName]);
				textureGroupTable.erase(textureGroupName);

				return true;
			}

			Resource::Texture* TextureManager::GetTexture(const std::wstring& textureGroupName, const std::wstring& textureName) const
			{
				return GetTexture(CommonUtility::StringUtility::ConvertToString(textureGroupName),
					CommonUtility::StringUtility::ConvertToString(textureName));
			}

			Resource::Texture* TextureManager::GetTexture(const std::string& textureGroupName, const std::string& textureName) const
			{
				return textureGroupTable.at(textureGroupName).at(textureName);
			}

			HRESULT TextureManager::CreateSamplerState(ID3D11Device* device)
			{
				Type::SamplerStateType samplerStateType;
				Factory::SamplerStateFactoryManager samplerStateFactoryManager;
				Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
				for (int i = 0; i < static_cast<int>(Type::SamplerStateType::END); ++i)
				{
					samplerStateType = static_cast<Type::SamplerStateType>(i);

					samplerState = samplerStateFactoryManager.CreateSamplerState(device, samplerStateType);
					if (!samplerState) // samplerState == nullptr
						return E_FAIL;

					samplerStates.insert(std::make_pair(samplerStateType, samplerState));
				}

				return S_OK;
			}
		} // Manager
	} // Graphics
} // Engine