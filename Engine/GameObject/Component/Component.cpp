#include "Component.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			Component::Component(GameObject* owner) : owner(owner), isActive(true)
			{
			}

			Component::~Component()
			{
			}

			void Component::Release()
			{
				owner = nullptr;
			}

			void Component::SetActive(bool isActive)
			{
				this->isActive = isActive;
			}

			ComponentType Component::GetType() const
			{
				return ComponentType::INVAILD;
			}

			GameObject* Component::GetGameObject() const
			{
				return owner;
			}

			bool Component::IsActive() const
			{
				return isActive;
			}
		} // Component
	} // GameObject
} // Game
