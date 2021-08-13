/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

/*	Outputs material properties to a gbuffer with the following format:

	GL_RGBA8 (location 1)
	rgb		= diffuse albedo/specular colour
	a			= 
	
	GL_RGBA8 (location 2)
	r			= specular level
	g			= specular exponent
	b			= metallic
	a			= bitmask (for special post processing, e.g. 
	
	GL_RGBA8 (location 3)
	rgb		= view space normal
	a			= local occlusion multiplier
	
	Note that there are numerous options for encoding the view-space normals, a
	few sources suggest using spherical coordinates which is the approach taken
	here (see toSpherical() and toCartesian() below). */

#include "../common/def.glsl"
#include "../common/utils.glsl"
#include "../common/normals.glsl"

//	surface textures:
layout(binding=0) uniform sampler2D uDiffuseTex;
layout(binding=1) uniform sampler2D uNormalTex;
layout(binding=2) uniform sampler2D uDetailTex;
layout(binding=3) uniform sampler2D uSpecularTex; // r = level, g = exponent, b = metallic

//	surface properties:
uniform vec3 uDiffuseColor = vec3(1.0);
uniform float uSpecularLevel = 1.0;
uniform float uSpecularExponent = 1.0;
uniform float uMetallicLevel = 1.0;
uniform vec2 uDetailTexScale = vec2(1.0);

in VertexDataT {
	smooth vec3 position; // view space position
	smooth vec2 texcoord; // copied through from aTexcoord
	smooth mat3 tangentViewMatrix; // tangent->view space matrix
	noperspective vec3 viewRay; // ray to far plane (use linear interpolation)
#ifdef VELOCITY_BUFFER
	smooth vec4 projPosition; // projected position
	smooth vec4 prevProjPosition; // previous projected position
#endif // VELOCITY_BUFFER
} vertexIn;
	
layout(location=0) out vec4 fAlbedoSpecularColor;
layout(location=1) out vec4 fMaterialProperties;
layout(location=2) out vec4 fGeometricProperties;
#ifdef VELOCITY_BUFFER
	layout(location=3) out vec2 fVelocity;
#endif // VELOCITY_BUFFER

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

//	output results:
	fAlbedoSpecularColor = kd;
	fMaterialProperties = vec4(ks, ke, km, 1.0);
	
	fGeometricProperties.rgb = encodeNormal(n);
	fGeometricProperties.a = 1.0;
	
#ifdef VELOCITY_BUFFER
	vec2 a = (vertexIn.projPosition.xy / vertexIn.projPosition.w) * 0.5 + 0.5;
	vec2 b = (vertexIn.prevProjPosition.xy / vertexIn.prevProjPosition.w) * 0.5 + 0.5;
	vec2 velocity = a - b;
	#ifdef VELOCITY_BUFFER_UINT
		velocity = velocity * 0.5 + 0.5;
		velocity = powv2(velocity, kVelocityPower);
	#endif
	fVelocity = velocity;
#endif
}