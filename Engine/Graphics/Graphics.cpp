#include "Graphics.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/GraphicsErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		Graphics::Graphics() : syncInterval(0u), grapicsCardMemory(0u),
			device(nullptr), deviceContext(nullptr), swapChain(nullptr), renderTargetView(nullptr), rasterizerState(nullptr), blendState(nullptr),
			factory(nullptr), adapter(nullptr), output(nullptr), backBuffer(nullptr), format(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM), displayModeCount(0u), displayModeList(nullptr)
		{
		}

		Graphics::~Graphics()
		{
			Release();
		}

		bool Graphics::Initialize(HWND hWnd, UINT width, UINT height, bool isFullScreen, bool isVsync)
		{
			try
			{
				HRESULT hr = E_FAIL;

				// ==== DirectX 그래픽스 기반 팩토리 생성 ====
				hr = CreateFactory();
				CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_FACTORY_FAILURE);
				// ===============================================

				// ==== 첫번째 그래픽카드 기반 어댑터 설정 ====
				hr = SetAdapter();
				CommonUtility::ComErrorFailed(hr, Message::Error::SET_ADAPTERS_FAILURE);
				// ==================================================

				// ==== 모니터 어댑터 설정 ====
				hr = SetOutput();
				CommonUtility::ComErrorFailed(hr, Message::Error::SET_OUTPUT_FAILURE);
				// ============================

				// ==== 모니터 모드 설정 ====
				hr = GetDisplayMode();
				CommonUtility::ComErrorFailed(hr, Message::Error::GET_DISPLAY_MODE_FAILURE);
				// ==========================

				// ==== 그래픽 카드 속성 설정 ====
				hr = SetGrapicsCardDescription();
				CommonUtility::ComErrorFailed(hr, Message::Error::SET_GRAPHICS_CARD_DESCRIPTION_FAILURE);
				// ===============================

				// ==== DirectX11 Device, DeviceContext, SwapChain 생성 ====
				hr = CreateDeviceAndSwapChain(hWnd, width, height, isFullScreen, isVsync);
				CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_DEVICE_AND_SWAP_CHAIN_FAILURE);
				// =========================================================

				// ==== 백 버퍼 설정 ====
				hr = SetBackBuffer();
				CommonUtility::ComErrorFailed(hr, Message::Error::SET_BACK_BUFFER_FAILURE);
				// ======================

				// ==== 렌더 타겟 뷰 생성 ====
				hr = CreateRenderTargetView();
				CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_RENDER_TARGET_VIEW_FAILURE);
				// ===========================

				// ==== 래스터라이저 생성 ====
				hr = CreateRasterizer();
				CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_RASTERIZER_STATE_FAILURE);
				// ===========================

				// ==== 블렌드 상태 생성 ====
				hr = CreateBlendState();
				CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_BLEND_STATE_FAILURE);
				// ==============================

				// ==== 그래픽스 설정 ====
				// 뷰포트 설정을 변경하지 않을 경우, 이 작업은 생략 가능
				CD3D11_VIEWPORT viewprot(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
				deviceContext->RSSetViewports(1u, &viewprot); // NumViewports(1) : 뷰 포트의 수

				// NumViews(1) : 스왑 체인을 만들 때 백 버퍼를 1개 사용으로 하였기 때문에 렌더 타겟 뷰 하나만 사용
				deviceContext->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), nullptr);

				float blendFactor[RGBA] = { 0.0f, 0.0f, 0.0f, 0.0f };
				deviceContext->OMSetBlendState(blendState.Get(), blendFactor, SAMPLE_MASK);
				deviceContext->RSSetState(rasterizerState.Get());
				// =====================
			}
			catch (CommonUtility::COMException& exception)
			{
				CommonUtility::Log::ErrorMessage(exception);
				return false;
			}

			return true;
		}

		void Graphics::Release()
		{
			// ComPtr 클래스는 유니크 포인터 역할을 하기에 할 필요는 없지만 명시적으로 하기위함
			// ==== DirectX11 Main Release ====
			CommonUtility::SafeComPtrRelease(blendState);

			CommonUtility::SafeComPtrRelease(rasterizerState);

			CommonUtility::SafeComPtrRelease(renderTargetView);

			CommonUtility::SafeComPtrRelease(swapChain);
			CommonUtility::SafeComPtrRelease(deviceContext);
			CommonUtility::SafeComPtrRelease(device);
			// ================================

			// ==== DirectX11 Sub Release ====
			CommonUtility::SafeComPtrRelease(backBuffer);

			CommonUtility::SafeDeleteArray(displayModeList);

			CommonUtility::SafeComPtrRelease(output);
			CommonUtility::SafeComPtrRelease(adapter);
			CommonUtility::SafeComPtrRelease(factory);
			// ===============================
		}

		void Graphics::BeginRender()
		{
			float backGroundColor[RGBA] = { 0.5f, 0.5f, 0.5f, 1.0f };
			deviceContext->ClearRenderTargetView(renderTargetView.Get(), backGroundColor);
		}

		void Graphics::EndRender()
		{
			swapChain->Present(syncInterval, 0u);
		}

		ID3D11Device* Graphics::DeviceRequest()
		{
			return device.Get();
		}

		ID3D11Device* Graphics::GetDevice() const
		{
			return device.Get();
		}

		ID3D11DeviceContext* Graphics::GetDeviceContext() const
		{
			return deviceContext.Get();;
		}

		HRESULT Graphics::CreateFactory()
		{
			return CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
		}

		HRESULT Graphics::CreateDeviceAndSwapChain(HWND hWnd, UINT width, UINT height, bool isFullScreen, bool isVsync)
		{
			// Swap Chain 생성에 사용될 Description 구조체 설정
			DXGI_SWAP_CHAIN_DESC swapChainDescription;
			std::memset(&swapChainDescription, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
			// 백 버퍼의 크기 설정
			swapChainDescription.BufferDesc.Width = width;
			swapChainDescription.BufferDesc.Height = height;
			swapChainDescription.BufferDesc.Format = format;

			// 백버퍼의 새로고침 비율
			if (isVsync)
			{
				UINT numerator = 0u;
				UINT denominator = 0u;

				// 화면 모드의 크기와 설정하려는 해상도의 크기가 같은것 중에 주사율이 가장 높은 것을 선택
				for (UINT i = 0u; i < displayModeCount; ++i)
				{
					if (displayModeList[i].Width == width && displayModeList[i].Height == height)
					{
						numerator = displayModeList[i].RefreshRate.Numerator;
						denominator = displayModeList[i].RefreshRate.Denominator;
					}
				}

				swapChainDescription.BufferDesc.RefreshRate.Numerator = numerator;
				swapChainDescription.BufferDesc.RefreshRate.Denominator = denominator;
			}
			else
			{
				// (0 / 1) : 1초에 최대한 빨리 그리게 한다
				swapChainDescription.BufferDesc.RefreshRate.Numerator = 0u;
				swapChainDescription.BufferDesc.RefreshRate.Denominator = 1u;
			}

			swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 순차 주사 대 비월 주사(지정하지 않음)
			swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // 이미지를 모니터에 맞게 확대/축소 하는 방식(지정하지 않음)

			// MSAA(Multi Sample Anti Aliasing)를 사용하지 않음
			swapChainDescription.SampleDesc.Count = 1u;
			swapChainDescription.SampleDesc.Quality = 0u;

			swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백 버퍼 사용처(용도) 설정
			swapChainDescription.BufferCount = 1; // 백 버퍼를 1개 사용

			swapChainDescription.OutputWindow = hWnd; // 렌더링이 이루어질 윈도우(의 핸들)를 설정
			swapChainDescription.Windowed = !isFullScreen;

			swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD; // 백 버퍼 내용을 사용(present, 화면에 출력)후 내용을 버린다(discard)
			swapChainDescription.Flags = 0u;

			// feature level : 어느 버전의 DirectX를 사용할 것인가
			// 사양이 낮은 컴퓨터를 지원하기 위하여 하위 버전의 DirectX 버전을 추가
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
			};
			UINT featureLevelCount = ARRAYSIZE(featureLevels);

			return  D3D11CreateDeviceAndSwapChain
			(
				nullptr,
				D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
				nullptr,	// D3D_DRIVER_TYPE_SOFTWARE가 아니라면 반드시 NULL이어야 하고 아니면 NULL이면 안된다
				0u,			// 런타임 계층에 대한 flags
				featureLevels,
				featureLevelCount,
				D3D11_SDK_VERSION,
				&swapChainDescription,
				swapChain.GetAddressOf(),
				device.GetAddressOf(),
				nullptr,	// 지원되는 기능 수준(feature level) output
				deviceContext.GetAddressOf()
			);
		}

		HRESULT Graphics::CreateRenderTargetView()
		{
			// 백 버퍼를 RenderTargetView에 연결
			return device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
		}

		HRESULT Graphics::CreateRasterizer()
		{
			// Rasterizer State는 직접 설정하지 않아도 기본적으로 설정되어 있다, 다만 제어 권한이 없다
			CD3D11_RASTERIZER_DESC rasterDescription(D3D11_DEFAULT);
			// rasterDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; // 면을 색으로 채우기
			rasterDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE; // 2D 엔진이므로 컬링모드 제거
			// rasterDescription.FrontCounterClockwise = false; 반시계방향(Counter Clock wise) 그리기를 뒷면으로

			return device->CreateRasterizerState(&rasterDescription, rasterizerState.GetAddressOf());
		}

		HRESULT Graphics::CreateBlendState()
		{
			CD3D11_BLEND_DESC blendDescription(D3D11_DEFAULT);
			blendDescription.AlphaToCoverageEnable = false;
			blendDescription.IndependentBlendEnable = false;
			blendDescription.RenderTarget[0].BlendEnable = true; // 블렌드 설정
			blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA;
			blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

			return device->CreateBlendState(&blendDescription, blendState.GetAddressOf());
		}

		HRESULT Graphics::SetAdapter(UINT adapterNumber)
		{
			return factory->EnumAdapters(adapterNumber, adapter.GetAddressOf());
		}

		HRESULT Graphics::SetOutput(UINT adapterNumber)
		{
			return adapter->EnumOutputs(adapterNumber, output.GetAddressOf());
		}

		HRESULT Graphics::SetGrapicsCardDescription()
		{
			DXGI_ADAPTER_DESC adapterDescription;
			HRESULT hr = adapter->GetDesc(&adapterDescription);

			grapicsCardMemory = static_cast<int>(adapterDescription.DedicatedVideoMemory / MEGA_BYTE);
			grapicsCardDescription = std::wstring(adapterDescription.Description);

			return hr;
		}

		HRESULT Graphics::SetBackBuffer()
		{
			// 스왑 체인을 이용하여 백 버퍼를 가져온다
			return swapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		}

		HRESULT Graphics::GetDisplayModeCount()
		{
			// 모니터에 대한 DXGI_FORMAT_XXX 표시 형식에 맞는 모드 수 가져오기
			return output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, nullptr);
		}

		HRESULT Graphics::GetDisplayModeList()
		{
			// 모든 모니터와 그래픽카드 조합을 저장할 리스트 생성
			displayModeList = new DXGI_MODE_DESC[displayModeCount];
			if (!displayModeList) // displayModeList == nullptr
				return E_FAIL;

			return output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, displayModeList);;
		}

		HRESULT Graphics::GetDisplayMode()
		{
			HRESULT hr = E_FAIL;

			hr = GetDisplayModeCount();
			CommonUtility::ComErrorFailed(hr, Message::Error::GET_DISPLAY_MODE_COUNT_FAILURE);

			hr = GetDisplayModeList();
			CommonUtility::ComErrorFailed(hr, Message::Error::GET_DISPLAY_MODE_LIST_FAILURE);

			return hr;
		}
	} // Graphics
} // Engine