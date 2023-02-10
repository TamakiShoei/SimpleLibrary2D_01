#include "Pipeline.h"

Pipeline::Pipeline() : pipeline(nullptr)
{

}

Pipeline::~Pipeline()
{
	if (pipeline != nullptr)
	{
		pipeline->Release();
	}
}

bool Pipeline::Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> rootSignature)
{
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;
	ID3DBlob* errorBlob;

	if (FAILED(D3DCompileFromFile(
		L"Engine/Res/Shader/VertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob)))
	{
		return false;
	}

	if (FAILED(D3DCompileFromFile(
		L"Engine/Res/Shader/PixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob)))
	{
		return false;
	}

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{	//À•Wî•ñ
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{	//uvî•ñ
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};


	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.LogicOpEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline = {};
	graphicsPipeline.pRootSignature = rootSignature.Get();
	graphicsPipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	graphicsPipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	graphicsPipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	graphicsPipeline.PS.BytecodeLength = psBlob->GetBufferSize();
	graphicsPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipeline.BlendState.AlphaToCoverageEnable = false;
	graphicsPipeline.BlendState.IndependentBlendEnable = false;
	graphicsPipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;
	graphicsPipeline.RasterizerState.MultisampleEnable = false;
	graphicsPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	graphicsPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	graphicsPipeline.RasterizerState.DepthClipEnable = true;
	graphicsPipeline.DepthStencilState.StencilEnable = false;
	graphicsPipeline.InputLayout.pInputElementDescs = inputLayout;
	graphicsPipeline.InputLayout.NumElements = _countof(inputLayout);
	graphicsPipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	graphicsPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipeline.NumRenderTargets = 1;
	graphicsPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipeline.SampleDesc.Count = 1;
	graphicsPipeline.SampleDesc.Quality = 0;

	if (FAILED(device->CreateGraphicsPipelineState(
		&graphicsPipeline, IID_PPV_ARGS(pipeline.GetAddressOf()))))
	{
		return false;
	}
	return true;

}

void Pipeline::Finalize()
{
	pipeline->Release();
}