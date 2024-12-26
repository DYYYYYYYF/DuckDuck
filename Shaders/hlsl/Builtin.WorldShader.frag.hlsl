const int SAMP_DIFFUSE = 0;
const int SAMP_SPECULAR = 1;
const int SAMP_NORMAL = 2;
const int SAMP_ROUGHNESS_METALLIC = 3;

struct LocalUniformObject
{
    float4 DiffusrColor;
    float Shiniess;
    float Metallic;
    float Roughness;
    float AlbientOcclusion;
};

struct PSInput
{
    [[vk::location(0)]] float2 vTexcoord : TEXCOORD0;
    [[vk::location(1)]] float3 vNormal : NORMAL0;
    [[vk::location(2)]] float4 vAlbientColor : COLOR0;
    [[vk::location(3)]] float3 vViewPosition : POSITION0;
    [[vk::location(4)]] float3 vFragPosition : POSITION1;
    [[vk::location(5)]] float4 vColor : COLOR1;
    [[vk::location(6)]] float4 vTangent;
	
};

struct DirectionalLight
{
    float3 direction;
    float4 color;
};

struct PointLight
{
    float3 position;
    float4 color;
    float fconstant;
    float linear_attenuation;
    float quadratic;
};


[[vk::binding(1, 1)]] Texture2D Textures[];
[[vk::binding(1, 1)]] SamplerState Samplers[];

cbuffer Localuniform : register(b0, space1)
{
    LocalUniformObject Localuniform;
}

// Temp
static DirectionalLight dir_light =
{
    float3(0.0f, -0.57735f, -0.57735f),
	float4(0.8f, 0.8f, 0.8f, 1.0f)
};

static PointLight PointLight0 =
{
    float3(-5.5f, -10.0f, -5.5f),
	float4(0.0f, 1.0f, 0.0f, 1.0f),
	1.0f, // Constant
	0.35f, // Linear
	0.44f // Quadratic
};

static PointLight PointLight1 =
{
    float3(5.5f, 0.0f, -5.5f),
	float4(1.0f, 0.0f, 0.0f, 1.0f),
	1.0f, // Constant
	0.35f, // Linear
	0.44f // Quadratic
};
// Temp

const float PI = 3.14159265359;
float4 CalculatePBR(PSInput pin);
float3 FresnelSchlick(float CosTheta, float3 F0);
float DistributionGGX(float3 N, float3 H, float Roughness);
float GeometrySchlickGGX(float NdotV, float Roughness);
float GeometrySmith(float3 N, float3 V, float3 L, float Roughness);

float4 CalculateDirectionalLight(PSInput pin, DirectionalLight light, float3 normal, float3 view_direction);
float4 CalculatePointLight(PSInput pin, PointLight light, float3 normal, float3 frag_position, float3 view_direction);

float4 main(PSInput pin) : SV_TARGET
{
    float3 Normal = pin.vNormal;
    float3 Tangent = pin.vTangent;
    
    float4 FragColor = CalculatePBR(pin);
    
    return FragColor;
}

float4 CalculatePBR(PSInput pin)
{
    float4 DiffuseColor = Textures[SAMP_DIFFUSE].Sample(Samplers[SAMP_DIFFUSE], pin.vTexcoord);
    float3 NormalColor = Textures[SAMP_NORMAL].Sample(Samplers[SAMP_NORMAL], pin.vTexcoord);
    float3 RMA = Textures[SAMP_ROUGHNESS_METALLIC].Sample(Samplers[SAMP_ROUGHNESS_METALLIC], pin.vTexcoord);
    float Roughness = RMA.r / 255;
    float Metallic = RMA.g / 255;
    float AO = RMA.b / 255;

    float3 Albedo = pin.vAlbientColor;
    //float3 N = normalize(NormalColor);
    float3 N = normalize(pin.vNormal);
    float3 V = normalize(pin.vViewPosition - pin.vFragPosition);
    
    float3 F0 = float3(0.04f);  // 非金属表面的F0反射率
    F0 = lerp(F0, Albedo, Metallic);
    
    // float3 L = normalize(dir_light.direction);
    float3 L = normalize(PointLight0.position - pin.vFragPosition);
    float3 H = normalize(V + L);
    
    // 高光 Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, Roughness);
    float G = GeometrySmith(N, V, L, Roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);
    
    float3 Nominator = NDF * G * F;
    float Denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
    float3 Specular = Nominator / Denominator;
    
    // 漫反射
    float3 KS = F;
    float3 KD = float3(1.0f) - KS;
    KD *= 1.0f - Metallic;
    
    float NdotL = max(dot(N, L), 0.0f);
    float3 Lo = (KD * Albedo / PI + Specular) * DiffuseColor.xyz * NdotL;
    
    // 环境光
    float3 Ambient = float3(0.03f) * Albedo * AO;
    float3 Color = Ambient + Lo;
    
    // HDR & GAMMA
    float3 HDR_MAP_PARAM = float3(1.0f);
    Color = Color / (Color + HDR_MAP_PARAM);
    float GAMMA_CORRECT_PARAM = 2.0f;
    Color = pow(Color, float3(1.0f / GAMMA_CORRECT_PARAM));
    
    return float4(Color, 1.0f);
}

float3 FresnelSchlick(float CosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - CosTheta, 5.0f);
}

float DistributionGGX(float3 N, float3 H, float Roughness)
{
    float R2 = Roughness * Roughness;
    float A2 = R2 * R2;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;
    
    float Nom = A2;
    float Denom = (NdotH2 * (A2 - 1.0f) + 1.0f);
    Denom = PI * Denom * Denom;
    return Nom / Denom;
}

float GeometrySchlickGGX(float NdotV, float Roughness)
{
    float R = (Roughness + 1.0f);
    float K = (R * R) / 8.0f;
    
    float Nom = NdotV;
    float Denom = NdotV * (1.0f - K) + K;
    
    return Nom / Denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float Roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float GGX1 = GeometrySchlickGGX(NdotV, Roughness);
    float GGX2 = GeometrySchlickGGX(NdotL, Roughness);
    
    return GGX1 * GGX2;
}

float4 CalculateDirectionalLight(PSInput pin, DirectionalLight light, float3 normal, float3 view_direction)
{
	
    return float4(1, 0, 0, 1);
}

float4 CalculatePointLight(PSInput pin, PointLight light, float3 normal, float3 frag_position, float3 view_direction)
{
    
    return float4(1, 0, 0, 1);
}