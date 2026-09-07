#pragma once

#include <wrl/client.h>
#include <d3d11.h>

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 인덱스 버퍼 클래스
			 * @date 2024-10-09
			 * @class
			 * 정점 버퍼에 있는 정점 순서 데이터를 저장하는 버퍼
			 */
			class IndexBuffer
			{
			private: // ==== [구성 요소] ====
				Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;	// DirectX11 버퍼
				UINT indexCount;								// 인덱스 개수

			public: // ==== [생성자 및 소멸자] ====
				IndexBuffer();
				/** @param moveIndexBuffer : 이동 생성할 인덱스 버퍼 */
				IndexBuffer(IndexBuffer&& moveIndexBuffer)  noexcept;
				~IndexBuffer();

			private: // ==== [생성자] ====
				/**
				 * @details
				 * GPU와 관련된 버퍼에서 복사 생성자는 복사 중복과
				 * GPU, CPU 동기화 및 추가 연산이 발생할 수 있어 사용을 방지
				 * @deprecated
				 */
				IndexBuffer(const IndexBuffer& copyIndexBuffer) = delete;

			public: // ==== [연산자 재정의] ====
				IndexBuffer& operator=(IndexBuffer&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				IndexBuffer& operator=(const IndexBuffer& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 인덱스 버퍼 초기화
				 * @details
				 * 인덱스 배열의 값들로 인덱스 버퍼를 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param indices : 인덱스 배열
				 * @param indexCount : 인덱스 개수
				 * @return 인덱스 버퍼 생성 결과
				 */
				HRESULT Initialize(ID3D11Device* device, DWORD* indices, UINT indexCount);
				/**
				 * @brief 인덱스 버퍼 자원 해제
				 * @details
				 * DirectX11 버퍼 자원 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/** @return 인덱스 버퍼 내부 포인터(참조용) */
				ID3D11Buffer* Get() const;
				/** @return 인덱스 버퍼 내부 포인터 주소 값(생성 및 초기화용) */
				ID3D11Buffer* const* GetAddressOf() const;

				/** @return 인덱스 개수 */
				UINT GetIndexCount() const;
			};
		} // Resource
	} // Graphics
} // Engine
