#pragma once
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3d11.lib")

#include <Windows.h>
// Microsoft::WRL::ComPtr를 사용하기 위해 추가
#include <wrl/client.h>
#include <string>
#include "Observer/IDeviceRequestObserver.h"

namespace Engine
{
	namespace Graphics
	{
		// DirectX 그래픽스 설명
		// Device는 자원(셰이더, 텍스쳐, 버퍼 등)을 생성 및 상태 관리
		// DeviceContext는 Device를 통해 생성된 자원들을 렌더링 명령을 실행 및 렌더링 파이프라인(렌더 타겟 뷰, 블렌드 상태, 깊이 버퍼 등)의 상태를 설정
		// SwapChain은 더블 버퍼링 기법을 수행해주며 렌더링된 화면을 출력
		// DirectX 그래픽스의 View는 서술자 역할을 함
		// RenderTargetView는 렌더링 결과물을 보여줄 화면 같은 역할(여러개이면 여러 장면 렌더링 가능)
		// RasterizerState는 기하학적 프리미티브(삼각형, 선 등)을 픽셀로 변환하여 화면에 표시를 해줌
		// BlendState는 렌더링 파이프라인의 출력 병합기(Output Merger) 단계에서 색상 값을 혼합하는 방식을 정함
		// 여러개의 텍스쳐나 버퍼에서 얻은 색상 값들을 합쳐서 최종적인 픽셀 색상을 결정(투명도가 적절한 예시)
		// DepthStencilState는 깊이/스텐실 테스트를 수행하기 위해 설정함(물체의 앞뒤 관계 판단, 특정 영역에 대한 쓰기/읽기 동작 제한 등 규칙을 설정함)
		// DepthStencilView는 깊이/스텐실 테스트에 사용되는 뷰, 깊이 정보와 스텐실 값을 저장하고 관리
		// IDXGIFactory는 DirectX Graphics Infrastructure (DXGI)에서 DXGI의 다른 객체들을 생성 및 관리함
		// IDXGIAdapter는 그래픽스 하드웨어(GPU)에 대한 정보 제공
		// IDXGIOutput는 IDXGIAdapter에 연결된 출력 장치(모니터)에 대한 정보 제공 및 설정 변경에 사용
		/**
		 * @brief 엔진 그래픽스 클래스 : DirectX11 장치 요청 관찰자 인터페이스 상속
		 * @date 2024-11-05
		 * @class
		 * DirectX11 API 그래픽스 라이브러리를 이용하여 렌더 처리
		 * @todo
		 * 리셋 및 세트 함수 제작
		 */
		class Graphics : public Observer::IDeviceRequestObserver
		{
		private: // ==== [선언] ====
			// 그래픽스 상수
			enum GraphicsConstant : UINT
			{
				MEGA_BYTE = 1048576u,		// 1024 kb * 1024 kb = 1,048,576 bit = 1 mb
				RGBA = 4u,					// 색상 종류 개수
				SAMPLE_MASK = 0xffffffff	// UINT 모든 비트가 1인 수
			};

		private: // ==== [화면 정보] ====
			UINT syncInterval;				// 수직 동기화 비율

		private: // ==== [그래픽 카드] ====
			UINT grapicsCardMemory;					// 그래픽 카드 메모리
			std::wstring grapicsCardDescription;	// 그래픽 카드 속성

		private: // ==== [DirectX11 Main] ====
			Microsoft::WRL::ComPtr<ID3D11Device> device;						// DirectX11 장치
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;			// DirectX11 장치 정보
			Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;					// DirectX11 더블 버퍼링용 스왑체인
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;	// DirectX11 렌더 타겟 뷰
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;		// DirectX11 래스터라이저 상태
			Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;				// DirectX11 블렌드 상태

		private: // ==== [DirectX11 Sub] ====
			Microsoft::WRL::ComPtr<IDXGIFactory> factory;		// DirectX 그래픽스 기반 팩토리
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;		// DirectX 그래픽스 기반 어댑터(GPU)
			Microsoft::WRL::ComPtr<IDXGIOutput> output;			// DirectX 그래픽스 기반 출력(모니터)
			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;	// 백 버퍼
			DXGI_FORMAT format;									// 그래픽스 기반 포맷(출력) 플래그
			UINT displayModeCount;								// 모니터 모드 개수
			DXGI_MODE_DESC* displayModeList;					// 모니터 모드 리스트

		public: // ==== [생성자 및 소멸자] ====
			Graphics();
			~Graphics();

		public: // ==== [Setup] ====
			/**
			 * @brief DirectX11 그래픽스 초기화
			 * @date 2024-11-05
			 * @details
			 * DirectX11 그래픽스 세팅에 필요한 자원 생성 및 설정
			 * @throw
			 * 필요한 자원 생성 및 설정에 실패시 예외처리 후 프로그램 종료
			 * @param hWnd : 윈도우 식별 값
			 * @param width : 백버퍼 너비
			 * @param height : 백버퍼 높이
			 * @param isFullScreen : 전체화면 여부
			 * @param isVsync : 수직 동기화 여부
			 * @return DirectX11 그래픽스 초기화 성공 여부
			*/
			bool Initialize(HWND hWnd, UINT width, UINT height, bool isFullScreen, bool isVsync);
			/**
			 * @brief 그래픽스 자원 해제
			 * @details
			 * DirectX11 그래픽스 렌더링에 사용된 자원 해제
			 */
			void Release();

