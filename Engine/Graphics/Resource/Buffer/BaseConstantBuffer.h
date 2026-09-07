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
			 * @brief 상수 버퍼 기반 추상화 클래스
			 * @date 2024-10-15
			 * @class
			 * 상수 버퍼의 기반 클래스
			 * 이후 클래스는 템플릿 클래스이므로 관리를 쉽게하기 위한 클래스
			 */
			class BaseConstantBuffer abstract
			{
			protected: // ==== [구성 요소] ====
				Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;	// DirectX11 버퍼

			public: // ==== [생성자 및 소멸자] ====
				BaseConstantBuffer();
				/** @param moveConstantBuffer : 이동 생성할 상수 버퍼 기반 */
				BaseConstantBuffer(BaseConstantBuffer&& moveBaseConstantBuffer) noexcept;
				virtual ~BaseConstantBuffer();

			private: // ==== [생성자] ====
				/**
				 * @details
				 * GPU와 관련된 버퍼에서 복사 생성자는 복사 중복과
				 * GPU, CPU 동기화 및 추가 연산이 발생할 수 있어 사용을 방지
				 * @deprecated
				 */
				BaseConstantBuffer(const BaseConstantBuffer& copyConstantBuffer) = delete;

			public: // ==== [연산자 재정의] ====
				BaseConstantBuffer& operator=(BaseConstantBuffer&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				BaseConstantBuffer& operator=(const BaseConstantBuffer& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 상수 버퍼 초기화
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 상수 버퍼 생성 결과
				 */
				virtual HRESULT Initialize(ID3D11Device* device) abstract;
				/**
				 * @brief 상수 버퍼 자원 해제
				 * @details
				 * DirectX11 버퍼 자원 해제
				 */
				void Release();

			public: // ==== [상수 버퍼 동작] ====
				/**
				 * @brief 상수 버퍼 변경사항 적용
				 * @param deviceContext : DirectX11 장치 정보 참조용 포인터
				 * @return 상수 버퍼 변경사항 적용 성공 여부
				 */
				virtual bool ApplyChanges(ID3D11DeviceContext* deviceContext) abstract;

			public: // ==== [Get] ====
				/** @return 상수 버퍼 내부 포인터(참조용) */
				ID3D11Buffer* Get() const;
				/** @return 상수 버퍼 내부 포인터 주소 값(생성 및 초기화용) */
				ID3D11Buffer* const* GetAddressOf() const;
			};
		} // Resource
	} // Graphics
} // Engine