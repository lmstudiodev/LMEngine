Texture2D TextureColor : register(t0);
sampler TextureColorSampler : register(s0);

struct PS_INPUT
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

float4 psmain(PS_INPUT input) : SV_TARGET
{
    //AMBIENT LIGHT
    float4 text_color = TextureColor.Sample(TextureColorSampler, (1.0 - input.texcoord) * 2.0f);
    float ka = 1.5f;
    float3 ia = float3(0.09f, 0.082f, 0.082f);
    ia *= text_color.rgb;
    float3 ambient_light = ka * ia;
    
    //DIFFUSE LIGHT
    float kd = 0.7f;
    float3 light_direction = normalize(m_lightPosition.xyz - input.world_pos.xyz);
    float distance_light_object = length(m_lightPosition.xyz - input.world_pos.xyz);
    float fade_area = max(0.0f, distance_light_object - m_light_radius);
    
    float constant_func = 1.0f;
    float linear_func = 2.0f;
    float quadratic_func = 2.0f;
    
    float attenuation = constant_func + linear_func * fade_area + quadratic_func * fade_area * fade_area;
    
    float amount_diffuse_light = max(0.0f, dot(light_direction.xyz, input.normal));
    float3 id = float3(1.0f, 1.0f, 1.0f);
    id *= text_color.rgb;
    float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation;

    //SPECULAR LIGHT
    float ks = 1.0f;
    float3 directionToCamera = normalize(input.world_pos.xyz - m_cameraPosition.xyz);
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflected_light = reflect(light_direction.xyz, input.normal);
    float shininess = 30.0f;
    float amount_specular_light = pow(max(0.0f, dot(reflected_light, directionToCamera)), shininess);
    float3 specular_light = (ks * amount_specular_light * is) / attenuation;
    
    //FINAL LIGHT
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    return float4(final_light, 1.0f);
}