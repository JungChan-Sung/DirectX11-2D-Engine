#include "Transform.h"
#include "CommonUtility/Utility/DirectXMathExtend.h"
#include "Engine/Config/EngineConfig.h"
#include "Engine/GameObject/GameObject.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			Transform::Transform(GameObject* gameObject) : Component(gameObject),
				parent(nullptr), updateBits(UpdateBits::UPDATE_NONE), setBits(SetBits::SET_NONE), isUpdated(false),
				localScale(1.0f, 1.0f), localRotation(0.0f, 0.0f, 0.0f, 1.0f), localRotationEuler(0.0f), localPosition(0.0f, 0.0f),
				scale(1.0f, 1.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), rotationEuler(0.0f), position(0.0f, 0.0f)
			{
				// 지역 좌표계 행렬 초기화
				DirectX::XMStoreFloat4x4(&localMatrix, DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&localScaleMatrix, DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&localRotationMatrix, DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&localTranslationMatrix, DirectX::XMMatrixIdentity());

				// 월드 좌표계 행렬 초기화
				float worldRatio = EngineConfig::Instance()->WorldRatio();
				DirectX::XMStoreFloat4x4(&scaleMatrix, DirectX::XMMatrixScaling(worldRatio, worldRatio, 1.0f)); // 월드 크기 행렬은 값이 월드 비율이 곱해진 값으로 존재
				DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixIdentity()); // 월드 위치 행렬도 크기 행렬과 마찬가지지만 처음 위치는 0이므로 기본 값으로 초기화
				worldMatrix = scaleMatrix; // 초기 크기 행렬과 월드 행렬은 같은 값

				// 지역 좌표계 기준 방향 초기화
				DirectX::XMStoreFloat2(&up, CommonUtility::DirectXMathExtend::up);
				DirectX::XMStoreFloat2(&right, CommonUtility::DirectXMathExtend::right);
			}

			Transform::~Transform()
			{
				Release();
			}

			void Transform::Release()
			{
				// 부모 존재시
				if (parent) // parent != nullptr
				{
					// 부모에서 해당 트랜스폼을 계층 구조에서 제거
					parent->RemoveChild(this);
					// 부모 참조 해제
					parent = nullptr;
				}

				// 현재 모든 자식 트랜스폼 제거
				childs.clear();
			}

			void XM_CALLCONV Transform::Update()
			{
				// 부모 트랜스폼이 있으면서 갱신이 안되었으면
				if (parent && !parent->isUpdated) // parent != nullptr && parent->isUpdated == false
					parent->Update();

				// 지역 좌표계 값만 설정되었으면
				if ((setBits & SetBits::LOCAL_VALUE_SET) && (~setBits & SetBits::WORLD_VALUE_SET))
				{
					isUpdated = true;

					// 지역 좌표계 크기, 회전, 위치 행렬 갱신
					UpdateLocalSRTMatrices();

					// 월드 좌표계 갱신
					UpdateWorldTransform();
				}
				// 월드 좌표계 값만 설정되었으면
				else if ((setBits & SetBits::WORLD_VALUE_SET) && (~setBits & SetBits::LOCAL_VALUE_SET))
				{
					isUpdated = true;

					// 월드 좌표계 크기, 회전, 위치 행렬 갱신
					UpdateWorldSRTMatrices();

					// 지역 좌표계 갱신
					UpdateLocalTransform();
				}
				// 지역 좌표계와 월드 좌표계 값이 둘다 설정 되었으면
				else if ((setBits & SetBits::LOCAL_VALUE_SET) && (setBits & SetBits::WORLD_VALUE_SET))
				{
					isUpdated = true;

					// 부모 트랜스폼이 존재하는 경우
					if (parent) // parent != nullptr
					{
						// 지역 좌표계 크기 값이 최근에 갱신되었으면
						if (setBits & SetBits::RECENT_LOCAL_SCALE_VALUE_UPDATED)
						{
							// 월드 좌표계 크기 갱신
							UpdateScale();
							// 월드 좌표계 크기 행렬 갱신
							UpdateScaleMatrix();
						}
						// 월드 좌표계 크기 값이 최근에 갱신되었으면
						else if (setBits & SetBits::RECENT_SCALE_VALUE_UPDATED)
						{
							// 지역 좌표계 크기 갱신
							UpdateLocalScale();
						}

						// 지역 좌표계 회전 값이 최근에 갱신되었으면
						if (setBits & SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED)
						{
							// 월드 좌표계 회전 갱신
							UpdateRotation();
							// 월드 좌표계 회전 행렬 갱신
							UpdateRotationMatrix();
						}
						// 월드 좌표계 회전 값이 최근에 갱신되었으면
						else if (setBits & SetBits::RECENT_ROTATION_VALUE_UPDATED)
						{
							// 지역 좌표계 회전 갱신
							UpdateLocalRotation();
						}

						// 지역 좌표계 위치 값이 최근에 갱신되었으면
						if (setBits & SetBits::RECENT_LOCAL_POSITION_VALUE_UPDATED)
						{
							// 월드 좌표계 위치 갱신
							UpdatePosition();
							// 월드 좌표계 위치 행렬 갱신
							UpdateTranslationMatrix();
						}
						// 월드 좌표계 위치 값이 최근에 갱신되었으면
						else if (setBits & SetBits::RECENT_POSITION_VALUE_UPDATED)
						{
							// 지역 좌표계 위치 갱신
							UpdateLocalPosition();
						}

						// 행렬 갱신은 지역 좌표계 -> 월드 좌표계 순서로(월드 좌표계 -> 지역 좌표계 순서면 역 행렬 연산이 필요)
						UpdateLocalSRTMatrices();

						// 부모 트랜스폼에의한 월드 행렬 갱신
						UpdateWorldMatrixFromParent();
					}
					// 부모 트랜스폼이 존재하지 않는 경우
					else
					{
						// 지역 좌표계 크기 값이 최근에 갱신되었으면
						if (setBits & SetBits::RECENT_LOCAL_SCALE_VALUE_UPDATED)
						{
							// 월드 좌표계 크기 갱신
							UpdateWorldToLocalScale();
							// 월드 좌표계 크기 행렬 갱신
							UpdateScaleMatrix();
						}
						// 월드 좌표계 크기 값이 최근에 갱신되었으면
						else if (setBits & SetBits::RECENT_SCALE_VALUE_UPDATED)
						{
							// 지역 좌표계 크기 갱신
							UpdateLocalToWorldScale();
						}

						// 지역 좌표계 회전 값이 최근에 갱신되었으면
						if (setBits & SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED)
						{
							// 월드 좌표계 회전 갱신
							UpdateWorldToLocalRotation();
							// 월드 좌표계 회전 행렬 갱신
							UpdateRotationMatrix();
						}
						// 월드 좌표계 회전 값이 최근에 갱신되었으면
						else if (setBits & SetBits::RECENT_ROTATION_VALUE_UPDATED)
						{
							// 지역 좌표계 회전 갱신
							UpdateLocalToWorldRotation();
						}

						// 지역 좌표계 위치 값이 최근에 갱신되었으면
						if (setBits & SetBits::RECENT_LOCAL_POSITION_VALUE_UPDATED)
						{
							// 월드 좌표계 위치 갱신
							UpdateWorldToLocalPosition();
							// 월드 좌표계 위치 행렬 갱신
							UpdateTranslationMatrix();
						}
						// 월드 좌표계 위치 값이 최근에 갱신되었으면
						else if (setBits & SetBits::RECENT_POSITION_VALUE_UPDATED)
						{
							// 지역 좌표계 위치 갱신
							UpdateLocalToWorldPosition();
						}

						// 지역 좌표계 행렬 갱신
						UpdateLocalSRTMatrices();

						// 월드 좌표계 행렬 갱신
						UpdateWorldMatrix();
					}
				}
				// 지역 좌표계, 월드 좌표계 둘 다 설정이 된적 없으면 월드 좌표계 갱신 체크
				else if (updateBits & UpdateBits::WORLD_MATRIX)
				{
					isUpdated = true;

					// 월드 좌표계 갱신
					UpdateWorldTransform();
				}

				// 트랜스폼 설정 비트 전체 초기화
				setBits = SetBits::SET_NONE;
			}

			void Transform::AddChild(Transform* child)
			{
				childs.insert(child);
			}

			void Transform::RemoveChild(Transform* child)
			{
				// 제거할 자식 탐색
				ChildTransforms::const_iterator constIterator = childs.find(child);
				if (constIterator != childs.end())
					childs.erase(constIterator);
			}

			void Transform::UpdateChilds()
			{
				// 월드 좌표계 행렬
				// 월드 좌표계 크기 행렬, 월드 좌표계 크기
				// 월드 좌표계 회전 행렬, 월드 좌표계 회전
				// 월드 좌표계 위치 행렬, 월드 좌표계 위치
				for (Transform* child : childs)
				{
					// 갱신 비트 설정
					child->updateBits |= UpdateBits::WORLD_MATRIX
						| UpdateBits::SCALE_MATRIX | UpdateBits::SCALE
						| UpdateBits::ROTATION_MATRIX | UpdateBits::ROTATION
						| UpdateBits::TRANSLATION_MATRIX | UpdateBits::POSITION;

					// 자식 게임 오브젝트의 부모 게임 오브젝트 활성화 여부 설정
					child->GetGameObject()->SetParentActive(owner->IsActive());

					child->UpdateChilds(); // 재귀
				}
			}

			void Transform::ParentActiveUpdateChilds(bool isActive)
			{
				for (Transform* child : childs)
				{
					// 자식 게임 오브젝트의 부모 게임 오브젝트 활성화 여부 설정
					child->GetGameObject()->SetParentActive(isActive);

					child->ParentActiveUpdateChilds(isActive); // 재귀
				}
			}

			void Transform::AdjustLocalScale(const DirectX::XMFLOAT2& localScaleValue)
			{
				SetLocalScale(DirectX::XMFLOAT2(localScale.x + localScaleValue.x, localScale.y + localScaleValue.y));
			}

			void XM_CALLCONV Transform::AdjustLocalScale(DirectX::FXMVECTOR localScaleVector)
			{
				SetLocalScale(DirectX::XMLoadFloat2(&localScale) + localScaleVector);
			}

			void Transform::AdjustLocalRotationEuler(float localRotationValue)
			{
				SetLocalRotationEuler(localRotationEuler + localRotationValue);
			}

			void XM_CALLCONV Transform::AdjustLocalRotationEuler(DirectX::FXMVECTOR localRotationVector)
			{
				SetLocalRotationEuler(DirectX::XMLoadFloat(&localRotationEuler) + localRotationVector);
			}

			void XM_CALLCONV Transform::AdjustLocalRotationQuaternion(const DirectX::XMFLOAT4 localQuaternionValue)
			{
				SetLocalRotationQuaternion(DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&localRotation), DirectX::XMLoadFloat4(&localQuaternionValue)));
			}

			void XM_CALLCONV Transform::AdjustLocalRotationQuaternion(DirectX::FXMVECTOR localQuaternionVector)
			{
				SetLocalRotationQuaternion(DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&localRotation), localQuaternionVector));
			}

			void Transform::AdjustLocalPosition(const DirectX::XMFLOAT2& localPositionValue)
			{
				SetLocalPosition(DirectX::XMFLOAT2(localPosition.x + localPositionValue.x, localPosition.y + localPositionValue.y));
			}

			void XM_CALLCONV Transform::AdjustLocalPosition(DirectX::FXMVECTOR localPositionVector)
			{
				SetLocalPosition(DirectX::XMLoadFloat2(&localPosition) + localPositionVector);
			}

			void Transform::AdjustScale(const DirectX::XMFLOAT2& scaleValue)
			{
				SetScale(DirectX::XMFLOAT2(scale.x + scaleValue.x, scale.y + scaleValue.y));
			}

			void XM_CALLCONV Transform::AdjustScale(DirectX::FXMVECTOR scaleVector)
			{
				SetScale(DirectX::XMLoadFloat2(&scale) + scaleVector);
			}

			void Transform::AdjustRotationEuler(float rotationValue)
			{
				SetRotationEuler(rotationEuler + rotationValue);
			}

			void XM_CALLCONV Transform::AdjustRotationEuler(DirectX::FXMVECTOR rotationVector)
			{
				SetRotationEuler(DirectX::XMLoadFloat(&rotationEuler) + rotationVector);
			}

			void XM_CALLCONV Transform::AdjustRotationQuaternion(const DirectX::XMFLOAT4& quaternionValue)
			{
				SetRotationQuaternion(DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&rotation), DirectX::XMLoadFloat4(&quaternionValue)));
			}

			void XM_CALLCONV Transform::AdjustRotationQuaternion(DirectX::FXMVECTOR quaternionVector)
			{
				SetRotationQuaternion(DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&rotation), quaternionVector));
			}

			void Transform::AdjustPosition(const DirectX::XMFLOAT2& positionValue)
			{
				SetPosition(DirectX::XMFLOAT2(position.x + positionValue.x, position.y + positionValue.y));
			}

			void XM_CALLCONV Transform::AdjustPosition(DirectX::FXMVECTOR positionVector)
			{
				SetPosition(DirectX::XMLoadFloat2(&position) + positionVector);
			}

			void Transform::SetActive(bool isActive)
			{
				this->isActive = true;
			}

			void Transform::SetParent(Transform* parent)
			{
				// 현재 부모가 있고 현재 부모가 변경할 부모가 아니면
				if (this->parent && this->parent != parent) // this->parent != nullptr && ...
					this->parent->RemoveChild(this);

				// 변경할 부모가 있으면
				if (parent) // parent != nullptr
				{
					// 변경할 부모 계층에 현재 트랜스폼 추가
					parent->AddChild(this);
					// 현재 게임 오브젝트의 부모 게임 오브젝트 활성화 여부 설정
					owner->SetParentActive(parent->GetGameObject()->IsActive());
				}
				else
				{
					// 현재 게임 오브젝트의 부모 게임 오브젝트가 없으므로 True
					owner->SetParentActive(true);
				}

				// 부모 참조 변경
				this->parent = parent;

				// 월드 좌표계 전체 갱신 비트 설정
				updateBits |= UpdateBits::WORLD_MATRIX
					| UpdateBits::SCALE_MATRIX | UpdateBits::SCALE
					| UpdateBits::ROTATION_MATRIX | UpdateBits::ROTATION
					| UpdateBits::TRANSLATION_MATRIX | UpdateBits::POSITION;

				// 자식 트랜스폼 갱신
				UpdateChilds();
			}

			void Transform::SetLocalScale(const DirectX::XMFLOAT2& localScaleValue)
			{
				localScale = localScaleValue;

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_SCALE_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_SCALE_VALUE_UPDATED;
				UpdateScaleFromParent();

				LocalScaleUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetLocalScale(DirectX::FXMVECTOR localScaleVector)
			{
				DirectX::XMStoreFloat2(&localScale, localScaleVector);

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_SCALE_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_SCALE_VALUE_UPDATED;
				UpdateScaleFromParent();

				LocalScaleUpdateBitSet();
			}

			void Transform::SetLocalRotationEuler(float localRotationValue)
			{
				localRotationEuler = localRotationValue;
				localRotation = CommonUtility::DirectXMathExtend::ToQuarternion(DirectX::XMFLOAT3(0.0f, 0.0f, localRotationValue * CommonUtility::DirectXMathExtend::XM_DEG2RAD));

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_ROTATION_VALUE_UPDATED;
				UpdateRotationFromParent();

				LocalRotationUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetLocalRotationEuler(DirectX::FXMVECTOR localRotationVector)
			{
				localRotationEuler = DirectX::XMVectorGetZ(localRotationVector);
				localRotation = CommonUtility::DirectXMathExtend::ToQuarternion(localRotationVector * CommonUtility::DirectXMathExtend::XM_DEG2RAD);

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_ROTATION_VALUE_UPDATED;
				UpdateRotationFromParent();

				LocalRotationUpdateBitSet();
			}

			void Transform::SetLocalRotationQuaternion(const DirectX::XMFLOAT4& localQuaternionValue)
			{
				localRotation = localQuaternionValue;
				localRotationEuler = CommonUtility::DirectXMathExtend::ToEulerAngle(localQuaternionValue).z * CommonUtility::DirectXMathExtend::XM_RAD2DEG;

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_ROTATION_VALUE_UPDATED;
				UpdateRotationFromParent();

				LocalRotationUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetLocalRotationQuaternion(DirectX::FXMVECTOR localQuaternionVector)
			{
				DirectX::XMStoreFloat4(&localRotation, localQuaternionVector);
				localRotationEuler = DirectX::XMVectorGetZ(CommonUtility::DirectXMathExtend::ToEulerAngleVector(localQuaternionVector)) * CommonUtility::DirectXMathExtend::XM_RAD2DEG;

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_ROTATION_VALUE_UPDATED;
				UpdateRotationFromParent();

				LocalRotationUpdateBitSet();
			}

			void Transform::SetLocalPosition(const DirectX::XMFLOAT2& localPositionValue)
			{
				localPosition = localPositionValue;

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_POSITION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_POSITION_VALUE_UPDATED;
				UpdatePositionFromParent();

				LocalPositionUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetLocalPosition(DirectX::FXMVECTOR localPositionVector)
			{
				DirectX::XMStoreFloat2(&localPosition, localPositionVector);

				setBits |= SetBits::LOCAL_VALUE_SET | SetBits::RECENT_LOCAL_POSITION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_POSITION_VALUE_UPDATED;
				UpdatePositionFromParent();

				LocalPositionUpdateBitSet();
			}

			void Transform::SetScale(const DirectX::XMFLOAT2& scaleValue)
			{
				scale = scaleValue;

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_SCALE_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_SCALE_VALUE_UPDATED;
				UpdateLocalScaleFromParent();

				ScaleUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetScale(DirectX::FXMVECTOR scaleVector)
			{
				DirectX::XMStoreFloat2(&scale, scaleVector);

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_SCALE_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_SCALE_VALUE_UPDATED;
				UpdateLocalScaleFromParent();

				ScaleUpdateBitSet();
			}

			void Transform::SetRotationEuler(float rotationValue)
			{
				rotationEuler = rotationValue;
				rotation = CommonUtility::DirectXMathExtend::ToQuarternion(DirectX::XMFLOAT3(0.0f, 0.0f, rotationValue * CommonUtility::DirectXMathExtend::XM_DEG2RAD));

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				UpdateLocalRotationFromParent();

				RotationUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetRotationEuler(DirectX::FXMVECTOR rotationVector)
			{
				rotationEuler = DirectX::XMVectorGetZ(rotationVector);
				rotation = CommonUtility::DirectXMathExtend::ToQuarternion(rotationVector * CommonUtility::DirectXMathExtend::XM_DEG2RAD);

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				UpdateLocalRotationFromParent();

				RotationUpdateBitSet();
			}

			void Transform::SetRotationQuaternion(const DirectX::XMFLOAT4& quaternionValue)
			{
				rotation = quaternionValue;
				rotationEuler = CommonUtility::DirectXMathExtend::ToEulerAngle(quaternionValue).z * CommonUtility::DirectXMathExtend::XM_RAD2DEG;

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				UpdateLocalRotationFromParent();

				RotationUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetRotationQuaternion(DirectX::FXMVECTOR quaternionVector)
			{
				DirectX::XMStoreFloat4(&rotation, quaternionVector);
				rotationEuler = DirectX::XMVectorGetZ(CommonUtility::DirectXMathExtend::ToEulerAngleVector(quaternionVector)) * CommonUtility::DirectXMathExtend::XM_RAD2DEG;

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_ROTATION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_ROTATION_VALUE_UPDATED;
				UpdateLocalRotationFromParent();

				RotationUpdateBitSet();
			}

			void Transform::SetPosition(const DirectX::XMFLOAT2& positionValue)
			{
				position = positionValue;

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_POSITION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_POSITION_VALUE_UPDATED;
				UpdateLocalPositionFromParent();

				PositionUpdateBitSet();
			}

			void XM_CALLCONV Transform::SetPosition(DirectX::FXMVECTOR positionVector)
			{
				DirectX::XMStoreFloat2(&position, positionVector);

				setBits |= SetBits::WORLD_VALUE_SET | SetBits::RECENT_POSITION_VALUE_UPDATED;
				setBits &= ~SetBits::RECENT_LOCAL_POSITION_VALUE_UPDATED;
				UpdateLocalPositionFromParent();

				PositionUpdateBitSet();
			}

			Transform* Transform::GetParent() const
			{
				return parent;
			}

			const std::unordered_set<Transform*>& Transform::GetChilds() const
			{
				return childs;
			}

			DirectX::XMFLOAT2 Transform::GetLocalScale()
			{
				UpdateLocalScale();

				return localScale;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetLocalScaleVector()
			{
				UpdateLocalScale();

				return DirectX::XMLoadFloat2(&localScale);
			}

			float Transform::GetLocalRotationEuler()
			{
				UpdateLocalRotation();

				return localRotationEuler;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetLocalRotationVectorEuler()
			{
				UpdateLocalRotation();

				return DirectX::XMLoadFloat(&localRotationEuler);
			}

			DirectX::XMFLOAT4 Transform::GetLocalRotationQuaternion()
			{
				UpdateLocalRotation();

				return localRotation;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetLocalRotationVectorQuaternion()
			{
				UpdateLocalRotation();

				return DirectX::XMLoadFloat4(&localRotation);
			}

			DirectX::XMFLOAT2 Transform::GetLocalPosition()
			{
				UpdateLocalPosition();

				return localPosition;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetLocalPositionVector()
			{
				UpdateLocalPosition();
				
				return DirectX::XMLoadFloat2(&localPosition);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetLocalMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&localMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetLocalScaleMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&localScaleMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetLocalRotationMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&localRotationMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetLocalTranslationMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&localTranslationMatrix);
			}

			DirectX::XMFLOAT2 Transform::GetScale()
			{
				UpdateScale();

				return scale;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetScaleVector()
			{
				UpdateScale();

				return DirectX::XMLoadFloat2(&scale);
			}

			float Transform::GetRotationEuler()
			{
				UpdateRotation();

				return rotationEuler;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetRotationVectorEuler()
			{
				UpdateRotation();

				return DirectX::XMLoadFloat(&rotationEuler);
			}

			DirectX::XMFLOAT4 Transform::GetRotationQuaternion()
			{
				UpdateRotation();

				return rotation;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetRotationVectorQuaternion()
			{
				UpdateRotation();

				return DirectX::XMLoadFloat4(&rotation);
			}

			DirectX::XMFLOAT2 Transform::GetPosition()
			{
				UpdatePosition();

				return position;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetPositionVector()
			{
				UpdatePosition();

				return DirectX::XMLoadFloat2(&position);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetWorldMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&worldMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetScaleMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&scaleMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetRotationMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&rotationMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::GetTranslationMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&translationMatrix);
			}

			DirectX::XMFLOAT2 Transform::GetUp() const
			{
				return up;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetUpVector() const
			{
				return DirectX::XMLoadFloat2(&up);
			}

			DirectX::XMFLOAT2 Transform::GetRight() const
			{
				return right;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::GetRightVector() const
			{
				return DirectX::XMLoadFloat2(&right);
			}

			bool Transform::IsUpdated() const
			{
				return isUpdated;
			}

			void Transform::ResetUpdated()
			{
				isUpdated = false;
			}

			void XM_CALLCONV Transform::UpdateDirection(DirectX::FXMMATRIX localRotationMatrix)
			{
				DirectX::XMStoreFloat2(&up, DirectX::XMVector2TransformCoord(CommonUtility::DirectXMathExtend::up, localRotationMatrix));
				DirectX::XMStoreFloat2(&right, DirectX::XMVector2TransformCoord(CommonUtility::DirectXMathExtend::right, localRotationMatrix));
			}

			void XM_CALLCONV Transform::UpdateLocalScale()
			{
				if (updateBits & UpdateBits::LOCAL_SCALE)
				{
					DirectX::XMStoreFloat2(&localScale, ComputeLocalScaleVector());
					updateBits &= ~UpdateBits::LOCAL_SCALE;
				}
			}

			void XM_CALLCONV Transform::UpdateLocalRotation()
			{
				if (updateBits & UpdateBits::LOCAL_ROTATION)
				{
					DirectX::XMStoreFloat4(&localRotation, ComputeLocalRotationVector());
					localRotationEuler = CommonUtility::DirectXMathExtend::ToEulerAngle(localRotation).z * CommonUtility::DirectXMathExtend::XM_RAD2DEG;
					updateBits &= ~UpdateBits::LOCAL_ROTATION;
				}
			}

			void XM_CALLCONV Transform::UpdateLocalPosition()
			{
				if (updateBits & UpdateBits::LOCAL_POSITION)
				{
					DirectX::XMStoreFloat2(&localPosition, ComputeLocalPositionVector());
					updateBits &= ~UpdateBits::LOCAL_POSITION;
				}
			}

			void XM_CALLCONV Transform::UpdateLocalScaleMatrix()
			{
				if (updateBits & UpdateBits::LOCAL_SCALE_MATRIX)
				{
					DirectX::XMStoreFloat4x4(&localScaleMatrix, DirectX::XMMatrixScaling(localScale.x, localScale.y, 1.0f));
					updateBits &= ~UpdateBits::LOCAL_SCALE_MATRIX;
				}
			}

			void XM_CALLCONV Transform::UpdateLocalRotationMatrix()
			{
				if (updateBits & UpdateBits::LOCAL_ROTATION_MATRIX)
				{
					DirectX::XMStoreFloat4x4(&localRotationMatrix, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&localRotation)));
					updateBits &= ~UpdateBits::LOCAL_ROTATION_MATRIX;
				}
			}

			void XM_CALLCONV Transform::UpdateLocalTranslationMatrix()
			{
				if (updateBits & UpdateBits::LOCAL_TRANSLATION_MATRIX)
				{
					DirectX::XMStoreFloat4x4(&localTranslationMatrix, DirectX::XMMatrixTranslation(localPosition.x, localPosition.y, 0.0f));
					updateBits &= ~UpdateBits::LOCAL_TRANSLATION_MATRIX;
				}
			}

			void XM_CALLCONV Transform::UpdateLocalMatrix()
			{
				// 지역 좌표계 행렬 갱신(LocalScaleMatrix * LocalRotationMatrix(Quaternion) * LocalTanslationMatrix)
				DirectX::XMStoreFloat4x4(&localMatrix, DirectX::XMLoadFloat4x4(&localScaleMatrix) * DirectX::XMLoadFloat4x4(&localRotationMatrix) * DirectX::XMLoadFloat4x4(&localTranslationMatrix));
				updateBits &= ~UpdateBits::LOCAL_MATRIX;
			}

			void Transform::UpdateLocalSRTMatrices()
			{
				// 지역 좌표계 크기 행렬 갱신
				UpdateLocalScaleMatrix();

				// 지역 좌표계 회전 행렬 갱신
				UpdateLocalRotationMatrix();

				// 지역 좌표계 위치 행렬 갱신
				UpdateLocalTranslationMatrix();

				// 지역 좌표계 행렬 갱신
				UpdateLocalMatrix();
			}

			void XM_CALLCONV Transform::UpdateScale()
			{
				if (updateBits & UpdateBits::SCALE)
				{
					DirectX::XMStoreFloat2(&scale, ComputeLossyScaleVector());
					updateBits &= ~UpdateBits::SCALE;
				}
			}

			void XM_CALLCONV Transform::UpdateRotation()
			{
				if (updateBits & UpdateBits::ROTATION)
				{
					DirectX::XMStoreFloat4(&rotation, ComputeWorldRotationVector());
					rotationEuler = CommonUtility::DirectXMathExtend::ToEulerAngle(rotation).z * CommonUtility::DirectXMathExtend::XM_RAD2DEG;
					updateBits &= ~UpdateBits::ROTATION;
				}
			}

			void XM_CALLCONV Transform::UpdatePosition()
			{
				if (updateBits & UpdateBits::POSITION)
				{
					DirectX::XMStoreFloat2(&position, ComputeWorldPositionVector());
					updateBits &= ~UpdateBits::POSITION;
				}
			}

			void XM_CALLCONV Transform::UpdateScaleMatrix()
			{
				if (updateBits & UpdateBits::SCALE_MATRIX)
				{
					float worldRatio = EngineConfig::Instance()->WorldRatio();
					DirectX::XMStoreFloat4x4(&scaleMatrix, DirectX::XMMatrixScaling(scale.x * worldRatio, scale.y * worldRatio, 1.0f));
					updateBits &= ~UpdateBits::SCALE_MATRIX;
				}
			}

			void XM_CALLCONV Transform::UpdateRotationMatrix()
			{
				if (updateBits & UpdateBits::ROTATION_MATRIX)
				{
					DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation)));
					updateBits &= ~UpdateBits::ROTATION_MATRIX;
				}
			}

			void XM_CALLCONV Transform::UpdateTranslationMatrix()
			{
				if (updateBits & UpdateBits::TRANSLATION_MATRIX)
				{
					float worldRatio = EngineConfig::Instance()->WorldRatio();
					DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslation(position.x * worldRatio, position.y * worldRatio, 0.0f));
					updateBits &= ~UpdateBits::TRANSLATION_MATRIX;
				}
			}

			void XM_CALLCONV Transform::UpdateWorldMatrix()
			{
				// 월드 좌표계 행렬 갱신(ScaleMatrix * RotationMatrix(Quaternion) * TanslationMatrix)
				DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMLoadFloat4x4(&scaleMatrix) * DirectX::XMLoadFloat4x4(&rotationMatrix) * DirectX::XMLoadFloat4x4(&translationMatrix));
				updateBits &= ~UpdateBits::WORLD_MATRIX;
			}

			void Transform::UpdateWorldSRTMatrices()
			{
				// 월드 좌표계 크기 행렬 갱신
				UpdateScaleMatrix();

				// 월드 좌표계 회전 행렬 갱신
				UpdateRotationMatrix();

				// 월드 좌표계 위치 행렬 갱신
				UpdateTranslationMatrix();

				// 월드 좌표계 행렬 갱신
				UpdateWorldMatrix();
			}

			void Transform::UpdateLocalToWorldScale()
			{
				if (updateBits & UpdateBits::LOCAL_SCALE)
				{
					localScale = scale;
					updateBits &= ~UpdateBits::LOCAL_SCALE;
				}
			}

			void Transform::UpdateLocalToWorldRotation()
			{
				if (updateBits & UpdateBits::LOCAL_ROTATION)
				{
					localRotationEuler = rotationEuler;
					localRotation = rotation;
					updateBits &= ~UpdateBits::LOCAL_ROTATION;
				}
			}

			void Transform::UpdateLocalToWorldPosition()
			{
				if (updateBits & UpdateBits::LOCAL_POSITION)
				{
					localPosition = position;
					updateBits &= ~UpdateBits::LOCAL_POSITION;
				}
			}

			void Transform::UpdateWorldToLocalScale()
			{
				if (updateBits & UpdateBits::SCALE)
				{
					scale = localScale;
					updateBits &= ~UpdateBits::SCALE;
				}
			}

			void Transform::UpdateWorldToLocalRotation()
			{
				if (updateBits & UpdateBits::ROTATION)
				{
					rotationEuler = localRotationEuler;
					rotation = localRotation;
					updateBits &= ~UpdateBits::ROTATION;
				}
			}

			void Transform::UpdateWorldToLocalPosition()
			{
				if (updateBits & UpdateBits::POSITION)
				{
					position = localPosition;
					updateBits &= ~UpdateBits::POSITION;
				}
			}

			void XM_CALLCONV Transform::UpdateWorldFromLocal()
			{
				// 월드 좌표계 크기 값 갱신
				UpdateWorldToLocalScale();
				// 월드 좌표계 크기 행렬 갱신
				UpdateScaleMatrix();

				// 월드 좌표계 회전 값 갱신
				UpdateWorldToLocalRotation();
				// 월드 좌표계 회전 행렬 갱신
				UpdateRotationMatrix();

				// 월드 좌표계 위치 값 갱신
				UpdateWorldToLocalPosition();
				// 월드 좌표계 위치 행렬 갱신
				UpdateTranslationMatrix();

				// 월드 좌표계 갱신(World = Local)
				UpdateWorldMatrix();
			}

			void XM_CALLCONV Transform::UpdateLocalFromWorld()
			{
				// 지역 좌표계 크기 값 갱신
				UpdateLocalToWorldScale();
				// 지역 좌표계 크기 행렬 갱신
				UpdateLocalScaleMatrix();

				// 지역 좌표계 회전 값 갱신
				UpdateLocalToWorldRotation();
				// 지역 좌표계 회전 행렬 갱신
				UpdateLocalRotationMatrix();

				// 지역 좌표계 위치 값 갱신
				UpdateLocalToWorldPosition();
				// 지역 좌표계 위치 행렬 갱신
				UpdateLocalTranslationMatrix();

				// 지역 좌표계 갱신(Local = World)
				UpdateLocalMatrix();
			}

			void XM_CALLCONV Transform::UpdateLocalTransform()
			{
				// 부모가 존재하는 경우 부모의 월드 행렬에 영향을 받음
				if (parent) // parent != nullptr
				{
#if NON_UNIFORM_SCALING // 지역 좌표계 행렬 분해(비균형 변형에 따라 분해 방식 차이 존재)
					// 지역 좌표계 갱신
					UpdateLocalMatrixFromParent();

					// 지역 좌표계 크기 값 갱신
					UpdateLocalScale();
					// 지역 좌표계 크기 행렬 갱신
					UpdateLocalScaleMatrix();

					// 지역 좌표계 회전 값 갱신
					UpdateLocalRotation();
					// 지역 좌표계 회전 행렬 갱신
					UpdateLocalRotationMatrix();

					// 지역 좌표계 위치 값 갱신
					UpdateLocalPosition();
					// 지역 좌표계 위치 행렬 갱신
					UpdateLocalTranslationMatrix();
#else
					// 지역 좌표계 갱신(WorldMatrix * Inverse Parent WorldMatrix)
					DirectX::XMMATRIX localXMMatrix = DirectX::XMLoadFloat4x4(&worldMatrix) * DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&parent->worldMatrix));
					DirectX::XMStoreFloat4x4(&localMatrix, localXMMatrix);
					updateBits &= ~UpdateBits::LOCAL_MATRIX;

					// 지역 좌표계 행렬 -> 크기, 회전, 위치 분해
					DirectX::XMVECTOR decomposeScaleVector, decomposeRotationVector, decomposePositionVector;
					DirectX::XMMatrixDecompose(&decomposeScaleVector, &decomposeRotationVector, &decomposePositionVector, localXMMatrix);

					// 지역 좌표계 크기 행렬 갱신
					if (updateBits & UpdateBits::LOCAL_SCALE_MATRIX)
					{
						DirectX::XMStoreFloat4x4(&localScaleMatrix, DirectX::XMMatrixScalingFromVector(decomposeScaleVector) * EngineConfig::Instance()->InverseWorldRatio());
						updateBits &= ~UpdateBits::LOCAL_SCALE_MATRIX;
					}
					// 지역 좌표계 크기 값 갱신
					if (updateBits & UpdateBits::LOCAL_SCALE)
					{
						DirectX::XMStoreFloat2(&localScale, decomposeScaleVector);
						updateBits &= ~UpdateBits::LOCAL_SCALE;
					}

					// 지역 좌표계 회전 행렬 갱신
					if (updateBits & UpdateBits::LOCAL_ROTATION_MATRIX)
					{
						DirectX::XMStoreFloat4x4(&localRotationMatrix, DirectX::XMMatrixRotationQuaternion(decomposeRotationVector));
						updateBits &= ~UpdateBits::LOCAL_ROTATION_MATRIX;
					}
					// 지역 좌표계 회전 값 갱신
					if (updateBits & UpdateBits::LOCAL_ROTATION)
					{
						localRotationEuler = CommonUtility::DirectXMathExtend::ToEulerAngle(decomposeRotationVector).z * CommonUtility::DirectXMathExtend::XM_RAD2DEG;
						DirectX::XMStoreFloat4(&localRotation, decomposeRotationVector);
						updateBits &= ~UpdateBits::LOCAL_ROTATION;
					}

					// 지역 좌표계 위치 행렬 갱신
					if (updateBits & UpdateBits::LOCAL_TRANSLATION_MATRIX)
					{
						DirectX::XMStoreFloat4x4(&localTranslationMatrix, DirectX::XMMatrixTranslationFromVector(decomposePositionVector) * EngineConfig::Instance()->InverseWorldRatio());
						updateBits &= ~UpdateBits::LOCAL_TRANSLATION_MATRIX;
					}
					// 지역 좌표계 위치 값 갱신
					if (updateBits & UpdateBits::LOCAL_POSITION)
					{
						DirectX::XMStoreFloat2(&localPosition, decomposePositionVector);
						updateBits &= ~UpdateBits::LOCAL_POSITION;
					}
#endif
				}
				// 부모가 존재하지 않는 경우 월드 좌표계로 지역 좌표계를 갱신
				else
				{
					// 지역 좌표계 갱신
					UpdateLocalFromWorld();
				}
			}

			void XM_CALLCONV Transform::UpdateWorldTransform()
			{
				// 부모가 존재하는 경우 부모의 월드 행렬에 영향을 받음
				if (parent) // parent != nullptr
				{
#if NON_UNIFORM_SCALING // 월드 좌표계 행렬 분해(비균형 변형에 따라 분해 방식 차이 존재)
					// 월드 좌표계 행렬 갱신
					UpdateWorldMatrixFromParent();

					// 월드 좌표계 크기 값 갱신
					UpdateScale();
					// 월드 좌표계 크기 행렬 갱신
					UpdateScaleMatrix();

					// 월드 좌표계 회전 값 갱신
					UpdateRotation();
					// 월드 좌표계 회전 행렬 갱신
					UpdateRotationMatrix();

					// 월드 좌표계 위치 값 갱신
					UpdatePosition();
					// 월드 좌표계 위치 행렬 갱신
					UpdateTranslationMatrix();
#else
					// 월드 좌표계 갱신(LocalMatrix * Parent WorldMatrix)
					DirectX::XMMATRIX worldXMMatrix = DirectX::XMLoadFloat4x4(&localMatrix) * DirectX::XMLoadFloat4x4(&parent->worldMatrix);
					DirectX::XMStoreFloat4x4(&worldMatrix, worldXMMatrix);
					updateBits &= ~UpdateBits::WORLD_MATRIX;

					// 월드 좌표계 행렬 -> 크기, 회전, 위치 분해
					DirectX::XMVECTOR decomposeScaleVector, decomposeRotationVector, decomposePositionVector;
					DirectX::XMMatrixDecompose(&decomposeScaleVector, &decomposeRotationVector, &decomposePositionVector, worldXMMatrix);

					// 월드 좌표계 크기 행렬 갱신
					if (updateBits & UpdateBits::SCALE_MATRIX)
					{
						DirectX::XMStoreFloat4x4(&scaleMatrix, DirectX::XMMatrixScalingFromVector(decomposeScaleVector) * EngineConfig::Instance()->InverseWorldRatio());
						updateBits &= ~UpdateBits::SCALE_MATRIX;
					}
					// 월드 좌표계 크기 값 갱신
					if (updateBits & UpdateBits::SCALE)
					{
						DirectX::XMStoreFloat2(&scale, decomposeScaleVector);
						updateBits &= ~UpdateBits::SCALE;
					}

					// 월드 좌표계 회전 행렬 갱신
					if (updateBits & UpdateBits::ROTATION_MATRIX)
					{
						DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixRotationQuaternion(decomposeRotationVector));
						updateBits &= ~UpdateBits::ROTATION_MATRIX;
					}
					// 월드 좌표계 회전 값 갱신
					if (updateBits & UpdateBits::ROTATION)
					{
						rotationEuler = CommonUtility::DirectXMathExtend::ToEulerAngle(decomposeRotationVector).z * CommonUtility::DirectXMathExtend::XM_RAD2DEG;
						DirectX::XMStoreFloat4(&rotation, decomposeRotationVector);
						updateBits &= ~UpdateBits::ROTATION;
					}

					// 월드 좌표계 위치 행렬 갱신
					if (updateBits & UpdateBits::TRANSLATION_MATRIX)
					{
						DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslationFromVector(decomposePositionVector) * EngineConfig::Instance()->InverseWorldRatio());
						updateBits &= ~UpdateBits::TRANSLATION_MATRIX;
					}
					// 월드 좌표계 위치 값 갱신
					if (updateBits & UpdateBits::POSITION)
					{
						DirectX::XMStoreFloat2(&position, decomposePositionVector);
						updateBits &= ~UpdateBits::POSITION;
					}
#endif
				}
				// 부모가 존재하지 않는 경우 지역 좌표계로 월드 좌표계를 갱신
				else
				{
					// 월드 좌표계 갱신
					UpdateWorldFromLocal();
				}
			}

			void Transform::UpdateLocalScaleFromParent()
			{
				// 부모 트랜스폼이 없는 경우 : 지역 좌표계 == 월드 좌표계
				if (!parent) // parent == nullptr
					localScale = scale;
				// 있으면 업데이트 비트 지역 좌표계 크기 값 설정
				else
					updateBits |= UpdateBits::LOCAL_SCALE;
			}

			void Transform::UpdateLocalRotationFromParent()
			{
				// 부모 트랜스폼이 없는 경우 : 지역 좌표계 == 월드 좌표계
				if (!parent) // parent == nullptr
				{
					localRotationEuler = rotationEuler;
					localRotation = rotation;
				}
				// 있으면 업데이트 비트 지역 좌표계 회전 값 설정
				else
				{
					updateBits |= UpdateBits::LOCAL_ROTATION;
				}
			}

			void Transform::UpdateLocalPositionFromParent()
			{
				// 부모 트랜스폼이 없는 경우 : 지역 좌표계 == 월드 좌표계
				if (!parent) // parent == nullptr
					localPosition = position;
				// 있으면 업데이트 비트 지역 좌표계 위치 값 설정
				else
					updateBits |= UpdateBits::LOCAL_POSITION;
			}

			void XM_CALLCONV Transform::UpdateLocalMatrixFromParent()
			{
				// 지역 좌표계 갱신(WorldMatrix * Inverse Parent WorldMatrix)
				DirectX::XMStoreFloat4x4(&localMatrix, DirectX::XMLoadFloat4x4(&worldMatrix) * DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&parent->worldMatrix)));
				updateBits &= ~UpdateBits::LOCAL_MATRIX;
			}

			void Transform::UpdateScaleFromParent()
			{
				// 부모 트랜스폼이 없는 경우 : 월드 좌표계 = 지역 좌표계
				if (!parent) // parent == nullptr
					scale = localScale;
				// 있으면 업데이트 비트 월드 좌표계 크기 값 설정
				else
					updateBits |= UpdateBits::SCALE;
			}

			void Transform::UpdateRotationFromParent()
			{
				// 부모 트랜스폼이 없는 경우 : 월드 좌표계 = 지역 좌표계
				if (!parent) // parent == nullptr
				{
					rotationEuler = localRotationEuler;
					rotation = localRotation;
				}
				// 있으면 업데이트 비트 월드 좌표계 회전 값 설정
				else
				{
					updateBits |= UpdateBits::ROTATION;
				}
			}

			void Transform::UpdatePositionFromParent()
			{
				// 부모 트랜스폼이 없는 경우 : 월드 좌표계 = 지역 좌표계
				if (!parent) // parent == nullptr
					position = localPosition;
				// 있으면 업데이트 비트 월드 좌표계 위치 값 설정
				else
					updateBits |= UpdateBits::POSITION;
			}

			void XM_CALLCONV Transform::UpdateWorldMatrixFromParent()
			{
				// 월드 좌표계 행렬 갱신(LocalMatrix * Parent WorldMatrix)
				DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMLoadFloat4x4(&localMatrix) * DirectX::XMLoadFloat4x4(&parent->worldMatrix));
				updateBits &= ~UpdateBits::WORLD_MATRIX;
			}

			void Transform::ChildScaleUpdateBitSet()
			{
				// 월드 좌표계 행렬
				// 월드 좌표계 크기 행렬, 월드 좌표계 크기
				// 월드 좌표계 위치 행렬, 월드 좌표계 위치
				for (Transform* child : childs)
				{
					child->updateBits |= UpdateBits::WORLD_MATRIX
						| UpdateBits::SCALE_MATRIX | UpdateBits::SCALE
						| UpdateBits::TRANSLATION_MATRIX | UpdateBits::POSITION;
					ChildScaleUpdateBitSet();
				}
			}

			void Transform::ChildRotationUpdateBitSet()
			{
				// 월드 좌표계 행렬
				// 월드 좌표계 크기 행렬, 월드 좌표계 크기
				// 월드 좌표계 회전 행렬, 월드 좌표계 회전
				// 월드 좌표계 위치 행렬, 월드 좌표계 위치
				for (Transform* child : childs)
				{
					child->updateBits |= UpdateBits::WORLD_MATRIX
						| UpdateBits::SCALE_MATRIX | UpdateBits::SCALE
						| UpdateBits::ROTATION_MATRIX | UpdateBits::ROTATION
						| UpdateBits::TRANSLATION_MATRIX | UpdateBits::POSITION;
					ChildRotationUpdateBitSet();
				}
			}

			void Transform::ChildPositionUpdateBitSet()
			{
				// 월드 좌표계 행렬
				// 월드 좌표계 위치 행렬, 월드 좌표계 위치
				for (Transform* child : childs)
				{
					child->updateBits |= UpdateBits::WORLD_MATRIX
						| UpdateBits::TRANSLATION_MATRIX | UpdateBits::POSITION;
					ChildPositionUpdateBitSet();
				}
			}

			void Transform::LocalScaleUpdateBitSet()
			{
				// 지역 좌표계 크기는 갱신 되었으므로 갱신 비트 Off
				updateBits &= ~UpdateBits::LOCAL_SCALE;

				// 지역 좌표계 행렬, 지역 좌표계 크기 행렬
				// 월드 좌표계 행렬, 월드 좌표계 크기 행렬
				updateBits |= UpdateBits::LOCAL_MATRIX | UpdateBits::LOCAL_SCALE_MATRIX | UpdateBits::WORLD_MATRIX | UpdateBits::SCALE_MATRIX;

				ChildScaleUpdateBitSet();
			}

			void Transform::LocalRotationUpdateBitSet()
			{
				// 지역 좌표계 회전은 갱신 되었으므로 갱신 비트 Off
				updateBits &= ~UpdateBits::LOCAL_ROTATION;

				// 지역 좌표계 행렬, 지역 좌표계 회전 행렬
				// 월드 좌표계 행렬, 월드 좌표계 회전 행렬
				updateBits |= UpdateBits::LOCAL_MATRIX | UpdateBits::LOCAL_ROTATION_MATRIX | UpdateBits::WORLD_MATRIX | UpdateBits::ROTATION_MATRIX;

				ChildRotationUpdateBitSet();
			}

			void Transform::LocalPositionUpdateBitSet()
			{
				// 지역 좌표계 위치는 갱신 되었으므로 갱신 비트 Off
				updateBits &= ~UpdateBits::LOCAL_POSITION;

				// 지역 좌표계 행렬, 지역 좌표계 위치 행렬
				// 월드 좌표계 행렬, 월드 좌표계 위치 행렬
				updateBits |= UpdateBits::LOCAL_MATRIX | UpdateBits::LOCAL_TRANSLATION_MATRIX | UpdateBits::WORLD_MATRIX | UpdateBits::TRANSLATION_MATRIX;

				ChildPositionUpdateBitSet();
			}

			void Transform::ScaleUpdateBitSet()
			{
				// 월드 좌표계 크기는 갱신 되었으므로 갱신 비트 Off
				updateBits &= ~UpdateBits::SCALE;

				// 월드 좌표계 행렬, 월드 좌표계 크기 행렬
				// 지역 좌표계 행렬, 지역 좌표계 크기 행렬
				updateBits |= UpdateBits::WORLD_MATRIX | UpdateBits::SCALE_MATRIX | UpdateBits::LOCAL_MATRIX | UpdateBits::LOCAL_SCALE_MATRIX;

				ChildScaleUpdateBitSet();
			}

			void Transform::RotationUpdateBitSet()
			{
				// 월드 좌표계 회전은 갱신 되었으므로 갱신 비트 Off
				updateBits &= ~UpdateBits::ROTATION;

				// 월드 좌표계 행렬, 월드 좌표계 회전 행렬
				// 지역 좌표계 행렬, 지역 좌표계 회전 행렬
				updateBits |= UpdateBits::WORLD_MATRIX | UpdateBits::ROTATION_MATRIX | UpdateBits::LOCAL_MATRIX | UpdateBits::LOCAL_ROTATION_MATRIX;

				ChildRotationUpdateBitSet();
			}

			void Transform::PositionUpdateBitSet()
			{
				// 지역 좌표계 크기는 갱신 되었으므로 갱신 비트 Off
				updateBits &= ~UpdateBits::LOCAL_SCALE;

				// 지역 좌표계 행렬, 지역 좌표계 위치 행렬
				// 월드 좌표계 행렬, 월드 좌표계 위치 행렬
				updateBits |= UpdateBits::WORLD_MATRIX | UpdateBits::TRANSLATION_MATRIX | UpdateBits::LOCAL_MATRIX | UpdateBits::LOCAL_TRANSLATION_MATRIX;

				ChildScaleUpdateBitSet();
			}

			DirectX::XMVECTOR Transform::ComputeLocalScaleVector()
			{
				DirectX::XMVECTOR quaternion = ComputeWorldRotationVector();
				DirectX::XMVECTOR scaleVector = DirectX::XMLoadFloat2(&scale);
				DirectX::XMVECTOR x = DirectX::XMVector3Rotate(DirectX::XMVectorSet(DirectX::XMVectorGetX(scaleVector), 0.0f, 0.0f, 0.0f), quaternion);
				DirectX::XMVECTOR y = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, DirectX::XMVectorGetY(scaleVector), 0.0f, 0.0f), quaternion);
				DirectX::XMVECTOR z = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, DirectX::XMVectorGetZ(scaleVector), 0.0f), quaternion);

				DirectX::XMMATRIX rotationScaleMatrix = DirectX::XMMatrixSet
				(
					DirectX::XMVectorGetX(x), DirectX::XMVectorGetY(x), DirectX::XMVectorGetZ(x), 0.0f,
					DirectX::XMVectorGetX(y), DirectX::XMVectorGetY(y), DirectX::XMVectorGetZ(y), 0.0f,
					DirectX::XMVectorGetX(z), DirectX::XMVectorGetY(z), DirectX::XMVectorGetZ(z), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);

				// LocalRotationMatrix 이지만 ScaleMatrix는 1.0f 1.0f 1.0f Scale과 같아서 생략
				DirectX::XMMATRIX scaleRotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&localRotation));
				if (parent)
					scaleRotationMatrix *= parent->ComputeWorldScaleRotationMatrix();
				
				//  R * S * S^-1 * R^-1
				DirectX::XMMATRIX localScaleRotationMatrix = rotationScaleMatrix * DirectX::XMMatrixInverse(nullptr, scaleRotationMatrix);

				return DirectX::XMVectorSet
				(
					DirectX::XMVectorGetX(localScaleRotationMatrix.r[0]),
					DirectX::XMVectorGetY(localScaleRotationMatrix.r[1]),
					DirectX::XMVectorGetZ(localScaleRotationMatrix.r[2]),
					DirectX::XMVectorGetW(localScaleRotationMatrix.r[3])
				);
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::ComputeLocalRotationVector()
			{
				// 부모가 없으면 변경하려는 회전 값이 월드 회전 값임
				if (!parent) // parent == nullptr
					return DirectX::XMLoadFloat4(&rotation);

				// 지역 좌표계 회전 값 = 현재 회전 값 * 부모의 역 회전 값
				return DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&rotation), DirectX::XMQuaternionInverse(parent->ComputeWorldRotationVector()));
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::ComputeLocalPositionVector()
			{
				DirectX::XMVECTOR localPosition = DirectX::XMLoadFloat2(&position);
				if (parent) // parent != nullptr
					localPosition = parent->ComputeInverseTransformPositionVector(localPosition);

				return localPosition;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::ComputeInverseTransformPositionVector(DirectX::FXMVECTOR positionVector)
			{
				DirectX::XMVECTOR inversePosition = positionVector;
				if (parent) // parent != nullptr
					inversePosition = parent->ComputeInverseTransformPositionVector(inversePosition);

				// S * R * T 순으로 행렬 곱셈을 하므로 역으로 변환을 적용
				// 역 위치 (-T)
				inversePosition -= DirectX::XMLoadFloat2(&localPosition);
				// 역 회전 (R^-1) 곱셈
				inversePosition = DirectX::XMVector3Rotate(inversePosition, DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(&localRotation)));
				// 역 크기 (1 / S)
				inversePosition /= DirectX::XMLoadFloat2(&localScale);

				return inversePosition;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::ComputeLossyScaleVector()
			{
				// 크기 * 회전 행렬과 역 회전 값 행렬을 곱하여 행렬에 회전 제거 (S * R * R^-1)
				DirectX::XMMATRIX scaleSkewMatrix = ComputeWorldScaleRotationMatrix() * DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionInverse(ComputeWorldRotationVector()));

				// 크기 왜곡 행렬에서 크기값 반환
				return DirectX::XMVectorSet
				(
					DirectX::XMVectorGetX(scaleSkewMatrix.r[0]),
					DirectX::XMVectorGetY(scaleSkewMatrix.r[1]),
					DirectX::XMVectorGetZ(scaleSkewMatrix.r[2]),
					DirectX::XMVectorGetW(scaleSkewMatrix.r[3])
				);
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::ComputeWorldRotationVector()
			{
				// 현재 트랜스폼의 지역 회전 값(쿼터니언)을 가져옴
				DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&localRotation);

				// 부모 트랜스폼으로 올라가면서 쿼터니언 곱셈
				Transform* iterator = parent;
				while (iterator) // iterator != nullptr
				{
					// 쿼터니언 곱셈 연산 (내부에서 곱셈 Q2 * Q1)
					quaternion = DirectX::XMQuaternionMultiply(quaternion, DirectX::XMLoadFloat4(&iterator->localRotation));
					iterator = iterator->parent;
				}

				// 계산된 월드 좌표계 회전(쿼터니언) 반환
				return quaternion;
			}

			DirectX::XMVECTOR XM_CALLCONV Transform::ComputeWorldPositionVector()
			{
				// 현재 트랜스폼의 지역 위치 값을 가져옴
				DirectX::XMVECTOR position = DirectX::XMLoadFloat2(&localPosition);

				// 부모 트랜스폼으로 올라가면서 위치 계산
				Transform* iterator = parent;
				while (iterator) // iterator != nullptr
				{
					// 부모 지역 좌표계 크기 곱셈
					position *= DirectX::XMLoadFloat2(&iterator->localScale);
					// 부모 지역 좌표계 회전 곱셈(벡터 * 쿼터니언)
					position = DirectX::XMVector3Rotate(position, DirectX::XMLoadFloat4(&iterator->localRotation));
					// 부모 지역 좌표계 위치 덧셈
					position += DirectX::XMLoadFloat2(&iterator->localPosition);

					iterator = iterator->parent;
				}

				// 계산된 월드 좌표계 위치 반환
				return position;
			}

			DirectX::XMMATRIX XM_CALLCONV Transform::ComputeWorldScaleRotationMatrix()
			{
				// LocalScaleMatrix * LocalRotationMatrix
				DirectX::XMMATRIX scaleRotationMatrix = DirectX::XMMatrixScaling(localScale.x, localScale.y, 1.0f) * DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&localRotation));

				// 부모 트랜스폼으로 올라가면서 크기 * 회전 행렬을 곱해줌
				Transform* iterator = parent;
				if (iterator) // iterator != nullptr
				{
					// (S * R) * (Parent (S * R))
					scaleRotationMatrix *= iterator->ComputeWorldScaleRotationMatrix(); // 재귀
				}

				// 계산된 월드 좌표계 회전 크기 행렬 반환
				return scaleRotationMatrix;
			}
		} // Component
	} // GameObject
} // Game
