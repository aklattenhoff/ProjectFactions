// =============================================================
// Blinn-Phong Parallax Mapping Shader
// With Ambient Occlusion Support Through Vertex Colors
//
// Licensed under the Creative Commons Attribution 2.5 license
// http://creativecommons.org/licenses/by/2.5/deed.en_CA
//
// By Renaud Bédard (Zaknafein)
// E-mail : renaud.bedard@gmail.com
//
// Reference for the Lyon reflection function :
// --------------------------------------------
// Richard F. Lyon, "Phong Shading Reformulation for Hardware Renderer Simplification"
// Apple Technical Report #43, Apple Computer, Inc. 1993
// http://dicklyon.com/tech/Graphics/Phong_TR-Lyon.pdf
// =============================================================

// -------------------------------------------------------------
// Parameters
// -------------------------------------------------------------
float diffuseOcclusionFactor = 0;

// -------------------------------------------------------------
// Constants
// -------------------------------------------------------------
#define FOG_TYPE_NONE    0
#define FOG_TYPE_EXP     1
#define FOG_TYPE_EXP2    2
#define FOG_TYPE_LINEAR  3 

#define PARALLAX_AMOUNT 0.035f			
#define HALF_PARALLAX_AMOUNT 0.0175f

// -------------------------------------------------------------
// Semantics
// -------------------------------------------------------------
float4x4 matWorldViewProj : WORLDVIEWPROJECTION;
float4x3 matWorldIT : WORLDINVERSETRANSPOSE;
float4x4 matWorld : WORLD;
float3 viewPosition : VIEWPOSITION;

float3 dirLightDir : LIGHTDIR0_DIRECTION;
float3 dirLightColor : LIGHTDIR0_COLOR;

float3 pointLight0Pos : LIGHTPOINT0_POSITION;
float3 pointLight1Pos : LIGHTPOINT1_POSITION;
float3 pointLight2Pos : LIGHTPOINT2_POSITION;
float3 pointLight3Pos : LIGHTPOINT3_POSITION;

float3 pointLight0Color : LIGHTPOINT0_COLOR;
float3 pointLight1Color : LIGHTPOINT1_COLOR;
float3 pointLight2Color : LIGHTPOINT2_COLOR;
float3 pointLight3Color : LIGHTPOINT3_COLOR;

int pointLightsCount : LIGHTPOINT_NUM;

int fogType : FOGTYPE;
float3 fogColor : FOGCOLOR;
float fogDensity : FOGDENSITY;
float fogStart : FOGSTART;
float fogEnd : FOGSTART;
	  
float3 materialEmissive : EMISSIVE;
float3 materialAmbient : AMBIENT;
float4 materialDiffuse : DIFFUSE;
float3 materialSpecular : SPECULAR;
float materialPower : SPECULARPOWER;

// -------------------------------------------------------------
// Textures & Samplers
// -------------------------------------------------------------
texture texTexture : TEXTURE0;
sampler sampTexture = sampler_state {
	Texture = (texTexture);
};

texture texNormal : TEXTURE1;
sampler sampNormal = sampler_state {
	Texture = (texNormal);
};

texture texHeight : TEXTURE2;
sampler sampHeight = sampler_state {
	Texture = (texHeight);
};

texture texEmissive : TEXTURE3;
sampler sampEmissive = sampler_state {
	Texture = (texEmissive);
};
 
// -------------------------------------------------------------
// Input/Output channels
// -------------------------------------------------------------
struct VS_INPUT {
	float4 position : POSITION;		// Vertex position in object space
	float2 texCoord : TEXCOORD;	// Vertex texture coordinates
	float4 color : COLOR;			// Vertex color
	float3 binormal : BINORMAL;		// Vertex binormal
	float3 tangent : TANGENT;		// Vertex tangent
	float3 normal : NORMAL;			// Vertex normal		
};

struct VS_COMMON_OUTPUT {
	float2 texCoord : TEXCOORD0;		// Pixel texture coordinates
	float3 view : TEXCOORD1;			// View vector in tangent space	
	float ambientOccl : TEXCOORD2;		// Ambient occlusion factor
	float3 dirLight : TEXCOORD3;		// Directional light vector in tangent space
	float fog : FOG;					// Fog coefficient	
};
#define	PS_COMMON_INPUT VS_COMMON_OUTPUT

struct VS_INTERMEDIARY_DATA {
	float3x3 worldItToTangentSpace;
	float3 worldPos;
};

struct PS_INTERMEDIARY_DATA {
	float2 texCoord;
	float3 view;
	float3 normal;
	float specularMask;
	float diffSpecOcclusion;
};

struct VS2_OUTPUT {
	float3 pointLight[2] : TEXCOORD4;	// Point lights 0-1 vectors in tangent space
};
#define	PS2_INPUT VS2_OUTPUT

