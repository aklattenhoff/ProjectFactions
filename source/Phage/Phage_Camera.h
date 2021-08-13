#ifndef _PHAGE_CAMERA_H
#define _PHAGE_CAMERA_H

#include <stdlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage_Math.h"
#include "Phage_Matrix.h"

namespace Phage
{
	class Matrix;
	class PhageCamera
	{
	public:
		PhageCamera();
		~PhageCamera();

		int Init();

		int SetScreenResolution(float w, float h);
		glm::vec2 GetScreenResolution();
		int SetPosition(float x, float y, float z);
		int MovePosition(float x, float y, float z);
		glm::vec3 GetPosition();
		int SetAngle(float a);
		float GetAngle();
		int SetRotation(float r);
		float GetRotation();

		int Set2DMousePosition(float x, float y);
		glm::vec2 GetMouseScreen();
		glm::vec2 GetMouseNormalized();
		glm::vec3 GetMouseNear();
		glm::vec3 GetMouseFar();
		glm::vec3 GetMouseZero();

		glm::vec4 Calculate2DPoint(float x, float y, float z);
		glm::vec4 Calculate3DPoint(float x, float y, float depth);
		glm::vec4 Calculate3DPointFromScreenPos(float x, float y, float depth);
		glm::vec3 Calculate3DPointAtZero(float x, float y);

		Phage::Matrix GetMatrix();
		Phage::Matrix GetPerspectiveMatrix();
		Phage::Matrix GetOrthroMatrix();
	private:
		glm::vec2 _screenResolution;
		float _cameraAngle;
		float _cameraRotation;

		glm::vec3 _cameraPosition;

		Phage::Matrix _projectionMatrix;
		Phage::Matrix _orthoMatrix;

		Phage::Matrix _matrix;

		glm::vec2 _mouseScreen;
		glm::vec2 _mouseNormalized;
		glm::vec4 _mouseNear4;
		glm::vec3 _mouseNear3;
		glm::vec4 _mouseFar4;
		glm::vec3 _mouseFar3;
		glm::vec3 _mouseZero;
	};
}

#endif
