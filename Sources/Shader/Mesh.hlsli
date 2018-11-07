cbuffer c0 : register(b1)
{
    
    row_major float4x4 world_view_projection;
    row_major float4x4 world_inverse_transpose;
    float4 material_color;
    float4 light_direction;
    float4 camera_location;
    float4 ambient_color;
};

struct VS_INPUT3D
{
    float3 position     :POSITION;
    float3 normal       : NORMAL;
    float2 texcoord     : TEXCOORD;
};

struct VS_OUT3D
{
    float4 position     : SV_POSITION;
    float4 normal       : NORMAL;
    float4 color        : COLOR;
    float2 texcoord     : TEXCOORD;
   
};
