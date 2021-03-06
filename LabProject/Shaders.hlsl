//#define _WITH_CONSTANT_BUFFER_SYNTAX

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
struct CB_PLAYER_INFO
{
	matrix		mtxWorld;
}; 

struct CB_GAMEOBJECT_INFO
{
	matrix		mtxWorld;
};

struct CB_CAMERA_INFO
{
	matrix		mtxView;
	matrix		mtxProjection;
};

ConstantBuffer<CB_PLAYER_INFO> gcbPlayerObjectInfo : register(b0);
ConstantBuffer<CB_CAMERA_INFO> gcbCameraInfo : register(b1);
ConstantBuffer<CB_GAMEOBJECT_INFO> gcbGameObjectInfo : register(b2);
#else
cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxWorld : packoffset(c0);
};
#endif

//#include "Light.hlsl"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSDiffused(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
#endif
	output.color = input.color;

	return(output);
}

float4 PSDiffused(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbPlayerObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
#endif
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTextures[6] : register(t0);

SamplerState gWrapSamplerState : register(s0);
SamplerState gClampSamplerState : register(s1);

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
#endif
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
{
/*
	float4 cColor;
	if (nPrimitiveID < 2)
		cColor = gtxtTextures[0].Sample(gWrapSamplerState, input.uv);
	else if (nPrimitiveID < 4)
		cColor = gtxtTextures[1].Sample(gWrapSamplerState, input.uv);
	else if (nPrimitiveID < 6)
		cColor = gtxtTextures[2].Sample(gWrapSamplerState, input.uv);
	else if (nPrimitiveID < 8)
		cColor = gtxtTextures[3].Sample(gWrapSamplerState, input.uv);
	else if (nPrimitiveID < 10)
		cColor = gtxtTextures[4].Sample(gWrapSamplerState, input.uv);
	else
		cColor = gtxtTextures[5].Sample(gWrapSamplerState, input.uv);
*/
	float4 cColor = gtxtTextures[NonUniformResourceIndex(nPrimitiveID / 2)].Sample(gWrapSamplerState, input.uv);
	if(cColor.a < 0.1)
        discard;
	
	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBaseTexture : register(t6);
Texture2D gtxtTerrainDetailTexture : register(t7);
Texture2D gtxtTerrainDetailNormalTexture : register(t8);

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};


struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;

    float height : HEIGHT;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
    output.height = input.position.y;
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
#endif
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;
    output.normalW = mul(input.normal, (float3x3) gmtxWorld);
    output.tangentW = mul(input.tangent, (float3x3) gmtxWorld);
	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
    float4 cColor;
    float colorweight;

	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);
    float3 normal = gtxtTerrainDetailNormalTexture.Sample(gWrapSamplerState, input.uv1);
    cDetailTexColor = cDetailTexColor * 0.8;
    
    colorweight = dot(input.normalW, float3(0, 1, 0));
    //colorweight = pow(colorweight,1.5);
    //colorweight = abs(pow(((input.height - 100) / 100) , 3));
   
    cColor = cBaseTexColor * colorweight + cDetailTexColor * (1 - colorweight);

    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);

    normal = 2.0f * normal - 1.0f;
    float3 normalW = mul(normal, TBN);

   //cColor = float4(normalW, cColor.a);

    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtSkyBox : register(t10);

float4 PSSkyBox(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtSkyBox.Sample(gClampSamplerState, input.uv);

	return(cColor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_INSTANCING_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float4x4 mtxTransform : WORLDMATRIX;
    //float4 instanceColor : INSTANCECOLOR;
};

struct VS_INSTANCING_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    uint textureid : INSTANCEID;
};

//StructuredBuffer<VS_INSTANCING_INPUT> gGameObjectInfos : register(t9);

VS_INSTANCING_OUTPUT VSInstancing(VS_INSTANCING_INPUT input, uint nInstanceID : SV_InstanceID)
{
    int r1 = 1234;
    int r2 = 43212;
    float r3 = r1 * r2 % 326;

    VS_INSTANCING_OUTPUT output;
    output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
    output.uv = input.uv;

    output.textureid = 0;
    if (nInstanceID > 11250)
    {
        output.textureid = 1 + (nInstanceID / cos(r3)) % 5;
    }

    return (output);
}

float4 PSInstancing(VS_INSTANCING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
{
    if (input.uv.y < 0.2)
        discard;
    float4 cColor = gtxtTextures[NonUniformResourceIndex(input.textureid)].Sample(gWrapSamplerState, input.uv);
    if (cColor.a < 0.1)
        discard;
    return (cColor);
}