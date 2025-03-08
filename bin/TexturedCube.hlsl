cbuffer per_object_data: register(b0)
{
    float4x4 wvp;
    float alpha;
}

struct VSInput
{
    float3 pos : POSITION0;
    float2 uv : UV0;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV0;
};

VSOutput vs_main(VSInput input)
{
    VSOutput output;
    output.pos = mul(float4(input.pos, 1.0f), wvp);
    output.uv = input.uv;
    return output;
}

Texture2D tex;
SamplerState tex_sampler;

float4 ps_main(VSOutput input) : SV_Target
{
    float4 color = tex.Sample(tex_sampler, input.uv);
    color.a = alpha;
    return color;
}
