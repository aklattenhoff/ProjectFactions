/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#ifndef NORMALS_GLSL_
#define NORMALS_GLSL_

//#define NORMAL_ENCODE_SPHEREMAP
#define NORMAL_ENCODE_UINT

/*----------------------------------------------------------------------------*/
vec3 encodeNormal(in vec3 normal) {
	vec3 result;

#ifdef NORMAL_ENCODE_SPHEREMAP
	result.xy = normalize(normal.xy) * sqrt(normal.z * 0.5 + 0.5);
	result.z = 0.0;
#else
	result = normal;
#endif
	
#ifdef NORMAL_ENCODE_UINT
	result = result * 0.5 + 0.5;
#endif
	return result;
}

/*----------------------------------------------------------------------------*/
vec3 decodeNormal(in vec3 normal) {
	vec3 n = normal;
#ifdef NORMAL_ENCODE_UINT
	n = n * 2.0 - 1.0;
#endif

	vec3 result = n;
#ifdef NORMAL_ENCODE_SPHEREMAP
	result.z = (n.x * n.x + n.y * n.y) * 2.0 - 1.0;
	result.xy = normalize(n.xy) * sqrt(1.0 - result.z * result.z);
#else
	result = n;
#endif

	return result;
}


#endif // NORMALS_GLSL_