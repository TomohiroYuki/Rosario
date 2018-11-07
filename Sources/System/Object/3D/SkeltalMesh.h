#pragma once
#include <vector>

#include "System\Object\3D\MeshBase.h"

class SkeltalMesh :public MeshBase
{
public:
	SkeltalMesh(const char* filename);
	virtual ~SkeltalMesh() {}
private:

	struct bone
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
	};


public:
};