#include "System\Object\3D\StaticMesh.h"
#include <d3dcompiler.h>
#include <fbxsdk.h>
#pragma comment(lib,"libfbxsdk-md.lib")
#if _DEBUG
#pragma comment(lib,"DirectXTexD.lib")
#else
#pragma comment(lib,"lib\\release\\DirectXTexR.lib")
#endif
#include "System\Framework.h"
#include "System\DXTex\DirectXTex.h"
#pragma comment(lib,"d3dcompiler.lib")
#include <crtdbg.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace DirectX;

void FindNodeHasMesh(FbxNode* in_node, std::vector<FbxNode*>& node_array)
{
	FbxNodeAttribute *fbx_node_attribute = in_node->GetNodeAttribute();
	if (fbx_node_attribute)
	{
		switch (fbx_node_attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			node_array.emplace_back(in_node);
			break;
		}
	}
	int childs = in_node->GetChildCount();

	for (int i = 0; i < childs; i++)
	{
		FindNodeHasMesh(in_node->GetChild(i), node_array);
	}


};



StaticMesh::StaticMesh(const char* file_name) :
	MeshBase()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3DBlob* pBlobVS;

	hr = D3DCompileFromFile(L"../Sources/Shader/MeshVS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "MeshVS",
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

#ifdef GS


	ID3DBlob* pBlobGS;

	hr = D3DCompileFromFile(L"shader\\GeometryShader.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
		"gs_5_0", 0, 0, &pBlobGS, NULL);

	if (FAILED(hr))return;


	hr = Framework::device->CreateGeometryShader(pBlobGS->GetBufferPointer(),
		pBlobGS->GetBufferSize(), NULL, geometry_shader.GetAddressOf());

	if (FAILED(hr))
		return;

	pBlobGS->Release();
#endif

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
	hr = D3DCompileFromFile(L"../Sources/Shader/MeshPS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "MeshPS",
		"ps_5_0", 0, 0, &pBlobPS, NULL);
	if (FAILED(hr))return;

	hr = Framework::device->CreatePixelShader(pBlobPS->GetBufferPointer(),
		pBlobPS->GetBufferSize(), NULL, pixel_shader.GetAddressOf());
	if (FAILED(hr))return;

	//char path[256];
	//size_t size = {};
	//wcstombs_s(&size, path, file_string, 256);
	//RESOURCE_BRAIN->pixel_shader_map.insert(std::make_pair(path, pixel_shader));



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


	//http://help.autodesk.com/view/FBX/2019/ENU/?guid=FBX_Developer_Help_getting_started_your_first_fbx_sdk_program_html

	FbxManager* fbx_manager = FbxManager::Create();

	FbxIOSettings *fbx_io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(fbx_io_settings);

	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");

	bool result = fbx_importer->Initialize(file_name, -1, fbx_manager->GetIOSettings());

	_ASSERTE(result);


	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");

	fbx_importer->Import(fbx_scene);


	std::vector<FbxNode*> node_has_mesh;


	/*if (fbx_root_node)
	{
		int child_node_count= fbx_root_node->GetChildCount();
		for (int i = 0; i < child_node_count; i++)
			fbx_root_node->GetChild(i).attr
	}// Convert mesh, NURBS and patch into triangle mesh
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene, /*replacetrue);*/
	// Convert mesh, NURBS and patch into triangle mesh
	fbxsdk::FbxGeometryConverter geometry_converter(fbx_manager);
	geometry_converter.Triangulate(fbx_scene, /*replace*/true);

	FbxNode* fbx_root_node = fbx_scene->GetRootNode();
	FindNodeHasMesh(fbx_root_node, node_has_mesh);



	//FbxMesh *fbx_mesh = fetched_meshes.at(0)->GetMesh();  // Currently only one mesh.
	auto meshes_num = node_has_mesh.size();
	meshes.resize(meshes_num);
	for (size_t i = 0; i < meshes_num; i++)
	{
		FbxMesh *fbx_mesh = node_has_mesh[i]->GetMesh();
		Mesh &mesh = meshes[i];


		std::vector<VertexData> vertices; // Vertex buffer
		std::vector<u_int> indices; // Index buffer
		u_int v_count = 0;


		FbxAMatrix global_transform = fbx_mesh->GetNode()->EvaluateGlobalTransform(0);
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				mesh.transform.m[row][column] = static_cast<float>(global_transform[row][column]);
			}
		}
		//Material

		//FbxTime::EMode time_mode = fbx_mesh->GetScene()->GetGlobalSettings().GetTimeMode();
		//FbxTime frame_time;
		//frame_time.SetTime(0, 0, 0, 1, 0, time_mode);


		const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();

		//fetch_bone_matrices(fbx_mesh, mesh.skeletal_animation, frame_time * 20); // pose at frame 20





		for (int index_of_material = 0; index_of_material < number_of_materials; ++index_of_material)
		{
			const FbxSurfaceMaterial *surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);
			const fbxsdk::FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			const fbxsdk::FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			Subset s;
			if (property.IsValid() && factor.IsValid())
			{

				FbxDouble3 color = property.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				s.diffuse.color.x = static_cast<float>(color[0] * f);
				s.diffuse.color.y = static_cast<float>(color[1] * f);
				s.diffuse.color.z = static_cast<float>(color[2] * f);
			}
			if (property.IsValid())
			{
				const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
				if (number_of_textures)
				{
					const FbxFileTexture* file_texture = property.GetSrcObject<FbxFileTexture>();
					if (file_texture)
					{
						const char *texture_name = file_texture->GetRelativeFileName();
						// Create "diffuse.shader_resource_view" from "texture_name".

						DirectX::TexMetadata metadata;
						DirectX::ScratchImage image;


						//変換前文字列

						//変換文字列格納バッファ
						wchar_t	w_texture_name[128];

						size_t wLen = 0;
						errno_t err = 0;

						//ロケール指定
						setlocale(LC_ALL, "japanese");
						//変換
						err = mbstowcs_s(&wLen, w_texture_name, 128, texture_name, _TRUNCATE);

						wchar_t* extention = Utility::YQUtility::FindExtention(static_cast<int>(wLen), w_texture_name);

						int ext_len = static_cast<int>(wcsnlen_s(extention, sizeof(ext_len)));

						for (int i = 0; i < ext_len; i++)
						{
							extention[i] = towupper(extention[i]);
						}

						wchar_t f[128];
						wcscpy_s(f, Utility::YQUtility::ChangeToWcharFromChar(file_name).c_str());


						std::wstring  filename = Utility::YQUtility::ConnectFilePathAndTextureName(f, w_texture_name);
						if (wcscmp(extention, L"TGA") == 0)
						{
							hr = DirectX::LoadFromTGAFile(filename.c_str(), &metadata, image);
						}

						else if (wcscmp(extention, L"PNG") == 0 || wcscmp(extention, L"JPG") == 0)
						{
							hr = DirectX::LoadFromWICFile(filename.c_str(), 0, &metadata, image);

						}

						if (FAILED(hr))
						{
							//return;
						}
						hr = DirectX::CreateShaderResourceView(Framework::device.Get(), image.GetImages(), image.GetImageCount(), metadata, s.diffuse.srv.GetAddressOf());
						if (FAILED(hr))
						{
							//return;
						}

					}

				}
				else
				{
					CreateDummyTexture(s.diffuse.srv.GetAddressOf());
					s.diffuse.color = XMFLOAT4(1, 1, 1, 1);
				}
			}
			mesh.subsets.push_back(s);


		}


		if (number_of_materials > 0)
		{
			const int number_of_polygons = fbx_mesh->GetPolygonCount();
			for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
			{
				const u_int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
				mesh.subsets.at(material_index).index_count += 3;
			}
			int offset = 0;
			for (Subset& s : mesh.subsets)
			{
				s.index_start = offset;
				offset += s.index_count;
				s.index_count = 0;
			}
		}
		const FbxVector4 *array_of_control_points = fbx_mesh->GetControlPoints();
		const int number_of_polygons = fbx_mesh->GetPolygonCount();
		indices.resize(number_of_polygons * 3);







		for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
		{
			int index_of_material = 0;
			if (number_of_materials > 0)
			{
				index_of_material = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
			}
			Subset &s = mesh.subsets.at(index_of_material);
			const int index_offset = s.index_start + s.index_count;

			for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++)
			{
				VertexData vertex;
				const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
				vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
				vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
				vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);
				//vertex.position.w = 0;
				FbxVector4 normal;
				fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
				vertex.normal.x = static_cast<float>(normal[0]);
				vertex.normal.y = static_cast<float>(normal[1]);
				vertex.normal.z = static_cast<float>(normal[2]);
				//vertex.normal.w = 0;






				FbxStringList uv_names;
				fbx_mesh->GetUVSetNames(uv_names);

				FbxVector2 uv;
				bool unmapped_uv;
				fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
				vertex.tex_coord.x = static_cast<float>(uv[0]);
				vertex.tex_coord.y = 1.0f - static_cast<float>(uv[1]);

				//vertex.bone_weights;
				vertices.push_back(vertex);
				//indices.push_back(v_count);
				indices.at(index_offset + index_of_vertex) += static_cast<u_int>(v_count);

				v_count += 1;

			}
			s.index_count += 3;
		}




		CreateBuffer(vertices.data(), v_count, indices.data(), v_count, mesh.vertex_buffer.GetAddressOf(), mesh.index_buffer.GetAddressOf());


	}


	fbx_scene->Destroy();
	fbx_importer->Destroy();
	fbx_manager->Destroy();



}






