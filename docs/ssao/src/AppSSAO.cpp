/*******************************************************************************
	Copyright (C) 2013 John Chapman

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#include "AppSSAO.h"

/*----------------------------------------------------------------------------*/
int main(int argc, char **argv) {
	AppSSAO app(
		"",
		frm::ivec2(1280, 720), // window size
		frm::ivec2(1280, 720), // render size
		false
	);
	app.run();
	return 0;
}

/*******************************************************************************
	AppSSAO implementation:
*******************************************************************************/

//	PROTECTED:

//	MEMBERS:

frm::CStringT AppSSAO::envMapList_[] = {
	"media/environments/grove.env.hdr",
	"media/environments/pisa.env.hdr",
	"media/environments/overcast.env.hdr",
	"media/environments/stpeters.env.hdr",
	0 // sentinel
};

//	SERVICES:

/*----------------------------------------------------------------------------*/
bool AppSSAO::initScene() {
	if (!App3d::initScene())
		return false;
//------------------------------------------------------------------------------
//	TEXTURES
//------------------------------------------------------------------------------
	texEnvMap_ = frm::TextureCube::loadFromFile(envMapList_[currentEnvMap_], frm::ITexture::TEX_MIPMAP);
	if (!texEnvMap_)
		return false;

//------------------------------------------------------------------------------
//	MATERIALS
//------------------------------------------------------------------------------
	frm::EnvmapMaterial *matEnvmap = frm::EnvmapMaterial::create(texEnvMap_);
	if (!matEnvmap)
		return false;

	frm::IMaterial *mat = 0;

	mat = frm::GenericMaterialDeferred::create(
		frm::Texture2d::loadFromFile("media/materials/wood1.diffuse.png", frm::ITexture::TEX_MIPMAP),		// diffuse tex
		sml::vec3f(1.0f),																												// diffuse color

		frm::Texture2d::loadFromFile("media/materials/wood1.normal.png", frm::ITexture::TEX_MIPMAP),		// normal tex
		frm::Texture2d::loadFromFile("media/materials/wood1.detail.png", frm::ITexture::TEX_MIPMAP),		// detail tex
		sml::vec2f(2.5f),																												// detail scale

		frm::Texture2d::loadFromFile("media/materials/wood1.specular.png", frm::ITexture::TEX_MIPMAP),	// specular tex
		1.0f,																																// specular level
		1.0f,																																// specular exp
		1.0f,																																// metallic level

		"materialWood1"
	);
	if (!mat)
		return false;

//------------------------------------------------------------------------------
//	MODELS:
//------------------------------------------------------------------------------
	frm::Mesh* mesh = 0;
	frm::Model* model = 0;

	mesh = frm::Mesh::sphere(1.0f, 32);
	mesh->transform(sml::mat4f::rotation3(sml::vec3f::xAxis(), sml::radians(-90.0f)));
	if (!mesh)
		return false;
	model = frm::Model::create(mesh, "meshSphere");
	if (!model)
		return false;
	frm::Mesh::destroy(&mesh);

	mesh = frm::Mesh::loadFromFile("media/models/teapot.obj");
	if (!mesh)
		return false;
	model = frm::Model::create(mesh, "meshTeapot");
	if (!model)
		return false;
	frm::Mesh::destroy(&mesh);

	mesh = frm::Mesh::loadFromFile("media/models/floor.obj");
	if (!mesh)
		return false;
	model = frm::Model::create(mesh, "meshFloor");
	if (!model)
		return false;
	frm::Mesh::destroy(&mesh);

//------------------------------------------------------------------------------
//	OBECTS:
//------------------------------------------------------------------------------
	frm::Node *node;

	const int NTEAPOTS = 7;
	const int HALF_NTEAPOTS = NTEAPOTS / 2;
	for (int x = 0; x < NTEAPOTS; ++x) {
		for (int z = 0; z < NTEAPOTS; ++z) {
			mat = frm::GenericMaterialDeferred::create(
				0,																																	// diffuse tex
				sml::vec3f((float)x / (float)NTEAPOTS, 0.3f, (float)z / (float)NTEAPOTS),									// diffuse color

				frm::Texture2d::loadFromFile("media/materials/plastic1.normal.png", frm::ITexture::TEX_MIPMAP),		// normal tex
				frm::Texture2d::loadFromFile("media/materials/plastic1.detail.png", frm::ITexture::TEX_MIPMAP),		// detail tex
				sml::vec2f(2.5f),																												// detail scale

				frm::Texture2d::loadFromFile("media/materials/plastic1.specular.png", frm::ITexture::TEX_MIPMAP),	// specular tex
				1.0f,																																// specular level
				1.0f,																																// specular exp
				1.0f,																																// metallic level
				""
			);
			if (!mat)
				return false;

			sml::vec3f pos(
				(float)(x - HALF_NTEAPOTS) * 2.0f,
				0.0f,
				(float)(z - HALF_NTEAPOTS) * 2.0f
			);

			node = addNode(frm::Node::create());
			node->addModifier(
				frm::PositionRotationScaleCtrl::create(
					pos,
					sml::quatf(sml::vec3f::yAxis(), sml::radians(38.0f))
				)
			);

			addObject(
				frm::Object::create(
					node,
					frm::Model::find("meshTeapot"),
					mat
				)
			);
		}
	}

	node = addNode(frm::Node::create());
	addObject(
		frm::Object::create(
			node,
			frm::Model::find("meshFloor"),
			frm::IMaterial::find("materialWood1")
		)
	);

//------------------------------------------------------------------------------
//	LIGHTS:
//------------------------------------------------------------------------------
	addLight( // render env map direct to hdr buffer
		frm::Object::create(
			addNode(frm::Node::create("ENVMAP")),
			frm::modelSAQuad,
			matEnvmap,
			true // never clip
		)
	);

	addLight(
		frm::Object::create(
			addNode(frm::Node::create("ENVMAP LIGHT")),
			frm::modelSAQuad,
			frm::LightDeferredIbl::create(texEnvMap_),
			true // never clip
		)
	);

//------------------------------------------------------------------------------
//	CAMERAS
//------------------------------------------------------------------------------


#ifndef NDEBUG
	getRootNode()->print();
#endif

	return true;
}

