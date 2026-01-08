struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_worldMatrix;
    row_major float4x4 m_viewMatrix;
    row_major float4x4 m_projectionMatrix;
    
    //unsigned int m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{    
    return float4(input.color, 1.0f);
}