Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

Texture2D Normal : register(t1);
sampler NormalSampler : register(s1);


struct PS_INPUT
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
    float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    float4 color = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
    float4 normal = Normal.Sample(NormalSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
    
    normal.xyz = (normal.xyz * 2.0f) - 1.0f;
    normal.xyz = mul(normal.xyz, input.tbn);
    
    float dot_nl = dot(m_lightDirection.xyz, input.tbn[2]);
    
    //AMBIENT LIGHT
    float ka = 8.5f;
    float3 ia = float3(0.09f, 0.082f, 0.082f);
    ia *= (color.rgb);
    float3 ambient_light = ka * ia;
    
    //DIFFUSE LIGHT
    float kd = 0.7f;
    float amount_diffuse_light = dot(m_lightDirection.xyz, normal.xyz);
    float3 id = float3(1.0f, 1.0f, 1.0f);
    id *= (color.rgb);
    float3 diffuse_light = kd * id * amount_diffuse_light;

    //SPECULAR LIGHT
    float ks = 1.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflected_light = reflect(m_lightDirection.xyz, normal.xyz);
    float shininess = 30.0f;
    float amount_specular_light = 0;
    
    if(dot_nl > 0)
        amount_specular_light =  pow(max(0.0f, dot(reflected_light, input.directionToCamera)), shininess);
    
    float3 specular_light = ks * amount_specular_light * is;
    
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    return float4(final_light, 1.0f);
}