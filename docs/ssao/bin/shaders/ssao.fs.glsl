/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "common/def.glsl"
#include "common/utils.glsl"
#include "common/normals.glsl"

layout(binding=0) uniform sampler2D uGBufferGeometricTex;
layout(binding=1) uniform sampler2D uGBufferDepthTex;
layout(binding=2) uniform sampler2D uNoiseTex;

uniform mat4 uProjectionMatrix; // current projection matrix, for linearized depth

//	ssao uniforms:
const int MAX_KERNEL_SIZE = 128;
uniform int uKernelSize;
uniform vec3 uKernelOffsets[MAX_KERNEL_SIZE];
uniform float uRadius = 1.5;
uniform float uPower = 2.0;

in VertexDataT {
	smooth vec3 position;
	smooth vec2 texcoord; // required
	smooth mat3 tangentViewMatrix;
	noperspective vec3 viewRay; // required
#ifdef VELOCITY_BUFFER
	smooth vec4 projPosition;
	smooth vec4 prevProjPosition;
#endif
} vertexIn;

layout(location=0) out vec4 fResult;

/*----------------------------------------------------------------------------*/
float ssao(in mat3 kernelBasis, in vec3 originPos, in float radius) {
	float occlusion = 0.0;
	for (int i = 0; i < uKernelSize; ++i) {
	//	get sample position:
		vec3 samplePos = kernelBasis * uKernelOffsets[i];
		samplePos = samplePos * radius + originPos;
		
	//	project sample position:
		vec4 offset = uProjectionMatrix * vec4(samplePos, 1.0);
		offset.xy /= offset.w; // only need xy
		offset.xy = offset.xy * 0.5 + 0.5; // scale/bias to texcoords
		
	//	get sample depth:
		float sampleDepth = texture(uGBufferDepthTex, offset.xy).r;
		sampleDepth = linearizeDepth(sampleDepth, uProjectionMatrix);
		
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(originPos.z - sampleDepth));
		occlusion += rangeCheck * step(sampleDepth, samplePos.z);
	}
	
	occlusion = 1.0 - (occlusion / float(uKernelSize));
	return pow(occlusion, uPower);
}

/*----------------------------------------------------------------------------*/
void main() {
//	get noise texture coords:
	vec2 noiseTexCoords = vec2(textureSize(uGBufferDepthTex, 0)) / vec2(textureSize(uNoiseTex, 0));
	noiseTexCoords *= vertexIn.texcoord;
	
//	get view space origin:
	float originDepth = texture(uGBufferDepthTex, vertexIn.texcoord).r;
	originDepth = linearizeDepth(originDepth, uProjectionMatrix);
	vec3 originPos = vertexIn.viewRay * originDepth;

//	get view space normal:
	vec3 normal = decodeNormal(texture(uGBufferGeometricTex, vertexIn.texcoord).rgb);
		
//	construct kernel basis matrix:
	vec3 rvec = texture(uNoiseTex, noiseTexCoords).rgb * 2.0 - 1.0;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 kernelBasis = mat3(tangent, bitangent, normal);
	
	fResult = vec4(ssao(kernelBasis, originPos, uRadius));
}