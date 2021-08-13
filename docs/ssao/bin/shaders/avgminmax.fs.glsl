/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
/*	Finds the average,min,max of a 4 pixel neighbourhood and writes to r,g,b.
	MUST use GL_NEAREST_MIPMAP_NEAREST to avoid intermediate filtering between
	mip levels.
*/

#include "common/def.glsl"
	
layout(binding=0) uniform sampler2D uInputTex;

//uniform int uCurrentMipLevel = 0;

noperspective in vec2 vTexcoord;

layout(location=0) out vec3 fResult; // r = average, g = min, b = max

/*----------------------------------------------------------------------------*/
void main() {
	const vec2 kSampleTexcoord[4] = {
		vec2(-1.0, -1.0),
		vec2(1.0, -1.0),
		vec2(1.0, 1.0),
		vec2(-1.0, 1.0)
	};
	
	vec2 texelSize = 1.0 / vec2(textureSize(uInputTex, 0));
	
	vec3 result = vec3(texture(uInputTex, vTexcoord + kSampleTexcoord[0] * texelSize).r);
	for (int i = 1; i < 4; ++i) {
		vec3 s = texture(uInputTex, vTexcoord + kSampleTexcoord[i] * texelSize).rgb;
		result.r += s.r;
		
		result.g = min(result.g, s.g);
		result.b = max(result.b, s.b);
	}
	result.r /= 4.0;
	
	fResult = result;
}
