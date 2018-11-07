#include "System\Object\3D\WirePrimitive.h"
#include "System\Framework.h"
#include <d3dcompiler.h>
#include "math\yuki_math.h"

using namespace DirectX;


const PrimitiveBase::Vertex box_vertices[] =
{
	{XMFLOAT3(-0.5f,0.5f,-0.5f)},{ XMFLOAT3(0.5f,0.5f,-0.5f) },{ XMFLOAT3(0.5f,0.5f,0.5f) },{ XMFLOAT3(-0.5f,0.5f,0.5f) },
	{XMFLOAT3(-0.5f,-0.5f,-0.5f) },{ XMFLOAT3(0.5f,-0.5f,-0.5f) },{ XMFLOAT3(0.5f,-0.5f,0.5f) },{ XMFLOAT3(-0.5f,-0.5f,0.5f) },
};
const int box_indeces[] =
{
	0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7,
};

PrimitiveBase::PrimitiveBase(PrimitiveType prim_type)
{

	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	ID3DBlob* pBlobVS;

	hr = D3DCompileFromFile(L"../Sources/Shader/GridVS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GridVS",
		"vs_5_0", 0, 0, &pBlobVS, NULL);

	if (FAILED(hr))return;


	hr = Framework::device->CreateVertexShader(pBlobVS->GetBufferPointer(),
		pBlobVS->GetBufferSize(), NULL, vertex_shader.GetAddressOf());

	if (FAILED(hr))
		return;

	hr = Framework::device->CreateInputLayout(input_element_desc, _countof(input_element_desc), pBlobVS->GetBufferPointer(),
		pBlobVS->GetBufferSize(), input_layout.GetAddressOf());
	if (FAILED(hr))return;


	pBlobVS->Release();


	D3D11_RASTERIZER_DESC rasterizer_desc = {};
	rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
	rasterizer_desc.CullMode = D3D11_CULL_BACK; //, D3D11_CULL_NONED3D11_CULL_FRONT, D3D11_CULL_BACK   
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = Framework::device->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf());
	if (FAILED(hr))
	{
		return;
	}

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_desc.StencilEnable = FALSE;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = Framework::device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_state.GetAddressOf());
	if (FAILED(hr))
	{
		return;
	}
	ID3DBlob* pBlobPS;
	hr = D3DCompileFromFile(L"../Sources/Shader/GridPS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
		"ps_5_0", 0, 0, &pBlobPS, NULL);
	if (FAILED(hr))return;

	hr = Framework::device->CreatePixelShader(pBlobPS->GetBufferPointer(),
		pBlobPS->GetBufferSize(), NULL, pixel_shader.GetAddressOf());
	if (FAILED(hr))return;

	pBlobPS->Release();


	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.ByteWidth = sizeof(ConstantBuffer);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	hr = Framework::device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
	if (FAILED(hr))
	{
		return;
	}

	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Framework::device->CreateSamplerState(&samDesc, sampler_state.GetAddressOf());


	//CreateData(1);
	switch (prim_type)
	{
	case PrimitiveBase::PRIMITIVE_TYPE_GRID:
		CreateGridData();
		break;
	case PrimitiveBase::PRIMITIVE_TYPE_BOX:
		CreateBoxData();
		break;
	case PrimitiveBase::PRIMITIVE_TYPE_SPHERE:
		CreateSphereData();
	default:
		break;
	}

	if(vertex_count>0)
		CreateBuffer(vertex_count, index_count);
}
void PrimitiveBase::UpdateBuffer()
{
	CreateBuffer(vertex_count, index_count);
	return;
	if (vertex_buffer)
	{
		Framework::device_context->UpdateSubresource(vertex_buffer.Get(), 0, nullptr, vertices.data(), 0, 0);
		Framework::device_context->UpdateSubresource(index_buffer.Get(), 0, nullptr, indices.data(), 0, 0);

	}
	else 
		CreateBuffer(vertex_count, index_count);
	/*if (index_buffer)
		Framework::device_context->UpdateSubresource(vertex_buffer.Get(), 0, nullptr, vertices.data(), 0, 0);
	else
		CreateBuffer(vertex_count, index_count);*/
}
void PrimitiveBase::CreateBuffer(int v_num, int ind_num)
{
	if (v_num <= 0)return;
	HRESULT hr;

	D3D11_BUFFER_DESC buffer_desc = {};
	D3D11_SUBRESOURCE_DATA subresource_data = {};

	//vertex bufferçÏê¨

	buffer_desc.ByteWidth = sizeof(Vertex)*v_num;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource_data.pSysMem = vertices.data();
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;

	hr = Framework::device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());

	//index bufferçÏê¨
	if (ind_num <= 0)return;
	buffer_desc.ByteWidth = sizeof(u_int)*ind_num;
	//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource_data.pSysMem = indices.data();
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;
	hr = Framework::device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());

}

