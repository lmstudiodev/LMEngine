struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VPS_INOUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : TEXCOORD1;
};

struct LightData
{
    float4 color;
    float4 direction;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_worldMatrix;
    row_major float4x4 m_viewMatrix;
    row_major float4x4 m_projectionMatrix;
    float4 m_cameraPosition;
    LightData m_lightData;
};

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    
    output.position = mul(input.position, m_worldMatrix);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, m_viewMatrix);
    output.position = mul(output.position, m_projectionMatrix);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) m_worldMatrix));

    return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float4 color = Color.Sample(ColorSampler, input.texcoord);
    float3 m_lightDirection = normalize(m_lightData.direction.xyz);
    float3 directionToCamera = normalize(input.worldPosition - m_cameraPosition.xyz);
    
    //AMBIENT COLOR
    float ka = 8.5f;
    float3 ia = float3(0.09f, 0.082f, 0.082f);
    ia *= (color.rgb);
    float3 ambient_light = ka * ia;
    
    //DIFFUSE COLOR
    float kd = 0.7f;
    float amount_diffuse_light = max(dot(m_lightDirection.xyz, input.normal), 0.0f);
    float3 id = m_lightData.color.rgb;
    id *= (color.rgb);
    float3 diffuse_light = kd * id * amount_diffuse_light;
    
    //SPECULAR COLOR
    float ks = 1.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflected_light = reflect(m_lightDirection.xyz, input.normal);
    float shininess = 30.0f;
    float amount_specular_light = pow(max(0.0f, dot(reflected_light, directionToCamera)), shininess);
    float3 specular_light = ks * amount_specular_light * is;
    
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    return float4(final_light, 1.0f);
}