#ifndef _LOS_MAP_H
#define _LOS_MAP_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Texturer/TextureLoader.h"
//#include "Graphics/Camera.h"

#include "Game/GUI/GUIIcon.h"

//LOSMap
//This class handles both the logical map and the framebuffer with attached texture.
//The texture is a linear filtered texture where all the channels are written to.
//The texture is cleared by clearing the color buffer bit for all channels except the red channel.
//This red channel is used to keep track of the discovery map in the framebuffer texture.

//NOTE: One main concern is that the texture and the logical map are completely decoupled.
//		There is no redundancy check to ensure that the texture is representative of the logical map.
//		I don't see how something bad could happen from inside the class, but if the object instance is in charge of it's LOS radius and LOS texture,
//		it could easily mess things up. This won't have any effect on the gameplay, only the visuals.
//TODO: The above note should be looked in to and perhaps a solution should be made?
//		This is tentative because if the LOS texture is generated dynamically by the instance based on its LOS radius, then it may work fine.

class LOSMap
{
public:
	LOSMap();
	~LOSMap();

	//Generates a map and a framebuffer for the line of sight
	//NOTE: The resolution is only for the framebuffer texture (as of now)
	int Init(int cX, int cY, int resolutionPerCell);

	//Sets an area of the line-of-sight, both on the map and in the framebuffer
	//NOTE: The framebuffer draws using the ortho matrix and the default shader
	//TODO: I should look into whether creating a blitting system would be less costly
	//TODO: Change the function to accept textures for rending to the framebuffer
	//int SetLineOfSight(Camera* camera, glm::vec2 center, float radius);

	//Binds the framebuffer texture to the gl active texture 17
	//TODO: This should probably be changable?
	//TODO: Also, this probably doesn't need to set every frame - if the active texture 17 never changes.
	//		I don't think it's worth the risk though.
	int BindTexture();

	//Clears the framebuffer texture in the specific way for the LOS map.
	int Clear();
private:
	int cellX;
	int cellY;
	int resolution;

	//Fog of war and discovery map
	bool* fogOfWarMap;
	bool* discoveryMap;

	//Framebuffer
	GLuint FBO;
	GLuint fbTexture;
	GLuint circleTexture;

	//This is for testing and debugging purposes
	GUIIcon* guiIcon;
	GUIIcon* guiScreen;
};
#endif
