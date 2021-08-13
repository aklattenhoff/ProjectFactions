/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

/*	Performs image-based lighting using the following model:

		intensity = lerp(Kd * Ld + Ks * Ls, Kd * Ls, Km)
	
	Where Kd is the diffuse reflectance of the material (and the specular 
	reflectance value for metals), Ks is the specular reflectance for non-metals,
	Ld/Ls are the diffuse/specular intensities of the incident light. Km is the 
	material's 'metallic' property, which blends between the metal/non-metal
	results.
	
	Ld is always taken from the highest mip level of the cubemap. For Ls, the
	mip level is chosen based on the material's specular exponent, Ke:
	
		lod = (1 - Ke) ^ 1/2 * lodMax
		
	Where lodMax is the highest mip level of the cubemap. Because each mip level
	is a power of two smaller than the preceeding level, resolution of the mip
	chain is not distributed linearly, hence 'linearize' the lod by raising to
	the power 1/2.
*/

#include "../common/def.glsl"

layout(binding=0) uniform samplerCube uEnvTex;
uniform float uEnvTexMaxLod = 8.0; // max lod for uEnvTex

//	surface textures:
layout(binding=1) uniform sampler2D uDiffuseTex;
layout(binding=2) uniform sampler2D uNormalTex;
layout(binding=3) uniform sampler2D uDetailTex;
layout(binding=4) uniform sampler2D uSpecularTex; // r = level, g = exponent, b = metallic

//	surface properties:
uniform vec3 uDiffuseColor = vec3(1.0);
uniform float uSpecularLevel = 1.0;
uniform float uSpecularExponent = 1.0;
uniform float uMetallicLevel = 1.0;
uniform vec2 uDetailTexScale = vec2(1.0);

uniform mat4 uInverseViewMatrix; // for view->world space conversion

in VertexDataT {
	smooth vec3 position; // view space position
	smooth vec2 texcoord; // copied through from aTexcoord
	smooth mat3 tangentViewMatrix; // tangent->view space matrix
	noperspective vec3 viewRay; // ray to far plane (use linear interpolation)
} vertexIn;
	
layout(location=0) out vec4 fResult;

/*----------------------------------------------------------------------------*/
void main() {
//	get surface properties:
	vec4 kd = texture(uDiffuseTex, vertexIn.texcoord) * vec4(uDiffuseColor, 1.0);

	vec3 n = texture(uNormalTex, vertexIn.texcoord).xyz * 2.0 - 1.0;
	n += texture(uDetailTex, vertexIn.texcoord * uDetailTexScale).xyz * 2.0 - 1.0;
	n = normalize(vertexIn.tangentViewMatrix * n);
	
	vec4 specular = texture(uSpecularTex, vertexIn.texcoord) * vec4(uSpecularLevel, uSpecularExponent, uMetallicLevel, 0.0);
	float ks = specular.r;
	float ke = specular.g;
	float km = specular.b;
	
//	get light properties:	
	vec4 ld = textureLod(uEnvTex, (uInverseViewMatrix * vec4(n, 0.0)).xyz, uEnvTexMaxLod);
	
	vec3 r = reflect(vertexIn.position, n);
	float lod = pow(1.0 - ke, 0.5); // linearize, since mips aren't linearly spaced	
	vec4 ls = textureLod(uEnvTex, (uInverseViewMatrix * vec4(r, 0.0)).xyz, uEnvTexMaxLod * lod);
	
//	compute final radiance:
	vec4 intensity = mix(kd * ld + ks * ls, ks * kd * ls, km);
	
	fResult = intensity;
}