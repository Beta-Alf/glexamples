#define _USE_MATH_DEFINES
#include <math.h>

#include <RigAnimatedObject.h>

#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/Program.h>

#include <gloperate/primitives/AbstractDrawable.h>
#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/primitives/PolygonalGeometry.h>

#include <gloperate-assimp/AssimpSceneLoader.h>

#include <RiggedDrawable.h>

using namespace gl;
using namespace globjects;

RigAnimatedObject::RigAnimatedObject(gloperate::Scene *animated, gloperate::Scene *animations)
{

    for(gloperate::PolygonalGeometry* curDrawable : animated->meshes())
    {
        m_animated.push_back(std::shared_ptr<RiggedDrawable>{new RiggedDrawable(*curDrawable)});
    }
    m_animations = std::shared_ptr<gloperate::Scene>(animations);

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

    for(auto& curDrawable : m_animated)
    {
        std::vector<glm::mat4> current = interpolate(time, curDrawable);

        m_program->use();
        m_program->setUniform(m_transformLocation,  viewProjection);

        m_bonesUniform->set(current);

        curDrawable->draw();
    }

    m_program->release();
}

std::vector<glm::mat4> RigAnimatedObject::interpolate(float t, std::shared_ptr<RiggedDrawable> curAnimated)
{
    std::vector<glm::mat4> boneTransforms{curAnimated->m_bindTransforms.size()};

    interpolateRecursively(*(m_animations->boneHierarchy()), t, boneTransforms,
                           glm::mat4(), curAnimated);

    return boneTransforms;
}

void RigAnimatedObject::interpolateRecursively(const BoneNode& Bone,
                                               float t,
                                               std::vector<glm::mat4> &into,
                                               glm::mat4 parentTransform,
                                               std::shared_ptr<RiggedDrawable> curAnimated)

{

    //Of course sometimes the model and the animation have different views about the start of the hierarchy
    //if(Bone.boneName == "<MD5_Hierarchy>")
    //{
    //    interpolateRecursively(Bone.children[0],t,into,parentTransform,curAnimated);
    //    return;
    //}
    gloperate::RigAnimationTrack* Animation = m_animations->animations()[0];
    gloperate::Channel* BoneChannel = nullptr;
    for(size_t i = 0; i < Animation->boneChannels.size(); i++)
    {
        if(Animation->boneChannels.at(i).boneName == Bone.boneName)
        {
            BoneChannel = &(Animation->boneChannels[i]);
        }
    }
    if(!BoneChannel)
    {
        interpolateRecursively(Bone.children[0],t,into,parentTransform,curAnimated);
        return;
    }

    float ticks = t * Animation->ticksPerSecond;

    //Find the corresponding Translate key and interpolate
    glm::vec3 translation;
    if(BoneChannel->translation.size() > 1)
    {
        gloperate::TranslationKey first,second;
        for(size_t i = 0; i < BoneChannel->translation.size(); i++)
        {
            if(ticks < BoneChannel->translation[i].time)
            {
                second = BoneChannel->translation[i];
            }
            else
            {
                first = BoneChannel->translation[i];
            }
        }
        float dist = second.time - first.time;
        float pos = t - first.time; // The distance to the first frame
        float normPos = pos/dist;	// Normalized position between 0 an 1
        normPos = normPos < 0 ? 0 : normPos;
        normPos = normPos > 1 ? 1 : normPos;
        translation = glm::mix(first.translation,second.translation,normPos);
    }
    else
    {
         if(BoneChannel->translation.size() != 0)
         {
            translation = BoneChannel->translation[0].translation;
         }
    }

    glm::quat rotation;
    if(BoneChannel->rotation.size() > 1)
    {
        gloperate::RotationKey first,second;
        for(size_t i = 0; i < BoneChannel->rotation.size(); i++)
        {
            if(ticks < BoneChannel->rotation[i].time)
            {
                second = BoneChannel->rotation[i];
            }
            else
            {
                first = BoneChannel->rotation[i];
            }
        }
        float dist = second.time - first.time;
        float pos = t - first.time; // The distance to the first frame
        float normPos = pos/dist;	// Normalized position between 0 an 1
        normPos = normPos < 0 ? 0 : normPos;
        normPos = normPos > 1 ? 1 : normPos;
        rotation = glm::mix(first.rotation,second.rotation,normPos);
    }
    else
    {
         if(BoneChannel->rotation.size() != 0)
         {
            rotation = BoneChannel->rotation[0].rotation;
         }
    }

    glm::vec3 scale;
    if(BoneChannel->scale.size() > 1)
    {
        gloperate::ScaleKey first,second;
        for(size_t i = 0; i < BoneChannel->scale.size(); i++)
        {
            if(ticks < BoneChannel->scale[i].time)
            {
                second = BoneChannel->scale[i];
            }
            else
            {
                first = BoneChannel->scale[i];
            }
        }
        float dist = second.time - first.time;
        float pos = t - first.time; // The distance to the first frame
        float normPos = pos/dist;	// Normalized position between 0 an 1
        normPos = normPos < 0 ? 0 : normPos;
        normPos = normPos > 1 ? 1 : normPos;
        scale = glm::mix(first.scale,second.scale,normPos);
    }
    else
    {
         if(BoneChannel->scale.size() != 0)
         {
            scale = BoneChannel->scale[0].scale;
         }
    }

    glm::mat4 transform;
    transform = glm::translate(transform, translation);
    //transform = glm::scale(transform, scale);
    transform = transform * glm::mat4_cast(rotation);
    transform = parentTransform * transform;

    if(curAnimated->m_boneMapping.count(BoneChannel->boneName) == 1)
    {
        auto boneIndex = curAnimated->m_boneMapping.at(BoneChannel->boneName);
        into[boneIndex] = transform * curAnimated->m_bindTransforms[boneIndex];
    }

    for (auto& child : Bone.children) {

        interpolateRecursively(child, t, into, transform, curAnimated);
    }
}
