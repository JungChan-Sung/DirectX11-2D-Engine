#pragma once

#include "Engine/Graphics/Manager/ShaderManager.h"
#include "Engine/Graphics/Manager/SpriteManager.h"
#include "Engine/Graphics/Observer/IDeviceRequestObserver.h"

namespace Engine
{
	namespace Manager
	{
		/**
		 * @brief 리소스 매니저 클래스 : DirectX11 장치 요청 관찰대상 인터페이스 상속
		 * @date 2024-10-21
		 * @class
		 * 그래픽스의 리소스들을 관리하는 매니저들의 통합 클래스
		 */
		class ResourceManager : public Graphics::Observer::IDeviceRequestSubject
		{
		private: // ==== [관리할 리소스 매니저] ====
			Graphics::Manager::BufferManager* bufferManager;	// 버퍼 매니저
			Graphics::Manager::ShaderManager* shaderManager;	// 셰이더 매니저
			Graphics::Manager::TextureManager* textureManager;	// 텍스처 매니저
			Graphics::Manager::SpriteManager* spriteManager;	// 스프라이트 매니저

		private: // ==== [관찰자 구성 요소] ====
			Graphics::Observer::IDeviceRequestObserver* deviceRequestObserver; // DirectX11 장치 요청 관찰자 참조용 포인터

		public: // ==== [생성자 및 소멸자] ====
			ResourceManager();
			~ResourceManager();

		public: // ==== [Setup] ====
			/**
			 * @brief 리소스 매니저 초기화
			 * @date 2024-10-19
			 * @details
			 * 버퍼, 셰이더, 텍스처, 스프라이트 매니저 생성 및 초기화
			 * @param deviceRequestObserver : DirectX11 장치 요청 관찰자 참조용 포인터
			 * @return 리소스 매니저 초기화 성공 여부
			 */
			bool Initialize(Graphics::Observer::IDeviceRequestObserver* deviceRequestObserver);
			/**
			 * @brief 리소스 매니저 자원 해제
			 * @details
			 * 버퍼, 셰이더, 텍스처, 스프라이트 매니저들의 자원 해제
			 */
			void Relese();

		public: // ==== [리소스 로드 및 로드 해제] ====
			/**
			 * @param textureGroupName : 텍스처 그룹 이름
			 * @return 로드 성공 여부
			 */
			bool LoadTexture(const std::wstring& textureGroupName);
			bool LoadTexture(const std::string& textureGroupName);
			/**
			 * @param textureGroupName : 텍스처 그룹 이름
			 * @return 로드 해제 성공 여부
			 */
			bool UnLoadTexture(const std::wstring& textureGroupName);
			bool UnLoadTexture(const std::string& textureGroupName);

			/**
			 * @param spriteGroupName : 스프라이트 그룹 이름
			 * @return 로드 성공 여부
			 */
			bool LoadSprite(const std::wstring& spriteGroupName);
			bool LoadSprite(const std::string& spriteGroupName);
			/**
			 * @param spriteGroupName : 스프라이트 그룹 이름
			 * @return 로드 해제 성공 여부
			 */
			bool UnLoadSprite(const std::wstring& spriteGroupName);
			bool UnLoadSprite(const std::string& spriteGroupName);

		public: // ==== [DirectX11 장치 요청 관찰대상] ====
			/**
			 * @brief DirectX11 장치 요청 관찰자 연결
			 * @date 2024-10-21
			 * @param deviceRequestObserver : DirectX11 장치 요청 관찰자 참조용 포인터
			 */
			void AttachDeviceRequestObserver(Graphics::Observer::IDeviceRequestObserver* deviceRequestObserver);
			/**
			 * @brief DirectX11 장치 요청 관찰자 연결 해제
			 * @date 2024-10-21
			 */
			void DetachDeviceRequestObserver();
			/**
			 * @brief DirectX11 장치 요청 알림
			 * @date 2024-10-21
			 * @return DirectX11 장치 참조용 포인터
			 */
			ID3D11Device* DeviceRequestNotify();

		public: // ==== [Get] ====
			/**
			 * @param shaderType : 셰이더 타입
			 * @return 셰이더 참조용 포인터
			 */
			Graphics::Resource::Shader* GetShader(Graphics::Type::ShaderType shaderType) const;

			/**
			 * @param textureGroupName : 텍스처 그룹 이름
			 * @param textureName : 텍스처 이름
			 * @return 텍스처 참조용 포인터
			 */
			Graphics::Resource::Texture* GetTexture(const std::wstring& textureGroupName, const std::wstring& textureName) const;
			Graphics::Resource::Texture* GetTexture(const std::string& textureGroupName, const std::string& textureName) const;

			/**
			 * @param spriteGroupName : 스프라이트 그룹 이름
			 * @param spriteName : 스프라이트 이름
			 * @return 스프라이트 참조용 포인터
			 */
			Graphics::Resource::Sprite* GetSprite(const std::wstring& spriteGroupName, const std::wstring& spriteName) const;
			Graphics::Resource::Sprite* GetSprite(const std::string& spriteGroupName, const std::string& spriteName) const;
		};
	} // Manager
} // Engine