/*----------------------------------------------------------------------------*/
bool AppSSAO::initGui() {
	if (!App3d::initGui())
		return false;

	frm::GuiListContainer *sliderList = frm::GuiListContainer::create();
	sliderList->setDirection(frm::IGuiComponent::VERTICAL);
	sliderList->setAlignment(frm::IGuiComponent::RIGHT, frm::IGuiComponent::TOP);
	sliderList->setSizeRef(frm::IGuiComponent::ABSOLUTE, frm::IGuiComponent::RELATIVE);
	sliderList->setPosition(sml::vec2i(20, 0));
	sliderList->setSize(sml::vec2i(260, 100));
	addGuiComponent(sliderList);

	addGuiComponent(
		frm::GuiCheckbox::create(
			&doSsao_,
			frm::Texture2d::loadFromFile("media/gui/checkbox16off.png"),
			frm::Texture2d::loadFromFile("media/gui/checkbox16on.png"),
			frm::fontDefault,
			"Enable SSAO"
		),
		sliderList
	);

	addGuiComponent(
		frm::GuiCheckbox::create(
			&doBlur_,
			frm::Texture2d::loadFromFile("media/gui/checkbox16off.png"),
			frm::Texture2d::loadFromFile("media/gui/checkbox16on.png"),
			frm::fontDefault,
			"Enable blur"
		),
		sliderList
	);

	addGuiComponent(
		frm::GuiCheckbox::create(
			&ssaoOnly_,
			frm::Texture2d::loadFromFile("media/gui/checkbox16off.png"),
			frm::Texture2d::loadFromFile("media/gui/checkbox16on.png"),
			frm::fontDefault,
			"SSAO Only"
		),
		sliderList
	);

	addGuiComponent(
		frm::GuiSlider::create(
			&setAuxBufferSize_,
			frm::ivec2(1.0f, 4.0f),
			frm::Texture2d::loadFromFile("media/gui/slider16.png"),
			frm::Texture2d::loadFromFile("media/gui/sliderKnob16.png"),
			frm::fontDefault,
			"Downsample 1/%1.0f"
		),
		sliderList
	);

	addGuiComponent(
		frm::GuiSlider::create(
			&setSsaoNoiseSize_,
			frm::ivec2(1.0f, 8.0f),
			frm::Texture2d::loadFromFile("media/gui/slider16.png"),
			frm::Texture2d::loadFromFile("media/gui/sliderKnob16.png"),
			frm::fontDefault,
			"SSAO Noise Size %2.0f"
		),
		sliderList
	);

	addGuiComponent(
		frm::GuiSlider::create(
			&setSsaoKernelSize_,
			frm::ivec2(4.0f, 128.0f),
			frm::Texture2d::loadFromFile("media/gui/slider16.png"),
			frm::Texture2d::loadFromFile("media/gui/sliderKnob16.png"),
			frm::fontDefault,
			"SSAO Kernel Size %2.0f"
		),
		sliderList
	);
	addGuiComponent(
		frm::GuiSlider::create(
			&ssaoRadius_,
			frm::ivec2(0.0f, 24.0f),
			frm::Texture2d::loadFromFile("media/gui/slider16.png"),
			frm::Texture2d::loadFromFile("media/gui/sliderKnob16.png"),
			frm::fontDefault,
			"SSAO Radius %1.2f"
		),
		sliderList
	);
	addGuiComponent(
		frm::GuiSlider::create(
			&ssaoPower_,
			frm::ivec2(1.0f, 16.0f),
			frm::Texture2d::loadFromFile("media/gui/slider16.png"),
			frm::Texture2d::loadFromFile("media/gui/sliderKnob16.png"),
			frm::fontDefault,
			"SSAO Power %1.2f"
		),
		sliderList
	);

	sliderList->setDirection(frm::IGuiComponent::VERTICAL); // KLUDGE

	return true;
}

