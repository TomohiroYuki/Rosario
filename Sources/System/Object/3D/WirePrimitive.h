#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "math\Transform.h"
#include "System\Utility.h"
#include "Shader\Shader.h"
#include "math\Vector4.h"


class PrimitiveBase
{
public:
	enum PrimitiveType
	{
		PRIMITIVE_TYPE_GRID=0,
		PRIMITIVE_TYPE_BOX ,
		PRIMITIVE_TYPE_SPHERE,
		PRIMITIVE_TYPE_LINE
	};
public:
	PrimitiveBase(PrimitiveBase::PrimitiveType prim_type);
	virtual ~PrimitiveBase() {}
private:
public:

	Yukitter::Transform transform;

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

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 wvp;
		//DirectX::XMFLOAT4 color;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 position; 
		DirectX::XMFLOAT4 color;
	};

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	int vertex_count=0;
	int index_count=0;



	void CreateGridData();
	void CreateBoxData();
	void CreateSphereData();

	

	void UpdateBuffer();
	void CreateBuffer(int v_num, int ind_num);

	void Render();
	void Render(Yukitter::Transform);
	void Render(Yukitter::Transform, Yukitter::Transform );
	void Render(Yukitter::Vector scale);

};



class Grid :public PrimitiveBase
{

public:
	Grid() :
		PrimitiveBase(PrimitiveBase::PrimitiveType::PRIMITIVE_TYPE_GRID)
	{
		//transform.SetScale(3.0f);
		
	}
private:
public:

	
	
};


class BOX :public PrimitiveBase
{

public:
	BOX() :
		PrimitiveBase(PrimitiveBase::PrimitiveType::PRIMITIVE_TYPE_BOX)
	{
		//transform.SetScale({ 50,100.0f,50 });
		
	}
private:

public:



};

class SPHERE :public PrimitiveBase
{

public:
	SPHERE() :
		PrimitiveBase(PrimitiveBase::PrimitiveType::PRIMITIVE_TYPE_SPHERE)
	{
		//transform.SetScale({ 50,100.0f,50 });

	}
private:

public:



};

class LINE :public PrimitiveBase
{
public:
	LINE() :
		PrimitiveBase(PrimitiveBase::PrimitiveType::PRIMITIVE_TYPE_LINE)
	{
		//transform.SetScale({ 50,100.0f,50 });

	}
private:
	//std::vector<Vertex> point_list;
public:
	Yukitter::Vector4 color{0,0,1,1};

	void AddPoint(Yukitter::Vector point)
	{
		if (vertex_count > 1000)return;
		Vertex p;
		//indices.push_back(index_count++);
		//indices.push_back(index_count++);
		//indices.push_back(++index_count);
		p.position = point.ToXMF3();
		p.color=DirectX::XMFLOAT4(color.x, color.y, color.z, 1);
		vertices.push_back(p);
		vertex_count++;
		//indices.push_back(index_count++);
		
		UpdateBuffer();
	}


	void Clear()
	{
		indices.clear();
		vertices.clear();
		index_count = 0;
		vertex_count = 0;
	}
};