#include "RootSignature.h"

RootSignature::RootSignature() : instance(nullptr)
{

}

RootSignature::~RootSignature()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool RootSignature::Initialize(ComPtr<ID3D12Device> device)
{
	//ディスクリプタレンジの作成
	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};

	//テクスチャー用レジスター0番
	descTblRange[0].NumDescriptors = 1;	//テクスチャ1つ
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	//種別はテクスチャ
	descTblRange[0].BaseShaderRegister = 0;	//0番スロットから
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//定数用レジスター0番
	descTblRange[1].NumDescriptors = 1;	//テクスチャ1つ
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;	//種別は定数
	descTblRange[1].BaseShaderRegister = 0;	//0番スロットから
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParam[2] = {};
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//ピクセルシェーダーから見える
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootParam[0].DescriptorTable.NumDescriptorRanges = 1;	//レンジ数

	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;	//レンジ数

	//サンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//アドレッシングモードは繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//アドレッシングモードは繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//アドレッシングモードは繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	//線形保管
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = 2;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3DBlob* blob;
	ID3DBlob* errorBlob;

	if (FAILED(D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&blob, &errorBlob)))
	{
		return false;
	}

	if (FAILED(device->CreateRootSignature(
		0,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_PPV_ARGS(&instance))))
	{
		return false;
	}

	return true;
}

void RootSignature::Finalize()
{
	instance->Release();
}