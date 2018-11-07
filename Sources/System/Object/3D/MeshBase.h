#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "System\Utility.h"
#include "Shader\Shader.h"


class MeshBase
{
public:
	MeshBase() {}
	virtual ~MeshBase() {}
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;
	
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>srv;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;

	//std::unique_ptr<ShaderStructure> shader_structure;
protected:
	struct material
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	};
	struct Subset
	{
		u_int index_start = 0; // start number of index buffer  
		u_int index_count = 0; // number of vertices (indices)  
		material diffuse;
	};


public:

	std::unique_ptr<ShaderStructure> shader_structure;

	




	virtual void Render() {};

	int vertex_count{};
public:

};