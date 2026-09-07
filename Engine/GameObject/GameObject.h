#pragma once

// C++ 17부터 추가된 헤더파일, std::reference_wrapper<std::type_info> 대신 사용
#include <typeindex>
#include <unordered_map>
#include <string>
#include "CommonUtility/Utility/SafeDelete.h"
#include "Component/Transform.h"
#include "Observer/IComponentObserver.h"

namespace Engine
{
	namespace GameObject
	{
		/**
		 * @brief 게임 오브젝트 클래스 : 컴포넌트 관찰대상 상속
		 * @date 2024-10-21
		 * @class
		 * 게임에 사용될 오브젝트의 기본 단위
		 * 컴포넌트 패턴의 컴포지트 역할
		 */
		class GameObject : public Observer::IComponentSubject
		{
		private: // ==== [선언] ====
			/**
			 * @brief 해시 함수 구조체
			 * @struct
			 * std::unordered_map의 해시 함수 Functor(함수 객체) 구조체
			 * std::type_info의 해시 코드를 사용하여 해시 테이블의 충돌을 방지함
			 */
			struct HashCode
			{
				std::size_t operator()(std::type_index hashCode) const
				{
					return hashCode.hash_code();
				}
			};

			/**
			 * @brief 비교 함수 구조체
			 * @struct
			 * 두 개의 키 값이 같은지 비교하는 함수 객체 구조체
			 * std::reference_wrapper를 통해 참조값을 포인터값으로 비교 가능
			 */
			struct EqualTo
			{
				bool operator()(std::type_index lhs, std::type_index rhs) const
				{
					return lhs == rhs;
				}
			};

			// std::unordered_map<Key, Value, Hash, EqualTo>
			// 순서대로 <키, 값, 해시값, 비교할 구조>
			using ComponentHashTable = std::unordered_map<std::type_index, Component::Component*, HashCode, EqualTo>;
			// <컴포넌트 관찰자 참조용 포인터>
			using ComponentObservers = std::vector<Observer::IComponentObserver*>;

		private: // ==== [구성 요소] ====
			bool isActive;						// 활성화 여부
			std::wstring name;					// 게임 오브젝트 이름
			Component::Transform* transform;	// 게임 오브젝트의 트랜스폼

		private: // ==== [부모 게임 오브젝트] ====
			bool isParentActive;	// 부모 게임 오브젝트 활성화 여부

		private: // ==== [컴포넌트] ====
			ComponentHashTable components;	// 컴포넌트 해시 테이블

		private: // ==== [관찰자 구성 요소] ====
			ComponentObservers componentObservers; // 컴포넌트 관찰자 배열

		public: // ==== [생성자 및 소멸자] ====
			GameObject();
			~GameObject();

		public: // ==== [Setup] ====
			/**
			 * @brief 게임 오브젝트 자원 해제
			 * @details
			 * 지니고 있는 컴포넌트들을 자원 해제 및 트랜스폼 자원 해제
			 */
			void Release();

