struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD0;
};

Texture2D tex : TEXCOORD : register(t0);

SamplerState sample : SAMPLER : register(s0); // 샘플러(슬롯 번호 0번과 연결)

float4 main(PixelInput input) : SV_TARGET // SV_TARGET은 출력값에 대한 정의(SV는 System Value 약자이며 렌더링 파이프라인에서 특별한 의미를 가진 시멘틱스 명으로 사용)
{
    return input.color * tex.Sample(sample, input.texcoord);
}