/*********************************************************************
 * @file   IComponentObserver.h
 * @brief  컴포넌트 관찰자 패턴
 * @date   2024-10-21
 *********************************************************************/

#pragma once

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			// 컴포넌트 전방 선언
			class Component;
		} // Component

		namespace Observer
		{
			/**
			 * @brief 컴포넌트 관찰자 인터페이스
			 * @date 2024-10-21
			 * @interface
			 * 컴포넌트 매니저 클래스
			 */
			__interface IComponentObserver
			{
			public: // ==== [컴포넌트 관찰자] ====
				void AddComponentUpdate(Component::Component* addComponent);
				void RemoveComponentUpdate(Component::Component* removeComponent);
			};

			/**
			 * @brief 컴포넌트 관찰대상 인터페이스
			 * @date 2024-10-21
			 * @interface
			 * 게임 오브젝트 클래스
			 */
			__interface IComponentSubject
			{
			public: // ==== [컴포넌트 관찰대상] ====
				void AttachComponentObserver(IComponentObserver* componentObserver);
				void DetachComponentObserver(IComponentObserver* componentObserver);
				void AddComponentNotify(Component::Component* addComponent);
				void RemoveComponentNotify(Component::Component* removeComponent);
			};
		} // Observer
	} // GameObject
} // Engine
