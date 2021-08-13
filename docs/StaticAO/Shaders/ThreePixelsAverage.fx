// =============================================================
// 3-Pixels-To-A-Single-One Shader
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
texture texTexture;
sampler sampTexture = sampler_state {
	Texture = (texTexture);
};

// -------------------------------------------------------------
// Vertex Shader function
// -------------------------------------------------------------
float4 VS(float4 position : POSITION) : POSITION {
	return position;
}

// -------------------------------------------------------------
// Pixel Shader function
// -------------------------------------------------------------
float4 PS() : COLOR {
	float3 colorArray;
	
	colorArray[0] = tex2D(sampTexture, 0.0f.xx).r;
	colorArray[1] = tex2D(sampTexture, float2(1, 0)).r;
	colorArray[2] = tex2D(sampTexture, float2(0, 1)).r;
	
	return float4(dot(colorArray, (1.0f / 3).xxx).rrr, 1);
}

// -------------------------------------------------------------
// Techniques
// -------------------------------------------------------------
technique TSM2 {
	pass P {
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();    
	}
}
