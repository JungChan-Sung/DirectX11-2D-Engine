#pragma once

#include <d3d11.h>
#include "Engine/Graphics/Resource/Texture/Texture.h"
#include "Engine/Graphics/Data/TextureData.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			/**
			 * @brief 텍스처 매니저 클래스
			 * @date 2024-10-20
			 * @class
			 * 텍스처 파일들을 관리하는 클래스
			 */
			class TextureManager
			{			
			private: // ==== [선언] ====
				// <샘플러 상태 타입, DirectX11 샘플러 상태 포인터>
				using SamplerStateTable = std::unordered_map<Type::SamplerStateType, Microsoft::WRL::ComPtr<ID3D11SamplerState>>;
				// <텍스처 이름, 텍스처 포인터>
				using TextureTable = std::unordered_map<std::string, Resource::Texture*>;
				// <텍스처 그룹 이름, 텍스처 테이블>
				using TextureGroupTable = std::unordered_map<std::string, TextureTable>;

			private: // ==== [DirectX 리소스] ====
				SamplerStateTable samplerStates; // 샘플러 상태 테이블

			private: // ==== [텍스처 데이터] ====
				Data::TextureDataGroupTable textureDataGroupTable; // 텍스처 데이터 그룹 테이블

			private: // ==== [텍스처] ====
				TextureGroupTable textureGroupTable; // 텍스처 그룹 테이블

			public: // ==== [생성자 및 소멸자] ====
				TextureManager();
				~TextureManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 텍스처 매니저 초기화
				 * @date 2024-10-20
				 * @details
				 * 샘플러 상태와 텍스처 데이터 파일 로드
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 텍스처 매니저 초기화 성공 여부
				 */
				bool Initialize(ID3D11Device* device);
				/**
				 * @brief 텍스처 매니저 자원 해제
				 * @date 2024-10-20
				 * @details
				 * 텍스처 테이블 자원 해제
				 * 샘플러 상태 자원 해제
				 * 텍스처 데이터 그룹 테이블 자원 해제
				 */
				void Release();

			public: // ==== [텍스처 로드 및 로드 해제] ====
				/**
				 * @brief 텍스처 로드
				 * @param device : DirectX11 장치 포인터
				 * @param textureGroupName : 텍스처 그룹 이름
				 * @return 로드 성공 여부
				 */
				bool LoadTexture(ID3D11Device* device, const std::wstring& textureGroupName);
				bool LoadTexture(ID3D11Device* device, const std::string& textureGroupName);
				/**
				 * @brief 텍스처 로드 해제
				 * @param textureGroupName : 텍스처 그룹 이름
				 * @return 로드 해제 여부
				 */
				bool UnLoadTexture(const std::wstring& textureGroupName);
				bool UnLoadTexture(const std::string& textureGroupName);

			public: // ==== [Get] ====
				/**
				 * @param textureGroupName : 텍스처 그룹 이름
				 * @param textureName : 텍스처 이름
				 * @return 텍스처 참조용 포인터
				 */
				Resource::Texture* GetTexture(const std::wstring& textureGroupName, const std::wstring& textureName) const;
				Resource::Texture* GetTexture(const std::string& textureGroupName, const std::string& textureName) const;

			private: // ==== [샘플러 상태 생성] ====
				/**
				 * @brief 샘플러 상태 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 메시지 처리 값
				 */
				HRESULT CreateSamplerState(ID3D11Device* device);
			};
		} // Manager
	} // Graphics
} // Engine
