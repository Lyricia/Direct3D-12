cbuffer cbGameObjectInfo : register(b0)
{
	//float w11, w12, w13, w14;
	//float w21, w22, w23, w24;
	//float w31, w32, w33, w34;
	//float w41, w42, w43, w44;
	matrix		gmtxWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	//float v11, v12, v13, v14;
	//float v21, v22, v23, v24;
	//float v31, v32, v33, v34;
	//float v41, v42, v43, v44;
	//float p11, p12, p13, p14;
	//float p21, p22, p23, p24;
	//float p31, p32, p33, p34;
	//float p41, p42, p43, p44;
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VSDiffused(VS_INPUT input)
{
	VS_OUTPUT output;
	//matrix gmtxWorld;
	//gmtxWorld[0] = float4(w11, w21, w31, w41);
	//gmtxWorld[1] = float4(w12, w22, w32, w42);
	//gmtxWorld[2] = float4(w13, w23, w33, w43);
	//gmtxWorld[3] = float4(w14, w24, w34, w44);
	//matrix gmtxView;
	//gmtxView[0] = float4(v11, v21, v31, v41);
	//gmtxView[1] = float4(v12, v22, v32, v42);
	//gmtxView[2] = float4(v13, v23, v33, v43);
	//gmtxView[3] = float4(v14, v24, v34, v44);
	//matrix gmtxProjection;
	//gmtxProjection[0] = float4(p11, p21, p31, p41);
	//gmtxProjection[1] = float4(p12, p22, p32, p42);
	//gmtxProjection[2] = float4(p13, p23, p33, p43);
	//gmtxProjection[3] = float4(p14, p24, p34, p44);

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

