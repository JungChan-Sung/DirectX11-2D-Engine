#pragma once

// std::move
#include <utility>
// std::memset, std::memcpy
#include <memory>
#include "BaseConstantBuffer.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 상수 버퍼 템플릿 클래스 : 상수 버퍼 기반 클래스 상속
			 * @date 2024-10-15
			 * @class
			 * 상수(고정된 크기를 지닌 데이터)의 값을 저장해두는 버퍼
			 */
			template<typename T>
			class ConstantBuffer : public BaseConstantBuffer
			{
			private: // ==== [데이터] ====
				T data;	// 상수 버퍼 데이터

			public: // ==== [생성자 및 소멸자] ====
				ConstantBuffer() : BaseConstantBuffer()
				{
					std::memset(&data, 0, sizeof(T));
				}
				/** @param moveConstantBuffer : 이동 생성할 상수 버퍼 */
				ConstantBuffer(ConstantBuffer<T>&& moveConstantBuffer) noexcept
					: BaseConstantBuffer(std::exchange(moveConstantBuffer, nullptr)),
					data(std::move(moveConstantBuffer.data))
				{
				}
				virtual ~ConstantBuffer()
				{
				}

			private: // ==== [생성자] ====
				/**
				 * @details
				 * GPU와 관련된 버퍼에서 복사 생성자는 복사 중복과
				 * GPU, CPU 동기화 및 추가 연산이 발생할 수 있어 사용을 방지
				 * @deprecated
				 */
				ConstantBuffer(const ConstantBuffer<T>& copyConstantBuffer) = delete;

			public: // ==== [연산자 재정의] ====
				ConstantBuffer<T>& operator=(ConstantBuffer<T>&& rhs) noexcept
				{
					if (this != &rhs)
					{
						buffer = std::exchange(rhs.buffer, nullptr);
						data = std::move(rhs.data);
					}

					return *this;
				}

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				ConstantBuffer<T>& operator=(const ConstantBuffer<T>& rhs) = delete;

			public: // ==== [Setup(상수 버퍼 기반) 재정의] ====
				/**
				 * @brief 상수 버퍼 초기화
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 상수 버퍼 생성 결과
				 */
				virtual HRESULT Initialize(ID3D11Device* device) override
				{
					// 버퍼가 있는 경우 리셋(내부 포인터를 해제하고 nullptr로 설정)
					if (buffer.Get()) // buffer != nullptr
						buffer.Reset();

					D3D11_BUFFER_DESC constantBufferDescription;
					std::memset(&constantBufferDescription, 0, sizeof(D3D11_BUFFER_DESC));
					// 셰이더의 cbuffer(Constant Buffer)는 16byte단위로 정렬
					constantBufferDescription.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
					// 게임 오브젝트의 컴포넌트 마다 갱신 되기 때문에 동적(Dynamic)으로 설정
					constantBufferDescription.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
					constantBufferDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
					// CPU는 쓰기(Write)만 하기 때문에 사용에 맞춘다
					constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

					// 상수 버퍼 생성
					return device->CreateBuffer(&constantBufferDescription, nullptr, buffer.GetAddressOf());
				}

			public: // ==== [상수 버퍼 동작(상수 버퍼 기반) 재정의] ====
				/**
				 * @brief 상수 버퍼 변경사항 적용
				 * @details
				 * 상수 버퍼의 데이터 값을 GPU와 동기화 시킴
				 * @param deviceContext : DirectX11 장치 정보 참조용 포인터
				 * @return 상수 버퍼 변경사항 적용 성공 여부
				 */
				virtual bool ApplyChanges(ID3D11DeviceContext* deviceContext) override
				{
					// UpdateSubresource는 아래 방식보다 간단한 방식으로 데이터를 전체 갱신해줌
					// deviceContext->UpdateSubresource(buffer.Get(), 0u, nullptr, &data, 0u, 0u);

					// DirectX11 리소스(버퍼)의 데이터를 CPU에서 접근 및 수정하도록 잠금(Lock)을 걸어줌
					// GPU에서 사용 중인 리소스의 데이터를 CPU가 직접 수정하도록함
					D3D11_MAPPED_SUBRESOURCE resource;
					HRESULT hr = deviceContext->Map(buffer.Get(), 0u, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0u, &resource);
					if (FAILED(hr))
					{
						CommonUtility::Log::ErrorMessage(hr, Message::Error::CONSTANT_BUFFER_MAP_FAILURE);
						return false;
					}

					// 상수 버퍼 데이터 복사(= 데이터 수정)
					std::memcpy(resource.pData, &data, sizeof(T));

					// 잠금 해제
					deviceContext->Unmap(buffer.Get(), 0u);

					return true;
				}

			public: // ==== [Set] ====
				/** @param data : 상수 버퍼 데이터 */
				void SetData(const T& data)
				{
					this->data = data;
				}
				void SetData(T&& data)
				{
					// 보편 참조인 경우 std::forward를 사용
					// void SetData(const T& data); 위 함수 없이 사용 가능하나
					// 현재 함수에서 data의 값이 변경될 수 있는 위험 존재해서 두개로 선언함
					this->data = std::move(data);
				}

			public: // ==== [Get] ====
				/** @return 상수 버퍼 데이터 */
				T GetData() const
				{
					return data;
				}
			};
		} // Resource
	} // Graphics
} // Engine