/*----------------------------------------------------------------------------*/
bool AppSSAO::initRender() {
	if (!App3d::initRender())
		return false;

//------------------------------------------------------------------------------
//	SHADERS
//------------------------------------------------------------------------------
	shaderPostProcess_ = frm::Shader::loadFromFiles("shaders/basic.vs.glsl", "shaders/postprocess.fs.glsl");
	if (!shaderPostProcess_)
		return false;
	shaderLuminance_ = frm::Shader::loadFromFiles("shaders/basic.vs.glsl", "shaders/lum.fs.glsl");
	if (!shaderLuminance_)
		return false;
	shaderAvgMinMax_ = frm::Shader::loadFromFiles("shaders/basic.vs.glsl", "shaders/avgminmax.fs.glsl");
	if (!shaderAvgMinMax_)
		return false;
	shaderLuminanceAdapt_ = frm::Shader::loadFromFiles("shaders/basic.vs.glsl", "shaders/adaptlum.fs.glsl");
	if (!shaderLuminanceAdapt_)
		return false;


	shaderSsao_ = frm::Shader::loadFromFiles("shaders/models/SAQuadModel.vs.glsl", "shaders/ssao.fs.glsl");
	if (!shaderSsao_)
		return false;
	if (!resizeSsaoKernel()) // generate & upload ssao kernel
		return false;

	shaderSsaoBlur_ = frm::Shader::loadFromFiles("shaders/basic.vs.glsl", "shaders/ssaoblur.fs.glsl");
	if (!shaderSsaoBlur_)
		return false;

	shaderSsaoOnly_ = frm::Shader::loadFromFiles("shaders/basic.vs.glsl", "shaders/ssaoonly.fs.glsl");
	if (!shaderSsaoOnly_)
		return false;

//------------------------------------------------------------------------------
//	RENDER TEXTURES
//------------------------------------------------------------------------------
	texSsaoNoise_ = frm::Texture2d::create(GL_RGB, ssaoNoiseSize_, ssaoNoiseSize_);
	if (!texSsaoNoise_)
		return false;
	if (!resizeSsaoNoise()) // generate & upload noise
		return false;

//------------------------------------------------------------------------------
//	FRAMEBUFFERS
//------------------------------------------------------------------------------
/*	HDR RENDER BUFFER */
	texHdr_ = frm::Texture2d::create(GL_RGBA16F, renderSize_.x(), renderSize_.y());
	if (!texHdr_)
		return false;
	texHdr_->setMagFilter(GL_NEAREST);
	texHdr_->setWrap(GL_CLAMP_TO_EDGE);
	texHdr_->setName("HDR");

	texDepth_ = frm::Texture2d::create(GL_DEPTH_COMPONENT, renderSize_.x(), renderSize_.y());
	if (!texDepth_)
		return false;
	texDepth_->setWrap(GL_CLAMP_TO_EDGE);
	texDepth_->setName("DEPTH");

	fboHdr_ = frm::Framebuffer::create();
	if (!fboHdr_)
		return false;
	fboHdr_->attach(texHdr_, GL_COLOR_ATTACHMENT0);
//	fboHdr_->attach(texDepth_, GL_DEPTH_ATTACHMENT);

/*	GBUFFER */
	texGBufferDiffuse_ = frm::Texture2d::create(GL_RGBA8, renderSize_.x(), renderSize_.y());
	if (!texGBufferDiffuse_)
		return false;
	texGBufferDiffuse_->setMagFilter(GL_NEAREST);
	texGBufferDiffuse_->setName("GB DIFFUSE");

	texGBufferMaterial_ = frm::Texture2d::create(GL_RGBA8, renderSize_.x(), renderSize_.y());
	if (!texGBufferMaterial_)
		return false;
	texGBufferMaterial_->setMagFilter(GL_NEAREST);
	texGBufferMaterial_->setName("GB MATERIAL");

	texGBufferGeometric_ = frm::Texture2d::create(GL_RGBA8, renderSize_.x(), renderSize_.y());
	//texGBufferGeometric_ = frm::Texture2d::create(GL_RGBA16F, renderSize_.x(), renderSize_.y());
	if (!texGBufferGeometric_)
		return false;
	texGBufferGeometric_->setMagFilter(GL_NEAREST);
	texGBufferGeometric_->setName("GB GEOMETRIC");

	fboGBuffer_ = frm::Framebuffer::create();
	if (!fboGBuffer_)
		return false;
	fboGBuffer_->attach(texGBufferDiffuse_, GL_COLOR_ATTACHMENT0);
	fboGBuffer_->attach(texGBufferMaterial_, GL_COLOR_ATTACHMENT1);
	fboGBuffer_->attach(texGBufferGeometric_, GL_COLOR_ATTACHMENT2);
	fboGBuffer_->attach(texDepth_, GL_DEPTH_ATTACHMENT);

	fboSsaoBlur_ = frm::Framebuffer::create();
	if (!fboSsaoBlur_)
		return false;
	fboSsaoBlur_->attach(texGBufferGeometric_, GL_COLOR_ATTACHMENT0);

/*	AUX RENDER BUFFERS */
	texAuxA_ = frm::Texture2d::create(GL_RGBA8, renderSize_.x() / auxBufferSize_, renderSize_.y() / auxBufferSize_);
	if (!texAuxA_)
		return false;
	texAuxA_->setMagFilter(GL_LINEAR);
	texAuxA_->setWrap(GL_CLAMP_TO_EDGE);
	texAuxA_->setName("AUX A");
	fboAuxA_ = frm::Framebuffer::create();
	if (!fboAuxA_)
		return false;
	fboAuxA_->attach(texAuxA_, GL_COLOR_ATTACHMENT0);

/*	AUTO EXPOSURE BUFFERS */
	texLuma_ = frm::Texture2d::create(GL_RGB16F, 256, 256);
	if (!texLuma_)
		return false;
	texLuma_->setMinFilter(GL_LINEAR_MIPMAP_NEAREST); // don't want filtering between mip levels!
	texLuma_->setWrap(GL_CLAMP_TO_EDGE);
	texLuma_->generateMipmap(); // allocate mipmap
	fboLuma_ = frm::Framebuffer::create();
	if (!fboLuma_)
		return false;
	fboLuma_->attach(texLuma_, GL_COLOR_ATTACHMENT0);

	for (int i = 0; i < 2; ++i) {
		texAdaptLuma_[i] = frm::Texture2d::create(GL_R16F, 1, 1);
		if (!texAdaptLuma_[i])
			return false;
		texAdaptLuma_[i]->setMinFilter(GL_NEAREST); // don't want filtering between mip levels!
		texAdaptLuma_[i]->setMagFilter(GL_NEAREST); // don't want filtering between mip levels!
		texAdaptLuma_[i]->setWrap(GL_REPEAT);
		fboAdaptLuma_[i] = frm::Framebuffer::create();
		if (!fboAdaptLuma_[i])
			return false;
		fboAdaptLuma_[i]->attach(texAdaptLuma_[i], GL_COLOR_ATTACHMENT0);
	}

/*	RENDER TEXTURES TO VISUALIZE IN OVERLAY */
	addRenderTexture(texGBufferDiffuse_);
	addRenderTexture(texGBufferMaterial_);
	addRenderTexture(texGBufferGeometric_);
	addRenderTexture(texAuxA_);

//------------------------------------------------------------------------------
//	OPENGL STATE
//------------------------------------------------------------------------------
	OOGL_CALL(glDisable(GL_DEPTH_TEST));
	OOGL_CALL(glFrontFace(GL_CCW)); // default is GL_CCW
	OOGL_CALL(glCullFace(GL_BACK));
	OOGL_CALL(glEnable(GL_CULL_FACE));
	OOGL_CALL(glClearDepth(1.0f));
	OOGL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	if (oogl::isSupported("GL_ARB_seamless_cube_map")) {
		OOGL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
	}

	return true;
}

