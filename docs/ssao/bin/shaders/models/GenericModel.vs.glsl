/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "../common/def.glsl"

uniform mat4 uModelViewMatrix;
uniform mat4 uModelViewProjectionMatrix;
#ifdef VELOCITY_BUFFER
	uniform mat4 uPrevModelViewProjectionMatrix;
#endif
uniform float uTanHalfFov;
uniform float uAspectRatio;

layout(location=0) in vec4 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec3 aTangent;
layout(location=3) in vec2 aTexcoord;

out VertexDataT {
	smooth vec3 viewPosition; // view space position
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
	vertexOut.viewPosition = (uModelViewMatrix * aPosition).xyz;
	vertexOut.texcoord = aTexcoord;
		
//	tangent->view space matrix:
	vec3 normal = normalize((uModelViewMatrix * vec4(aNormal, 0.0)).xyz);
	vec3 tangent = normalize((uModelViewMatrix * vec4(aTangent, 0.0)).xyz);
	vec3 bitangent = cross(tangent, normal);
	vertexOut.tangentViewMatrix = mat3(tangent, bitangent, normal);
	
//	view ray:
	vertexOut.viewRay = vec3(
		vertexOut.viewPosition.x * uTanHalfFov * uAspectRatio,
		vertexOut.viewPosition.y * uTanHalfFov, 
		1.0
	);
	
//	projected position:
	gl_Position = uModelViewProjectionMatrix * aPosition;
	
#ifdef VELOCITY_BUFFER
	vertexOut.projPosition = gl_Position;
	vertexOut.prevProjPosition = uPrevModelViewProjectionMatrix * aPosition;
#endif
}