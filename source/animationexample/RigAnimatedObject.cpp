#include <RigAnimatedObject.h>

#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/Program.h>

#include <gloperate/primitives/AbstractDrawable.h>
#include <gloperate/primitives/PolygonalDrawable.h>

#include <gloperate-assimp/AssimpSceneLoader.h>

#include <RiggedDrawable.h>

using namespace gl;
using namespace globjects;

RigAnimatedObject::RigAnimatedObject(RiggedDrawable *animated)
{
    m_animated = std::unique_ptr<RiggedDrawable>(animated);

    m_program = new Program{};
    m_program->attach(
        Shader::fromFile(GL_VERTEX_SHADER, "data/animationexample/rigAnim.vert"),
        Shader::fromFile(GL_FRAGMENT_SHADER, "data/animationexample/rigAnim.frag"));

    m_transformLocation = m_program->getUniformLocation("transform");
    m_bonesUniform = new globjects::Uniform<std::vector<glm::mat4>>{"bones"};
    m_program->addUniform(m_bonesUniform);
}

void RigAnimatedObject::draw(float time, const glm::mat4 &viewProjection)
{
    std::vector<glm::mat4> current = interpolate(time);

    m_program->use();
    m_program->setUniform(m_transformLocation, viewProjection);
    m_bonesUniform->set(current);

    m_animated->draw();

    m_program->release();
}

std::vector<glm::mat4> RigAnimatedObject::interpolate(float t)
{
    t=t;
    std::vector<glm::mat4> boneTransforms{m_animated->m_bindTransforms.size()};
    /*
    float dist = second.time - first.time;
    float pos = t - first.time; // The distance to the first frame
    float normPos = pos / dist; // Normalized position between 0 an 1
    normPos = normPos < 0 ? 0 : normPos;
    normPos = normPos > 1 ? 1 : normPos;

    interpolateRecursively(first.root, second.root, normPos, boneTransforms,
                           glm::mat4()); // calculate the current Bone transforms
    */
    return boneTransforms;
}

void RigAnimatedObject::loadAnimationScene(std::string Filename)
{
    gloperate_assimp::AssimpSceneLoader Loader;

    m_animations = std::shared_ptr<gloperate::Scene>{Loader.load(Filename, std::function<void(int,int)>())};

}

// pass in the transform so far
/*
void RigAnimatedObject::interpolateRecursively(const BoneNode &First,
                                               const BoneNode &Second, float normPos,
                                               std::vector<glm::mat4> &into,
                                               glm::mat4 parentTransform)
{
    assert(First.boneName == Second.boneName);
    std::vector<glm::mat4> Bones;
    // Prefix Traversal to flatten the BoneState tree into the BoneTransforms vector
    // Calculate the transform of the current nodes from First and Second

    glm::vec3 translation = glm::mix(First.translation, Second.translation, normPos);
    glm::quat rotation = glm::mix(First.rotation, Second.rotation, normPos);
    glm::vec3 scale = glm::mix(First.scale, Second.scale, normPos);
    glm::mat4 transform;
    transform = glm::translate(transform, translation);
    transform = glm::scale(transform, scale);
    transform = transform * glm::mat4_cast(rotation);
    transform = parentTransform * transform;

    auto boneIndex = m_animated->m_boneMapping.at(First.boneName);
    into[boneIndex] = transform;

    // Calculate the transform vectors of all subtrees
    // This is really imperformant right now but for under 64 bones it should be okay
    for (size_t i = 0; i < First.children.size(); i++) {
        interpolateRecursively(First.children[i], Second.children[i], normPos, into, transform);
    }
}

*/
