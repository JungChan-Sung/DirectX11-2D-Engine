#include "SpriteManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Config/EngineConfig.h"
#include "Engine/Config/ResourceConfig.h"
#include "Engine/Message/Error/ResourceManagerErrorMessage.h"
#include "Engine/Graphics/Constant/SpriteJsonConstants.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			SpriteManager::SpriteManager()
			{
			}

			SpriteManager::~SpriteManager()
			{
				Release();
			}

			bool SpriteManager::Initialize()
			{
				std::ifstream spriteDataFile(ResourceConfig::Instance()->SpriteDataFilePath());
				if (!spriteDataFile.is_open()) // spriteDataFile.is_open == false
				{
					CommonUtility::Log::ErrorMessage(Message::Error::SPRITE_DATA_FILE_OPEN_FAILURE);
					return false;
				}

				// 스프라이트 데이터 파일 Json 파싱
				nlohmann::json spriteDataJson;
				spriteDataJson = nlohmann::json::parse(spriteDataFile);

				Data::SpriteData* spriteData;
				float inverseWorldRatio = EngineConfig::Instance()->InverseWorldRatio();
				for (const auto& [spriteGroupName, spriteGroup] : spriteDataJson.items())
				{
					for (const auto& [spriteName, sprite] : spriteGroup.items())
					{
						// 스프라이트 데이터 생성
						spriteData = new Data::SpriteData;

						// 스프라이트 데이터 구조화
						spriteData->vertexBufferType = sprite[Constant::SPRITE_VERTEX_BUFFER_TYPE].get<Type::RenderResourceBufferType>();
						spriteData->indexBufferType = sprite[Constant::SPRITE_INDEX_BUFFER_TYPE].get<Type::RenderResourceBufferType>();
						spriteData->textureGroupName = sprite[Constant::SPRITE_TEXTURE_GROUP_NAME].get<std::string>();
						spriteData->textureName = sprite[Constant::SPRITE_TEXTURE_NAME].get<std::string>();
						spriteData->pivot = sprite[Constant::SPRITE_PIVOT].get<UINT>();
						spriteData->offset = DirectX::XMFLOAT2(sprite[Constant::SPRITE_OFFSET_X].get<float>() * inverseWorldRatio,
							sprite[Constant::SPRITE_OFFSET_Y].get<float>() * inverseWorldRatio);
						spriteData->order = sprite[Constant::SPRITE_ORDER].get<int>();

						// 스프라이트 데이터 삽입
						spriteDataGroupTable[spriteGroupName][spriteName] = spriteData;
					}
				}

				spriteDataJson.clear();
				spriteDataFile.close();

				return true;
			}

			void SpriteManager::Release()
			{
				CommonUtility::SafeNestedMapContainerRelease(spriteGroupTable);

				CommonUtility::SafeNestedMapContainerRelease(spriteDataGroupTable);
			}

			bool SpriteManager::LoadSprite(const std::wstring& spriteGroupName, BufferManager* bufferManager, TextureManager* textureManager)
			{
				return LoadSprite(CommonUtility::StringUtility::ConvertToString(spriteGroupName), bufferManager, textureManager);
			}

			bool SpriteManager::LoadSprite(const std::string& spriteGroupName, BufferManager* bufferManager, TextureManager* textureManager)
			{
				// 이미 스프라이트 그룹 로드됨
				if (spriteGroupTable.find(spriteGroupName) != spriteGroupTable.end())
					return false;

				// 스프라이트 그룹 이름으로 스프라이트 테이블 생성
				spriteGroupTable.insert(std::make_pair(spriteGroupName, SpriteTable()));

				// 스프라이트 데이터에 스프라이트 그룹 이름으로 된 스프라이트 데이터 테이블을 읽어 스프라이트 생성
				Data::SpriteData* spriteData;
				Resource::Sprite* sprite;
				for (const std::pair<std::string, Data::SpriteData*>& spriteDataTable : spriteDataGroupTable[spriteGroupName])
				{
					spriteData = spriteDataTable.second;
					sprite = new Resource::Sprite(bufferManager->GetVertexBuffer(spriteData->vertexBufferType),
						bufferManager->GetIndexBuffer(spriteData->indexBufferType),
						textureManager->GetTexture(spriteData->textureGroupName, spriteData->textureName));

					sprite->SetPivot(spriteData->pivot);
					sprite->SetOffset(spriteData->offset);
					sprite->SetOrder(spriteData->order);

					spriteGroupTable[spriteGroupName].insert(std::make_pair(spriteDataTable.first, sprite));
				}

				return true;
			}

			bool SpriteManager::UnLoadSprite(const std::wstring& spriteGroupName)
			{
				return UnLoadSprite(CommonUtility::StringUtility::ConvertToString(spriteGroupName));
			}

			bool SpriteManager::UnLoadSprite(const std::string& spriteGroupName)
			{
				// 이미 스프라이트 그룹 로드 해제됨
				if (spriteGroupTable.find(spriteGroupName) == spriteGroupTable.end())
					return false;

				// 스프라이트 그룹 자원 해제 및 제거
				CommonUtility::SafeMapContainerRelease(spriteGroupTable[spriteGroupName]);
				spriteGroupTable.erase(spriteGroupName);

				return false;
			}

			Resource::Sprite* SpriteManager::GetSprite(const std::wstring& spriteGroupName, const std::wstring& spriteName) const
			{
				return GetSprite(CommonUtility::StringUtility::ConvertToString(spriteGroupName),
					CommonUtility::StringUtility::ConvertToString(spriteName));
			}

			Resource::Sprite* SpriteManager::GetSprite(const std::string& spriteGroupName, const std::string& spriteName) const
			{
				return spriteGroupTable.at(spriteGroupName).at(spriteName);
			}
		} // Manager
	} // Graphics
} // Engine