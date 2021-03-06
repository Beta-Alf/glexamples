#define _USE_MATH_DEFINES
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
#include <reflectionzeug/variant/Variant.hpp>

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
#include <RigAnimatedObject.h>
#include <RiggedDrawable.h>
#include <md2Loader.h>
#include <FrameDrawable.h>


using namespace gl;
using namespace glm;
using namespace globjects;

using widgetzeug::make_unique;

AnimationExample::AnimationExample(gloperate::ResourceManager & resourceManager, const std::string & relDataPath)
:   Painter("AnimationExample", resourceManager, relDataPath)
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
	// Drop-down menu to switch between Animation types
	auto animationTypes = addProperty<AnimationTypes>("Animation_Types", this,
		&AnimationExample::animationType,
		&AnimationExample::setAnimationType);

	animationTypes->setStrings({
		{ ParameterAnimation, "parameter animation" },
		{ VertexAnimation, "vertex animation" },
		{ RigAnimation, "rig animation" } 
	});

	animationTypes->setChoices({ ParameterAnimation, VertexAnimation, RigAnimation });

	// Time control
	auto TimeControllGroup = addGroup("TimeControll");

	TimeControllGroup->addProperty<bool>("control_time", this,
        &AnimationExample::timeControlled, &AnimationExample::setTimeControlled);
	
	auto setTime = TimeControllGroup->addProperty<float>("time", this,
		&AnimationExample::getControlledTime, &AnimationExample::setControlledTime);
	
	setTime->setOptions({
			{ "minimum", 0.0f },
			{ "maximum", 10.0f },
			{ "step", 0.1f }
	});

	auto VertexAnimationGroup = addGroup("VertexAnimation");
	// Drop-down menu to switch between Vertex Animations
	auto vertexAnimationOptions = VertexAnimationGroup->addProperty<VertexAnimationOptions>("Vertex_Animations", this,
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

// Getter and setter for the properties
bool AnimationExample::timeControlled() const{
	return m_timeControlled;
}

void AnimationExample::setTimeControlled(bool controlled){
	m_timeControlled = controlled;
	if (controlled){
		setControlledTime(m_timeCapability->time()); 
	}
}

float AnimationExample::getControlledTime() const{
	return m_controlledTime;	
}

void AnimationExample::setControlledTime(float newTime){
	m_controlledTime = newTime;
}

AnimationTypes AnimationExample::animationType() const{
	return m_currentAnimationType;
}

void AnimationExample::setAnimationType(const AnimationTypes & type){
	m_currentAnimationType = type;
	m_initializeNewAnimation = true;
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
	}
    m_currentVertexAnimation = animation;
	if (m_currentAnimationType == VertexAnimation){
		m_timeCapability->setLoopDuration(static_cast<float>(m_lastFrame - m_firstFrame + 1) / m_fps);
	}
}

void AnimationExample::initializeParameterAnimation(){

	gloperate::PolygonalGeometry* buddha = m_resourceManager.load<gloperate::PolygonalGeometry>(std::string("data/animationexample/buddha.obj"));
	auto ParObj = new ParameterAnimatedObject(new gloperate::PolygonalDrawable(*buddha));
	m_animation = std::unique_ptr<ParameterAnimatedObject>(ParObj);
	ParameterKeyframe keyframe;
	keyframe.time = 0.f;
	keyframe.translation = glm::vec3{ 0.f, 0.f, 0.f };
	keyframe.rotation = glm::quat();
	keyframe.scale = glm::vec3(1.f, 1.f, 1.f);
	m_animation->addKeyframe(keyframe);
    keyframe.time = 2.5f;
    keyframe.translation = glm::vec3{ 3.f, -3.f, 7.f };
	m_animation->addKeyframe(keyframe);
    keyframe.time = 5.f;
	keyframe.rotation = glm::quat{ 0.f, 0.7f, 0.f, 0.7f };
    m_animation->addKeyframe(keyframe);
    keyframe.time = 7.5f;
    keyframe.scale = glm::vec3(0.5f, 2.f, 1.f);
    m_animation->addKeyframe(keyframe);
    keyframe.time = 10.f;
    keyframe.translation = glm::vec3{ 0.f, 0.f, 0.f };
    keyframe.rotation = glm::quat();
    keyframe.scale = glm::vec3(1.f, 1.f, 1.f);
    m_animation->addKeyframe(keyframe);
    m_timeCapability->setLoopDuration(10.f);
}

