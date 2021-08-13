/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "../common/def.glsl"

in VertexDataT {
	smooth vec3 position; // view space position
	smooth vec2 texcoord; // copied through from aTexcoord
	smooth mat3 tangentViewMatrix; // tangent->view space matrix
	noperspective vec3 viewRay; // ray to far plane (use linear interpolation)
} vertexIn;
	
layout(location=0) out float fDepth;

/*----------------------------------------------------------------------------*/
void main() {
	fDepth = gl_FragCoord.z;
}