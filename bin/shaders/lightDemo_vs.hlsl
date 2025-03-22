cbuffer mvp : register(b0)
{
    float4x4 model;
    float4x4 view;
    float4x4 proj;
    
    float4x4 inverseModel;
};

struct VertexOutput
{
    float4 pos : SV_Position;
    float4 worldPos : WORLD_POS;
    float3 normal : NORMAL;
    float2 texCoords : TEX_COORDS;
};

VertexOutput Main(float3 pos : POSITION0, float3 normal : NORMAL0, float2 texCoords : UV0)
{
    VertexOutput o;
    
    float4x4 finalMVP = mul(mul(model, view), proj);
    
    o.pos = mul(float4(pos.xyz, 1.0f), finalMVP);
    
    o.worldPos = mul(float4(pos.xyz, 1.0f), model);
    
    o.texCoords = texCoords;
    
    o.normal = normalize(mul(normal, float3x3(inverseModel[0].xyz, inverseModel[1].xyz, inverseModel[2].xyz)));
    
    return o;
}