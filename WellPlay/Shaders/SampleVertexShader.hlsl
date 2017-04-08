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


// �洢���ڹ��ɼ���ͼ�ε��������������Ⱦ���ĳ�����������
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 projection;
	float4x4 Transforms[96];
};

// ����������ɫ�������ÿ����������ݡ�
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

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
};

// ������ GPU ��ִ�ж��㴦��ļ���ɫ����
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
	// ������λ��ת��ΪͶӰ�ռ䡣
	//pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// �����޸ĵش�����ɫ��
	output.color = input.color;
	output.uv0 = input.uv0;

	return output;
}
