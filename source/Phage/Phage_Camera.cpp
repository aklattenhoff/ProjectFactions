#include "Phage_Camera.h"

//#include "Graphics/Camera.h"

Phage::PhageCamera::PhageCamera()
{
	//_camera = NULL;
}

Phage::PhageCamera::~PhageCamera()
{
	//delete _camera;
}

int Phage::PhageCamera::Init()
{
	/*_camera = new Camera();
	_camera->CreateCamera();
	_camera->SetScreenResolution(glm::vec2(800, 600));
	_camera->setCameraPosition(0.0f, 0.0f, 0.0f);*/

	_projectionMatrix.Init();
	_projectionMatrix.Perspective(25.0f, 800.0f / 600.0f, 0.1f, 2000.0f);

	_orthoMatrix.Init();
	_orthoMatrix.Ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.1f, 2000.0f);

	_matrix.Init();
	_matrix.Identity();

	_cameraAngle = 55.0f;
	_cameraRotation = 0.0f;

	return 0;
}

int Phage::PhageCamera::SetScreenResolution(float w, float h)
{
	_screenResolution.x = w;
	_screenResolution.y = h;

	_projectionMatrix.Perspective(25.0f, w / h, 0.1f, 2000.0f);

	_orthoMatrix.Ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

	return 0;
}

glm::vec2 Phage::PhageCamera::GetScreenResolution()
{
	return _screenResolution;
}

int Phage::PhageCamera::SetPosition(float x, float y, float z)
{
	//if (!_camera)
		//return -1;

	//_camera->setCameraPosition(x, y, z);

	_cameraPosition.x = x;
	_cameraPosition.y = y;
	_cameraPosition.z = z;

	_matrix.SetTranslate(-x, -y, -z);

	return 0;
}

int Phage::PhageCamera::MovePosition(float x, float y, float z)
{
	//glm::vec3 camPos = _camera->getCameraPosition();
	//_camera->setCameraPosition(camPos.x + x, camPos.y + y, camPos.z + z);

	_cameraPosition.x += x;
	_cameraPosition.y += y;
	_cameraPosition.z += z;

	_matrix.Translate(-x, -y, -z);

	return 0;
}

glm::vec3 Phage::PhageCamera::GetPosition()
{
	//if (!_camera)
		//return Phage::vec3();

	//glm::vec3 glmVec = _camera->getCameraPosition();

	//return Phage::vec3(glmVec.x, glmVec.y, glmVec.z);
	return _cameraPosition;
}

int Phage::PhageCamera::SetAngle(float a)
{
	_cameraAngle = a;

	_matrix.SetRotate(_cameraAngle, 1.0f, 0.0f, 0.0f);

	return 0;
}

float Phage::PhageCamera::GetAngle()
{
	return _cameraAngle;
}

int Phage::PhageCamera::SetRotation(float r)
{
	_cameraRotation = r;

	_matrix.SetRotate(_cameraAngle, 1.0f, 0.0f, 0.0f);
	_matrix.Rotate(r, 0.0f, 1.0f, 0.0f);

	return 0;
}

float Phage::PhageCamera::GetRotation()
{
	return _cameraRotation;
}

int Phage::PhageCamera::Set2DMousePosition(float x, float y)
{
	_mouseScreen = glm::vec2(x, y);
	_mouseNormalized.x = (x-(_screenResolution.x/2.0f))/(_screenResolution.x/2.0f);
	_mouseNormalized.y = (y-(_screenResolution.y/2.0f))/(_screenResolution.y/2.0f);

	_mouseNear4 = Calculate3DPoint(_mouseNormalized.x, -_mouseNormalized.y, -1.0f);
	_mouseNear4 = _mouseNear4/_mouseNear4[3];
	_mouseNear3 = glm::vec3(_mouseNear4);

	_mouseFar4 = Calculate3DPoint(_mouseNormalized.x, -_mouseNormalized.y, 1.0f);
	_mouseFar4 = _mouseFar4/_mouseFar4[3];
	_mouseFar3 = glm::vec3(_mouseFar4);

	float x1 = _mouseNear3.x;
	float y1 = _mouseNear3.y;
	float z1 = _mouseNear3.z;
	float x2 = _mouseFar3.x;
	float y2 = _mouseFar3.y;
	float z2 = _mouseFar3.z;

	_mouseZero.x = (((x2 - x1)*y1) / (y1 - y2) + _cameraPosition.x);
	_mouseZero.y = 0.0f;
	_mouseZero.z = (((z2 - z1)*y1) / (y1 - y2) + _cameraPosition.z);

	return 0;
}

