#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

//#include <RiggedDrawable.h>

class ParameterAnimatedObject;

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AdaptiveGrid;
    class Icosahedron;
    class PolygonalDrawable;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
	class AbstractVirtualTimeCapability;
}

class AnimationExample : public gloperate::Painter
{
public:
    AnimationExample(gloperate::ResourceManager & resourceManager);
    virtual ~AnimationExample();

    void setupProjection();
	void setupPropertyGroup();
	
	int maxDistance() const;
	void setMaxDistance(int maxDistance);

protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;

protected:
    /* capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;
	gloperate::AbstractVirtualTimeCapability * m_timeCapability;
    globjects::ref_ptr<globjects::Program> m_program;
    gl::GLint m_transformLocation;

    /* members */
    globjects::ref_ptr<gloperate::AdaptiveGrid> m_grid;
    std::unique_ptr<gloperate::PolygonalDrawable> m_animation;

private:
	int m_maxDistance;
};
