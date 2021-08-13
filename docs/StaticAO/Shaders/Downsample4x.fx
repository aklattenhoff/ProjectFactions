// =============================================================
// 4x Downsampling Shader
// For Ambient Occlusion Pre-Computation Via Hemicubes
//
// Licensed under the Creative Commons Attribution 2.5 license
// http://creativecommons.org/licenses/by/2.5/deed.en_CA
//
// By Renaud Bédard (Zaknafein)
// E-mail : renaud.bedard@gmail.com
// =============================================================

// -------------------------------------------------------------
// Texture & Sampler
// -------------------------------------------------------------
texture texTexture : TEXTURE0;
sampler sampTexture = sampler_state {
	Texture = (texTexture);
};

// -------------------------------------------------------------
// Parameter
// -------------------------------------------------------------
float texelSize;

// -------------------------------------------------------------
// Input/Output channels
// -------------------------------------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 position : POSITION;
	float2 taps[4] : TEXCOORD;
};
#define PS_INPUT VS_OUTPUT

// -------------------------------------------------------------
// Vertex Shader
// -------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	
	OUT.position = IN.position;
	
	OUT.taps[0] = IN.texCoord - texelSize;
	OUT.taps[1] = IN.texCoord + float2(texelSize, -texelSize);
	OUT.taps[2] = IN.texCoord + float2(-texelSize, texelSize);
	OUT.taps[3] = IN.texCoord + texelSize;

	return OUT;
}

// -------------------------------------------------------------
// Pixel Shaders
// -------------------------------------------------------------
float4 PS(PS_INPUT IN) : COLOR
{
	float4 samples;
	for(int i=0; i<4; i++)
		samples[i] = tex2D(sampTexture, IN.taps[i]).r;

	return float4(dot(0.25f.xxxx, samples).rrr, 1);
}

// -------------------------------------------------------------
// Techniques
// -------------------------------------------------------------
technique TSM3
{
	pass P
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}
technique TSM2a
{
	pass P
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_a PS();
	}
}
technique TSM2b
{
	pass P
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_b PS();
	}
}
technique TSM2
{
	pass P
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}