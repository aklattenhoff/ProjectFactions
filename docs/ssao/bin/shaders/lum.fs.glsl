/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "common/def.glsl"
#include "common/utils.glsl"
	
layout(binding=0) uniform sampler2D uInputTex;

noperspective in vec2 vTexcoord;

layout(location=0) out vec4 fResult;

/*----------------------------------------------------------------------------*/
void main() {
	vec3 color = texture(uInputTex, vTexcoord).rgb;
	fResult = vec4(luminance(color));
}
