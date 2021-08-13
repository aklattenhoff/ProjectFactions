#include "LOSMap.h"

LOSMap::LOSMap()
{
	fogOfWarMap = NULL;
	discoveryMap = NULL;

	guiIcon = NULL;
	guiScreen = NULL;
}

LOSMap::~LOSMap()
{
	delete[] fogOfWarMap;
	delete[] discoveryMap;

	delete guiIcon;
	delete guiScreen;
}

int LOSMap::Init(int cX, int cY, int resolutionPerCell)
{
	DebugOutput("Initializing LOSMAP\n");
	cellX = cX;
	cellY = cY;
	resolution = resolutionPerCell;

	//Fog of war and discovery map
	fogOfWarMap = new bool[cellX*cellY];
	for (int i = 0;i < cellX*cellY;i ++)
		fogOfWarMap[i] = false;

	discoveryMap = new bool[cellX*cellY];
	for (int i = 0;i < cellX*cellY;i ++)
		discoveryMap[i] = false;

	//Framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, cellX*resolution, cellY*resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	circleTexture = TextureLoader_LoadFromFile("textures/circle.png");

	guiIcon = new GUIIcon();
	guiIcon->Init();
	guiIcon->Generate(glm::vec2(12.0f*resolution, 12.0f*resolution));

	guiScreen = new GUIIcon();
	guiScreen->Init();
	guiScreen->Generate(glm::vec2(500.0f, 500.0f));

	return 0;
}

/*int LOSMap::SetLineOfSight(Camera* camera, glm::vec2 center, float radius)
{
	//Set the outer rectangle of the circle
	glm::vec2 outer1 = center - radius;
	glm::vec2 outer2 = center + radius;

	//Clamp points
	if (outer1.x < 0.0f) outer1.x = 0.0f;
	if (outer1.y < 0.0f) outer1.y = 0.0f;
	if (outer2.x >= (float)cellX) outer2.x = (float)cellX - 1.0f;
	if (outer2.y >= (float)cellY) outer2.y = (float)cellY - 1.0f;

	//Calculate the inner rectangle of the circle
	float innerRadius = sqrt(pow(radius, 2.0f) / 2.0f);
	glm::vec2 inner1 = center - innerRadius;
	glm::vec2 inner2 = center + innerRadius;

	//Clamp points
	if (inner1.x < 0.0f) inner1.x = 0.0f;
	if (inner1.y < 0.0f) inner1.y = 0.0f;
	if (inner2.x >= (float)cellX) inner2.x = (float)cellX - 1.0f;
	if (inner2.y >= (float)cellY) inner2.y = (float)cellY - 1.0f;

	//Set all the points for the inner rectangle
	for (int j = (int)inner1.y;j <= (int)inner2.y;j ++)
		for (int i = (int)inner1.x;i <= (int)inner2.x;i ++)
		{
			fogOfWarMap[i + (j*cellX)] = true;
			discoveryMap[i + (j*cellX)] = true;
		}

	//Set all the points outside the inner rectangle and inside the outer recatngle
	//Top rectangle
	for (int j = (int)outer1.y;j <= (int)inner1.y;j ++)
		for (int i = (int)outer1.x;i <= (int)outer2.x;i ++)
		{
			if (sqrt(pow((float)i - center.x, 2.0f) + pow((float)j - center.y, 2.0f)) <= radius)
			{
				fogOfWarMap[i + (j*cellX)] = true;
				discoveryMap[i + (j*cellX)] = true;
			}
		}
	//Bot rectangle
	for (int j = (int)inner2.y;j <= (int)outer2.y;j ++)
		for (int i = (int)outer1.x;i <= (int)outer2.x;i ++)
		{
			if (sqrt(pow((float)i - center.x, 2.0f) + pow((float)j - center.y, 2.0f)) <= radius)
			{
				fogOfWarMap[i + (j*cellX)] = true;
				discoveryMap[i + (j*cellX)] = true;
			}
		}
	//Left rectangle
	for (int j = (int)inner1.y;j <= (int)inner2.y;j ++)
		for (int i = (int)outer1.x;i <= (int)inner1.x;i ++)
		{
			if (sqrt(pow((float)i - center.x, 2.0f) + pow((float)j - center.y, 2.0f)) <= radius)
			{
				fogOfWarMap[i + (j*cellX)] = true;
				discoveryMap[i + (j*cellX)] = true;
			}
		}
	//Right rectangle
	for (int j = (int)inner1.y;j <= (int)inner2.y;j ++)
		for (int i = (int)inner2.x;i <= (int)outer2.x;i ++)
		{
			if (sqrt(pow((float)i - center.x, 2.0f) + pow((float)j - center.y, 2.0f)) <= radius)
			{
				fogOfWarMap[i + (j*cellX)] = true;
				discoveryMap[i + (j*cellX)] = true;
			}
		}

	//Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	camera->GetShader()->SetLighting(0);
	camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	camera->GetShader()->UseProgram(SHADER_DEFAULT);
	camera->UpdateMatrix(SHADER_DEFAULT);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.99f);
	glDisable(GL_DEPTH_TEST);
	guiIcon->DrawAtLocation(camera, (center.x - 6.0f)*resolution, camera->GetScreenResolution().y - (center.y + 6.0f)*resolution, circleTexture);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//camera->GetShader()->SetLighting(0);
	//camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	//camera->GetShader()->UseProgram(SHADER_DEFAULT);
	//camera->UpdateMatrix(SHADER_DEFAULT);
	//guiScreen->DrawAtLocation(camera, camera->GetScreenResolution().x - 500, 100, fbTexture);
	camera->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);

	return 0;
}*/

int LOSMap::BindTexture()
{
	glActiveTexture(GL_TEXTURE0+17);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glActiveTexture(GL_TEXTURE0+0);

	return 0;
}

int LOSMap::Clear()
{
	for (int i = 0;i < cellX*cellY;i ++)
		fogOfWarMap[i] = false;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}
