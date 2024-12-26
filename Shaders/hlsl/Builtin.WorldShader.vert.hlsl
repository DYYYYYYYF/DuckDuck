struct UBO
{
    float4x4 proj;
    float4x4 view;
    float4 ambient_color;
    float3 view_position;
};

struct PushConstant
{
    float4x4 model;
};

struct VSInput
{
    [[vk::location(0)]] float3 vPosition : POSITION0;
    [[vk::location(1)]] float3 vNormal : NORMAL0;
    [[vk::location(2)]] float2 vTexCoord : TEXCOORD0;
    [[vk::location(3)]] float4 vColor : COLOR0;
    [[vk::location(4)]] float4 vTangent;
};

struct VSOutput
{
    float4 outPosition : SV_POSITION;
    [[vk::location(0)]] float2 outTexcoord;
    [[vk::location(1)]] float3 outNormal;
    [[vk::location(2)]] float4 outAmbientColor;
    [[vk::location(3)]] float3 outViewPosition;
    [[vk::location(4)]] float3 outFragPosition;
    [[vk::location(5)]] float4 outColor;
    [[vk::location(6)]] float4 outTangent;
};

[[vk::binding(0, 0)]] UBO ubo;
[[vk::push_constant]] ConstantBuffer<PushConstant> push_constants;

VSOutput main(VSInput input) 
{
	VSOutput output = (VSOutput)0;
    output.outPosition = mul(ubo.proj, mul(ubo.view, mul(push_constants.model, float4(input.vPosition, 1.0f))));
    output.outColor = input.vColor;
	output.outTexcoord = input.vTexCoord;
    output.outAmbientColor = ubo.ambient_color;
    output.outViewPosition = ubo.view_position;
    output.outFragPosition = mul(push_constants.model, float4(input.vPosition, 1.0f));
    output.outNormal = normalize(mul(push_constants.model, float4(input.vNormal, 1.0f)));
    output.outTangent = float4(normalize(mul(push_constants.model, input.vTangent)).xyz, input.vTangent.w);

	return output;
}