/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
/*	Visualizing the SSAO result (extracts ao factor from gbuffer, outputs as
	a vec4). */

#include "common/def.glsl"

layout(binding=0) uniform sampler2D uGBufferGeometricTex;

noperspective in vec2 vTexcoord;

layout(location=0) out vec4 fResult;

/*----------------------------------------------------------------------------*/
void main() {
	fResult = vec4(texture(uGBufferGeometricTex, vTexcoord).a);
}