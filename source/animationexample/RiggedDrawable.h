#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/primitives/PolygonalDrawable.h>

namespace globjects
{
    class Buffer;
}

class RiggedDrawable : public gloperate::PolygonalDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] geometry
    *    CPU mesh representation
    *
    *  @remarks
    *    The geometry is only used once to generate the mesh representation
    *    on the GPU and not used afterwards.
    */
    RiggedDrawable(const gloperate::PolygonalGeometry & geometry);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RiggedDrawable();

    /**
    *  @brief
    *    Draw geometry
    *
    *  @remarks
    *    The geometry is drawn as an indexed geometry of type GL_TRIANGLES.
    */
    //virtual void draw() override;


    std::vector<std::string>                   m_boneMapping;         /**< Mapping from string to boneIndex > */
    std::vector<glm::mat4>                     m_bindTransforms;      /**< bind Matrices per bone> */

protected:
    globjects::ref_ptr<globjects::Buffer>      m_vertexBoneIndices;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBoneWeights;
};