		public: // ==== [렌더] ====
			/**
			 * @brief 렌더 시작
			 * @details
			 * 렌더링에 필요한 초기 설정 수행
			 * 2D 엔진에서는 렌더 타겟 뷰만 초기화
			 * 3D 엔진에서는 깊이/스텐실 뷰 초기화
			 */
			void BeginRender();
			/**
			 * @brief 렌더 종료
			 * @details
			 * 스왑 체인을 프레젠테이션하여 화면에 출력
			 */
			void EndRender();

		public: // ==== [DirectX11 장치 요청 관찰자] ====
			/**
			 * @brief DirectX11 장치 요청
			 * @date 2024-10-21
			 * @return DirectX11 참조용 포인터
			 */
			ID3D11Device* DeviceRequest();

		public: // ==== [Get] ====
			/** @return DirectX 장치 포인터 */
			ID3D11Device* GetDevice() const;
			/** @return DirectX 장치 정보 포인터 */
			ID3D11DeviceContext* GetDeviceContext() const;

		private: // ==== [DirectX11 그래픽스 생성] ====
			/**
			 * @brief DirectX 그래픽스 기반 팩토리 생성
			 * @return 메시지 처리 값
			 */
			HRESULT CreateFactory();
			/**
			 * @brief DirectX 장치 및 스왑 체인 생성
			 * @date 2024-11-05
			 * @details
			 * 수직 동기화 여부에 따라 스왑체인의 새로고침 비율을 설정 후
			 * DirectX 장치, 장치 정보 및 스왑 체인 생성
			 * @param hWnd : 윈도우 식별 값
			 * @param width : 백버퍼 너비
			 * @param height : 백버퍼 높이
			 * @param isFullScreen : 전체화면 여부
			 * @param isVsync : 수직 동기화 여부
			 * @return 메시지 처리 값
			 */
			HRESULT CreateDeviceAndSwapChain(HWND hWnd, UINT width, UINT height, bool isFullScreen, bool isVsync);
			/**
			 * @brief 렌더 타겟 뷰 생성
			 * @details
			 * 백 버퍼를 렌더 타겟 뷰에 연결 후 렌더 타겟 뷰 생성
			 * @return 메시지 처리 값
			 */
			HRESULT CreateRenderTargetView();
			/**
			 * @brief 래스터라이저 상태 생성
			 * @details
			 * 2D 엔진에서는 컬링 모드를 해제한 후 래스터라이저 상태 생성
			 * @return 메시지 처리 값
			 */
			HRESULT CreateRasterizer();
			// 
			/**
			 * @brief 블렌드 상태 생성
			 * @details
			 * 텍스쳐의 알파값 혼합방식 설정 후 블렌드 상태 생성
			 * @return 메시지 처리 값
			 */
			HRESULT CreateBlendState();

		private: // ==== [DirectX11 그래픽스 설정] ====
			/**
			 * @brief DirectX 그래픽스 기반 어댑터(GPU) 설정
			 * @param adapterNumber : 설정할 어댑터(GPU) 번호
			 * @return 메시지 처리 값
			 */
			HRESULT SetAdapter(UINT adapterNumber = 0u);
			// 
			/**
			 * @brief DirectX 그래픽스 기반 출력(모니터) 설정
			 * @param adapterNumber : 설정할 어댑터(GPU) 번호
			 * @return 메시지 처리 값
			 */
			HRESULT SetOutput(UINT adapterNumber = 0u);
			/**
			 * @brief 그래픽스 카드 속성 설정
			 * @details
			 * 설정한 어댑터의 그래픽스 카드의 이름과 메모리를 설정
			 * @return 메시지 처리 값
			 */
			HRESULT SetGrapicsCardDescription();
			/**
			 * @brief 백 버퍼 설정
			 * @details
			 * 스왑 체인을 이용하여 버퍼로 사용할 정보를 백 버퍼에 가져옴
			 * @return 메시지 처리 값
			 */
			HRESULT SetBackBuffer();
			/**
			 * @brief 모니터 모드 개수 설정
			 * @details
			 * 출력 장치를 통해 DXGI_FORMAT_XXX의 형식에 맞는 모드 개수 설정
			 * @return 메시지 처리 값
			 */
			HRESULT GetDisplayModeCount();
			/**
			 * @brief 모니터 모드 리스트 설정
			 * @details
			 * 출력 장치를 통해 모니터 모드의 배열을 생성 및 설정
			 * @return 메시지 처리 값
			 */
			HRESULT GetDisplayModeList();
			/**
			 * @brief 모니터 모드 설정
			 * @details
			 * 모니터 모드의 개수와 리스트를 설정
			 * @return 메시지 처리 값
			 */
			HRESULT GetDisplayMode();
		};
	} // Graphics
} // Engine
