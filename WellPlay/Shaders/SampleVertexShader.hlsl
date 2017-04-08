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


// 存储用于构成几何图形的三个基本列优先矩阵的常量缓冲区。
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 projection;
	float4x4 Transforms[96];
};

// 用作顶点着色器输入的每个顶点的数据。
struct VertexShaderInput
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 weights : BONEWEIGHTS;
	uint4 indices : BONEINDICES;
};

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
};

// 用于在 GPU 上执行顶点处理的简单着色器。
[RootSignature(ModelViewer_RootSig)]
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);
	float4 truepos = float4(0.0, 0.0, 0.0, 1.0f);

	float weight[4] = { input.weights.x,input.weights.y,input.weights.z,input.weights.w };

	for (int i = 0; i < 4; i++)
	{
		if (input.indices[i] != 65536)
		{
			float4 temp = mul(pos, Transforms[input.indices[i]]);
			truepos += temp*weight[i];
		}
	}
	pos = truepos;
	// 将顶点位置转换为投影空间。
	//pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// 不加修改地传递颜色。
	output.color = input.color;
	output.uv0 = input.uv0;

	return output;
}
