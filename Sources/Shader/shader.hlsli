cbuffer c0 : register(b0)
{
    row_major float4x4 world_view_projection;
};


struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR1;
};