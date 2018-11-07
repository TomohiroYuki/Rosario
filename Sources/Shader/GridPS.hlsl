#include "shader.hlsli"



float4 main(VS_OUT ps_input) : SV_TARGET
{
    return ps_input.color;
}