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
        auto compare =
        m_keyframes.insert(
                    std::upper_bound(m_keyframes.begin(),
                                     m_keyframes.end(),
                                     Keyframe,
                                     compare),
                    Keyframe);
    }
}

void ParameterAnimatedObject::draw(double time)
{

    if(m_keyframes.empty)
        return;


    Frame& before, after;
    before = after = m_keyframes[0];
    for(auto& curFrame : m_keyframes)
    {
        if(curFrame.time == time)
        {
            before=after=curFrame;
            break;
        }
        if(curFrame.time < before)
        {
            before = curFrame;
        }
        else
        {
            after = curFrame;
            break;
        }
    }

    auto transform = interpolate(before, after, t);

    m_program->use();

    m_animated->draw();
}

glm::mat4 ParameterAnimatedObject::interpolate(ParameterKeyframe First, ParameterKeyframe Second, double time)
{
    double dist = Second.time - First.time;
    double pos = time - First.time; // The distance to the first frame
    double normPos = pos/dist;	// Normalized position between 0 an 1
    glm::vec3 translation = glm::lerp(First.translation, Second.translation, normPos);
    glm::quat rotation = glm::lerp(First.rotation, Second.rotation, normPos);
    glm::vec3 scale = glm::lerp(First.scale, Second.scale, normPos);

}
