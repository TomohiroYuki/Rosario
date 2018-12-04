#pragma once
#include <vector>

#include "System\Object\3D\MeshBase.h"
#include "math\Transform.h"


class SkeltalMesh :public MeshBase
{
public:
	SkeltalMesh(const char* filename);
	virtual ~SkeltalMesh() {}
private:

	/*struct bone
	{
		DirectX::XMFLOAT4X4 transform;
	};
	typedef std::vector<bone> Skeletal;

	struct SkeltalAnimation : public std::vector<Skeletal>
	{
	public:
		float sampling_time = 1 / 24.0f;
		float animation_tick = 0.0f;
	};

	struct Mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
		std::vector<Subset> subsets;
		
		SkeltalAnimation skeletal_animation;
		float animation_frame = 0.0f;
	};*/
	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex_coord;

	};

	struct Mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
		std::vector<Subset> subsets;
		DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

	};


	std::vector<Mesh> meshes;

	bool CreateBuffer(VertexData* v, int v_num, unsigned int* indices, int i_num, ID3D11Buffer** v_buffer, ID3D11Buffer** i_buffer);

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;
		DirectX::XMFLOAT4X4 world_inverse_transpose;
		DirectX::XMFLOAT4   material_color;
		DirectX::XMFLOAT4   light_direction;
		DirectX::XMFLOAT4   camera_location;
		DirectX::XMFLOAT4   ambient_color;
	};

public:
	void Render(float dt, DirectX::XMFLOAT3 pos, float angle, float scale = 1.0f);
	void Render(const Yukitter::Transform& transform);
	void Render(const DirectX::XMMATRIX& world_mat);
	void Render(const Yukitter::Vector& scale, const DirectX::XMMATRIX& rotation_matrix, const Yukitter::Vector& translation);
	void Render(const Yukitter::Transform& transform, const Yukitter::Transform& parnt_transform);
};