void PrimitiveBase::Render()
{


	//UINT strides = { sizeof(Vertex) };
	//UINT offsets[2] = { 0, 0 };
	//Framework::device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &strides, offsets);
	//Framework::device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//Framework::device_context->IASetInputLayout(input_layout.Get());
	//Framework::device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//Framework::device_context->VSSetShader(vertex_shader.Get(), NULL, 0);

	//Framework::device_context->PSSetShader(pixel_shader.Get(), NULL, 0);
	//Framework::device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	//Framework::device_context->RSSetState(rasterizer_state.Get());
	//FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	////XMVECTOR eye = XMVectorSet(0, 0, 10, 1.0f);
	////XMVECTOR At = XMVectorSet(0, 0, 0, 1.0f);
	//Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	//Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	//XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	//XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	//XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);
	//GameBrain::projection_mat = XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 1.0f, 5000.0f);



	//ConstantBuffer cb;

	//XMMATRIX world, S, R, T;

	//S = XMMatrixScaling(transform.scale3d.x, transform.scale3d.y, transform.scale3d.z);

	//R = XMMatrixRotationRollPitchYaw(transform.rotation.pitch, transform.rotation.yaw, transform.rotation.roll);
	//T = XMMatrixTranslation(transform.translation.x, transform.translation.y, transform.translation.z);
	//world = S * R*T;

	//XMFLOAT4X4 wvp;
	//XMFLOAT4X4 inverse;
	////XMStoreFloat4x4(&wvp, Scene::world_mat * Scene::view_mat * GameBrain::projection_mat);
	//XMStoreFloat4x4(&wvp, world*GameBrain::view_mat   * GameBrain::projection_mat);
	//XMStoreFloat4x4(&inverse, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(0, world)));



	//cb.wvp = wvp;
	////cb.color = XMFLOAT4(1, 1, 1, 1);

	//Framework::device_context->UpdateSubresource(constant_buffer.Get(), 0, NULL, &cb, 0, 0);
	//Framework::device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//Framework::device_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	////Framework::device_context->PSSetShaderResources(0, 1, s.diffuse.srv.GetAddressOf());
	////Framework::device_context->PSSetShaderResources(1, 1, shader_structure->srv.GetAddressOf());

	//Framework::device_context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	//Framework::device_context->DrawIndexed(index_count, 0, 0);
	assert(true);

}

