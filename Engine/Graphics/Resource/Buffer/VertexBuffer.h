#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <type_traits>
// std::memset
#include <memory>
#include "Engine/Graphics/Common/Vertex.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 정점 버퍼 클래스
			 * @date 2024-10-09
			 * @class
			 * 정점의 정보들을 연속적으로 저장해두는 버퍼
			 * 정점 구조체를 상속 받은 구조체만 템플릿 가능
			 */
			class VertexBuffer
			{
			private: // ==== [구성 요소] ====
				Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;	// DirectX11 버퍼
				UINT memoryStride;								// 정점 메모리 간격
				UINT vertexCount;								// 정점 개수

			public: // ==== [생성자 및 소멸자] ====
				VertexBuffer();
				/** @param moveVertexBuffer : 이동 생성할 정점 버퍼 */
				VertexBuffer(VertexBuffer&& moveVertexBuffer) noexcept;
				~VertexBuffer();

			private: // ==== [생성자] ====
				/**
				 * @details
				 * GPU와 관련된 버퍼에서 복사 생성자는 복사 중복과
				 * GPU, CPU 동기화 및 추가 연산이 발생할 수 있어 사용을 방지
				 * @deprecated
				 */
				VertexBuffer(const VertexBuffer& copyVertexBuffer) = delete;

			public: // ==== [연산자 재정의] ====
				VertexBuffer& operator=(VertexBuffer&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				VertexBuffer& operator=(const VertexBuffer& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 템플릿 정점 버퍼 초기화
				 * @details
				 * 정점 배열의 값들로 정점 버퍼를 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @tparam vertices : 정점 배열
				 * @param vertexCount : 정점 개수
				 * @return 정점 버퍼 생성 결과
				 */
				template<typename T, typename = std::enable_if<std::is_base_of_v<Common::Vertex, T>>>
				HRESULT Initialize(ID3D11Device* device, T* vertices, UINT vertexCount)
				{
					// 버퍼가 있는 경우 리셋(내부 포인터를 해제하고 nullptr로 설정)
					if (buffer.Get()) // buffer != nullptr
						buffer.Reset();

					memoryStride = sizeof(T);
					this->vertexCount = vertexCount;

					D3D11_BUFFER_DESC vertexBufferDescription;
					std::memset(&vertexBufferDescription, 0, sizeof(D3D11_BUFFER_DESC));
					vertexBufferDescription.ByteWidth = memoryStride * vertexCount; // 버퍼 크기 설정
					vertexBufferDescription.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
					vertexBufferDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

					D3D11_SUBRESOURCE_DATA vertexBufferData;
					std::memset(&vertexBufferData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
					vertexBufferData.pSysMem = vertices; // 버퍼 데이터 초기화

					// 정점 버퍼 생성
					return device->CreateBuffer(&vertexBufferDescription, &vertexBufferData, buffer.GetAddressOf());
				}
				/**
				 * @brief 정점 버퍼 자원 해제
				 * @details
				 * DirectX11 버퍼 자원 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/** @return 정점 버퍼 내부 포인터(참조용) */
				ID3D11Buffer* Get() const;
				/** @return 정점 버퍼 내부 포인터 주소 값(생성 및 초기화용) */
				ID3D11Buffer* const* GetAddressOf() const;

				/** @return 정점 개수 */
				UINT GetVertexCount() const;

				/** @return 정점 메모리 간격 */
				UINT GetStride() const;
				/** @return 정점 메모리 간격 참조용 포인터 */
				const UINT* GetStridePtr() const;
			};
		} // Resource
	} // Graphics
} // Engine