void StaticMesh::CreateDummyTexture(ID3D11ShaderResourceView** dpsrv)
{


	D3D11_TEXTURE2D_DESC texture2d_desc = {};
	texture2d_desc.Width = 1;
	texture2d_desc.Height = 1;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	u_int color = 0xFFFFFFFF;
	subresource_data.pSysMem = &color;
	subresource_data.SysMemPitch = 4;
	subresource_data.SysMemSlicePitch = 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;

	HRESULT hr = Framework::device->CreateTexture2D(&texture2d_desc, &subresource_data, texture2d.GetAddressOf());


	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;

	hr = Framework::device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, dpsrv);

	//srv.Reset();
}


bool StaticMesh::CreateBuffer(VertexData* v, int v_num, unsigned int* indices, int i_num, ID3D11Buffer** v_buffer, ID3D11Buffer** i_buffer)
{
	HRESULT hr;

	D3D11_BUFFER_DESC buffer_desc = {};
	D3D11_SUBRESOURCE_DATA subresource_data = {};


	//vertex buffer作成

	buffer_desc.ByteWidth = sizeof(VertexData)*v_num;
	//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource_data.pSysMem = v;
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;

	hr = Framework::device->CreateBuffer(&buffer_desc, &subresource_data, v_buffer);




	//index buffer作成

	buffer_desc.ByteWidth = sizeof(u_int)*i_num;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource_data.pSysMem = indices;
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;
	hr = Framework::device->CreateBuffer(&buffer_desc, &subresource_data, i_buffer);

	return true;


}