void PrimitiveBase::Render(Transform transform)
{


	UINT strides = { sizeof(Vertex) };
	UINT offsets[2] = { 0, 0 };
	Framework::device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &strides, offsets);
	Framework::device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Framework::device_context->IASetInputLayout(input_layout.Get());
	Framework::device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	Framework::device_context->VSSetShader(vertex_shader.Get(), NULL, 0);

	Framework::device_context->PSSetShader(pixel_shader.Get(), NULL, 0);
	Framework::device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	Framework::device_context->RSSetState(rasterizer_state.Get());
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//XMVECTOR eye = XMVectorSet(0, 0, 10, 1.0f);
	//XMVECTOR At = XMVectorSet(0, 0, 0, 1.0f);
	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);
	GameBrain::projection_mat = XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 1.0f, 5000.0f);



	ConstantBuffer cb;

	XMMATRIX world, S, R, T;

	S = XMMatrixScaling(transform.scale3d.x, transform.scale3d.y, transform.scale3d.z);

	R = XMMatrixRotationRollPitchYaw(transform.rotation.pitch, transform.rotation.yaw, transform.rotation.roll);
	T = XMMatrixTranslation(transform.translation.x, transform.translation.y, transform.translation.z);
	world = S * R*T;

	XMFLOAT4X4 wvp;
	XMFLOAT4X4 inverse;
	//XMStoreFloat4x4(&wvp, Scene::world_mat * Scene::view_mat * GameBrain::projection_mat);
	XMStoreFloat4x4(&wvp, world*GameBrain::view_mat   * GameBrain::projection_mat);
	XMStoreFloat4x4(&inverse, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(0, world)));



	cb.wvp = wvp;
	//cb.color = XMFLOAT4(1, 1, 1, 1);

	Framework::device_context->UpdateSubresource(constant_buffer.Get(), 0, NULL, &cb, 0, 0);
	Framework::device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	Framework::device_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//Framework::device_context->PSSetShaderResources(0, 1, s.diffuse.srv.GetAddressOf());
	//Framework::device_context->PSSetShaderResources(1, 1, shader_structure->srv.GetAddressOf());

	Framework::device_context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	Framework::device_context->DrawIndexed(index_count, 0, 0);


}
void PrimitiveBase::Render(Transform transform, Transform parent)
{


	UINT strides = { sizeof(Vertex) };
	UINT offsets[2] = { 0, 0 };
	Framework::device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &strides, offsets);
	//Framework::device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Framework::device_context->IASetInputLayout(input_layout.Get());
	Framework::device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	Framework::device_context->VSSetShader(vertex_shader.Get(), NULL, 0);

	Framework::device_context->PSSetShader(pixel_shader.Get(), NULL, 0);
	Framework::device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	Framework::device_context->RSSetState(rasterizer_state.Get());
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//XMVECTOR eye = XMVectorSet(0, 0, 10, 1.0f);
	//XMVECTOR At = XMVectorSet(0, 0, 0, 1.0f);
	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);
	GameBrain::projection_mat = XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 1.0f, 5000.0f);



	ConstantBuffer cb;

	XMMATRIX world, S, R, T,w0,w1;

	S = XMMatrixScaling(transform.scale3d.x, transform.scale3d.y, transform.scale3d.z);

	R = XMMatrixRotationRollPitchYaw(transform.rotation.pitch, transform.rotation.yaw, transform.rotation.roll);
	T = XMMatrixTranslation(transform.translation.x, transform.translation.y, transform.translation.z);
	w0 = S * R*T;
	S = XMMatrixScaling(parent.scale3d.x, parent.scale3d.y, parent.scale3d.z);

	R = XMMatrixRotationRollPitchYaw(parent.rotation.pitch, parent.rotation.yaw, parent.rotation.roll);
	T = XMMatrixTranslation(parent.translation.x, parent.translation.y, parent.translation.z);
	w1 = S * R*T;;
	world = w0 * w1;
	XMFLOAT4X4 wvp;
	XMFLOAT4X4 inverse;
	//XMStoreFloat4x4(&wvp, Scene::world_mat * Scene::view_mat * GameBrain::projection_mat);
	XMStoreFloat4x4(&wvp, world*GameBrain::view_mat   * GameBrain::projection_mat);
	XMStoreFloat4x4(&inverse, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(0, world)));



	cb.wvp = wvp;
	//cb.color = XMFLOAT4(1, 1, 1, 1);

	Framework::device_context->UpdateSubresource(constant_buffer.Get(), 0, NULL, &cb, 0, 0);
	Framework::device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	Framework::device_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//Framework::device_context->PSSetShaderResources(0, 1, s.diffuse.srv.GetAddressOf());
	//Framework::device_context->PSSetShaderResources(1, 1, shader_structure->srv.GetAddressOf());

	Framework::device_context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	Framework::device_context->DrawIndexed(index_count, 0, 0);


}
void PrimitiveBase::Render(Vector scale)
{


	UINT strides = { sizeof(Vertex) };
	UINT offsets[2] = { 0, 0 };
	Framework::device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &strides, offsets);
	//Framework::device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Framework::device_context->IASetInputLayout(input_layout.Get());
	Framework::device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//Framework::device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	Framework::device_context->VSSetShader(vertex_shader.Get(), NULL, 0);

	Framework::device_context->PSSetShader(pixel_shader.Get(), NULL, 0);
	Framework::device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	Framework::device_context->RSSetState(rasterizer_state.Get());
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//XMVECTOR eye = XMVectorSet(0, 0, 10, 1.0f);
	//XMVECTOR At = XMVectorSet(0, 0, 0, 1.0f);
	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);
	GameBrain::projection_mat = XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 1.0f, 5000.0f);



	ConstantBuffer cb;

	XMMATRIX world, S, R, T;

	S = XMMatrixScaling(scale.x, scale.y, scale.z);

	R = XMMatrixRotationRollPitchYaw(0,0, 0);
	T = XMMatrixTranslation(0,0, 0);
	world = S * R*T;

	XMFLOAT4X4 wvp;
	XMFLOAT4X4 inverse;
	//XMStoreFloat4x4(&wvp, Scene::world_mat * Scene::view_mat * GameBrain::projection_mat);
	XMStoreFloat4x4(&wvp, world*GameBrain::view_mat   * GameBrain::projection_mat);
	XMStoreFloat4x4(&inverse, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(0, world)));



	cb.wvp = wvp;
	//cb.color = XMFLOAT4(1, 1, 1, 1);

	Framework::device_context->UpdateSubresource(constant_buffer.Get(), 0, NULL, &cb, 0, 0);
	Framework::device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	Framework::device_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//Framework::device_context->PSSetShaderResources(0, 1, s.diffuse.srv.GetAddressOf());
	//Framework::device_context->PSSetShaderResources(1, 1, shader_structure->srv.GetAddressOf());

	Framework::device_context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	//Framework::device_context->DrawIndexed(index_count, 0, 0);
	Framework::device_context->Draw(vertex_count, 0);

}


