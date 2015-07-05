#include "AnimationExample.h"

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
    static const auto zNear = 0.1f, zFar = 100.f, fovy = 50.f;
	
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
        Shader::fromFile(GL_VERTEX_SHADER, "data/animationexample/icosahedron.vert"),
        Shader::fromFile(GL_FRAGMENT_SHADER, "data/animationexample/icosahedron.frag")
    );

    m_transformLocation = m_program->getUniformLocation("transform");

    glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

    setupProjection();

	md2LoaderInstance = md2Loader();
	md2LoaderInstance.loadModel("data/animationexample/Samourai.md2");
	md2ModelDrawable = md2LoaderInstance.modelToGPU();
}

void AnimationExample::onPaint()
{
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

    const auto objectTransform = transform * glm::translate(glm::mat4(), glm::vec3(m_maxDistance*0.1f, 0.f, 0.2f) * m_timeCapability->time());
    m_program->use();
    m_program->setUniform(m_transformLocation, transform);

    //m_icosahedron->draw();
	md2ModelDrawable.draw(m_timeCapability->time());

    m_program->release();

    Framebuffer::unbind(GL_FRAMEBUFFER);
}