void StaticMesh::Render(float dt, DirectX::XMFLOAT3 pos, float angle, float scale)
{

	//return;

	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);
	GameBrain::projection_mat = XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 1.0f, 5000.0f);




	XMMATRIX world, S, R, T;

	S = XMMatrixScaling(scale, scale, scale);
	R = XMMatrixRotationRollPitchYaw(0, angle, 0);
	T = XMMatrixTranslation(pos.x, pos.y, pos.z);
	world = S * R*T;

	Render(world);
}

void StaticMesh::Render(const Yukitter::Transform& transform)
{

	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);



	XMMATRIX world, S, R, T;

	S = XMMatrixScaling(transform.scale3d.x, transform.scale3d.y, transform.scale3d.z);
	R = XMMatrixRotationRollPitchYaw(transform.rotation.pitch, transform.rotation.yaw, transform.rotation.roll);
	T = XMMatrixTranslation(transform.translation.x, transform.translation.y, transform.translation.z);
	world = S * R * T;

	Render(world);
}


void StaticMesh::Render(const Yukitter::Transform& transform, const Yukitter::Transform& parnt_transform)
{
	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);

	

	//Parent
	XMMATRIX world_p, world_c, S, R, T;

	S = XMMatrixScaling(parnt_transform.scale3d.x, parnt_transform.scale3d.y, parnt_transform.scale3d.z);
	R = XMMatrixRotationRollPitchYaw(parnt_transform.rotation.pitch, parnt_transform.rotation.yaw, parnt_transform.rotation.roll);
	T = XMMatrixTranslation(parnt_transform.translation.x, parnt_transform.translation.y, parnt_transform.translation.z);
	world_p = S * R * T;


	//Cild
	S = XMMatrixScaling(transform.scale3d.x, transform.scale3d.y, transform.scale3d.z);
	R = XMMatrixRotationRollPitchYaw(transform.rotation.pitch, transform.rotation.yaw, transform.rotation.roll);
	T = XMMatrixTranslation(transform.translation.x, transform.translation.y, transform.translation.z);
	world_c = S * R * T;
	XMMATRIX world =  world_c *world_p;

	Render(world);

}

void StaticMesh::Render(const Yukitter::Vector& scale, const DirectX::XMMATRIX& rotation_matrix, const Yukitter::Vector& translation)
{
	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	XMVECTOR eye = XMVectorSet(camera_pos.x, camera_pos.y, camera_pos.z, 1.0f);
	XMVECTOR At = XMVectorSet(camera_focus.x + 0.01f, camera_focus.y, camera_focus.z, 1.0f);


	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	GameBrain::view_mat = XMMatrixLookAtLH(eye, At, Up);



	XMMATRIX world, S, R, T;

	S = XMMatrixScaling(scale.x, scale.y, scale.z);
	R =rotation_matrix;
	T = XMMatrixTranslation(translation.x, translation.y, translation.z);
	world = S * R * T;

	Render(world);
}


