#pragma once

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>
#include <memory>

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AbstractDrawable;
}

struct ParameterKeyframe
{
    float time;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::quat rotation;
};

class ParameterAnimatedObject
{
public:
    ParameterAnimatedObject(gloperate::AbstractDrawable* animated);
    void addKeyframe(ParameterKeyframe Keyframe);
	void draw(float time, const glm::mat4& viewProjection);
    glm::mat4 interpolate(ParameterKeyframe First, ParameterKeyframe Second, float t);

private:
    std::vector<ParameterKeyframe> m_keyframes;
    globjects::ref_ptr<globjects::Program> m_program;
    std::shared_ptr<gloperate::AbstractDrawable> m_animated;
	gl::GLint m_transformLocation;
};
