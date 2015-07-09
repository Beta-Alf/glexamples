#include "AnimationExample.h"

#include<iostream>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>
#include <globjects/Program.h>

#include <widgetzeug/make_unique.hpp>

#include <gloperate/base/RenderTargetType.h>

#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>

#include <gloperate/primitives/AdaptiveGrid.h>
#include <gloperate/primitives/Icosahedron.h>


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
,	m_maxDistance(1)
{
	setupPropertyGroup();
}

AnimationExample::~AnimationExample() = default;

void AnimationExample::setupPropertyGroup()
{

	auto property = addProperty<int>("MaximumDistance", this,
		&AnimationExample::maxDistance,
		&AnimationExample::setMaxDistance);

	property->setOptions({
		{ "minimum", 0 },
		{ "maximum", 255 },
		{ "step", 1 } });

	auto vertexAnimationOptions = addProperty<VertexAnimationOptions>("Vertex_Animations", this,
		&AnimationExample::vertexAnimation,
		&AnimationExample::setVertexAnimation);

	vertexAnimationOptions->setChoices({ STAND, RUN, JUMP, SALUTE });
	vertexAnimationOptions->setStrings({
		{ STAND, "stand" },
		{ RUN, "run" },
		{ JUMP, "jump" },                  
		{ SALUTE, "salute" } 
	});
}

VertexAnimationOptions AnimationExample::vertexAnimation() const{
	return m_currentVertexAnimation;
}

void AnimationExample::setVertexAnimation(VertexAnimationOptions animation){
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
	m_oldFrame = m_firstFrame;
	m_currentFrame = m_firstFrame + 1;
}

int AnimationExample::maxDistance() const
{
	return m_maxDistance;
}

void AnimationExample::setMaxDistance(int maxDistance)
{
	m_maxDistance = maxDistance;
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

    m_icosahedron = new gloperate::Icosahedron{3};

    m_program = new Program{};
    m_program->attach(
        Shader::fromFile(GL_VERTEX_SHADER, "data/animationexample/vertexanimation.vert"),
        Shader::fromFile(GL_FRAGMENT_SHADER, "data/animationexample/vertexanimation.frag")
    );

    m_transformLocation = m_program->getUniformLocation("transform");
	m_interpolationLocation = m_program->getUniformLocation("interpol");

    glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

    setupProjection();

	m_cameraCapability->setEye(vec3(100.0, 0.0, 0.0));

	md2LoaderInstance = md2Loader();
	md2LoaderInstance.loadModel("data/animationexample/Samourai.md2");
	md2ModelDrawable = md2LoaderInstance.modelToGPU();
	m_timeCapability->setLoopDuration(6);
	m_oldTime = 0;
	m_Offset = 0;
	m_interpolationFactor = 0;
	setVertexAnimation(STAND);
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

    const auto objectTransform = transform * glm::translate(glm::mat4(), glm::vec3(m_maxDistance*0.1f, 0.f, 0.2f) * m_currentTime);
    m_program->use();
    m_program->setUniform(m_transformLocation, transform);

	//calculate which frame to draw and how much to interpolate
	int numFramesAnim = m_lastFrame - m_firstFrame + 1; // the number of frames in the animation
	float delta = 1.0 / (float)m_fps; // how much time between frames
	if (m_currentTime < m_oldTime){
		m_oldTime -= 6; //because I set the Duration to 100. It's ugly, I know.
	}
	if (m_currentTime - m_oldTime >= delta){ 
		m_oldTime = m_currentTime;
		m_oldFrame = m_currentFrame;
		m_Offset = (m_Offset + 1) % numFramesAnim; 
		m_currentFrame = m_firstFrame + m_Offset;
	}

	m_interpolationFactor = (m_currentTime - m_oldTime) * m_fps;
	m_program->setUniform(m_interpolationLocation, m_interpolationFactor);
	
	md2ModelDrawable.draw(m_oldFrame, m_currentFrame);
    //m_icosahedron->draw();
	

    m_program->release();

    Framebuffer::unbind(GL_FRAMEBUFFER);
}
