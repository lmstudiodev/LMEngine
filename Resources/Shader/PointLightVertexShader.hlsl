struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 world_pos : TEXCOORD1;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_worldMatrix;
    row_major float4x4 m_viewMatrix;
    row_major float4x4 m_projectionMatrix;
    float4 m_lightDirection;
    float4 m_cameraPosition;
    float4 m_lightPosition;
    float m_light_radius;
    float m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.position = mul(input.position, m_worldMatrix);
    output.world_pos = output.position.xyz;
    output.position = mul(output.position, m_viewMatrix);
    output.position = mul(output.position, m_projectionMatrix);
    
    
    
    output.texcoord = input.texcoord;
    output.normal = input.normal;
    
    return output;
}