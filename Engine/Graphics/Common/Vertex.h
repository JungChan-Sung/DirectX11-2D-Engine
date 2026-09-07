/*********************************************************************
 * @file   Vertex.h
 * @brief  정점 구조체를 모아두는 헤더 파일
 * @date   2024-10-09
 *********************************************************************/

#pragma once

#include <DirectXMath.h>

namespace Engine
{
	namespace Graphics
	{
		namespace Common
		{
			/**
			 * @brief 정점 구조체
			 * @date 2024-10-09
			 * @struct
			 * 정점의 위치 데이터를 지닌 구조체
			 * 정점 버퍼에 사용
			 */
			struct Vertex
			{
			public: // ==== [구성 요소] ====
				DirectX::XMFLOAT2 position; // 정점 위치

			public: // ==== [생성자] ====
				Vertex();
				/**
				 * @param x : 정점 X축 위치
				 * @param y : 정점 Y축 위치
				 */
				Vertex(float x, float y);
			};

			/**
			 * @brief 색상 정점 : 정점 상속
			 * @date 2024-10-09
			 * @struct
			 * 정점의 위치, 색상 데이터를 지닌 구조체
			 * 색상 값으로 정점을 표현할때 사용
			 * 정점 버퍼에 사용
			 */
			struct VertexColor : public Vertex
			{
			public: // ==== [구성 요소] ====
				DirectX::XMFLOAT4 color; // 정점 색상

			public: // ==== [생성자] ====
				VertexColor();
				/**
				 * @param x : 정점 X축 위치
				 * @param y : 정점 Y축 위치
				 * @param r : 정점 Red 색상
				 * @param g : 정점 Green 색상
				 * @param b : 정점 Blue 색상
				 * @param a : 정점 Alpha 투명도
				 */
				VertexColor(float x, float y, float r, float g, float b, float a);
				/**
				 * @param x : 정점 X축 위치
				 * @param y : 정점 Y축 위치
				 * @param r : 정점 Red 색상
				 * @param g : 정점 Green 색상
				 * @param b : 정점 Blue 색상
				 */
				VertexColor(float x, float y, float r, float g, float b);
			};

			/**
			 * @brief 텍스쳐 정점 : 색상 정점 상속
			 * @date 2024-10-09
			 * @struct
			 * 정점의 위치, 색상, 텍스쳐 맵핑 데이터를 지닌 구조체
			 * 정점 버퍼에 사용
			 */
			struct VertexTexture : public VertexColor
			{
			public: // ==== [구성 요소] ====
				DirectX::XMFLOAT2 textureCoordinate;

			public: // ==== [생성자] ====
				VertexTexture();
				/**
				 * @param x : 정점 X축 위치
				 * @param y : 정점 Y축 위치
				 * @param r : 정점 Red 색상
				 * @param g : 정점 Green 색상
				 * @param b : 정점 Blue 색상
				 * @param a : 정점 Alpha 투명도
				 * @param u : 텍스쳐 가로 방향 위치
				 * @param v : 텍스쳐 세로 방향 위치
				 */
				VertexTexture(float x, float y, float r, float g, float b, float a, float u, float v);
				/**
				 * @param x : 정점 X축 위치
				 * @param y : 정점 Y축 위치
				 * @param r : 정점 Red 색상
				 * @param g : 정점 Green 색상
				 * @param b : 정점 Blue 색상
				 * @param u : 텍스쳐 가로 방향 위치
				 * @param v : 텍스쳐 세로 방향 위치
				 */
				VertexTexture(float x, float y, float r, float g, float b, float u, float v);
				/**
				 * @param x : 정점 X축 위치
				 * @param y : 정점 Y축 위치
				 * @param u : 텍스쳐 가로 방향 위치
				 * @param v : 텍스쳐 세로 방향 위치
				 */
				VertexTexture(float x, float y, float u, float v);
			};
		} // Common
	} // Graphics
} // Engine
