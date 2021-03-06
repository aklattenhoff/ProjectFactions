/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "common/def.glsl"
#include "common/utils.glsl"

layout(binding=0) uniform sampler2D uInputTex;
layout(binding=1) uniform sampler2D uDepthTex;
layout(binding=2) uniform sampler2D uAdaptedLuminanceTex;

uniform float uExposure = 1.0;

const int MAX_MOTION_SAMPLES = 64;
uniform float uMotionScale = 1.0; // current fps / target fps
uniform mat4 uInverseViewProjectionMatrix;
uniform mat4 uPrevViewProjectionMatrix;

noperspective in vec2 vTexcoord;

layout(location=0) out vec4 fResult;

/*----------------------------------------------------------------------------*/
vec4 motionBlur() {
	float depth = texture(uDepthTex, vTexcoord).r;
	vec4 cpos = vec4(vTexcoord * 2.0 - 1.0, depth, 1.0);
	cpos = uInverseViewProjectionMatrix * cpos;
	vec4 ppos = uPrevViewProjectionMatrix * cpos;
	ppos.xyz /= ppos.w;
	ppos.xy = ppos.xy * 0.5 + 0.5;
	
	vec2 velocity = (ppos.xy - vTexcoord) * uMotionScale;
	
	vec2 texelSize = 1.0 / vec2(textureSize(uInputTex, 0));
	
	float tspeed = length(velocity / texelSize); // speed in pixels/second
	int nSamples = clamp(int(tspeed), 1, MAX_MOTION_SAMPLES);

	vec4 result = texture(uInputTex, vTexcoord);
	for (int i = 1; i < nSamples; ++i) {
		vec2 offset = vTexcoord + velocity * (float(i) / float(nSamples - 1) - 0.5);
		result += texture(uInputTex, offset);
	}
	return result / float(nSamples);
}

/*----------------------------------------------------------------------------*/
/*	Grzegorz Krawczyk's dynamic key function. */
float autokey(in float lum) {
	return 1.03 - 2.0 / (2.0 + log10(lum + 1.0));
}

/*----------------------------------------------------------------------------*/
/*	Jim Hejl's ALU filmic tonemapping. This incorporates the output gamma 
	correction. */
vec4 hejl(in vec4 color) {
	vec4 x = max(vec4(0.0), color - vec4(0.004));
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

/*----------------------------------------------------------------------------*/
void main() {
	float vignette = vec2(1.0) - length(vec2(0.5) - vTexcoord) / length(vec2(0.5));
	vignette = pow(vignette, 0.4);
	
	vec4 result = motionBlur();

	float adaptedLum = texture(uAdaptedLuminanceTex, vec2(0.5)).r;
	float exposure = uExposure * autokey(adaptedLum) / adaptedLum;
	exposure *= vignette;
	result *= exposure;		
	
	fResult = hejl(result);
	
	//fResult = hejl(texture(uInputTex, vTexcoord));
}