		public: // ==== [게임 오브젝트 컴포넌트] ====
			// <정의된 클래스 이름, 의존타입이므로 typename을 붙이며, 무명 디폴트 인자(typename = )로 enable_if TMP(Template Meta Programing) 사용하여 Component 클래스의 하위 클래스인지 감별>
			// enable_if_t = is_enable_if::type, is_base_of_v = is_base_of::value
			/**
			 * @brief 게임 오브젝트의 컴포넌트 추가
			 * @date 2024-10-05
			 * @details
			 * TMP 기법을 사용하여 Component의 하위 클래스만 추가 가능하도록 컴파일됨
			 * 중복 컴포넌트는 현재 허용하지 않음
			 * 생성된 컴포넌트를 해시 테이블(std::unordered_map)에 삽입 후 해당 컴포넌트의 포인터를 반환
			 * @return 추가된 컴포넌트 포인터
			 */
			template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component::Component, T>>>
			T* AddComponent()
			{
				std::type_index typeIndex = typeid(T);
				// 중복 컴포넌트 검사
				if (components.find(typeIndex) != components.end())
					return nullptr;

				// 중복 컴포넌트가 아닌 경우 헤시 테이블에 추가 후 포인터 반환
				components[typeIndex] = new T(this);
				return static_cast<T*>(components[typeIndex]);
			}
			/**
			 * @brief 게임 오브젝트의 컴포넌트 추가
			 * @date 2024-10-05
			 * @details
			 * 내부가 아닌 외부에서 생성된 컴포넌트를 추가하는 함수
			 * @tparam addComponent : 추가할 외부 컴포넌트 참조용 포인터
			 * @return 컴포넌트 추가 성공 여부
			 */
			template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component::Component, T>>>
			bool AddComponent(const T* addComponent)
			{
				std::type_index typeIndex = typeid(addComponent);
				// 중복 컴포넌트 검사
				if (components.find(typeIndex) != components.end())
					return false;

				// 중복 컴포넌트가 아닌 경우 헤시 테이블에 추가
				components[typeIndex] = addComponent;
				return true;
			}
			/**
			 * @brief 게임 오브젝트의 컴포넌트 제거
			 * @date 2024-10-05
			 * @details
			 * TMP 기법을 사용하여 Component의 하위 클래스만 추가 가능하도록 컴파일됨
			 * 컴포넌트 해시 테이블에서 T타입의 컴포넌트를 탐색 후 제거
			 * 제거한 후 제거 성공 여부 반환
			 * @return 컴포넌트 제거 성공 여부
			 */
			template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component::Component, T>>>
			bool RemoveComponent()
			{
				// 컴포넌트 탐색
				ComponentHashTable::iterator iterator = components.find(typeid(T));
				if (iterator != components.end())
				{
					// 해당 컴포넌트 제거 및 자원 해제
					CommonUtility::SafeDelete(iterator->second);
					components.erase(iterator);

					// 컴포넌트 제거 성공
					return true;
				}

				// 컴포넌트 제거 실패
				return false;
			}
			/**
			 * @brief 게임 오브젝트의 컴포넌트 제거
			 * @date 2024-10-05
			 * @details
			 * 내부가 아닌 외부에서 생성된 컴포넌트를 제거하는 함수
			 * @warning
			 * 외부 컴포넌트이므로 함수 내부에서 자원 해제를 하지 않음
			 * @tparam removeComponent : 제거할 외부 컴포넌트 참조용 포인터
			 * @return 컴포넌트 제거 성공 여부
			 */
			template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component::Component, T>>>
			bool RemoveComponent(const T* removeComponent)
			{
				// 컴포넌트 탐색
				ComponentHashTable::const_iterator constIterator = components.find(typeid(removeComponent));
				if (constIterator != components.end())
				{
					// 해당 컴포넌트 제거
					components.erase(constIterator);
					return true;
				}

				// 컴포넌트 제거 실패
				return false;
			}
			/**
			 * @brief 게임 오브젝트의 컴포넌트 획득
			 * @date 2024-10-05
			 * @details
			 * TMP 기법을 사용하여 Component의 하위 클래스만 추가 가능하도록 컴파일됨
			 * 컴포넌트 해시 테이블에서 T타입의 컴포넌트를 탐색 후 반환
			 * 탐색 실패시 Null 값 반환
			 * @return 획득하려는 타입의 컴포넌트 참조용 포인터
			 */
			template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component::Component, T>>>
			T* GetComponent()
			{
				ComponentHashTable::const_iterator constIterator = components.find(typeid(T));
				if (constIterator == components.end())
				{
					// 탐색 실패시 nullptr 반환
					return nullptr;
				}

				// 탐색 성공시 반환
				return constIterator->second;
			}
			/**
			 * @brief 게임 오브젝트의 컴포넌트 획득
			 * @date 2024-10-05
			 * @details
			 * 내부가 아닌 외부에서 생성된 컴포넌트가 존재하는지 여부를 알려주는 함수
			 * @tparam getComponent : 존재 확인할 외부 컴포넌트 참조용 포인터
			 * @return 해당 컴포넌트의 존재 여부
			 */
			template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component::Component, T>>>
			bool GetComponent(const T* getComponent)
			{
				return components.find(typeid(getComponent) != components.end());
			}

