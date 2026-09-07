#pragma once

namespace Engine
{
	namespace GameObject
	{
		// 게임 오브젝트 전방 선언
		class GameObject;

		namespace Component
		{
			// 컴포넌트 종류
			enum class ComponentType
			{
				INVAILD,	// 유효하지 않은 컴포넌트
				RENDER,		// 렌더 컴포넌트
				PHYSICS,	// 물리 컴포넌트
				INPUT,		// 입력 컴포넌트
				AUDIO,		// 오디오 컴포넌트
				ANIMATOR,	// 애니메이터 컴포넌트
				AI,			// AI 컴포넌트
				UI,			// UI 컴포넌트
				SCRIPT,		// 스크립트 컴포넌트
				NETWORK,	// 네트워크 컴포넌트
				OTHER		// 기타 컴포넌트
			};

			/**
			 * @brief 컴포넌트 추상 클래스
			 * @date 2024-10-04
			 * @class
			 * 컴포넌트 패턴의 기초가 되는 클래스
			 */
			class Component abstract
			{
			protected: // ==== [구성 요소] ====
				GameObject* owner;	// 컴포넌트 소유자(게임 오브젝트) 참조용 포인터
				bool isActive;			// 컴포넌트 활성화 여부

			public: // ==== [생성자 및 소멸자] ====
				/** @param owner : 컴포넌트 소유자의 게임 오브젝트 참조용 포인터 */
				Component(GameObject* owner);
				virtual ~Component();

			private: // ==== [생성자] ====
				Component() = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 컴포넌트 자원 해제
				 * @details
				 * 소유자 참조 해제
				 */
				virtual void Release();

			public: // ==== [Set] ====
				/** @param isActive : 컴포넌트 활성화 여부 */
				virtual void SetActive(bool isActive);

			public: // ==== [Get] ====
				/** @return 컴포넌트 종류 */
				virtual ComponentType GetType() const;
				/** @return 컴포넌트를 소유중인 게임 오브젝트 포인터 */
				GameObject* GetGameObject() const;
				/** @return 컴포넌트 활성화 여부 */
				bool IsActive() const;
			};
		} // Component
	} // GameObject
} // Game