glm::vec2 Phage::PhageCamera::GetMouseScreen()
{
	return _mouseScreen;
}

glm::vec2 Phage::PhageCamera::GetMouseNormalized()
{
	return _mouseNormalized;
}

glm::vec3 Phage::PhageCamera::GetMouseNear()
{
	return _mouseNear3;
}

glm::vec3 Phage::PhageCamera::GetMouseFar()
{
	return _mouseFar3;
}

glm::vec3 Phage::PhageCamera::GetMouseZero()
{
	return _mouseZero;
}

glm::vec4 Phage::PhageCamera::Calculate2DPoint(float x, float y, float z)
{
	glm::vec4 returnVec = (_projectionMatrix.GetMatrix(0) * _matrix.GetMatrix(0)) * glm::vec4(x, y, z, 1.0f);
	returnVec = returnVec / returnVec[3];

	returnVec.x = ((returnVec.x + 1.0f) / 2.0f) * _screenResolution.x;
	returnVec.y = ((1.0f - returnVec.y) / 2.0f) * _screenResolution.y;

	return returnVec;
}

glm::vec4 Phage::PhageCamera::Calculate3DPoint(float x, float y, float depth)
{
	glm::vec4 outVec = glm::inverse((_projectionMatrix.GetMatrix(0) * _matrix.GetMatrix(0))) * glm::vec4(x, y, depth, 1.0f);
	outVec /= outVec[3];

	return outVec;
}

glm::vec4 Phage::PhageCamera::Calculate3DPointFromScreenPos(float x, float y, float depth)
{
	glm::vec2 pointNormal;
	pointNormal.x = (x - (_screenResolution.x / 2.0f)) / (_screenResolution.x / 2.0f);
	pointNormal.y = (y - (_screenResolution.y / 2.0f)) / (_screenResolution.y / 2.0f);

	glm::vec4 point;
	point = Calculate3DPoint(pointNormal.x, -pointNormal.y, depth);
	//point = point / point[3];

	return point;
}

glm::vec3 Phage::PhageCamera::Calculate3DPointAtZero(float x, float y)
{
	glm::vec2 pointNormal;
	pointNormal.x = (x-(_screenResolution.x/2.0f))/(_screenResolution.x/2.0f);
	pointNormal.y = (y-(_screenResolution.y/2.0f))/(_screenResolution.y/2.0f);

	glm::vec4 near4;
	near4 = Calculate3DPoint(pointNormal.x, -pointNormal.y, -1.0f);
	//near4 = near4 / near4[3];
	glm::vec3 near3 = glm::vec3(near4);

	glm::vec4 far4;
	far4 = Calculate3DPoint(pointNormal.x, -pointNormal.y, 1.0f);
	//far4 = far4 / far4[3];
	glm::vec3 far3 = glm::vec3(far4);

	float x1 = near3.x;
	float y1 = near3.y;
	float z1 = near3.z;
	float x2 = far3.x;
	float y2 = far3.y;
	float z2 = far3.z;

	glm::vec3 pointZero;
	pointZero.x = (((x2 - x1)*y1) / (y1 - y2) + _cameraPosition.x);
	pointZero.y = 0.0f;
	pointZero.z = (((z2 - z1)*y1) / (y1 - y2) + _cameraPosition.z);

	return pointZero;
}

Phage::Matrix Phage::PhageCamera::GetMatrix()
{
	return _matrix;
}

Phage::Matrix Phage::PhageCamera::GetPerspectiveMatrix()
{
	return _projectionMatrix;
}

Phage::Matrix Phage::PhageCamera::GetOrthroMatrix()
{
	return _orthoMatrix;
}
