/*#include <RigAnimatedObject.h>
#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/Program.h>

#include <gloperate/primitives/AbstractDrawable.h>
#include <gloperate/primitives/PolygonalDrawable.h>

using namespace gl;
using namespace globjects;
using Frame = RigKeyframe;

RigAnimatedObject::RigAnimatedObject(gloperate::PolygonalDrawable *animated)
{
    m_animated = std::unique_ptr<gloperate::PolygonalDrawable>(animated);

    m_program = new Program{};
    m_program->attach(
        Shader::fromFile(GL_VERTEX_SHADER, "data/animationexample/paramAnim.vert"),
        Shader::fromFile(GL_FRAGMENT_SHADER, "data/animationexample/icosahedron.frag")
        );

    m_transformLocation = m_program->getUniformLocation("transform");
    m_frameLocation = m_program->getUniformLocation("frame");
    m_boneIDLocation = m_program->getUniformLocation("BoneIDs");
    m_weightsLocation = m_program->getUniformLocation("Weights");
}

void RigAnimatedObject::addKeyframe(Frame Keyframe)
{

    auto compare = [](const Frame& frame1, const Frame& frame2)
        {return frame1.time < frame2.time;};

    if(m_keyframes.empty())
    {
        m_keyframes.push_back(Keyframe);
    }
    else
    {
        m_keyframes.insert(
                    std::upper_bound(m_keyframes.begin(),
                                     m_keyframes.end(),
                                     Keyframe,
                                     compare),
                    Keyframe);
    }
}

void RigAnimatedObject::draw(float time, const glm::mat4& viewProjection)
{
    Frame current;

    if(!m_keyframes.empty())
    {
        Frame before = m_keyframes[0];
        Frame after = m_keyframes[0];
        for(auto& curFrame : m_keyframes)
        {
            if(curFrame.time == time)
            {
                before=after=curFrame;
                break;
            }
            if(curFrame.time < time && curFrame.time > before.time)
            {
                before = curFrame;
            }
            if(curFrame.time > time)
            {
                after = curFrame;
                break;
            }
        }

        current = interpolate(before, after, time);
    }

    while(glGetError() != GL_NO_ERROR);

    m_program->use();
    m_program->setUniform(m_transformLocation, viewProjection);
    m_program->setUniform(m_frameLocation, );

    m_animated->draw();

    m_program->release();

}

BoneTransforms RigAnimatedObject::interpolate(RigKeyframe First, RigKeyframe Second, float t)
{
    float dist = Second.time - First.time;
    float pos = time - First.time; // The distance to the first frame
    float normPos = pos/dist;	// Normalized position between 0 an 1
    normPos = normPos < 0 ? 0 : normPos;
    normPos = normPos > 1 ? 1 : normPos;

    auto boneTransforms = interpolateRecursively(First.root, Second.root, normPos); //calculate the current Bone transforms

    return boneTransforms;

}

BoneTransforms RigAnimatedObject::interpolateRecursively(const BoneState& First, const BoneState& Second, float normPos)
{
    BoneTransforms boneTransform;
    //Prefix Traversal to flatten the BoneState tree into the BoneTransforms vector
    //Calculate the transform of the current nodes from First and Second

    glm::vec3 translation = glm::mix(First.translation, Second.translation, normPos);
    glm::quat rotation = glm::mix(First.rotation, Second.rotation, normPos);
    glm::vec3 scale = glm::mix(First.scale, Second.scale, normPos);
    glm::mat4 transform;
    transform = glm::translate(transform, translation);
    transform = glm::scale(transform, scale);
    transform = transform * glm::mat4_cast(rotation);
    boneTransform.push_back(transform);

    assert(First.children.size() == Second.children.size());

    //Calculate the transform vectors of all subtrees
    //This is really imperformant right now but for under 64 bones it should be okay
    for(int i = 0; i < First.children.size(); i++)
    {
        BoneTransforms childTransform = interpolateRecursively(First.children[i], Second.children[i], normPos);
        boneTransform.insert(boneTransform.end(),childTransform.begin(),childTransform.end());
    }

    return boneTransform;
}
*/
