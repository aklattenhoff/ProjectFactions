/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#ifndef DEF_GLSL_
#define DEF_GLSL_

#version 420

#define VELOCITY_BUFFER
#define VELOCITY_BUFFER_UINT
#ifdef VELOCITY_BUFFER_UINT
	const float kVelocityPower = 3.0;
#endif

#endif // DEF_GLSL_