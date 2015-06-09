#pragma once

#include <globjects/base/ref_ptr.h>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>

namespace globjects
{
    class Program;
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
    ParameterAnimatedObject() = default;
    void addKeyframe(ParameterKeyframe Keyframe);
    void draw(float time);
    glm::mat4 interpolate(ParameterKeyframe First, ParameterKeyframe Second, float t);

    std::vector<ParameterKeyframe> m_keyframes;
    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<globjects::AbstractDrawable> m_animated;

};
