#pragma once

#include <DirectXMath.h>
#include <unordered_set>
#include "Engine/GameObject/Component/Component.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			// 연산자 오버로딩(XMVECTOR, XMMATRIX)을 위해 사용
			using namespace DirectX;

			// XMVECTOR 타입은 SIMD(Single Instruction Multiple Data) 하드웨어 레스터에 대응
			// 128비트 크기의 타입을 한번에 처리 가능
			// XMMATRIX는 4개의 XMVECTOR로 이루어진 행렬
			// 지역 변수나 전역변수는 XMVECTOR 또는 XMMATRIX를 사용하여도 되지만
			// 클래스 멤버 변수로 저장할때는 XMFLAOT(Number) 또는 XMFLOAT(Number)X(Number)으로 저장
			// 단, 2D에서도 XMMATRIX로 변환과정이 더 우세한 XMFLOAT4X4로 사용할 것
			// XM_CALLCONV 호출 규약은 DirectXMath 함수를 이용하거나 변수를 이용할때 함수 앞에 붙일 것
			// 트랜스폼의 경우 Matrix 누적 방식과 Transform 누적 방식이 있음
			// Matrix 누적 방식은 정확한 값 계산이 되나 왜곡(Skew) 현상이 발생
			// Transform 누적 방식은 직관성이 좋으나 오차가 발생할 수 있음
			/**
			 * @brief 트랜스폼 클래스 : 컴포넌트 상속
			 * @date 2024-10-14
			 * @class
			 * 게임 오브젝트의 위치, 회전, 크기의 정보를 저장 및 제어
			 * @warning
			 * 트랜스폼의 Get 함수는 게임 로직에서만 사용할 것
			 * 더티 비트를 사용하기에 클래스 함수 내에서 사용할 경우 오류 발생 가능
			 * @todo
			 * 트랜스폼 버그 테스트하기
			 */
			class Transform : public Component
			{
			private: // ==== [선언] ====
				// 더티비트 자료형
				using DirtyBit = unsigned int;
				// <트랜스폼 참조용 포인터>
				using ChildTransforms = std::unordered_set<Transform*>;

				// 트랜스폼 갱신 더티 비트
				enum UpdateBits : DirtyBit
				{
					UPDATE_NONE = 0u,						// 처음 세팅
					LOCAL_SCALE = 1u << 0u,					// 지역 좌표계 크기
					LOCAL_ROTATION = 1u << 1u,				// 지역 좌표계 회전
					LOCAL_POSITION = 1u << 2u,				// 지역 좌표계 위치
					LOCAL_MATRIX = 1u << 3u,				// 지역 좌표계 행렬
					LOCAL_SCALE_MATRIX = 1u << 4u,			// 지역 좌표계 크기 행렬
					LOCAL_ROTATION_MATRIX = 1u << 5u,		// 지역 좌표계 회전 행렬
					LOCAL_TRANSLATION_MATRIX = 1u << 6u,	// 지역 좌표계 위치 행렬
					SCALE = 1u << 7u,						// 월드 좌표계 크기
					ROTATION = 1u << 8u,					// 월드 좌표계 회전
					POSITION = 1u << 9u,					// 월드 좌표계 위치
					WORLD_MATRIX = 1u << 10u,				// 월드 좌표계 행렬
					SCALE_MATRIX = 1u << 11u,				// 월드 좌표계 크기 행렬
					ROTATION_MATRIX = 1u << 12u,			// 월드 좌표계 회전 행렬
					TRANSLATION_MATRIX = 1u << 13u,			// 월드 좌표계 위치 행렬
				};

				// 트랜스폼 설정 더티 비트
				enum SetBits : DirtyBit
				{
					SET_NONE = 0u,									// 처음 세팅
					LOCAL_VALUE_SET = 1u << 1u,						// 지역 좌표계 값 설정됨
					WORLD_VALUE_SET = 1u << 2u,						// 월드 좌표계 값 설정됨
					RECENT_LOCAL_SCALE_VALUE_UPDATED = 1u << 3u,	// 최근에 지역 좌표계 크기 값 갱신됨
					RECENT_LOCAL_ROTATION_VALUE_UPDATED = 1u << 4u,	// 최근에 지역 좌표계 회전 값 갱신됨
					RECENT_LOCAL_POSITION_VALUE_UPDATED = 1u << 5u,	// 최근에 지역 좌표계 위치 값 갱신됨
					RECENT_SCALE_VALUE_UPDATED = 1u << 6u,			// 최근에 월드 좌표계 크기 값 갱신됨
					RECENT_ROTATION_VALUE_UPDATED = 1u << 7u,		// 최근에 월드 좌표계 회전 값 갱신됨
					RECENT_POSITION_VALUE_UPDATED = 1u << 8u,		// 최근에 월드 좌표계 위치 값 갱신됨
				};

			private: // ==== [구성 요소] ====
				Transform* parent;		// 부모 트랜스폼 참조용 포인터
				ChildTransforms childs;	// 자식 트랜스폼 집합
				DirtyBit updateBits;	// 갱신 더티 비트
				DirtyBit setBits;		// 설정 더티 비트
				bool isUpdated;			// 트랜스폼 갱신 되었는지 유무 여부

			private: // ==== [지역 좌표계 수치] ====
				DirectX::XMFLOAT2 localScale;		// 지역 좌표계 크기
				DirectX::XMFLOAT4 localRotation;	// 지역 좌표계 회전(쿼터니언)
				float localRotationEuler;			// 지역 좌표계 회전(오일러)
				DirectX::XMFLOAT2 localPosition;	// 지역 좌표계 위치

			private: // ==== [지역 좌표계 행렬] ====
				DirectX::XMFLOAT4X4 localMatrix;			// 지역 좌표계 행렬
				DirectX::XMFLOAT4X4 localScaleMatrix;		// 지역 좌표계 크기 행렬
				DirectX::XMFLOAT4X4 localRotationMatrix;	// 지역 좌표계 회전 행렬(쿼터니언)
				DirectX::XMFLOAT4X4 localTranslationMatrix;	// 지역 좌표계 위치 행렬

			private: // ==== [월드 좌표계 수치] ====
				DirectX::XMFLOAT2 scale;			// 월드 좌표계 크기
				DirectX::XMFLOAT4 rotation;			// 월드 좌표계 회전(쿼터니언)
				float rotationEuler;				// 월드 좌표계 회전(오일러)
				DirectX::XMFLOAT2 position;			// 월드 좌표계 위치

			private: // ==== [월드 좌표계 행렬] ====
				DirectX::XMFLOAT4X4 worldMatrix;		// 월드 좌표계 행렬
				DirectX::XMFLOAT4X4 scaleMatrix;		// 월드 좌표계 크기 행렬
				DirectX::XMFLOAT4X4 rotationMatrix;		// 월드 좌표계 회전 행렬(쿼터니언)
				DirectX::XMFLOAT4X4 translationMatrix;	// 월드 좌표계 위치 행렬

			private: // ==== [방향 수치] ====
				DirectX::XMFLOAT2 up;		// 지역 좌표계 기준 위쪽 방향
				DirectX::XMFLOAT2 right;	// 지역 좌표계 기준 오른쪽 방향

			public: // ==== [생성자 및 소멸자] ====
				/** @param gameObject : 컴포넌트 소유자의 게임 오브젝트 참조용 포인터 */
				Transform(GameObject* gameObject);
				virtual ~Transform();

			public: // ==== [Setup(컴포넌트) 재정의] ====
				/**
				 * @brief 트랜스폼 자원 해제
				 * @details
				 * 부모 참조 해제 및 부모에서 해당 트랜스폼을 계층 구조에서 제거
				 */
				virtual void Release() override;

			public: // ==== [컴포넌트] ====
				// 단일 책임의 원칙에 따라 권장되지않으나 편의성을 위한 함수
				/**
				 * @brief 트랜스폼에서 게임 오브젝트의 컴포넌트 추가
				 * @return 추가된 컴포넌트 포인터
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
				T* AddComponent()
				{
					// GameObject의 전방 선언만 존재하므로
					// GameObject::함수이름 으로 컴파일러에게 명시적으로 함수가 있음을 알림
					return owner->GameObject::AddComponent<T>();
				}
				/**
				 * @brief 트랜스폼에서 게임 오브젝트의 컴포넌트 추가
				 * @tparam addComponent : 추가할 외부 컴포넌트 참조용 포인터
				 * @return 컴포넌트 추가 성공 여부
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
				bool AddComponent(const T* addComponent)
				{
					return owner->GameObject::AddComponent(addComponent);
				}
				/**
				 * @brief 트랜스폼에서 게임 오브젝트의 컴포넌트 제거
				 * @return 컴포넌트 제거 성공 여부
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
				bool RemoveComponent()
				{
					return owner->GameObject::RemoveComponent<T>();
				}
				/**
				 * @brief 트랜스폼에서 게임 오브젝트의 컴포넌트 제거
				 * @tparam removeComponent : 제거할 외부 컴포넌트 참조용 포인터
				 * @return 컴포넌트 제거 성공 여부
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
				bool RemoveComponent(const T* removeComponent)
				{
					return owner->GameObject::RemoveComponent(removeComponent);
				}
				/**
				 * @brief 트랜스폼에서 게임 오브젝트의 컴포넌트 획득
				 * @return 획득하려는 타입의 컴포넌트 참조용 포인터
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
				T* GetComponent()
				{
					return owner->GameObject::GetComponent<T>();
				}
				/**
				 * @brief 트랜스폼에서 게임 오브젝트의 컴포넌트 획득
				 * @tparam getComponent : 존재 확인할 외부 컴포넌트 참조용 포인터
				 * @return 해당 컴포넌트의 존재 여부
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
				bool GetComponent(const T* getComponent)
				{
					return owner->GameObject::GetComponent(getComponent);
				}

			public: // ==== [트랜스폼 동작] ====
				/**
				 * @brief 트랜스폼 갱신
				 * @date 2024-10-06
				 */
				void XM_CALLCONV Update();

			public: // ==== [자식 트랜스폼] ====
				/**
				 * @brief 자식 트랜스폼 추가
				 * @param child : 추가될 자식 참조용 포인터
				 */
				void AddChild(Transform* child);
				/**
				 * @brief 자식 트랜스폼 제거
				 * @param child : 제거될 자식 참조용 포인터
				 */
				void RemoveChild(Transform* child);
				/**
				 * @brief 자식 트랜스폼 갱신
				 * @date 2024-10-14
				 * @details
				 * 자식 게임 오브젝트의 갱신 비트와 부모 게임 오브젝트 활성화 여부 갱신
				 */
				void UpdateChilds();
				/**
				 * @brief 자식 게임 오브젝트의 부모 게임 오브젝트 활성화 여부 갱신
				 * @date 2024-10-14
				 * @param isActive : 활성화 여부
				 */
				void ParentActiveUpdateChilds(bool isActive);

			public: // ==== [좌표계 조정] ====
				/**
				 * @brief 지역 좌표계 크기 조정
				 * @date 2024-10-06
				 * @param localScaleValue : 조정할 지역 좌표계 크기 값
				 */
				void AdjustLocalScale(const DirectX::XMFLOAT2& localScaleValue);
				/**
				 * @brief 지역 좌표계 크기 조정
				 * @date 2024-10-06
				 * @param localScaleVector : 조정할 지역 좌표계 크기 벡터
				 */
				void XM_CALLCONV AdjustLocalScale(DirectX::FXMVECTOR localScaleVector);
				/**
				 * @brief 지역 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param localRotationValue : 조정할 지역 좌표계 회전 값(오일러)
				 */
				void AdjustLocalRotationEuler(float localRotationValue);
				/**
				 * @brief 지역 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param localRotationVector : 조정할 지역 좌표계 회전 벡터(오일러)
				 */
				void XM_CALLCONV AdjustLocalRotationEuler(DirectX::FXMVECTOR localRotationVector);
				/**
				 * @brief 지역 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param localQuaternionValue : 조정할 지역 좌표계 회전 값(쿼터니언)
				 */
				void XM_CALLCONV AdjustLocalRotationQuaternion(const DirectX::XMFLOAT4 localQuaternionValue);
				/**
				 * @brief 지역 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param localQuaternionVector : 조정할 지역 좌표계 회전 벡터(쿼터니언)
				 */
				void XM_CALLCONV AdjustLocalRotationQuaternion(DirectX::FXMVECTOR localQuaternionVector);
				/**
				 * @brief 지역 좌표계 위치 조정
				 * @date 2024-10-06
				 * @param localPositionValue : 조정할 지역 좌표계 위치 값
				 */
				void AdjustLocalPosition(const DirectX::XMFLOAT2& localPositionValue);
				/**
				 * @brief 지역 좌표계 위치 조정
				 * @date 2024-10-06
				 * @param localPositionVector : 조정할 지역 좌표계 위치 벡터
				 */
				void XM_CALLCONV AdjustLocalPosition(DirectX::FXMVECTOR localPositionVector);

				/**
				 * @brief 월드 좌표계 크기 조정
				 * @date 2024-10-06
				 * @param scaleValue : 조정할 월드 좌표계 크기 값
				 */
				void AdjustScale(const DirectX::XMFLOAT2& scaleValue);
				/**
				 * @brief 월드 좌표계 크기 조정
				 * @date 2024-10-06
				 * @param scaleVector : 조정할 월드 좌표계 크기 벡터
				 */
				void XM_CALLCONV AdjustScale(DirectX::FXMVECTOR scaleVector);
				/**
				 * @brief 월드 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param rotationValue : 조정할 월드 좌표계 회전 값(오일러)
				 */
				void AdjustRotationEuler(float rotationValue);
				/**
				 * @brief 월드 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param rotationVector : 조정할 월드 좌표계 회전 벡터(오일러)
				 */
				void XM_CALLCONV AdjustRotationEuler(DirectX::FXMVECTOR rotationVector);
				/**
				 * @brief 월드 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param quaternionValue : 조정할 월드 좌표계 회전 값(쿼터니언)
				 */
				void XM_CALLCONV AdjustRotationQuaternion(const DirectX::XMFLOAT4& quaternionValue);
				/**
				 * @brief 월드 좌표계 회전 조정
				 * @date 2024-10-06
				 * @param quaternionVector : 조정할 월드 좌표계 회전 벡터(쿼터니언)
				 */
				void XM_CALLCONV AdjustRotationQuaternion(DirectX::FXMVECTOR quaternionVector);
				/**
				 * @brief 월드 좌표계 위치 조정
				 * @date 2024-10-06
				 * @param positionValue : 조정할 월드 좌표계 위치 값
				 */
				void AdjustPosition(const DirectX::XMFLOAT2& positionValue);
				/**
				 * @brief 월드 좌표계 위치 조정
				 * @date 2024-10-06
				 * @param positionVector : 조정할 월드 좌표계 위치 벡터
				 */
				void XM_CALLCONV AdjustPosition(DirectX::FXMVECTOR positionVector);

			public: // ==== [Set] ====
				/**
				 * @brief 트랜스폼은 활성화 여부에 상관 없이 무조건 활성화됨
				 * @param isActive : 트랜스폼 활성화 여부
				 * @deprecated
				 */
				virtual void SetActive(bool isActive) override;

				/**
				 * @brief 트랜스폼의 부모 설정
				 * @details
				 * 이전 부모가 있을 시 이전 부모 계층에서 해당 트랜스폼을 제거
				 * 설정할 부모 계층에 해당 트랜스폼을 추가 후 트랜스폼 갱신
				 * @param parent : 부모 트랜스폼 참조용 포인터
				 */
				void SetParent(Transform* parent);

				/** @param localScaleValue : 지역 좌표계 크기 값 */
				void SetLocalScale(const DirectX::XMFLOAT2& localScaleValue);
				/** @param localScaleVector : 지역 좌표계 크기 벡터 */
				void XM_CALLCONV SetLocalScale(DirectX::FXMVECTOR localScaleVector);
				/** @param localRotationValue : 지역 좌표계 회전 값(오일러) */
				void SetLocalRotationEuler(float localRotationValue);
				/** @param localRotationVector : 지역 좌표계 회전 벡터(오일러) */
				void XM_CALLCONV SetLocalRotationEuler(DirectX::FXMVECTOR localRotationVector);
				/** @param localQuaternionValue : 지역 좌표계 회전 값(쿼터니언) */
				void SetLocalRotationQuaternion(const DirectX::XMFLOAT4& localQuaternionValue);
				/** @param localQuaternionVector : 지역 좌표계 회전 벡터(쿼터니언) */
				void XM_CALLCONV SetLocalRotationQuaternion(DirectX::FXMVECTOR localQuaternionVector);
				/** @param localPositionValue : 지역 좌표계 위치 값 */
				void SetLocalPosition(const DirectX::XMFLOAT2& localPositionValue);
				/** @param localPositionVector : 지역 좌표계 위치 벡터 */
				void XM_CALLCONV SetLocalPosition(DirectX::FXMVECTOR localPositionVector);

				/**
				 * @deprecated
				 * @param scaleValue : 월드 좌표계 크기 값
				 */
				void SetScale(const DirectX::XMFLOAT2& scaleValue);
				/**
				 * @deprecated
				 * @param scaleValue : 월드 좌표계 크기 벡터
				 */
				void XM_CALLCONV SetScale(DirectX::FXMVECTOR scaleVector);
				/** @param rotationValue : 월드 좌표계 회전 값(오일러) */
				void SetRotationEuler(float rotationValue);
				/** @param rotationVector : 월드 좌표계 회전 벡터(오일러) */
				void XM_CALLCONV SetRotationEuler(DirectX::FXMVECTOR rotationVector);
				/** @param quaternionValue : 월드 좌표계 회전 값(쿼터니언) */
				void SetRotationQuaternion(const DirectX::XMFLOAT4& quaternionValue);
				/** @param quaternionVector : 월드 좌표계 회전 벡터(쿼터니언) */
				void XM_CALLCONV SetRotationQuaternion(DirectX::FXMVECTOR quaternionVector);
				/** @param positionValue : 월드 좌표계 위치 값 */
				void SetPosition(const DirectX::XMFLOAT2& positionValue);
				/** @param positionVector : 월드 좌표계 위치 벡터 */
				void XM_CALLCONV SetPosition(DirectX::FXMVECTOR positionVector);

			public: // ==== [Get] ====
				/** @return 부모 트랜스폼 참조용 포인터 */
				Transform* GetParent() const;
				/** @return 자식 트랜스폼 참조용 포인터 집합 상수 참조형 */
				const std::unordered_set<Transform*>& GetChilds() const;

				/** @return 지역 좌표계 크기 값 */
				DirectX::XMFLOAT2 GetLocalScale();
				/** @return 지역 좌표계 크기 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetLocalScaleVector();
				/** @return 지역 좌표계 회전 값(오일러) */
				float GetLocalRotationEuler();
				/** @return 지역 좌표계 회전 벡터(오일러) */
				DirectX::XMVECTOR XM_CALLCONV GetLocalRotationVectorEuler();
				/** @return 지역 좌표계 회전 값(쿼터니언) */
				DirectX::XMFLOAT4 GetLocalRotationQuaternion();
				/** @return 지역 좌표계 회전 벡터(쿼터니언) */
				DirectX::XMVECTOR XM_CALLCONV GetLocalRotationVectorQuaternion();
				/** @return 지역 좌표계 위치 값 */
				DirectX::XMFLOAT2 GetLocalPosition();
				/** @return 지역 좌표계 위치 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetLocalPositionVector();

				/** @return 지역 좌표계 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetLocalMatrix() const;
				/** @return 지역 좌표계 크기 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetLocalScaleMatrix() const;
				/** @return 지역 좌표계 회전 행렬(쿼터니언) */
				DirectX::XMMATRIX XM_CALLCONV GetLocalRotationMatrix() const;
				/** @return 지역 좌표계 위치 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetLocalTranslationMatrix() const;

				/** @return 월드 좌표계 크기 값 */
				DirectX::XMFLOAT2 GetScale();
				/** @return 월드 좌표계 크기 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetScaleVector();
				/** @return 월드 좌표계 회전 값(오일러) */
				float GetRotationEuler();
				/** @return 월드 좌표계 회전 벡터(오일러) */
				DirectX::XMVECTOR XM_CALLCONV GetRotationVectorEuler();
				/** @return 월드 좌표계 회전 값(쿼터니언) */
				DirectX::XMFLOAT4 GetRotationQuaternion();
				/** @return 월드 좌표계 회전 벡터(쿼터니언) */
				DirectX::XMVECTOR XM_CALLCONV GetRotationVectorQuaternion();
				/** @return 월드 좌표계 위치 값 */
				DirectX::XMFLOAT2 GetPosition();
				/** @return 월드 좌표계 위치 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetPositionVector();

				/** @return 월드 좌표계 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetWorldMatrix() const;
				/** @return 월드 좌표계 크기 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetScaleMatrix() const;
				/** @return 월드 좌표계 회전 행렬(쿼터니언) */
				DirectX::XMMATRIX XM_CALLCONV GetRotationMatrix() const;
				/** @return 월드 좌표계 위치 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetTranslationMatrix() const;

				/** @return 지역 좌표계 위쪽 방향 값 */
				DirectX::XMFLOAT2 GetUp() const;
				/** @return 지역 좌표계 위쪽 방향 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetUpVector() const;
				/** @return 지역 좌표계 오른쪽 방향 값 */
				DirectX::XMFLOAT2 GetRight() const;
				/** @return 지역 좌표계 오른쪽 방향 벡터 */
				DirectX::XMVECTOR XM_CALLCONV GetRightVector() const;


			public: // ==== [트랜스폼 갱신] ====
				/** @return 트랜스폼 갱신 되었는지 여부 */
				bool IsUpdated() const;
				/** @brief 트랜스폼 갱신 되었는지 여부 리셋 */
				void ResetUpdated();

			private: // ==== [좌표계 갱신] ====
				/**
				 * @brief 트랜스폼 방향 갱신
				 * @details
				 * 지역 좌표계 방향(위쪽, 오른쪽) 갱신
				 * @param localRotationMatrix : 갱신할 지역 회전 행렬
				 */
				void XM_CALLCONV UpdateDirection(DirectX::FXMMATRIX localRotationMatrix);

				/** @brief 지역 좌표계 크기 값 갱신 */
				void XM_CALLCONV UpdateLocalScale();
				/** @brief 지역 좌표계 회전 값 갱신 */
				void XM_CALLCONV UpdateLocalRotation();
				/** @brief 지역 좌표계 위치 값 갱신 */
				void XM_CALLCONV UpdateLocalPosition();

				/** @brief 지역 좌표계 크기 행렬 갱신 */
				void XM_CALLCONV UpdateLocalScaleMatrix();
				/** @brief 지역 좌표계 회전 행렬 갱신 */
				void XM_CALLCONV UpdateLocalRotationMatrix();
				/** @brief 지역 좌표계 위치 행렬 갱신 */
				void XM_CALLCONV UpdateLocalTranslationMatrix();
				/** @brief 지역 좌표계 행렬 갱신 */
				void XM_CALLCONV UpdateLocalMatrix();
				/** @brief 지역 좌표계 크기, 회전, 위치 행렬들 갱신 */
				void UpdateLocalSRTMatrices();

				/** @brief 월드 좌표계 크기 값 갱신 */
				void XM_CALLCONV UpdateScale();
				/** @brief 월드 좌표계 회전 값 갱신 */
				void XM_CALLCONV UpdateRotation();
				/** @brief 월드 좌표계 위치 값 갱신 */
				void XM_CALLCONV UpdatePosition();

				/** @brief 월드 좌표계 크기 행렬 갱신 */
				void XM_CALLCONV UpdateScaleMatrix();
				/** @brief 월드 좌표계 회전 행렬 갱신 */
				void XM_CALLCONV UpdateRotationMatrix();
				/** @brief 월드 좌표계 위치 행렬 갱신 */
				void XM_CALLCONV UpdateTranslationMatrix();
				/** @brief 월드 좌표계 행렬 갱신 */
				void XM_CALLCONV UpdateWorldMatrix();
				/** @brief 월드 좌표계 크기, 회전, 위치 행렬들 갱신 */
				void UpdateWorldSRTMatrices();

				/** @brief 지역 좌표계 크기를 월드 좌표계 크기로 갱신 */
				void UpdateLocalToWorldScale();
				/** @brief 지역 좌표계 회전을 월드 좌표계 회전으로 갱신 */
				void UpdateLocalToWorldRotation();
				/** @brief 지역 좌표계 위치를 월드 좌표계 위치로 갱신 */
				void UpdateLocalToWorldPosition();

				/** @brief 월드 좌표계 크기를 지역 좌표계 크기로 갱신 */
				void UpdateWorldToLocalScale();
				/** @brief 월드 좌표계 회전을 지역 좌표계 회전으로 갱신 */
				void UpdateWorldToLocalRotation();
				/** @brief 월드 좌표계 위치를 지역 좌표계 위치로 갱신 */
				void UpdateWorldToLocalPosition();

				/** @brief 지역 좌표계로 월드 좌표계 갱신 */
				void XM_CALLCONV UpdateWorldFromLocal();
				/** @brief 월드 좌표계로 지역 좌표계 갱신 */
				void XM_CALLCONV UpdateLocalFromWorld();
				/** @brief 지역 좌표계 갱신 */
				void XM_CALLCONV UpdateLocalTransform();
				/** @brief 월드 좌표계 갱신 */
				void XM_CALLCONV UpdateWorldTransform();

				/** @brief 부모 트랜스폼에 의한 지역 좌표계 크기 갱신 */
				void UpdateLocalScaleFromParent();
				/** @brief 부모 트랜스폼에 의한 지역 좌표계 회전 갱신 */
				void UpdateLocalRotationFromParent();
				/** @brief 부모 트랜스폼에 의한 지역 좌표계 위치 갱신 */
				void UpdateLocalPositionFromParent();
				/** @brief 부모 트랜스폼에 의한 지역 좌표계 행렬 갱신 */
				void XM_CALLCONV UpdateLocalMatrixFromParent();

				/** @brief 부모 트랜스폼에 의한 월드 좌표계 크기 갱신 */
				void UpdateScaleFromParent();
				/** @brief 부모 트랜스폼에 의한 월드 좌표계 회전 갱신 */
				void UpdateRotationFromParent();
				/** @brief 부모 트랜스폼에 의한 월드 좌표계 위치 갱신 */
				void UpdatePositionFromParent();
				/** @brief 부모 트랜스폼에 의한 월드 좌표계 행렬 갱신 */
				void XM_CALLCONV UpdateWorldMatrixFromParent();

			private: // ==== [트랜스폼 갱신 비트 설정] ====
				/** @brief 자식 트랜스폼 월드 좌표계 크기 갱신 비트 설정 */
				void ChildScaleUpdateBitSet();
				/** @brief 자식 트랜스폼 월드 좌표계 회전 갱신 비트 설정 */
				void ChildRotationUpdateBitSet();
				/** @brief 자식 트랜스폼 월드 좌표계 위치 갱신 비트 설정 */
				void ChildPositionUpdateBitSet();

				/**
				 * @brief 지역 좌표계 크기 갱신 비트 설정
				 * @details
				 * 지역 좌표계 크기 갱신시
				 * 현재 트랜스폼과 자식 트랜스폼들의 갱신 비트 설정
				 */
				void LocalScaleUpdateBitSet();
				/**
				 * @brief 지역 좌표계 회전 갱신 비트 설정
				 * @details
				 * 지역 좌표계 회전 갱신시
				 * 현재 트랜스폼과 자식 트랜스폼들의 갱신 비트 설정
				 */
				void LocalRotationUpdateBitSet();
				/**
				 * @brief 지역 좌표계 위치 갱신 비트 설정
				 * @details
				 * 지역 좌표계 위치 갱신시
				 * 현재 트랜스폼과 자식 트랜스폼들의 갱신 비트 설정
				 */
				void LocalPositionUpdateBitSet();

				/**
				 * @brief 월드 좌표계 크기 갱신 비트 설정
				 * @details
				 * 월드 좌표계 크기 갱신시
				 * 현재 트랜스폼과 자식 트랜스폼들의 갱신 비트 설정
				 */
				void ScaleUpdateBitSet();
				/**
				 * @brief 월드 좌표계 회전 갱신 비트 설정
				 * @details
				 * 월드 좌표계 회전 갱신시
				 * 현재 트랜스폼과 자식 트랜스폼들의 갱신 비트 설정
				 */
				void RotationUpdateBitSet();
				/**
				 * @brief 월드 좌표계 위치 갱신 비트 설정
				 * @details
				 * 월드 좌표계 위치 갱신시
				 * 현재 트랜스폼과 자식 트랜스폼들의 갱신 비트 설정
				 */
				void PositionUpdateBitSet();

			private: // ==== [월드 및 지역 좌표계 수치 계산] ====
				/**
				 * @brief 지역 좌표계 크기 벡터 계산
				 * @date 2024-10-06
				 * @details
				 * 
				 * @return 계산된 지역 좌표계 크기 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeLocalScaleVector();
				/**
				 * @brief 지역 좌표계 회전 벡터 계산
				 * @date 2024-10-06
				 * @details
				 * 
				 * @return 계산된 지역 좌표계 회전 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeLocalRotationVector();
				/**
				 * @brief 지역 좌표계 위치 벡터 계산
				 * @date 2024-10-06
				 * @details
				 * 
				 * @return 계산된 지역 좌표계 위치 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeLocalPositionVector();
				/**
				 * @brief 역 위치 변환 계산
				 * @date 2024-10-06
				 * @details
				 * 부모 트랜스폼의 크기, 회전, 위치를 역순으로 계산하여
				 * 역 위치로 변환함
				 * @param pointVector : 변환할 위치 벡터
				 * @return 변환된 역 위치 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeInverseTransformPositionVector(DirectX::FXMVECTOR positionVector);

				/**
				 * @brief 월드 좌표계 크기(손실) 벡터 계산
				 * @date 2024-10-06
				 * @details
				 * 월드 좌표계 역 회전 값 행렬과 월드 좌표계 회전 * 크기 행렬을 곱하여
				 * 크기(손실) 행렬을 구한 다음 벡터 값으로 변환
				 * @return 계산된 월드 좌표계 크기(손실) 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeLossyScaleVector();
				/**
				 * @brief 월드 좌표계 회전 벡터 계산
				 * @date 2024-10-06
				 * @details
				 * 부모 트랜스폼의 지역 좌표계 회전 값을 통해
				 * 월드 좌표계 회전 벡터를 구함
				 * @return 계산된 월드 좌표계 회전 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeWorldRotationVector();
				/**
				 * @brief 월드 좌표계 위치 벡터 계산
				 * @date 2024-10-06
				 * @details
				 * 부모 트랜스폼의 지역 좌표계 크기, 회전, 위치 값을 통해
				 * 월드 좌표계 위치 벡터를 구함
				 * @return 계산된 월드 좌표계 위치 벡터
				 */
				DirectX::XMVECTOR XM_CALLCONV ComputeWorldPositionVector();
				/**
				 * @brief 월드 좌표계 크기 * 회전 행렬 계산
				 * @date 2024-10-06
				 * @details
				 * 부모 트랜스폼의 크기 * 회전 행렬을 곱셈 누적하여
				 * 뒤틀린(Skew) 크기 행렬을 구함
				 * @return 계산된 월드 좌표계 크기 * 회전 행렬
				 */
				DirectX::XMMATRIX XM_CALLCONV ComputeWorldScaleRotationMatrix();
			};
		} // Component
	} // GameObject
} // Game

