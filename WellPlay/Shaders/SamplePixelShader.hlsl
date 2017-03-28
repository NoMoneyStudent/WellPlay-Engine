#define ModelViewer_RootSig \
	"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
	"CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
	"CBV(b0, visibility = SHADER_VISIBILITY_PIXEL), " \
	"SRV(t0, visibility = SHADER_VISIBILITY_VERTEX), " \
	"DescriptorTable(SRV(t0, numDescriptors = 6), visibility = SHADER_VISIBILITY_PIXEL)," \
	"DescriptorTable(SRV(t64, numDescriptors = 3), visibility = SHADER_VISIBILITY_PIXEL)," \
	"RootConstants(b1, num32BitConstants = 1, visibility = SHADER_VISIBILITY_VERTEX), " \
	"StaticSampler(s0, maxAnisotropy = 8, visibility = SHADER_VISIBILITY_PIXEL)," \
	"StaticSampler(s1, visibility = SHADER_VISIBILITY_PIXEL," \
		"addressU = TEXTURE_ADDRESS_CLAMP," \
		"addressV = TEXTURE_ADDRESS_CLAMP," \
		"addressW = TEXTURE_ADDRESS_CLAMP," \
		"comparisonFunc = COMPARISON_GREATER_EQUAL," \
		"filter = FILTER_MIN_MAG_LINEAR_MIP_POINT)"

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
};

Texture2D<float3> texDiffuse : register(t0);
Texture2D<float3> texSpecular : register(t1);
//Texture2D<float4> texEmissive : register(t2);
Texture2D<float3> texNormal : register(t3);
//Texture2D<float4> texLightmap : register(t4);
//Texture2D<float4> texReflection : register(t5);
Texture2D<float> texSSAO : register(t64);
Texture2D<float> texShadow : register(t65);

cbuffer PSConstants : register(b0)
{
	float3 SunDirection;
	float3 SunColor;
	float3 AmbientColor;
	uint _pad;
	float  ShadowTexelSize;
}

SamplerState sampler0 : register(s0);
SamplerComparisonState shadowSampler : register(s1);

// (内插)颜色数据的传递函数。

[RootSignature(ModelViewer_RootSig)]
float4 main(PixelShaderInput input) : SV_Target0
{
	float3 c = texDiffuse.Sample(sampler0,input.uv0);
	return float4(c, 1.0f);
	//return float4(input.uv0,0.0f, 1.0f);
}
