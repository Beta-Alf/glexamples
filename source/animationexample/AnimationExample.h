#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <md2Loader.h>

#include <FrameDrawable.h>

class ParameterAnimatedObject;

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AdaptiveGrid;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
	class AbstractVirtualTimeCapability;
}

enum VertexAnimationOptions { STAND, RUN, JUMP, SALUTE };
enum AnimationTypes { ParameterAnimation, VertexAnimation, RigAnimation};

class AnimationExample : public gloperate::Painter
{
public:
    AnimationExample(gloperate::ResourceManager & resourceManager);
    virtual ~AnimationExample();

    void setupProjection();
	void setupPropertyGroup();

protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;

	void initializeParameterAnimation();

    /* capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;
	gloperate::AbstractVirtualTimeCapability * m_timeCapability;

    /* members */
    globjects::ref_ptr<gloperate::AdaptiveGrid> m_grid;
	std::unique_ptr<ParameterAnimatedObject> m_animation;
	
	AnimationTypes m_currentAnimationType;

	float m_currentTime;
	
	//Vertex Animation specifieres
	int m_firstFrame;
	int m_lastFrame;
	int m_fps;
	float m_interpolationFactor;
	VertexAnimationOptions m_currentVertexAnimation;
	md2Loader md2LoaderInstance;
	FrameDrawable md2ModelDrawable;

	//Property-functions
	VertexAnimationOptions vertexAnimation() const;
	void setVertexAnimation(const VertexAnimationOptions & animation);

	AnimationTypes animationType() const;
	void setAnimationType(const AnimationTypes & type);

private:
	int m_maxDistance;
};