void StaticMesh::Render(const DirectX::XMMATRIX& world_mat)
{
	XMFLOAT4X4 wvp;
	XMFLOAT4X4 inverse;
	Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();
	Vector camera_focus = GameBrain::GetInstance()->GetActiveCameraFocus();
	//XMStoreFloat4x4(&wvp, Scene::world_mat * Scene::view_mat * GameBrain::projection_mat);

	float angle = GameBrain::GetInstance()->GetActiveCameraReference()->transform.rotation.pitch;
	//angle += 0.001f;

	float yaw= GameBrain::GetInstance()->GetActiveCameraReference()->transform.rotation.yaw,pitch= GameBrain::GetInstance()->GetActiveCameraReference()->transform.rotation.pitch, roll= GameBrain::GetInstance()->GetActiveCameraReference()->transform.rotation.roll;
	XMMATRIX vm;

#if 0
	XMFLOAT4X4 vx = {
		1,0,	0,	0,
		0,cosf(pitch),sinf(pitch),	0,
		0,-sinf(pitch), cosf(pitch),	0,
		0,0,0,1
	};

	XMFLOAT4X4 vy = {
		cosf(yaw),0, -sinf(yaw),	0,
		0,		    1,		  0,    0,
		sinf(yaw),0,  cosf(yaw),	0,
		0,0,0,1
	};
	XMFLOAT4X4 vz = {
		cosf(roll),sinf(roll),	0,0,
		-sinf(roll), cosf(roll),0,0,
		0,0,1,0,
		0,0,0,1
	};
	vm=XMLoadFloat4x4(&vz)*XMLoadFloat4x4(&vy)*XMLoadFloat4x4(&vx);
#else
	Vector vz = (camera_focus - camera_pos).GetNormalize();

	Vector vx = Cross({ 0, 1.0f, 0 },vz );
	Vector vy = Cross(vz, vx);
	XMFLOAT4X4 m0 =
	{
		vx.x,vx.y,vx.z,0,
		vy.x,vy.y,vy.z,0,
		vz.x,vz.y,vz.z,0,
		0,0,0,1
	};
	vm = XMLoadFloat4x4(&m0);
#endif
	/*XMFLOAT4X4 vm = {
	cosf(angle),sinf(angle),0,	camera_pos.x,
	-sinf(angle), cosf(angle),			 0,	camera_pos.y,
		0,	0, 1,	camera_pos.z,
		0,0,0,	1
	};*/
	
	XMFLOAT4X4 vt=
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		camera_pos.x,camera_pos.y,camera_pos.z,1
	};
	XMMATRIX vt1= XMLoadFloat4x4(&vt);
	
	auto mmm = vm * vt1;//XMMatrixTranspose(vm);
	GameBrain::view_mat = XMMatrixInverse(nullptr,mmm);;


	XMStoreFloat4x4(&wvp, world_mat*GameBrain::view_mat   * GameBrain::projection_mat);
	XMStoreFloat4x4(&inverse, /*DirectX::XMMatrixTranspose*/(DirectX::XMMatrixInverse(0, world_mat)));


	for (auto& mesh : meshes)
	{

		UINT strides = { sizeof(VertexData) };
		UINT offsets[2] = { 0, 0 };
		Framework::device_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &strides, offsets);
		Framework::device_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		Framework::device_context->IASetInputLayout(input_layout.Get());
		Framework::device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Framework::device_context->VSSetShader(vertex_shader.Get(), NULL, 0);

		Framework::device_context->PSSetShader(pixel_shader.Get(), NULL, 0);
		Framework::device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
		Framework::device_context->RSSetState(rasterizer_state.Get());
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Vector camera_pos = GameBrain::GetInstance()->GetActiveCameraLocation();


		for (auto& s : mesh.subsets)
		{
			ConstantBuffer cb;

			cb.world_view_projection = wvp;
			cb.world_inverse_transpose = inverse;

			cb.material_color = s.diffuse.color;
			//cb.material_color.w = 1.0f;
			//cb.material_color = { 1.0f,1.0f,1.0f ,1.0f };
			Vector ligit_direction(-1, -1, 0);
			ligit_direction = ligit_direction.GetNormalize();
			cb.light_direction = { ligit_direction.x, ligit_direction.y, ligit_direction.z,1.0f };
			cb.ambient_color = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
			cb.camera_location = DirectX::XMFLOAT4(camera_pos.x, camera_pos.y, camera_pos.z, 1);

			Framework::device_context->UpdateSubresource(constant_buffer.Get(), 0, NULL, &cb, 0, 0);
			Framework::device_context->VSSetConstantBuffers(1, 1, constant_buffer.GetAddressOf());
			Framework::device_context->PSSetConstantBuffers(1, 1, constant_buffer.GetAddressOf());
			Framework::device_context->PSSetShaderResources(0, 1, s.diffuse.srv.GetAddressOf());
			//Framework::device_context->PSSetShaderResources(1, 1, shader_structure->srv.GetAddressOf());

			Framework::device_context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

			Framework::device_context->DrawIndexed(s.index_count, s.index_start, 0);
		}
	}
}