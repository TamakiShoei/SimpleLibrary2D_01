#include "ShaderHeader.hlsli"

cbuffer ConstantBuffer
{
	float4x4	World;
	float4		Viewport;
};

struct VS_IN
{
	float4 pos : POSITION;
};

Output VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	Output output;
	output.svpos = mul(mat, pos);
	output.uv = uv;

	return output;
}