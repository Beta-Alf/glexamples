#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <md2Loader.h>

#include <FrameDrawable.h>

class RigAnimatedObject;

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
    AnimationExample(gloperate::ResourceManager & resourceManager, const std::string & relDataPath);
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
    globjects::ref_ptr<globjects::Program> m_program;
    gl::GLint m_transformLocation;

    /* members */
    globjects::ref_ptr<gloperate::AdaptiveGrid> m_grid;

	std::unique_ptr<ParameterAnimatedObject> m_animation;
	std::unique_ptr<RigAnimatedObject> m_animated;
	
	AnimationTypes m_currentAnimationType;
	bool m_initializeNewAnimation;

	float m_currentTime;
	bool m_timeControlled;
	float m_controlledTime;

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

	bool timeControlled() const;
	void setTimeControlled(bool controlled);
	float getControlledTime() const;
	void setControlledTime(float newtime);


	AnimationTypes animationType() const;
	void setAnimationType(const AnimationTypes & type);


private:
	int m_maxDistance;
};
