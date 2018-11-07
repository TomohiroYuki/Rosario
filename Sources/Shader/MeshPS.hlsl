#include "Mesh.hlsli"
Texture2D diffuse_map : register(t0);

SamplerState g_samLinear : register(s0); //ƒTƒ“ƒvƒ‰[



float4 MeshPS(VS_OUT3D input) : SV_TARGET
{
    float4 c = input.color;
   
    c.r += 0.6f;
    c.g += 0.6f;
    c.b += 0.6f;
    c.a = 1;

    //return c;
    return saturate(diffuse_map.Sample(g_samLinear, input.texcoord) * c );
}