/*----------------------------------------------------------------------------*/
bool AppSSAO::handleEvent(const sf::Event &event) {
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::B: // cycle environment maps:
					++currentEnvMap_;
					if (envMapList_[currentEnvMap_] == 0)
						currentEnvMap_ = 0;

					texEnvMap_->setFileName(envMapList_[currentEnvMap_]);
					texEnvMap_->reload();
					return true;

				case sf::Keyboard::F9: // reloads shaders, so reupload kernel
					if (!App3d::handleEvent(event))
						return false;
					return resizeSsaoKernel();

				case sf::Keyboard::Return: // override switch to fullscreen
					return true;
				default:
					break;


			};
			break;

		default:
			break;
	};

	return App3d::handleEvent(event);
}

/*----------------------------------------------------------------------------*/
void AppSSAO::renderToMipmap(
	frm::Framebuffer *fbo,
	frm::Shader *shader,
	int bindLocation
) {
	assert(fbo);

	oogl::Texture2D *tex = (oogl::Texture2D*)fbo->getAttachment(GL_COLOR_ATTACHMENT0);
	assert(tex);

	sml::vec2i texSize(tex->getWidth(), tex->getHeight());
	int nMipLevels = (int)sml::log((float)texSize.y(), 2.0f);

	assert(shader);
	frm::modelSAQuad->enable(shader, this);
	fbo->bindDraw();
	glActiveTexture(GL_TEXTURE0 + bindLocation);
	tex->bind();

	for (int i = 1; i <= nMipLevels; ++i) {
		fbo->attach(tex, GL_COLOR_ATTACHMENT0, i);
		tex->setBaseLevel(i - 1);
		tex->setMaxLevel(i - 1);
		texSize /= 2;
		OOGL_CALL(glViewport(0, 0, texSize.x(), texSize.y()));
		frm::modelSAQuad->render();
	}

//	restore mip min/max level:
	fbo->attach(tex, GL_COLOR_ATTACHMENT0, 0); // attach the top layer
	tex->setBaseLevel(0);
	tex->setMaxLevel(nMipLevels);

	frm::modelSAQuad->disable();
	frm::Framebuffer::bindNone();
}