void PrimitiveBase::CreateGridData()
{
	int count = 0;
	//Framework::device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	for (int i = 0; i < 11; i++)
	{
		Vertex v;
		v.position = DirectX::XMFLOAT3(static_cast<float>(i) * 100.0f - 500.0f, 0, 500.0f);
		vertices.push_back(v);
		count++;
		v.position = DirectX::XMFLOAT3(static_cast<float>(i) * 100.0f - 500.0f, 0, -500.0f);
		vertices.push_back(v);
		count++;
	}
	for (int i = 0; i < 11; i++)
	{
		Vertex v;
		v.position = DirectX::XMFLOAT3(500.0f, 0, static_cast<float>(i) * 100.0f - 500.0f);
		vertices.push_back(v);
		count++;
		v.position = DirectX::XMFLOAT3(-500.0f, 0, static_cast<float>(i) * 100.0f - 500.0f);
		vertices.push_back(v);
		count++;
	}
	Vertex v;
	v.position = DirectX::XMFLOAT3(0, 500, 0);
	vertices.push_back(v);
	count++;
	v.position = DirectX::XMFLOAT3(0, -500, 0);
	vertices.push_back(v);
	count++;
	
	for (auto& a : vertices)
	{
		a.color= DirectX::XMFLOAT4(1, 1, 1,1);
	}

	for (int i = 0; i < count; i++)
		indices.push_back(i);

	vertex_count = count;
	index_count = count;
	//grid_vertices.push_back()

}


void PrimitiveBase::CreateBoxData()
{
	box_vertices;

	Vertex v;
	for (int i = 0; i < 8; i++)
	{
		v.position = box_vertices[i].position;
		vertices.push_back(v);
		//indices.push_back(box_indeces[i]);
	}

	for (int i = 0; i < 24; i++)
	{
		
		indices.push_back(box_indeces[i]);
	}
	for (auto& a : vertices)
	{
		a.color = DirectX::XMFLOAT4(1, 0, 0, 1);
	}


	vertex_count = 8;
	index_count = 24;

}
const int SPHERE_INDEX[] =
{
	0,1,1,2,2,3,3,4,4,5,6,6,7,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,
};

void PrimitiveBase::CreateSphereData()
{
	Vertex v;
	const float _PI = 3.14159265f;
	const int COUNT = 14;
	for (int i = 0; i < COUNT; i++)
	{
		v.position = XMFLOAT3(sinf((_PI)*(F_CAST(i)/F_CAST(COUNT/2.0f))),0,cosf((_PI)*(F_CAST(i) / F_CAST(COUNT / 2.0f))));
		vertices.push_back(v);
		indices.push_back(i);
		
		indices.push_back((i + 1) == COUNT ? 0 : i + 1);
		vertex_count++;
		index_count++;
		index_count++;
	}
	int count = COUNT;
	for (int i = 0; i < COUNT; i++)
	{
		v.position = XMFLOAT3(0,sinf((_PI)*(F_CAST(i) / F_CAST(COUNT / 2.0f))),  cosf((_PI)*(F_CAST(i) / F_CAST(COUNT / 2.0f))));
		vertices.push_back(v);
		indices.push_back(i+ count);

		indices.push_back((i + 1) == COUNT ? count : i + 1+count);
		vertex_count++;
		index_count++;
		index_count++;
	}
	for (auto& a : vertices)
	{
		a.color = DirectX::XMFLOAT4(1, 0, 0, 1);
	}
	/*for(int i = 0; i < 37; i++)
	{

	}*/
}


