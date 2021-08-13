/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "common/def.glsl"

layout(binding=0) uniform sampler2D uLuminanceTex;
layout(binding=1) uniform sampler2D uPrevLuminanceTex; // previous adapted luminance

uniform float uAdaptionRate = 1.0;

layout(location=0) out float fResult;

/*----------------------------------------------------------------------------*/
void main() {
	float avgLum = textureLod(uLuminanceTex, vec2(0.5), 9999).r;
	float prevLum = texture(uPrevLuminanceTex, vec2(0.5));
	float adaptedLum = prevLum + (avgLum - prevLum) * (1.0 - exp2(-uAdaptionRate));
	fResult = adaptedLum;
}