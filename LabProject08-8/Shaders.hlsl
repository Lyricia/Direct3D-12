cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	float3		gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
	uint		gnMaterial : packoffset(c4);
};

#include "Light.hlsl"

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

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
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

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

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

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	return(cColor);
}

#define _WITH_VERTEX_LIGHTING

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR;
#endif
};

VS_LIGHTING_OUTPUT VSLighting(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output;

	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
	return(output);
}

float4 PSLighting(VS_LIGHTING_OUTPUT input) : SV_TARGET
{
#ifdef _WITH_VERTEX_LIGHTING
	float4 cIllumination = input.color;
#else
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
#endif
	return(cIllumination);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	//	nointerpolation float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR;
#endif
};

VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output;

	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
	return(output);
}

float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
#ifdef _WITH_VERTEX_LIGHTING
	float4 cIllumination = input.color;
#else
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
#endif
	return(lerp(cColor, cIllumination, 0.5f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBase : register(t1);
Texture2D gtxtTerrainDetail : register(t2);
Texture2D gtxtTerrainNormal : register(t3);

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
    output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
#endif
    output.color = input.color;
    output.uv0 = input.uv0;
    output.uv1 = input.uv1;
    output.normalW = mul(input.normal, (float3x3) gmtxGameObject);
    output.tangentW = mul(input.tangent, (float3x3) gmtxGameObject);
    return (output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
    float4 cColor;
    float colorweight;

    float4 cBaseTexColor = gtxtTerrainBase.Sample(gSamplerState, input.uv0);
    float4 cDetailTexColor = gtxtTerrainDetail.Sample(gSamplerState, input.uv1);
    float3 normal = gtxtTerrainNormal.Sample(gSamplerState, input.uv1);
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
    float4 cIllumination = Lighting(input.positionW, normalW);

    cColor = lerp(cColor, cIllumination, 0.5f);
    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtSkyBox : register(t4);

float4 PSSkyBox(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtSkyBox.Sample(gSamplerState, input.uv);

    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtBillBoardTextures[6] : register(t5);

struct VS_BILLBOARD_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
};

struct VS_BILLBOARD_OUTPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
};

struct GS_BILLBOARD_OUTPUT
{
    float4 positionH : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
    uint primitiveID : SV_PrimitiveID;
};

VS_BILLBOARD_OUTPUT VS_BillBoard(VS_BILLBOARD_INPUT input)
{
    VS_BILLBOARD_OUTPUT output;

    output.position = input.position;
    output.size = input.size;

    return output;
}

[maxvertexcount(4)]
void GS_BillBoard(point VS_BILLBOARD_OUTPUT input[1], uint primitiveID : SV_PrimitiveID, inout TriangleStream<GS_BILLBOARD_OUTPUT> outStream)
{
    float3 vUp = float3(0.0f, 1.0f, 0.0f);
    float3 vLook = gvCameraPosition.xyz - input[0].position;
    vLook = normalize(vLook);
    float3 vRight = cross(vUp, vLook);
    float fHalfW = input[0].size.x * 0.5f;
    float fHalfH = input[0].size.y * 0.5f;

    float4 pVertices[4];
    pVertices[0] = float4(input[0].position + fHalfW * vRight - fHalfH * vUp, 1.0f);
    pVertices[1] = float4(input[0].position + fHalfW * vRight + fHalfH * vUp, 1.0f);
    pVertices[2] = float4(input[0].position - fHalfW * vRight - fHalfH * vUp, 1.0f);
    pVertices[3] = float4(input[0].position - fHalfW * vRight + fHalfH * vUp, 1.0f);


    float2 pUVs[4] =
    {
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f)
    };

    GS_BILLBOARD_OUTPUT output;

    for (int i = 0; i < 4; ++i)
    {
        output.positionW = pVertices[i].xyz;
        output.positionH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
        //output.positionH = mul(pVertices[i], gmtxProjection);
        output.normalW = vLook;
        output.uv = pUVs[i];
        output.primitiveID = primitiveID;
        outStream.Append(output);
    }
}

float4 PS_BillBoard(GS_BILLBOARD_OUTPUT input) : SV_Target
{
    //float4 cIllumination = Lighting(input.positionW, input.normalW);
    if(input.uv.y < 0.05)
        discard;
    
    float4 cColor = gtxtBillBoardTextures[NonUniformResourceIndex(input.primitiveID % 6)].Sample(gSamplerState, input.uv);

    if (cColor.a < 0.2 || cColor.r + cColor.g + cColor.b > 2.0f)
        discard;

    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtParticleBillBoardTextures : register(t11);

struct VS_PARTICLE_BILLBOARD_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    float2 spritepos : SPRITEPOSITION;
};

struct VS_PARTICLE_BILLBOARD_OUTPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    float2 spritepos : SPRITEPOSITION;
};

