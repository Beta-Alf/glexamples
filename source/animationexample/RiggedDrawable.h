#pragma once

#include <vector>
#include <map>
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
    virtual ~RiggedDrawable() = default;



    std::map<std::string,size_t>               m_boneMapping;         /**< Mapping from string to boneIndex > */
    std::vector<glm::mat4>                     m_bindTransforms;      /**< bind Matrices per bone> */

protected:
    globjects::ref_ptr<globjects::Buffer>      m_vertexBoneIndices;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBoneWeights;
};
