
cbuffer constants : register(b0)
{
    row_major float4x4 transform;
    row_major float4x4 projection;
              float3   lightvector;
}

struct vertexdesc
{
    float3 position : POS;
    float3 normal   : NOR;
    float2 texcoord : TEX;
    float3 color    : COL;
};

struct pixeldesc
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX;
    float4 color    : COL;
};

Texture2D    mytexture : register(t0);
SamplerState mysampler : register(s0);

pixeldesc VertexShaderMain(vertexdesc vertex)
{
    float light = clamp(dot(mul(vertex.normal, transform), normalize(-lightvector)), 0.0f, 1.0f) * 0.8f + 0.2f;
    
    pixeldesc output;

    output.position = mul(float4(vertex.position, 1.0f), mul(transform, projection));
    output.texcoord = vertex.texcoord;
    output.color = float4(vertex.color * light, 1.0f);

    return output;
}

float4 PixelShaderMain(pixeldesc pixel) : SV_TARGET
{
    return mytexture.Sample(mysampler, pixel.texcoord) * pixel.color;
}