/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "../common/def.glsl"

uniform float uTanHalfFov;
uniform float uAspectRatio;

layout(location=0) in vec4 aPosition;

out VertexDataT {
	smooth vec3 position; // view space position
	smooth vec2 texcoord; // copied through from aTexcoord
	smooth mat3 tangentViewMatrix; // tangent->view space matrix
	noperspective vec3 viewRay; // ray to far plane (use linear interpolation)
#ifdef VELOCITY_BUFFER
	smooth vec4 projPosition; // projected position
	smooth vec4 prevProjPosition; // previous projected position
#endif
} vertexOut;

/*----------------------------------------------------------------------------*/
void main() {
//	view space position/texcoord:
	vertexOut.position = aPosition.xyz;
	vertexOut.texcoord = aPosition.xy * 0.5 + 0.5;
		
//	tangent->view space matrix:
	vec3 normal = vec3(0.0, 0.0, -1.0);
	vec3 tangent = vec3(1.0, 0.0, 0.0);
	vec3 bitangent = vec3(0.0, 1.0, 0.0);
	vertexOut.tangentViewMatrix = mat3(tangent, bitangent, normal);
	
//	view ray:
	vertexOut.viewRay = vec3(
		aPosition.x * uTanHalfFov * uAspectRatio,
		aPosition.y * uTanHalfFov, 
		1.0
	);
	
//	projected position:
	gl_Position = aPosition;
	
#ifdef VELOCITY_BUFFER
	vertexOut.projPosition = gl_Position;
	vertexOut.prevProjPosition = gl_Position; // never varies for SA quads
#endif
}