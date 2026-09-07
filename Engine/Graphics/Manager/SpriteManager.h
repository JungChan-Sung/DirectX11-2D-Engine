#pragma once

#include "Engine/Graphics/Resource/Render/Sprite.h"
#include "Engine/Graphics/Data/SpriteData.h"
#include "BufferManager.h"
#include "TextureManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			/**
			 * @brief 스프라이트 매니저
			 * @date 2024-10-19
			 * @class
			 * 버퍼와 텍스처로 스프라이트를 생성하며 관리하는 클래스
			 * 씬을 로드할때 사용할 스프라이트들을 로드해놓음
			 */
			class SpriteManager
			{
			private: // ==== [선언] ====
				// <스프라이트 이름, 스프라이트 포인터>
				using SpriteTable = std::unordered_map<std::string, Resource::Sprite*>;
				// <스프라이트 그룹 이름, 스프라이트 테이블>
				using SpriteGroupTable = std::unordered_map<std::string, SpriteTable>;

			private: // ==== [스프라이트 데이터] ====
				Data::SpriteDataGroupTable spriteDataGroupTable; // 스프라이트 데이터 그룹 테이블

			private: // ==== [스프라이트] ====
				SpriteGroupTable spriteGroupTable; // 스프라이트 그룹 테이블

			public: // ==== [생성자 및 소멸자] ====
				SpriteManager();
				~SpriteManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 스프라이트 매니저 초기화
				 * @date 2024-10-20
				 * @details
				 * 스프라이트 데이터 파일 로드
				 * @return 스프라이트 매니저 초기화 성공 여부
				 */
				bool Initialize();
				/**
				 * @brief 스프라이트 매니저 자원 해제
				 * @date 2024-10-20
				 * @details
				 * 스프라이트 테이블 자원 해제
				 * 스프라이트 데이터 그룹 테이블 자원 해제
				 */
				void Release();

			public: // ==== [스프라이트 로드 및 로드 해제] ====
				/**
				 * @brief 스프라이트 로드
				 * @param spriteGroupName : 스프라이트 그룹 이름
				 * @param bufferManager : 버퍼 매니저 참조용 포인터
				 * @param textureManager : 텍스처 매니저 참조용 포인터
				 * @return 로드 성공 여부
				 */
				bool LoadSprite(const std::wstring& spriteGroupName, BufferManager* bufferManager, TextureManager* textureManager);
				bool LoadSprite(const std::string& spriteGroupName, BufferManager* bufferManager, TextureManager* textureManager);
				/**
				 * @brief 스프라이트 로드 해제
				 * @param spriteGroupName : 스프라이트 그룹 이름
				 * @return 로드 성공 여부
				 */
				bool UnLoadSprite(const std::wstring& spriteGroupName);
				bool UnLoadSprite(const std::string& spriteGroupName);

			public: // ==== [Get] ====
				/**
				 * @param spriteName : 스프라이트 이름
				 * @return 스프라이트 참조용 포인터
				 */
				Resource::Sprite* GetSprite(const std::wstring& spriteGroupName, const std::wstring& spriteName) const;
				Resource::Sprite* GetSprite(const std::string& spriteGroupName, const std::string& spriteName) const;
			};
		} // Manager
	} // Graphics
} // Engine