#pragma once

#include <gloperate/primitives/PolygonalDrawable.h>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/primitives/AbstractDrawable.h>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>

#include <vector>

namespace globjects
{
	class Program;
}

class FrameDrawable : gloperate::AbstractDrawable
{
public:
	FrameDrawable() = default;
	FrameDrawable(const std::vector<gloperate::PolygonalGeometry> &  geometries);
	~FrameDrawable();
	void draw(int firstFrame, int lastFrame, int fps, float time, const glm::mat4& transform);
	virtual void draw();

protected:									
	gl::GLsizei m_size;																// Number of elements in m_indice 
	globjects::ref_ptr<globjects::Buffer>					m_indices;				// Index buffer 
	std::vector<globjects::ref_ptr<globjects::Buffer>>      m_frame_vertices;       // Vertex buffer 
	std::vector<globjects::ref_ptr<globjects::Buffer>>      m_frame_normals;        // Normal buffer (may be empty) 

	gl::GLint m_transformLocation;
	gl::GLint m_interpolationLocation;

private:
	globjects::ref_ptr<globjects::Program> m_program;
};

