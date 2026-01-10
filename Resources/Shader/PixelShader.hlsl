Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 directionToCamera : TEXCOORD2;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_worldMatrix;
    row_major float4x4 m_viewMatrix;
    row_major float4x4 m_projectionMatrix;
    float4 m_lightDirection;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{    
    float ka = 0.1f;
    float3 ia = float3(1.0f, 1.0f, 1.0f);
    float3 ambient_light = ka * ia;
    
    float kd = 0.7f;
    float3 id = float3(1.0f, 1.0f, 1.0f);
    float amount_diffuse_light = max(0.0f, dot(m_lightDirection.xyz, input.normal));
    float3 diffuse_light = kd * amount_diffuse_light * id;
    
    float ks = 1.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflected_light = reflect(m_lightDirection.xyz, input.normal);
    float shininess = 30.0f;
    float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.directionToCamera)), shininess);
    float3 specular_light = ks * amount_specular_light * is;
    
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    return float4(final_light, 1.0f);
    
    //Texture.Sample(TextureSampler, input.texcoord * 0.5f);
}