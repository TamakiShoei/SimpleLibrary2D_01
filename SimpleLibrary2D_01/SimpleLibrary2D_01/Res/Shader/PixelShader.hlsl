#include "ShaderHeader.hlsli"

float4 PS(Output input) : SV_TARGET
{
	return float4(tex.Sample(smp, input.uv));
}