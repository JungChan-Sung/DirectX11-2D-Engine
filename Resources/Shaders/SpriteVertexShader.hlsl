//#pragma pack_matrix(row_major) : 행 우선으로 바꾸는 명령어 하지만 컴퓨터 행렬연산은 열 우선이 더 빠름

cbuffer Constants
{
    matrix worldMatrix;
    matrix viewProjectionMatrix;
};

struct VertexInput
{
    float2 position : POSITION;     // POSITION(시멘틱스)
    float4 color : COLOR;           // COLOR(시멘틱스)
    float2 texcoord : TEXCOORD0;    // TEXCOORD(시멘틱스) 0(식별자)
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD0;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;

	// world matrix
    output.position = mul(float4(input.position, 0.0f, 1.0f), worldMatrix);
	// view * projection matrix
    output.position = mul(output.position, viewProjectionMatrix);
	
	// copy value
    output.color = input.color;
    output.texcoord = input.texcoord;

    return output;
}