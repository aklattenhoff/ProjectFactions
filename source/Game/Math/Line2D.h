#ifndef _LINE_2D_H
#define _LINE_2D_H

#include <string>
#include <stack>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

class Line2D
{
public:
	Line2D();
	~Line2D();

	//Creates and stores a line equation, given two points on the line
	//This also sets the line segment based on these two points
	//It can be changed by calling the SetLineSegment function
	int CreateLineFromTwoPoints(float x1, float y1, float x2, float y2);

	//Sets the new line segment for the given line
	//NOTE: This does not change the line equation for the line, and the points don't have to be on the line
	//This only changes the bounding box that will be used to compute the active line segment
	//If the points are not on the line it will behave like this:
	//   \
	// p1-\------  ---
	//  |  \    |   |
	//  |   \   |  CalculateX will only use this much of the segment
	//  |    \  |   |
	//  ------\-p2 ---
	//         \
	//  |-------|
	//  CalculateY will use all of this segment
	int SetLineSegment(float x1, float y1, float x2, float y2);

	//Compute the x value, given a y value for an infinite line
	float CalculateX(float y);
	//Compute the y value, given a x value for an infinite line
	float CalculateY(float x);

	//Compute the x value, given a y value for the line segment
	//Returns false if the y value does not intersect with the line segment
	bool CalculateXFromSegment(float y, float* returnX);
	//Compute the y value, given a x value for the line segment
	//Returns false if the x value does not intersect with the line segment
	bool CalculateYFromSegment(float x, float* returnY);
private:
	float m;
	float b;

	glm::vec2 point1;
	glm::vec2 point2;
};
#endif