void AnimationExample::setupProjection()
{
    static const auto zNear = 0.3f, zFar = 200.f, fovy = 50.f;
	
    m_projectionCapability->setZNear(zNear);
    m_projectionCapability->setZFar(zFar);
    m_projectionCapability->setFovy(radians(fovy));

    m_grid->setNearFar(zNear, zFar);
}

void AnimationExample::onInitialize()
{
    // Create program

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

    setAnimationType(ParameterAnimation);
	setVertexAnimation(STAND); 
	m_initializeNewAnimation = true;

	setTimeControlled(false);
	setControlledTime(0.0);
}

void AnimationExample::onPaint()
{
	// Dependend on displayed animation 
    if (m_initializeNewAnimation == true){
		switch (m_currentAnimationType) {
		case ParameterAnimation:
			m_cameraCapability->setEye(vec3(0.0, 0.0, 30.0));
			initializeParameterAnimation();
			break;
		case VertexAnimation:
			m_cameraCapability->setEye(vec3(100.0, 0.0, 0.0)); // adjust viewpoint to the size of the models 
			m_md2LoaderInstance = std::unique_ptr < md2Loader > {new md2Loader()};
            m_md2LoaderInstance->loadModel("data/animationexample/Samourai.md2");
			m_md2ModelDrawable = std::unique_ptr < FrameDrawable > {m_md2LoaderInstance->modelToGPU()};
			m_timeCapability->setLoopDuration(static_cast<float>(m_lastFrame - m_firstFrame + 1) / m_fps);
			break;
        case RigAnimation:
            m_cameraCapability->setEye(vec3(100.0,0.0,0.0));
            m_timeCapability->setLoopDuration(5.7f);
			m_program = new Program{};
			m_program->attach(
				Shader::fromFile(GL_VERTEX_SHADER, "data/animationexample/rigAnim.vert"),
				Shader::fromFile(GL_FRAGMENT_SHADER, "data/animationexample/rigAnim.frag")
				);

			m_transformLocation = m_program->getUniformLocation("transform");

            auto model = std::string("data/animationexample/boblampclean.md5mesh");
            auto anim = std::string("data/animationexample/boblampclean.md5anim");
            gloperate::Scene* guard = m_resourceManager.load<gloperate::Scene>(model);
            gloperate::Scene* scene = m_resourceManager.load<gloperate::Scene>(anim);
            m_animated = std::unique_ptr<RigAnimatedObject>{new RigAnimatedObject(guard, scene)};
			glClearColor(0.85f, 0.87f, 0.91f, 1.0f);
			break;
		}
		m_initializeNewAnimation = false;
	}

	if (m_timeControlled){
		m_currentTime = m_controlledTime;
	}
	else {
		m_currentTime = m_timeCapability->time();
	}
	

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
    //const auto eye = m_cameraCapability->eye();

    //m_grid->update(eye, transform);
    //m_grid->draw();
	
	switch (m_currentAnimationType){
	case ParameterAnimation:
		m_animation->draw(m_currentTime, transform);
		break;
	case VertexAnimation:
		m_md2ModelDrawable->draw(m_firstFrame, m_lastFrame, m_fps, m_currentTime, transform);
		break;
	case RigAnimation:
        //Accomodate model rotation
        glm::quat rotation;
        rotation = glm::rotate(rotation, static_cast<float>(M_PI)*0.5f,glm::vec3{0.0,0.0,1.0});
        rotation = glm::rotate(rotation, static_cast<float>(M_PI)*0.5f,glm::vec3{0.0,1.0,0.0});
        auto rotated = glm::mat4_cast(rotation);
        rotated = glm::translate(rotated, glm::vec3{0,9,-25});

        m_animated->draw(m_currentTime, transform * rotated);//m_timeCapability->time(), transform);
		break;
	}
	
    Framebuffer::unbind(GL_FRAMEBUFFER);
}
