#include "ParameterAnimatedObject.h"
#include <algorithm>
#include <gloperate/primitives/AbstractDrawable.h>
#include <globjects/Program.h>

typedef ParameterKeyframe Frame;

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

void ParameterAnimatedObject::draw(float time)
{

    if(m_keyframes.empty())
        return;


	Frame& before = m_keyframes[0]; 
	Frame& after = m_keyframes[0];
    for(auto& curFrame : m_keyframes)
    {
        if(curFrame.time == time)
        {
            before=after=curFrame;
            break;
        }
        if(curFrame.time < before.time)
        {
            before = curFrame;
        }
        else
        {
            after = curFrame;
            break;
        }
    }

    auto transform = interpolate(before, after, time);

    m_program->use();

    //m_animated->draw();
}

glm::mat4 ParameterAnimatedObject::interpolate(ParameterKeyframe First, ParameterKeyframe Second, float time)
{
    float dist = Second.time - First.time;
    float pos = time - First.time; // The distance to the first frame
    float normPos = pos/dist;	// Normalized position between 0 an 1
    glm::vec3 translation = glm::mix(First.translation, Second.translation, normPos);
    glm::quat rotation = glm::mix(First.rotation, Second.rotation, normPos);
    glm::vec3 scale = glm::mix(First.scale, Second.scale, normPos);

	return glm::mat4();
}
