Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

cbuffer cbuff0 : register(b0)	//�萔�o�b�t�@�[
{
	matrix mat; //�s��ϊ�
};


struct Output
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};
