Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

cbuffer cbuff0 : register(b0)	//定数バッファー
{
	matrix mat; //行列変換
};


struct Output
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};