		public: // ==== [트랜스폼 컴포넌트] ====
			/**
			 * @brief 게임 오브젝트의 트랜스폼 추가
			 * @date 2024-10-05
			 * @details
			 * Transform 클래스 컴포넌트 추가 템플릿 특수화 함수
			 * @return 추가된 트랜스폼 포인터
			 */
			template<>
			Component::Transform* AddComponent()
			{
				// 이미 생성 되어 있으면
				if (transform) // transform != nullptr
					return transform;

				// 없으면 새로 생성 후 반환
				transform = new Component::Transform(this);
				return transform;
			}
			/**
			 * @brief 게임 오브젝트의 트랜스폼 제거
			 * @date 2024-10-05
			 * @details
			 * Transform 클래스 컴포넌트 제거 템플릿 특수화 함수
			 * @return 트랜스폼 제거 성공 여부(실패시 해당 컴포넌트를 찾지 못함을 의미)
			 */
			template<>
			bool RemoveComponent<Component::Transform>()
			{
				// 트랜스폼이 존재하면
				if (transform) // transform != nullptr
				{
					CommonUtility::SafeDelete(transform);
					return true;
				}

				// 존재 하지 않으면
				return false;
			}
			/**
			 * @brief 게임 오브젝트의 트랜스폼 획득
			 * @date 2024-10-05
			 * @details
			 * Transform 클래스 컴포넌트 획득 템플릿 특수화 함수
			 * GetTransform 함수가 따로 있으므로 불편하게 사용하지 않았으면 함
			 * @return 트랜스폼 포인터(없으면 Null 값)
			 * @deprecated
			 */
			template<>
			Component::Transform* GetComponent()
			{
				return transform;
			}

		public: // ==== [컴포넌트 관찰대상] ====
			/**
			 * @brief 컴포넌트 관찰자 연결
			 * @date 2024-10-21
			 * @param componentObserver : 컴포넌트 관찰자 참조용 포인터
			 */
			void AttachComponentObserver(Observer::IComponentObserver* componentObserver);
			/**
			 * @brief 컴포넌트 관찰자 연결 해제
			 * @date 2024-10-21
			 * @param componentObserver : 컴포넌트 관찰자 참조용 포인터
			 */
			void DetachComponentObserver(Observer::IComponentObserver* componentObserver);
			/**
			 * @brief 컴포넌트 추가 알림
			 * @date 2024-10-21
			 * @param component : 추가된 컴포넌트 참조용 포인터
			 */
			void AddComponentNotify(Component::Component* addComponent);
			/**
			 * @brief 컴포넌트 제거 알림
			 * @date 2024-10-21
			 * @param component : 제거된 컴포넌트 참조용 포인터
			 */
			void RemoveComponentNotify(Component::Component* removeComponent);

		public: // ==== [Set] ====
			/** @param isActive : 게임 오브젝트 활성화 여부 */
			void SetActive(bool isActive);

			/** @param name : 게임 오브젝트 이름 */
			void SetName(const std::string& name);
			void SetName(const std::wstring& name);

			/** @param isParentActive : 부모 게임 오브젝트 활성화 여부 */
			void SetParentActive(bool isParentActive);

		public: // ==== [Get] ====
			/** @return 게임 오브젝트 활성화 여부 */
			bool IsActive() const;
			/** @return 게임 오브젝트 이름 */
			const std::wstring& GetName() const;
			/** @return 트랜스폼 포인터 */
			Component::Transform* GetTransform() const;
		};
	} // GameObject
} // Game

