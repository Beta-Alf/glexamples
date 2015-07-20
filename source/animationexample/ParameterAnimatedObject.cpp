#include <iostream>
#include "ParameterAnimatedObject.h"
#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/Program.h>

#include <gloperate/primitives/AbstractDrawable.h>


using namespace gl;
using namespace globjects;

typedef ParameterKeyframe Frame;

ParameterAnimatedObject::ParameterAnimatedObject(gloperate::AbstractDrawable *animated)
{
    m_animated = std::unique_ptr<gloperate::AbstractDrawable>(animated);

    m_program = new Program{};
    m_program->attach(
		Shader::fromFile(GL_VERTEX_SHADER, "data/animationexample/paramAnim.vert"),
		Shader::fromFile(GL_FRAGMENT_SHADER, "data/animationexample/icosahedron.frag")
        );

    m_transformLocation = m_program->getUniformLocation("transform");
}

void ParameterAnimatedObject::addKeyframe(ParameterKeyframe Keyframe)
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

void ParameterAnimatedObject::draw(float time, const glm::mat4& viewProjection)
{
    glm::mat4 model;

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

        model = interpolate(before, after, time);
    }
    while(glGetError() != GL_NO_ERROR);

	m_program->use();
    m_program->setUniform(m_transformLocation, viewProjection * model);

    m_animated->draw();

    m_program->release();

    while(auto a = glGetError() != GL_NO_ERROR)
    {
        std::cout << "Error detected" << std::endl;
        std::cout << a << std::endl;
    }
}

glm::mat4 ParameterAnimatedObject::interpolate(Frame First, Frame Second, float time)
{
	float normPos = 0.0;
	if (First.time != Second.time){
		float dist = Second.time - First.time;
		float pos = time - First.time; // The distance to the first frame
		normPos = pos / dist;	// Normalized position between 0 an 1
		normPos = normPos < 0 ? 0 : normPos;
		normPos = normPos > 1 ? 1 : normPos;
	}
    glm::vec3 translation = glm::mix(First.translation, Second.translation, normPos);
    glm::quat rotation = glm::mix(First.rotation, Second.rotation, normPos);
    glm::vec3 scale = glm::mix(First.scale, Second.scale, normPos);
    glm::mat4 transform;
    transform = glm::translate(transform, translation);
    transform = glm::scale(transform, scale);
    transform = transform * glm::mat4_cast(rotation);
    return transform;
}
