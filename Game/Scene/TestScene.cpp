#include "TestScene.h"
#include "Engine/GameObject/Component/Render/SpriteRenderer.h"

namespace Game
{
	namespace Scene
	{
		TestScene::TestScene(const std::wstring& sceneName) : Scene(sceneName)
		{
		}
		TestScene::TestScene(const std::string& sceneName) : Scene(sceneName)
		{
		}
		bool TestScene::LoadScene(Manager::ResourceManager* resourceManager, Manager::GameObjectManager* gameObjectManager)
		{
			resourceManager->LoadTexture("otherTextures");
			resourceManager->LoadSprite("otherSprites");

			GameObject::GameObject* gameObject;
			GameObject::Component::SpriteRenderer* spriteRenderer;
			GameObject::Component::Transform* transform;
			gameObject = gameObjectManager->CreateGameObject(L"Box1");
			transform = gameObject->GetTransform();
			spriteRenderer = gameObject->GetTransform()->AddComponent<GameObject::Component::SpriteRenderer>();
			spriteRenderer->SetSprite(resourceManager->GetSprite("otherSprites", "blue"));
			spriteRenderer->SetShader(resourceManager->GetShader(Graphics::Type::ShaderType::SPRITE));

			gameObject = gameObjectManager->CreateGameObject(L"Box2");
			transform = gameObject->GetTransform();
			transform->SetLocalPosition(DirectX::XMFLOAT2(0.69f, 1.47f));
			spriteRenderer = gameObject->AddComponent<GameObject::Component::SpriteRenderer>();
			spriteRenderer->SetSprite(resourceManager->GetSprite("otherSprites", "red"));
			spriteRenderer->SetShader(resourceManager->GetShader(Graphics::Type::ShaderType::SPRITE));

			gameObject = gameObjectManager->CreateGameObject(L"Box3");
			transform = gameObject->GetTransform();
			transform->SetLocalPosition(DirectX::XMFLOAT2(0.96f, 1.0f));
			spriteRenderer = gameObject->AddComponent<GameObject::Component::SpriteRenderer>();
			spriteRenderer->SetSprite(resourceManager->GetSprite("otherSprites", "white"));
			spriteRenderer->SetShader(resourceManager->GetShader(Graphics::Type::ShaderType::SPRITE));

			gameObject = gameObjectManager->CreateGameObject(L"Box4");
			transform = gameObject->GetTransform();
			transform->SetLocalPosition(DirectX::XMFLOAT2(2.0f, 2.0f));
			spriteRenderer = gameObject->AddComponent<GameObject::Component::SpriteRenderer>();
			spriteRenderer->SetSprite(resourceManager->GetSprite("otherSprites", "yellow"));
			spriteRenderer->SetShader(resourceManager->GetShader(Graphics::Type::ShaderType::SPRITE));

			return true;
		}
		bool TestScene::UnLoadScene(Manager::ResourceManager* resourceManager, Manager::GameObjectManager* gameObjectManager)
		{
			resourceManager->UnLoadSprite("otherSprites");
			resourceManager->UnLoadTexture("otherTextures");

			return true;
		}
	} // Scene
} // Game