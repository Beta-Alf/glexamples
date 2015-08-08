#include "RiggedDrawable.h"

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/primitives/PolygonalGeometry.h>

using namespace gl;

RiggedDrawable::RiggedDrawable(const gloperate::PolygonalGeometry &geometry)
    : gloperate::PolygonalDrawable(geometry)
{
    if (geometry.isRigged())
    {
        m_vertexBoneIndices = new globjects::Buffer;
        m_vertexBoneIndices->setData(geometry.vertexBoneIndices(), GL_STATIC_DRAW);

        m_vertexBoneWeights = new globjects::Buffer;
        m_vertexBoneWeights->setData(geometry.vertexBoneWeights(), GL_STATIC_DRAW);

        m_bindTransforms = geometry.bindTransforms();
        m_boneMapping = geometry.boneMapping();
    }

    m_vao->bind();

    if (geometry.isRigged())
    {
        auto vertexBinding = m_vao->binding(3);
        vertexBinding->setAttribute(3);
        auto length = sizeof(glm::ivec4);
        length = length;
        vertexBinding->setBuffer(m_vertexBoneIndices,0,sizeof(glm::ivec4));
        vertexBinding->setIFormat(4, gl::GL_INT, 0);
        m_vao->enable(3);

        vertexBinding = m_vao->binding(4);
        vertexBinding->setAttribute(4);
        vertexBinding->setBuffer(m_vertexBoneWeights,0,sizeof(glm::vec4));
        vertexBinding->setFormat(4, gl::GL_FLOAT);
        m_vao->enable(4);
    }

    m_vao->unbind();
}
