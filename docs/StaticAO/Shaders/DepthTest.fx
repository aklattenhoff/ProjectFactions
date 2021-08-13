// =============================================================
// Depth Testing Shader
// For Ambient Occlusion Pre-Computation Via Hemicubes
//
// Licensed under the Creative Commons Attribution 2.5 license
// http://creativecommons.org/licenses/by/2.5/deed.en_CA
//
// By Renaud Bédard (Zaknafein)
// E-mail : renaud.bedard@gmail.com
// =============================================================

// -------------------------------------------------------------
// Semantics
// -------------------------------------------------------------
float4x4 matWorldViewProj : WORLDVIEWPROJECTION;
int fogType : FOGTYPE = 0; // TV3D Bug...

// -------------------------------------------------------------
// Vertex Shader function
// -------------------------------------------------------------
float4 VS(float4 position : POSITION) : POSITION {
	return mul(position, matWorldViewProj);
}

// -------------------------------------------------------------
// Pixel Shader function
// -------------------------------------------------------------
float4 PS() : COLOR {
	return float4(0.0f.rrr, 1);
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
