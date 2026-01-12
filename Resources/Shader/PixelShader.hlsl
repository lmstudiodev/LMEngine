Texture2D EarthColor : register(t0);
sampler EarthColorSampler : register(s0);

Texture2D EarthSpecular : register(t1);
sampler EarthSpecularSampler : register(s1);

Texture2D Clouds : register(t2);
sampler CloudsSampler : register(s2);

Texture2D EarthNight : register(t3);
sampler EarthNightSampler : register(s3);

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
    float4 m_cameraPosition;
    float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{    
    float4 earth_color = EarthColor.Sample(EarthColorSampler, 1.0 - input.texcoord);
    float earth_specular = EarthSpecular.Sample(EarthSpecularSampler, 1.0 - input.texcoord).r;
    float clouds = Clouds.Sample(CloudsSampler, 1.0 - input.texcoord + float2(m_time / 100.0f, 0.0f)).r;
    float4 earth_night = EarthNight.Sample(EarthNightSampler, 1.0 - input.texcoord);
    
    float ka = 1.5f;
    float3 ia = float3(0.09f, 0.082f, 0.082f);
    ia *= earth_color.rgb;
    float3 ambient_light = ka * ia;
    
    float kd = 0.7f;
    float3 id_day = float3(1.0f, 1.0f, 1.0f);
    id_day *= (earth_color.rgb + clouds);
    
    float3 id_night = float3(1.0f, 1.0f, 1.0f);
    id_night *= (earth_night.rgb + (clouds * 0.3f));
    
    float amount_diffuse_light = dot(m_lightDirection.xyz, input.normal);
    
    float3 id = lerp(id_night, id_day, (amount_diffuse_light + 1.0f) / 2.0f);
    
    float3 diffuse_light = kd * id;
    
    float ks = earth_specular;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflected_light = reflect(m_lightDirection.xyz, input.normal);
    float shininess = 30.0f;
    float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.directionToCamera)), shininess);
    float3 specular_light = ks * amount_specular_light * is;
    
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    return float4(final_light, 1.0f);
}