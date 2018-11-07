#include "shader.hlsli"


VS_OUT GridVS( VS_INPUT vs_input) 
{
    
    VS_OUT Out = (VS_OUT) 0;
    Out.position = mul(vs_input.position, world_view_projection);
    Out.color = vs_input.color;

    return Out;

}