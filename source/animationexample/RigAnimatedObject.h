#pragma once

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Uniform.h>

#include <gloperate/primitives/Scene.h>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>
#include <memory>


using gloperate::BoneNode;

class RiggedDrawable;

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AbstractDrawable;
    class PolygonalDrawable;
}


class RigAnimatedObject
{
public:
    RigAnimatedObject(gloperate::Scene *animated, gloperate::Scene *animations);
    void draw(float time, const glm::mat4& viewProjection);
    std::vector<glm::mat4> interpolate(float t, std::shared_ptr<RiggedDrawable> curAnimated);

private:
    void interpolateRecursively(const gloperate::BoneNode& Bone, float t, std::vector<glm::mat4>& into, glm::mat4 parentTransform, std::shared_ptr<RiggedDrawable> curAnimated);

    int numBones;
    globjects::ref_ptr<globjects::Program> m_program;
    std::vector<std::shared_ptr<RiggedDrawable>> m_animated;
    std::shared_ptr<gloperate::Scene> m_animations;
    globjects::ref_ptr<globjects::Uniform<std::vector<glm::mat4>>> m_bonesUniform;
    gl::GLint m_transformLocation;
    gl::GLint m_frameLocation;
    gl::GLint m_boneIDLocation;
    gl::GLint m_weightsLocation;
};
