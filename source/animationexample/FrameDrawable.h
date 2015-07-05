#pragma once

#include <gloperate/primitives/PolygonalDrawable.h>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/primitives/AbstractDrawable.h>

#include <vector>

class FrameDrawable : gloperate::AbstractDrawable
{
public:
	FrameDrawable() = default;
	FrameDrawable(const std::vector<gloperate::PolygonalGeometry> &  geometries);
	~FrameDrawable();
	void draw(float time);
	virtual void draw();

protected:

	int m_numFrames;									
	gl::GLsizei m_size;																	/**< Number of elements (m_indices) */
	globjects::ref_ptr<globjects::Buffer>					m_indices;					  /**< Index buffer */
	std::vector<globjects::ref_ptr<globjects::Buffer>>      m_frame_vertices;            /**< Vertex buffer */
	std::vector<globjects::ref_ptr<globjects::Buffer>>      m_frame_normals;             /**< Normal buffer (may be empty) */
	//globjects::ref_ptr<globjects::Buffer>      m_textureCoordinates;  /**< Texture coordinate buffer (may be empty) */
		
};