/*----------------------------------------------------------------------------*/
bool AppSSAO::resizeAuxBuffers() {
//	resize textures:
	texAuxA_->bind();
	texAuxA_->image(0, texAuxA_->getFormat(), renderSize_.x() / auxBufferSize_, renderSize_.y() / auxBufferSize_);
	frm::Texture2d::bindNone();

//	rebind to fbos - this is only necessary so that OOGL can recompute the
//	attachment properties:
	fboAuxA_->bind();
	fboAuxA_->attach(texAuxA_, GL_COLOR_ATTACHMENT0);
	frm::Framebuffer::bindNone();

	return true;
}

/*----------------------------------------------------------------------------*/
bool AppSSAO::resizeSsaoKernel() {
	if (!shaderSsao_)
		return false;

//	generate kernel:
	sml::vec3f *kernel = new(std::nothrow) sml::vec3f[ssaoKernelSize_];
	assert(kernel);
	sml::LCG lcg(123456); // random number generator
	for (int i = 0; i < ssaoKernelSize_; ++i) {
		kernel[i] = sml::vec3f(
			lcg.get(-1.0f, 1.0f),
			lcg.get(-1.0f, 1.0f),
			lcg.get(0.0f, 1.0f)
		);
		kernel[i].normalize();

		float scale = (float)i / (float)ssaoKernelSize_;
		kernel[i] *= sml::lerp(0.1f, 1.0f, scale * scale);
	}

//	upload the kernel:
	shaderSsao_->use();

	OOGL_CALL(glUniform1i(shaderSsao_->getUniformLocation("uKernelSize"), ssaoKernelSize_));
	OOGL_CALL(glUniform3fv(shaderSsao_->getUniformLocation("uKernelOffsets"), ssaoKernelSize_, (const GLfloat*) kernel));

	frm::Shader::useNone();

	delete[] kernel;

	return true;
}

