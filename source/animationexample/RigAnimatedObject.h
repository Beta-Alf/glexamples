/*#pragma once

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
    class PolygonalDrawable;
}

struct RigKeyframe
{
    float time;
};



class RigAnimatedObject
{
public:
    RigAnimatedObject(gloperate::PolygonalDrawable* animated);
    void addKeyframe(RigKeyframe Keyframe);
    void draw(float time, const glm::mat4& viewProjection);
    RigKeyframe interpolate(const RigKeyframe& First,const RigKeyframe& Second, float t);

private:
    BoneTransforms interpolateRecursively(const BoneState& First, const BoneState& Second, float normPos);
    int numBones;
    std::vector<RigKeyframe> m_keyframes;
    globjects::ref_ptr<globjects::Program> m_program;
    std::shared_ptr<gloperate::PolygonalGeometry> m_animated;
    gl::GLint m_transformLocation;
    gl::GLint m_frameLocation;
    gl::GLint m_boneIDLocation;
    gl::GLint m_weightsLocation;
};


*/
