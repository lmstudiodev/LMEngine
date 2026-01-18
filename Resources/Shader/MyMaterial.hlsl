struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_worldMatrix;
    row_major float4x4 m_viewMatrix;
    row_major float4x4 m_projectionMatrix;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.position = mul(input.position, m_worldMatrix);    
    output.position = mul(output.position, m_viewMatrix);
    output.position = mul(output.position, m_projectionMatrix);

    output.texcoord = input.texcoord;

    return output;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(PS_INPUT input) : SV_TARGET
{
    float4 color = Color.Sample(ColorSampler, input.texcoord);
    
    return color;
}