/*----------------------------------------------------------------------------*/
bool AppSSAO::resizeSsaoNoise() {
	if (!texSsaoNoise_)
		return false;

//	generate noise data:
	int noiseDataSize = ssaoNoiseSize_ * ssaoNoiseSize_;
	sml::vec3f *noiseData = new(std::nothrow) sml::vec3f[noiseDataSize];
	assert(noiseData);
	sml::LCG lcg(123456); // random number generator
	for (int i = 0; i < noiseDataSize; ++i) {
		noiseData[i] = sml::vec3f(
			lcg.get(-1.0f, 1.0f),
			lcg.get(-1.0f, 1.0f),
			0.0f
		);
		noiseData[i].normalize();
	}

//	reize noise texture & upload:
	texSsaoNoise_->bind();
	texSsaoNoise_->image(0, texSsaoNoise_->getFormat(), ssaoNoiseSize_, ssaoNoiseSize_, GL_RGB, GL_FLOAT, noiseData);
	frm::Texture2d::bindNone();

	delete[] noiseData;

	return true;
}


/*----------------------------------------------------------------------------*/
bool AppSSAO::render(float dt) {
	if (!App3d::render(dt))
		return false;

	if ((int)setAuxBufferSize_ != auxBufferSize_) {
		auxBufferSize_ = (int)setAuxBufferSize_;
		if (!resizeAuxBuffers())
			return false;
	}
	if ((int)setSsaoKernelSize_ != ssaoKernelSize_) {
		ssaoKernelSize_ = (int)setSsaoKernelSize_;
		if (!resizeSsaoKernel())
			return false;
	}
	if ((int)setSsaoNoiseSize_ != ssaoNoiseSize_) {
		ssaoNoiseSize_ = (int)setSsaoNoiseSize_;
		if (!resizeSsaoNoise())
			return false;
	}

	frm::Camera &camera = *getCurrentCamera();;
	frm::FirstPersonCtrl &fpCtrl = *fpCtrls_[currentFpCtrl_];
	int previousAdaptLuma = currentAdaptLuma_;
	currentAdaptLuma_ = (currentAdaptLuma_ + 1) % 2;

	frm::Profiler::start("FRAME");

	//---------------------------------------------------------------------------
	//	RENDER SCENE TO GBUFFER
	//---------------------------------------------------------------------------
	//frm::Profiler::start("GBUFFER");
		fboGBuffer_->bindDraw();
		oogl::viewport(fboGBuffer_);
		OOGL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

		renderObjects();
	//frm::Profiler::stop("GBUFFER");

	//---------------------------------------------------------------------------
	//	SSAO PREPASS
	//---------------------------------------------------------------------------
	if (doSsao_) {
		frm::Profiler::start("SSAO");

			frm::Profiler::start("SSAO GENERATE");
				fboAuxA_->bindDraw();
				oogl::viewport(fboAuxA_);

				shaderSsao_->use();

				glActiveTexture(GL_TEXTURE0);
				texGBufferGeometric_->bind();
				glActiveTexture(GL_TEXTURE1);
				texDepth_->bind();
				glActiveTexture(GL_TEXTURE2);
				texSsaoNoise_->bind();

				OOGL_CALL(glUniform1f(shaderSsao_->getUniformLocation("uRadius"), ssaoRadius_));
				OOGL_CALL(glUniform1f(shaderSsao_->getUniformLocation("uPower"), ssaoPower_));
				OOGL_CALL(glUniformMatrix4fv(shaderSsao_->getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, camera.getProjectionMatrix()));

				frm::modelSAQuad->enable(shaderSsao_, this);
				frm::modelSAQuad->render();
				frm::modelSAQuad->disable();
			frm::Profiler::stop("SSAO GENERATE");

			frm::Profiler::start("SSAO BLUR");
				fboSsaoBlur_->bindDraw();
				oogl::viewport(fboSsaoBlur_);

				shaderSsaoBlur_->use();

				glActiveTexture(GL_TEXTURE0);
				texAuxA_->bind();

				if (doBlur_) {
					OOGL_CALL(glUniform1i(shaderSsaoBlur_->getUniformLocation("uBlurSize"), ssaoNoiseSize_));
				} else {
					OOGL_CALL(glUniform1i(shaderSsaoBlur_->getUniformLocation("uBlurSize"), 1));
				}


			//	multiply into alpha channel of gbuffer geometric:
				OOGL_CALL(glEnable(GL_BLEND));
				OOGL_CALL(glBlendFunc(GL_DST_COLOR, GL_ZERO)); // multiplicative blend
				OOGL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE)); // only write alpha

				frm::modelSAQuad->enable(shaderSsaoBlur_, this);
				frm::modelSAQuad->render();
				frm::modelSAQuad->disable();

				OOGL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
				OOGL_CALL(glDisable(GL_BLEND));

			frm::Profiler::stop("SSAO BLUR");


		frm::Profiler::stop("SSAO");
	}

	//---------------------------------------------------------------------------
	//	RENDER LIGHTS TO HDR BUFFER
	//---------------------------------------------------------------------------
	//frm::Profiler::start("LIGHTING");
		fboHdr_->bindDraw();
		oogl::viewport(fboHdr_);
		OOGL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//	bind gbuffer textures starting at texture unit 1, leave 0 to allow for
	//	lights to bind a texture
		OOGL_CALL(glActiveTexture(GL_TEXTURE1));
		texGBufferDiffuse_->bind();
		OOGL_CALL(glActiveTexture(GL_TEXTURE2));
		texGBufferMaterial_->bind();
		OOGL_CALL(glActiveTexture(GL_TEXTURE3));
		texGBufferGeometric_->bind();
		OOGL_CALL(glActiveTexture(GL_TEXTURE4));
		texDepth_->bind();

		renderLights();

	//frm::Profiler::stop("LIGHTING");


	frm::Profiler::start("POST PROCESS");

	//---------------------------------------------------------------------------
	//	PREP AUTO EXPOSURE
	//---------------------------------------------------------------------------
	/*	RENDER INITIAL LOG LUMINANCE: */
		fboLuma_->bindDraw();
		oogl::viewport(fboLuma_);
		shaderLuminance_->use();
		glActiveTexture(GL_TEXTURE0);
		texHdr_->bind();
		frm::modelSAQuad->enable(shaderLuminance_, this);
		frm::modelSAQuad->render();
		frm::modelSAQuad->disable();

		// progressive downsample into mip levels:
		shaderAvgMinMax_->use(); // renderToMipmap() assumes this call was made
		renderToMipmap(fboLuma_, shaderAvgMinMax_);

		//	adapt luminance value based on previous frame:
		fboAdaptLuma_[currentAdaptLuma_]->bind();
		oogl::viewport(fboAdaptLuma_[currentAdaptLuma_]);
		shaderLuminanceAdapt_->use();
		OOGL_CALL(glUniform1f(shaderLuminanceAdapt_->getUniformLocation("uAdaptionRate"), dt * adaptionRate_));
		glActiveTexture(GL_TEXTURE0);
		texLuma_->bind();
		glActiveTexture(GL_TEXTURE1);
		texAdaptLuma_[previousAdaptLuma]->bind();
		frm::modelSAQuad->enable(shaderLuminance_, this);
		frm::modelSAQuad->render();
		frm::modelSAQuad->disable();

	//---------------------------------------------------------------------------
	//	POST PROCESS (AUTO EXPOSURE, TONEMAP, MBLUR, ETC.):
	//---------------------------------------------------------------------------
		//	matrix inputs for camera mblur:
		static sml::mat4f prevViewProjMat = camera.getViewProjectionMatrix();
		sml::mat4f inverseViewProjMat = sml::inverse(camera.getViewProjectionMatrix());

		fboDefault_->bindDraw();
		oogl::viewport(fboDefault_);
		shaderPostProcess_->use();
		OOGL_CALL(glUniform1f(shaderPostProcess_->getUniformLocation("uExposure"), exposure_));
		OOGL_CALL(glUniform1f(shaderPostProcess_->getUniformLocation("uFrameTime"), dt));
		OOGL_CALL(glUniformMatrix4fv(shaderPostProcess_->getUniformLocation("uInverseViewProjectionMatrix"), 1, GL_FALSE, inverseViewProjMat));
		OOGL_CALL(glUniformMatrix4fv(shaderPostProcess_->getUniformLocation("uPrevViewProjectionMatrix"), 1, GL_FALSE, prevViewProjMat));
		OOGL_CALL(glUniform1f(shaderPostProcess_->getUniformLocation("uMotionScale"), (1.0f / 60.0f) / dt));

		OOGL_CALL(glActiveTexture(GL_TEXTURE0));
		texHdr_->bind();
		OOGL_CALL(glActiveTexture(GL_TEXTURE1));
		texDepth_->bind();
		OOGL_CALL(glActiveTexture(GL_TEXTURE2));
		texAdaptLuma_[currentAdaptLuma_]->bind();

		frm::modelSAQuad->enable(shaderPostProcess_, this);
		frm::modelSAQuad->render();
		frm::modelSAQuad->disable();

		prevViewProjMat = camera.getViewProjectionMatrix();

	frm::Profiler::stop("POST PROCESS");

	frm::Profiler::stop("FRAME");


	// extract AO factor from gbuffer:
	if (ssaoOnly_) {
		shaderSsaoOnly_->use();
		OOGL_CALL(glActiveTexture(GL_TEXTURE0));
		texGBufferGeometric_->bind();
		frm::modelSAQuad->enable(shaderSsaoOnly_, this);
		frm::modelSAQuad->render();
		frm::modelSAQuad->disable();
	}

	return true;
}

