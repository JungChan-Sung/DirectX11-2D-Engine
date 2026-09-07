#include "GameObject.h"
#include <algorithm>
#include "CommonUtility/Utility/StringUtility.h"

namespace Engine
{
	namespace GameObject
	{
		GameObject::GameObject() : isActive(true), transform(nullptr),
			isParentActive(true)
		{
			// 게임 오브젝트 생성시 트랜스폼 컴포넌트 추가
			AddComponent<Component::Transform>();
		}

		GameObject::~GameObject()
		{
			Release();
		}

		void GameObject::Release()
		{
			componentObservers.clear();

			// 컴포넌트 자원 해제
			CommonUtility::SafeMapContainerRelease(components);

			// 트랜스폼 자원 해제
			CommonUtility::SafeDelete(transform);
		}

		void GameObject::AttachComponentObserver(Observer::IComponentObserver* componentObserver)
		{
			if (std::find(componentObservers.begin(), componentObservers.end(), componentObserver) == componentObservers.end())
				componentObservers.push_back(componentObserver);
		}

		void GameObject::DetachComponentObserver(Observer::IComponentObserver* componentObserver)
		{
			ComponentObservers::const_iterator constIterator = std::find(componentObservers.cbegin(), componentObservers.cend(), componentObserver);
			if (constIterator != componentObservers.end())
				componentObservers.erase(constIterator);
		}

		void GameObject::AddComponentNotify(Component::Component* addComponent)
		{
			for (Observer::IComponentObserver* componentObserver : componentObservers)
				componentObserver->AddComponentUpdate(addComponent);
		}

		void GameObject::RemoveComponentNotify(Component::Component* removeComponent)
		{
			for (Observer::IComponentObserver* componentObserver : componentObservers)
				componentObserver->RemoveComponentUpdate(removeComponent);
		}

		void GameObject::SetActive(bool isActive)
		{
			this->isActive = isActive;

			// 자식 게임 오브젝트의 부모 게임 오브젝트 활성화 여부 갱신
			transform->ParentActiveUpdateChilds(IsActive());
		}

		void GameObject::SetName(const std::string& name)
		{
			this->name = CommonUtility::StringUtility::ConvertToWideString(name);
		}

		void GameObject::SetName(const std::wstring& name)
		{
			this->name = name;
		}

		void GameObject::SetParentActive(bool isParentActive)
		{
			this->isParentActive = isParentActive;
		}

		bool GameObject::IsActive() const
		{
			return isActive && isParentActive;
		}

		const std::wstring& GameObject::GetName() const
		{
			return name;
		}

		Component::Transform* GameObject::GetTransform() const
		{
			return transform;
		}
	} // GameObject
} // Game
