/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#ifndef NDEBUG
	#define OOGL_DEBUG
#endif
#include <oogl/oogl.h>
#include <oogl/utils.h>

#ifndef NDEBUG
	#define SML_DEBUG
	//#define SML_STRICT_DEBUG
#endif
#define SML_NO_ALIGN
#include <sml/sml.h>
#include <sml/itpl.h>
#include <sml/random.h>

#include "framework/framework.h"

/*	*/
class AppSSAO: public frm::App3dDeferred {
public:
//	CTORS/DTORS:
	AppSSAO(
		const frm::CStringT &appName,
		const frm::ivec2 &windowSize,
		const frm::ivec2 &renderSize,
		bool fullscreen = false
	);

protected:
//	MEMBERS:
	frm::Shader *shaderEnvMap_;
	frm::TextureCube *texEnvMap_;
	static frm::CStringT envMapList_[];
	int currentEnvMap_;

	frm::Shader *shaderPostProcess_;
	float exposure_; // exposure override

	frm::Shader *shaderSsao_, *shaderSsaoBlur_, *shaderSsaoOnly_;
	float ssaoRadius_, ssaoPower_;
	int ssaoKernelSize_;
	float setSsaoKernelSize_; // slider controlled; sets ssaoKernelSize_
	int ssaoNoiseSize_;
	float setSsaoNoiseSize_; // slider controlled; sets ssaoNoiseSize_
	frm::Texture2d *texSsaoNoise_;
	frm::Framebuffer *fboSsaoBlur_; // fbo to write to in blur step, bind texGBufferGeometric_

	bool doSsao_, doBlur_, ssaoOnly_;

	//	render textures/framebuffers:
	frm::Framebuffer *fboHdr_;
	frm::Texture2d *texHdr_, *texDepth_; // depth also used for gbuffer

	frm::Framebuffer *fboGBuffer_;
	frm::Texture2d
		*texGBufferDiffuse_,		// diffuse albedo/specular colour
		*texGBufferMaterial_,   // material properties
		*texGBufferGeometric_;  // normals/occlusion

	//	aux buffer
	int auxBufferSize_; // divides render size
	float setAuxBufferSize_; // slider controlled; sets auxBufferSize_
	frm::Framebuffer *fboAuxA_;
	frm::Texture2d *texAuxA_;

	//	luminance buffer (for eye adaption):
	frm::Framebuffer *fboLuma_, *fboAdaptLuma_[2];
	frm::Texture2d *texLuma_, *texAdaptLuma_[2];
	frm::Shader *shaderLuminance_, *shaderAvgMinMax_, *shaderLuminanceAdapt_;
	int currentAdaptLuma_; // swaps between luminance buffers per frame
	float adaptionRate_;

//	SERVICES:
	/*	Initialize scene resources/scene graph. */
	virtual bool initScene();

	/*	Initialize GUI components. */
	virtual bool initGui();

	/*	Initialize render textures/framebuffers/shaders. */
	virtual bool initRender();

	/*	Custom event handling for this app. */
	virtual bool handleEvent(const sf::Event &event);

	/*	Renders to every mip level of the 0th color attachment of fbo. Assumes
		that shader is already bound. */
	void renderToMipmap(
		frm::Framebuffer *fbo,
		frm::Shader *shader,
		int bindLocation = 0 // used as GL_TEXTURE0 + bindLocation
	);

	/*	Resize aux textures; call when changing auxBufferSize_. */
	bool resizeAuxBuffers();

	/*	Resize/upload SSAO kernel; call when changing ssaoKernelSize_. */
	bool resizeSsaoKernel();

	/*	Resize/upload SSAO noise texture; call when changing ssaoNoiseSize_. */
	bool resizeSsaoNoise();

	virtual bool render(float dt);
};

/*******************************************************************************
	AppSSAO inline implementation:
*******************************************************************************/

//	PUBLIC:

//	CTORS/DTORS:

/*----------------------------------------------------------------------------*/
AppSSAO::AppSSAO(
	const frm::CStringT &appName,
	const frm::ivec2 &windowSize,
	const frm::ivec2 &renderSize,
	bool fullscreen
):	App3dDeferred(appName, windowSize, renderSize, fullscreen),
	currentEnvMap_(0),
	exposure_(1.0f),
	ssaoRadius_(3.0f), ssaoPower_(3.0f),
	ssaoKernelSize_(16), setSsaoKernelSize_(16.0f),
	ssaoNoiseSize_(4), setSsaoNoiseSize_(4.0f),
	doSsao_(true), doBlur_(true), ssaoOnly_(false),
	auxBufferSize_(1), setAuxBufferSize_(1.0f),
	currentAdaptLuma_(0),
	adaptionRate_(2.5f) {
}

