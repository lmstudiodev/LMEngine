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
    float3 directionToCamera : TEXCOORD1;
    row_major float3x3 tbn : TEXCOORD2;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_worldMatrix;
    row_major float4x4 m_viewMatrix;
    row_major float4x4 m_projectionMatrix;
    float4 m_lightDirection;
    float4 m_cameraPosition;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.position = mul(input.position, m_worldMatrix);
    output.directionToCamera = normalize(output.position.xyz - m_cameraPosition.xyz);
    
    output.position = mul(output.position, m_viewMatrix);
    output.position = mul(output.position, m_projectionMatrix);

    output.texcoord = input.texcoord;

    output.tbn[0] = normalize(mul(input.tangent, m_worldMatrix));
    output.tbn[1] = normalize(mul(input.binormal, m_worldMatrix));
    output.tbn[2] = normalize(mul(input.normal, m_worldMatrix));
    
    
    
    
    
    
    
    return output;
}