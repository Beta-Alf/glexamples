#include "AnimationExample.h"

#include<iostream>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>


#include <widgetzeug/make_unique.hpp>

#include <gloperate/base/RenderTargetType.h>

#include <gloperate/resources/ResourceManager.h>

#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate-assimp/AssimpMeshLoader.h>

#include <gloperate/primitives/AdaptiveGrid.h>
#include <gloperate/primitives/AbstractDrawable.h>
#include <gloperate/primitives/PolygonalDrawable.h>


#include <ParameterAnimatedObject.h>


using namespace gl;
using namespace glm;
using namespace globjects;

using widgetzeug::make_unique;

AnimationExample::AnimationExample(gloperate::ResourceManager & resourceManager)
:   Painter(resourceManager)
,   m_targetFramebufferCapability(addCapability(new gloperate::TargetFramebufferCapability()))
,   m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
,   m_projectionCapability(addCapability(new gloperate::PerspectiveProjectionCapability(m_viewportCapability)))
,   m_cameraCapability(addCapability(new gloperate::CameraCapability()))
,	m_timeCapability(addCapability(new gloperate::VirtualTimeCapability()))
{
	setupPropertyGroup();
}

AnimationExample::~AnimationExample() = default;

void AnimationExample::setupPropertyGroup()
{
	// drop-down menu to switc between Animation types
	auto animationTypes = addProperty<AnimationTypes>("Animation_Types", this,
		&AnimationExample::animationType,
		&AnimationExample::setAnimationType);

	animationTypes->setStrings({
		{ ParameterAnimation, "parameter animation" },
		{ VertexAnimation, "vertex animation" },
		{ RigAnimation, "rig animation" } 
	});

	animationTypes->setChoices({ ParameterAnimation, VertexAnimation, RigAnimation });


	// drop-down menu to switch between Vertex Animations
	auto vertexAnimationOptions = addProperty<VertexAnimationOptions>("Vertex_Animations", this,
		&AnimationExample::vertexAnimation,
		&AnimationExample::setVertexAnimation);

	vertexAnimationOptions->setStrings({
		{ STAND, "stand" },
		{ RUN, "run" },
		{ JUMP, "jump" },                  
		{ SALUTE, "salute" } 
	});

	vertexAnimationOptions->setChoices({ STAND, RUN, JUMP, SALUTE });
}

void AnimationExample::initializeParameterAnimation(){

	std::cout << "I started Parameter Animation";
	gloperate::PolygonalGeometry* buddha = m_resourceManager.load<gloperate::PolygonalGeometry>(std::string("data/animationexample/buddha.obj"));
	auto ParObj = new ParameterAnimatedObject(new gloperate::PolygonalDrawable(*buddha));
	std::cout << "Buddah is loaded.";
	m_animation = std::unique_ptr<ParameterAnimatedObject>(ParObj);
	ParameterKeyframe keyframe;
	keyframe.time = 0.f;
	keyframe.translation = glm::vec3{ 0.f, 0.f, 0.f };
	keyframe.rotation = glm::quat();
	keyframe.scale = glm::vec3(1.f, 1.f, 1.f);
	m_animation->addKeyframe(keyframe);
	keyframe.time = 1.f;
	keyframe.translation = glm::vec3{ 1.f, 0.f, 0.f };
	m_animation->addKeyframe(keyframe);
	keyframe.time = 4.f;
	keyframe.translation = glm::vec3{ -1.f, 0.f, 5.f };
	keyframe.scale = glm::vec3(3.f, 1.f, 1.f);
	keyframe.rotation = glm::quat{ 0.f, 0.7f, 0.f, 0.7f };
	m_animation->addKeyframe(keyframe);
}

AnimationTypes AnimationExample::animationType() const{
	return m_currentAnimationType;
}

void AnimationExample::setAnimationType(const AnimationTypes & type){
	switch (type) {
	case ParameterAnimation:
		initializeParameterAnimation();
		break;
	case VertexAnimation:
		md2LoaderInstance = md2Loader();
		md2LoaderInstance.loadModel("data/animationexample/Samourai.md2");
		md2ModelDrawable = md2LoaderInstance.modelToGPU();
		break;
	case RigAnimation:
		break;
	default:
		std::cout << "You have to chose an Animation first.";
		break;
	}

	m_currentAnimationType = type;
}

VertexAnimationOptions AnimationExample::vertexAnimation() const{
	return m_currentVertexAnimation;
}

void AnimationExample::setVertexAnimation(const VertexAnimationOptions & animation){
	switch (animation) {
	case STAND: 
		m_firstFrame = 0;
		m_lastFrame = 39;
		m_fps = 9;
		break;
	case RUN: 
		m_firstFrame = 40;
		m_lastFrame = 45;
		m_fps = 10;
		break;
	case JUMP: 
		m_firstFrame = 66;
		m_lastFrame = 71;
		m_fps = 7;
		break;
	case SALUTE:
		m_firstFrame = 84;
		m_lastFrame = 94;
		m_fps = 7;
		break;
	default:
		m_firstFrame = 0;
		m_lastFrame = 0;
		m_fps = 1;
		break;
	}
	m_currentVertexAnimation = animation;
}

void AnimationExample::setupProjection()
{
    static const auto zNear = 0.1f, zFar = 150.f, fovy = 50.f;
	
    m_projectionCapability->setZNear(zNear);
    m_projectionCapability->setZFar(zFar);
    m_projectionCapability->setFovy(radians(fovy));

    m_grid->setNearFar(zNear, zFar);
}

void AnimationExample::onInitialize()
{
    // create program

    globjects::init();

#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

    m_grid = new gloperate::AdaptiveGrid{};
    m_grid->setColor({0.6f, 0.6f, 0.6f});

	glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

	setupProjection();

	m_cameraCapability->setEye(vec3(100.0, 0.0, 0.0));

	setAnimationType(RigAnimation);
	setVertexAnimation(STAND); // has to be set even if we are in other animations

	m_timeCapability->setLoopDuration(6); 
}

void AnimationExample::onPaint()
{
	//dependend on displayed animation (switch with menu later)

	m_currentTime = m_timeCapability->time();

    if (m_viewportCapability->hasChanged())
    {
        glViewport(
            m_viewportCapability->x(),
            m_viewportCapability->y(),
            m_viewportCapability->width(),
            m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }

    auto fbo = m_targetFramebufferCapability->framebuffer();

    if (!fbo)
        fbo = globjects::Framebuffer::defaultFBO();

    fbo->bind(GL_FRAMEBUFFER);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
	
    const auto transform = m_projectionCapability->projection() * m_cameraCapability->view();
    const auto eye = m_cameraCapability->eye();

    m_grid->update(eye, transform);
    m_grid->draw();
	
	switch (m_currentAnimationType){
	case ParameterAnimation:
		m_animation->draw(m_currentTime, transform);
		break;
	case VertexAnimation:
		md2ModelDrawable.draw(m_firstFrame, m_lastFrame, m_fps, m_currentTime, transform);
		break;
	case RigAnimation:
		break;
	}
	


    Framebuffer::unbind(GL_FRAMEBUFFER);
}
