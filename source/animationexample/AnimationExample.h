#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <md2Loader.h>

#include <FrameDrawable.h>

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AdaptiveGrid;
    class Icosahedron;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
	class AbstractVirtualTimeCapability;
}

enum VertexAnimationOptions { STAND, RUN, JUMP, SALUTE };

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



    /* members */
    globjects::ref_ptr<gloperate::AdaptiveGrid> m_grid;
    globjects::ref_ptr<gloperate::Icosahedron> m_icosahedron;
    globjects::ref_ptr<globjects::Program> m_program;
    gl::GLint m_transformLocation;
	gl::GLint m_timeLocation;
	gl::GLint m_interpolationLocation;
	float m_currentTime;
	float m_oldTime;
	int m_currentFrame;
	int m_oldFrame;
	int m_Offset;
	int m_firstFrame;
	int m_lastFrame;
	int m_fps;
	float m_interpolationFactor;
	VertexAnimationOptions m_currentVertexAnimation;

	VertexAnimationOptions vertexAnimation() const;
	void setVertexAnimation(VertexAnimationOptions animation);

	md2Loader md2LoaderInstance;
	FrameDrawable md2ModelDrawable;
	

private:
	int m_maxDistance;
};
