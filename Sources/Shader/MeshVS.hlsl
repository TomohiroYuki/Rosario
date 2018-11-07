#include "Mesh.hlsli"

VS_OUT3D MeshVS(VS_INPUT3D input)
{
    VS_OUT3D Out = (VS_OUT3D) 0;
    float4 pos = float4(input.position, 1);
    Out.position = mul(pos, world_view_projection);
    float4 nor = float4(input.normal, 0);
   
    float4 N = normalize(mul(nor, world_inverse_transpose));

    
    float4 L = normalize(-light_direction);
    Out.color = material_color * max(0, dot(L, N));
    Out.normal = N;
    Out.texcoord = input.texcoord;

 
    //normal.w = 0.0;
    //normal = normalize(normal);

    ////vout.outline_pos = position + normal * 0.05f;
    ////vout.outline_pos = position;
    ////vout.outline_pos += normal *1.15f;
    //vout.position = mul(position, world_view_projection);

    ////vout.outline_pos = mul(vout.outline_pos, world_view_projection);
    //normal.w = 0;
    //float4 N = normalize(mul(normal, world_inverse_transpose));
    //float4 L = normalize(-light_direction);
    //vout.color = material_color * max(0, dot(L, N));
    ////vout.color = material_color;

    ////vout.color.a = 1;
    //vout.texcoord = texcoord;
    //vout.normal = N;
    //return vout;


    return Out;
}