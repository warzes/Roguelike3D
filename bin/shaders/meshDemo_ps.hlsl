Texture2D tex;
SamplerState samplerState : register(s0);

cbuffer PerViewData : register(b0)
{
    float4x4 mat_view;
    float4x4 mat_inv_view;
    float4x4 mat_view_projection;
    float4x4 mat_inv_view_projection;
    float4 pos_camera_ws;
};

cbuffer PerObjectData : register(b1)
{
    float4x4 mat_world;
};

struct PSInput
{
    float4 pos_cs : SV_POSITION;
    float4 pos_ws : POSITION0;
    float4 normal_ws : NORMAL0;
    float2 uv : UV0;
};

void Main(PSInput input, out float4 out_color : SV_Target0)
{
    out_color = tex.Sample(samplerState, input.uv);

#ifdef ALPHA_CUTOFF
    clip(out_color.a < .1f ? -1.0f : 1.0f);
#endif
}