struct VS3_OUTPUT {
	float3 pointLight[4] : TEXCOORD4;	// Point lights 0-3 vectors in tangent space
};
#define	PS3_INPUT VS3_OUTPUT

// -------------------------------------------------------------
// Vertex Shader functions
// -------------------------------------------------------------
// Common PS 2.0 and PS 3.0 function and intermediary data provider
VS_COMMON_OUTPUT VS_Common(VS_INPUT IN, out VS_INTERMEDIARY_DATA interData, out float4 clipPosition : POSITION) {
	VS_COMMON_OUTPUT commonOut;
	
	// Basic transformation of untransformed vertex into clip-space
	clipPosition = mul(IN.position, matWorldViewProj);
	
	// Vertex fog
	float dist = clipPosition.z;
	commonOut.fog = (fogType == FOG_TYPE_NONE) +
			1 / exp(dist * fogDensity) * (fogType == FOG_TYPE_EXP) +
			1 / exp(pow(dist * fogDensity, 2)) * (fogType == FOG_TYPE_EXP2) +
			saturate((fogEnd - dist) / (fogEnd - fogStart)) * (fogType == FOG_TYPE_LINEAR);	
	
	// Texture coords
	commonOut.texCoord = IN.texCoord;
	
	// Tangent-space matrices
	float3x3 TbnMatrix = { IN.tangent, IN.binormal, IN.normal };
	interData.worldItToTangentSpace = mul(TbnMatrix, matWorldIT);
	float3x3 worldToTangentSpace = mul(TbnMatrix, (float4x3)matWorld);
	
	// View vector
	interData.worldPos = mul(IN.position, matWorld).xyz;
	commonOut.view = mul(worldToTangentSpace, viewPosition - interData.worldPos);
	
	// Lights vectors
	commonOut.dirLight = mul(interData.worldItToTangentSpace, -dirLightDir);
		
	// Vertex color for ambient occlusion
	commonOut.ambientOccl = IN.color.r;
	
	return commonOut;
}

// VS 3.0 function
VS3_OUTPUT VS3(VS_INPUT IN, out VS_COMMON_OUTPUT commonOut, out float4 clipPosition : POSITION) {
	VS3_OUTPUT outVS3;
	VS_INTERMEDIARY_DATA interData;
	
	commonOut = VS_Common(IN, interData, clipPosition);

	// Full-blown 4 point lights in SM3, it doesn't matter with dynamic branching
	outVS3.pointLight[0] = mul(interData.worldItToTangentSpace, pointLight0Pos - interData.worldPos);
	outVS3.pointLight[1] = mul(interData.worldItToTangentSpace, pointLight1Pos - interData.worldPos);
	outVS3.pointLight[2] = mul(interData.worldItToTangentSpace, pointLight2Pos - interData.worldPos);
	outVS3.pointLight[3] = mul(interData.worldItToTangentSpace, pointLight3Pos - interData.worldPos);
	
	return outVS3;
}

// VS 2.0 function
VS2_OUTPUT VS2(VS_INPUT IN, out VS_COMMON_OUTPUT commonOut, out float4 clipPosition : POSITION) {
	VS2_OUTPUT outVS2;
	VS_INTERMEDIARY_DATA interData;
	
	commonOut = VS_Common(IN, interData, clipPosition);

	// Only 2 point lights (per pass?) in SM2.0 and 2.x
	outVS2.pointLight[0] = mul(interData.worldItToTangentSpace, pointLight0Pos - interData.worldPos);
	outVS2.pointLight[1] = mul(interData.worldItToTangentSpace, pointLight1Pos - interData.worldPos);
	
	return outVS2;
}

// -------------------------------------------------------------
// Pixel Shader functions
// -------------------------------------------------------------
// Offset/parallax bump-mapping function
float2 ParallaxTexCoord(float2 oldCoord, float2 vecViewXY) {
	float level = tex2D(sampHeight, oldCoord).r;
	return (level * PARALLAX_AMOUNT - HALF_PARALLAX_AMOUNT) * vecViewXY + oldCoord;
}

// Common PS 2.0 and PS 3.0 function
PS_INTERMEDIARY_DATA PreparePSData(PS_COMMON_INPUT IN) {
	PS_INTERMEDIARY_DATA interData;

	// Normalize view vector per-pixel
    interData.view = normalize(IN.view);
       
    // Get offset texture coords
	interData.texCoord = ParallaxTexCoord(IN.texCoord, interData.view.xy);    
    
    // Get tangent-space normal and specular mask from map
    float4 sampledNormal = tex2D(sampNormal, interData.texCoord);
	interData.normal = sampledNormal.rgb * 2 - 1;
	interData.specularMask = sampledNormal.a;
	
	interData.diffSpecOcclusion = (1 - diffuseOcclusionFactor * (1 - IN.ambientOccl));	
	
	return interData;
}

