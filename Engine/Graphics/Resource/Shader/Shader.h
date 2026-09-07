#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 셰이더 클래스
			 * @date 2024-10-16
			 * @class
			 * 정점 셰이더와 픽셀 셰이더를 짝으로 참조하는 클래스
			 */
			class Shader
			{
			private: // ==== [셰이더] ====
				VertexShader* vertexShader; // 정점 셰이더 참조용 포인터
				PixelShader* pixelShader;	// 픽셀 셰이더 참조용 포인터

			public: // ==== [생성자 및 소멸자] ====
				/**
				 * @param vertexShader : 정점 셰이더 참조용 포인터
				 * @param pixelShader : 픽셀 셰이더 참조용 포인터
				 */
				Shader(VertexShader* vertexShader, PixelShader* pixelShader);
				/** @param moveShader : 이동 생성할 셰이더 */
				Shader(Shader&& moveShader) noexcept;
				~Shader();

			private: // ==== [생성자] ====
				/** @deprecated */
				Shader() = delete;
				Shader(const Shader& copyShader) = delete;

			public: // ==== [연산자 재정의] ====
				Shader& operator=(Shader&& rhs) noexcept;

			public: // ==== [연산자 재정의] ====
				/** @deprecated */
				Shader& operator=(const Shader& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 셰이더 자원 해제
				 * @details
				 * 정점, 픽셀 셰이더 참조 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/** @return 정점 셰이더 참조용 포인터 */
				VertexShader* GetVertexShader() const;
				/** @return 픽셀 셰이더 참조용 포인터 */
				PixelShader* GetPixelShader() const;
			};
		} // Resource
	} // Graphics
} // Engine
