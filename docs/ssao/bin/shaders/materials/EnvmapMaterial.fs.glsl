/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "../common/def.glsl"

layout(binding=0) uniform samplerCube uEnvTex;
layout(binding=4) uniform sampler2D uDepthTex;

uniform mat4 uInverseViewMatrix;
#ifdef VELOCITY_BUFFER
	uniform mat4 uInverseViewProjectionMatrix;
	uniform mat4 uPreviousViewProjectionMatrix;
#endif

in VertexDataT {
	smooth vec3 position; // view space position
	smooth vec2 texcoord; // copied through from aTexcoord
	smooth mat3 tangentViewMatrix; // tangent->view space matrix
	noperspective vec3 viewRay; // ray to far plane (use linear interpolation)
#ifdef VELOCITY_BUFFER
	smooth vec4 projPosition; // projected position
	smooth vec4 prevProjPosition; // previous projected position
#endif
} vertexIn;

layout(location=0) out vec4 fResult;

/*----------------------------------------------------------------------------*/
void main() {
	float depth = texture(uDepthTex, vertexIn.texcoord).r;
	if (depth >= 1.0) {
		vec3 texcoord = (uInverseViewMatrix * vec4(vertexIn.viewRay, 0.0)).xyz;	
		fResult = textureLod(uEnvTex, texcoord, 0);
	} else {
		fResult = vec4(0.0);
	}
}