// Lambertian reflectance
float Lambert(float3 normal, float3 light)
{
	return saturate(dot(normal, light));
}

// Lyon/Blinn halfway method for k = 2 and D = H* – N with Lambertian diffuse reflection
void Lyon(float3 normal, float3 view, float3 light, out float diffuse, out float specular)
{					
	// Diffuse contribution
	diffuse = Lambert(normal, light);
	
	// Specular contribution using half vector
	float3 halfway = normalize(light + view);
	float3 difference = halfway - normal;
	float halfMaterialPower = materialPower / 2;
	float xs = saturate(dot(difference, difference) * halfMaterialPower);
	specular = pow(1 - xs, 3);
}

// Calculate the final pixel color using all calculated components
float4 FinalizeColor(float ambientOcclusion, float diffSpecOcclusion, float3 diffuse, float3 specular, float2 texCoord, float specularMask)
{
	// Constant color contributions
	float4 albedo = tex2D(sampTexture, texCoord);
	float3 emissive = materialEmissive * tex2D(sampEmissive, texCoord).rgb;
	float3 ambient = materialAmbient * ambientOcclusion;
    			
    // Mask components		
	diffuse *= materialDiffuse.rgb * diffSpecOcclusion;    
	specular *= materialSpecular * specularMask * diffSpecOcclusion;
	
	// Pixel alpha
	float alpha = albedo.a * materialDiffuse.a;
   
	// Compute output color and return it
	float3 color = saturate(ambient + diffuse) * albedo + specular + emissive;     	  
    return float4(color, alpha);
}

// PS 3.0 Function
float4 PS3(PS_COMMON_INPUT commonIn, PS3_INPUT ps3In) : COLOR {
	PS_INTERMEDIARY_DATA interData = PreparePSData(commonIn);
	
	float3 diffuse, specular;
	float diffuseContribution, specularContribution;
	
	// Directional light
    Lyon(interData.normal, interData.view, normalize(commonIn.dirLight), diffuseContribution, specularContribution);
	diffuse = diffuseContribution * dirLightColor;
	specular = specularContribution * dirLightColor;
	
	// Point lights	
	float4x3 pointLightsColor = {
		pointLight0Color, pointLight1Color, pointLight2Color, pointLight3Color
	};
	
	for(int i=0; i<pointLightsCount; i++)
	{
		Lyon(interData.normal, interData.view, normalize(ps3In.pointLight[i]), diffuseContribution, specularContribution);
		diffuse += diffuseContribution * pointLightsColor[i];
		specular += specularContribution * pointLightsColor[i];	
	}
	
	return FinalizeColor(commonIn.ambientOccl, interData.diffSpecOcclusion,
						 diffuse, specular, interData.texCoord, interData.specularMask);	
}

// PS 2.0 Function
float4 PS2(PS_COMMON_INPUT commonIn, PS2_INPUT ps2In) : COLOR {
	PS_INTERMEDIARY_DATA interData = PreparePSData(commonIn);

	float2x3 diffuseSpecular;
    float2x3 diffSpecContribs;
	float3x3 lightColors = { dirLightColor,
		pointLight0Color * (pointLightsCount > 0), 
		pointLight1Color * (pointLightsCount > 1)
	};
	       
	// Cram them all in a matrix
	Lyon(interData.normal, interData.view, normalize(commonIn.dirLight), diffSpecContribs._m00, diffSpecContribs._m10);
	Lyon(interData.normal, interData.view, normalize(ps2In.pointLight[0]), diffSpecContribs._m01, diffSpecContribs._m11);
	Lyon(interData.normal, interData.view, normalize(ps2In.pointLight[1]), diffSpecContribs._m02, diffSpecContribs._m12);
    
    // And multiply it with the colors matrix
    diffuseSpecular = mul(diffSpecContribs, lightColors);
    
	return FinalizeColor(commonIn.ambientOccl, interData.diffSpecOcclusion,
						 diffuseSpecular[0], diffuseSpecular[1], interData.texCoord, interData.specularMask);		
}

// -------------------------------------------------------------
// Techniques
// -------------------------------------------------------------
technique TSM3 {
    pass P {
		VertexShader = compile vs_3_0 VS3();
		PixelShader  = compile ps_3_0 PS3();    
    }
}

technique TSM2a {
    pass P {  
		VertexShader = compile vs_2_0 VS2();
		PixelShader  = compile ps_2_a PS2();		
    }
}

technique TSM2b {
    pass P {  
		VertexShader = compile vs_2_0 VS2();
		PixelShader  = compile ps_2_b PS2();		
    }
}

technique TSM2 {
	pass P {
		VertexShader = compile vs_2_0 VS2();
		PixelShader  = compile ps_2_0 PS2();    
	}
}