struct GS_PARTICLE_BILLBOARD_OUTPUT
{
    float4 positionH : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
    uint primitiveID : SV_PrimitiveID;
};

VS_PARTICLE_BILLBOARD_OUTPUT VS_ParticleBillBoard(VS_PARTICLE_BILLBOARD_INPUT input)
{
    VS_PARTICLE_BILLBOARD_OUTPUT output;

    output.position = input.position;
    output.size = input.size;
    output.spritepos = input.spritepos;

    return output;
}

[maxvertexcount(4)]
void GS_ParticleBillBoard(point VS_PARTICLE_BILLBOARD_OUTPUT input[1], uint primitiveID : SV_PrimitiveID, inout TriangleStream<GS_PARTICLE_BILLBOARD_OUTPUT> outStream)
{
    float3 vUp = float3(0.0f, 1.0f, 0.0f);
    float3 vLook = gvCameraPosition.xyz - input[0].position;
    vLook = normalize(vLook);
    float3 vRight = cross(vUp, vLook);
    float fHalfW = input[0].size.x * 0.5f;
    float fHalfH = input[0].size.y * 0.5f;

    float4 pVertices[4];
    pVertices[0] = float4(input[0].position + fHalfW * vRight - fHalfH * vUp, 1.0f);
    pVertices[1] = float4(input[0].position + fHalfW * vRight + fHalfH * vUp, 1.0f);
    pVertices[2] = float4(input[0].position - fHalfW * vRight - fHalfH * vUp, 1.0f);
    pVertices[3] = float4(input[0].position - fHalfW * vRight + fHalfH * vUp, 1.0f);

    //pUVs[0];
    //pUVs[1];
    //pUVs[2];
    //pUVs[3];

    float spritewidth = input[0].spritepos.x;
    float spriteheight = input[0].spritepos.y;

    float2 pUVs[4] =
    {
        float2(spritewidth * 0.125f, (spriteheight + 1) * 0.125f),
        float2(spritewidth * 0.125f, spriteheight * 0.125f),
        float2((spritewidth + 1) * 0.125f, (spriteheight + 1) * 0.125f),
        float2((spritewidth + 1) * 0.125f, spriteheight * 0.125f)
    };

    GS_PARTICLE_BILLBOARD_OUTPUT output;

    for (int i = 0; i < 4; ++i)
    {
        output.positionW = pVertices[i].xyz;
        output.positionH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
        output.normalW = vLook;
        output.uv = pUVs[i];
        output.primitiveID = primitiveID;
        outStream.Append(output);
    }
}

float4 PS_ParticleBillBoard(GS_PARTICLE_BILLBOARD_OUTPUT input) : SV_Target
{
    float4 cColor = gtxtParticleBillBoardTextures.Sample(gSamplerState, input.uv);

    if (cColor.a < 0.2)
        discard;

    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_MINIMAP_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D gtxtScreen : register(t1);
Texture2D gtxtMinimap : register(t2);  

VS_MINIMAP_OUTPUT VS_FullScreen(uint nVertexID : SV_VertexID)
{
    VS_MINIMAP_OUTPUT output;

    if (nVertexID == 0)
    {
        output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    return (output);
}

float4 PS_FullScreen(VS_MINIMAP_OUTPUT input) : SV_Target
{
    float3 cColor = gtxtScreen[int2(input.position.xy)].rgb;
    return (float4(cColor, 1.0f));
}

float4 PS_Minimap(VS_MINIMAP_OUTPUT input) : SV_Target
{
    float3 cColor = gtxtMinimap[int2(input.position.xy)].rgb;
    return (float4(cColor, 1.0f));
}