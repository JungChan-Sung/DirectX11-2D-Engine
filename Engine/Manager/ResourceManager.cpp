#include "ResourceManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Manager
	{
		ResourceManager::ResourceManager() : bufferManager(nullptr), shaderManager(nullptr),
			textureManager(nullptr), spriteManager(nullptr),
			deviceRequestObserver(nullptr)
		{
		}

		ResourceManager::~ResourceManager()
		{
			Relese();
		}

		bool ResourceManager::Initialize(Graphics::Observer::IDeviceRequestObserver* deviceRequestObserver)
		{
			AttachDeviceRequestObserver(deviceRequestObserver);
			ID3D11Device* device = DeviceRequestNotify();

			// ==== 버퍼 매니저 생성 및 초기화 ====
			bufferManager = new Graphics::Manager::BufferManager;
			if (!bufferManager) // bufferManager == nullptr
				return false;

			if (!bufferManager->Initialize(device)) // bufferManager->Initialize == false
				return false;
			// ====================================

			// ==== 셰이더 매니저 생성 및 초기화 ====
			shaderManager = new Graphics::Manager::ShaderManager;
			if (!shaderManager) // shaderManager == nullptr
				return false;

			if (!shaderManager->Initialize(device, bufferManager)) // shaderManager->Initialize == false
				return false;
			// ======================================

			// ==== 텍스처 매니저 생성 및 초기화 ====
			textureManager = new Graphics::Manager::TextureManager;
			if (!textureManager) // textureManager == nullptr
				return false;

			if (!textureManager->Initialize(device)) // textureManager->Initialize == false
				return false;
			// ======================================

			// ==== 스프라이트 매니저 생성 및 초기화 ====
			spriteManager = new Graphics::Manager::SpriteManager;
			if (!spriteManager) // spriteManager == nullptr
				return false;

			if (!spriteManager->Initialize()) // spriteManager->Initialize == false
				return false;
			// ==========================================

			return true;
		}

		void ResourceManager::Relese()
		{
			CommonUtility::SafeDelete(spriteManager);
			CommonUtility::SafeDelete(textureManager);
			CommonUtility::SafeDelete(shaderManager);
			CommonUtility::SafeDelete(bufferManager);

			DetachDeviceRequestObserver();
		}

		bool ResourceManager::LoadTexture(const std::wstring& textureGroupName)
		{
			return textureManager->LoadTexture(DeviceRequestNotify(), textureGroupName);
		}

		bool ResourceManager::LoadTexture(const std::string& textureGroupName)
		{
			return textureManager->LoadTexture(DeviceRequestNotify(), textureGroupName);
		}

		bool ResourceManager::UnLoadTexture(const std::wstring& textureGroupName)
		{
			return textureManager->UnLoadTexture(textureGroupName);
		}

		bool ResourceManager::UnLoadTexture(const std::string& textureGroupName)
		{
			return textureManager->UnLoadTexture(textureGroupName);
		}

		bool ResourceManager::LoadSprite(const std::wstring& spriteGroupName)
		{
			return spriteManager->LoadSprite(spriteGroupName, bufferManager, textureManager);
		}

		bool ResourceManager::LoadSprite(const std::string& spriteGroupName)
		{
			return spriteManager->LoadSprite(spriteGroupName, bufferManager, textureManager);
		}

		bool ResourceManager::UnLoadSprite(const std::wstring& spriteGroupName)
		{
			return spriteManager->UnLoadSprite(spriteGroupName);
		}

		bool ResourceManager::UnLoadSprite(const std::string& spriteGroupName)
		{
			return spriteManager->UnLoadSprite(spriteGroupName);
		}

		Graphics::Resource::Shader* ResourceManager::GetShader(Graphics::Type::ShaderType shaderType) const
		{
			return shaderManager->GetShader(shaderType);
		}

		Graphics::Resource::Texture* ResourceManager::GetTexture(const std::wstring& textureGroupName, const std::wstring& textureName) const
		{
			return textureManager->GetTexture(textureGroupName, textureName);
		}

		Graphics::Resource::Texture* ResourceManager::GetTexture(const std::string& textureGroupName, const std::string& textureName) const
		{
			return textureManager->GetTexture(textureGroupName, textureName);
		}

		Graphics::Resource::Sprite* ResourceManager::GetSprite(const std::wstring& spriteGroupName, const std::wstring& spriteName) const
		{
			return spriteManager->GetSprite(spriteGroupName, spriteName);
		}

		Graphics::Resource::Sprite* ResourceManager::GetSprite(const std::string& spriteGroupName, const std::string& spriteName) const
		{
			return spriteManager->GetSprite(spriteGroupName, spriteName);
		}

		void ResourceManager::AttachDeviceRequestObserver(Graphics::Observer::IDeviceRequestObserver* deviceRequestObserver)
		{
			this->deviceRequestObserver = deviceRequestObserver;
		}

		void ResourceManager::DetachDeviceRequestObserver()
		{
			deviceRequestObserver = nullptr;
		}

		ID3D11Device* ResourceManager::DeviceRequestNotify()
		{
			return deviceRequestObserver->DeviceRequest();
		}
	} // Manager
} // Engine