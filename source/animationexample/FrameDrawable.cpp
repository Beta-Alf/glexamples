#include "FrameDrawable.h"

#include <glm/glm.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/primitives/PolygonalGeometry.h>

FrameDrawable::FrameDrawable(const std::vector<gloperate::PolygonalGeometry> & geometries)
{
	//indices are the same for every frame
	m_indices->setData(geometries[0].indices(), gl::GL_STATIC_DRAW);

	numFrames = geometries.size();
	for (int i = 0; i < numFrames; i++){

		// Save number of elements in index buffer
		int m_size = static_cast<gl::GLsizei>(geometries[i].indices().size());

		// Create and copy vertex buffer
		globjects::ref_ptr<globjects::Buffer> m_vertices = new globjects::Buffer;
		m_vertices->setData(geometries[i].vertices(), gl::GL_STATIC_DRAW);

		// Create and copy normal buffer
		globjects::ref_ptr<globjects::Buffer> m_normals;
		if (geometries[i].hasNormals())
		{
			m_normals = new globjects::Buffer;
			m_normals->setData(geometries[i].normals(), gl::GL_STATIC_DRAW);
		}

		//put Data into frame_vectors
		frame_normals.push_back(m_normals);
		frame_sizes.push_back(m_size);
		frame_vertices.push_back(m_vertices);
	}
}


FrameDrawable::~FrameDrawable()
{
}

void FrameDrawable::draw(){
	this->draw(0);
}

void FrameDrawable::draw(float time){
	time *= 10; //for the moment 10 fps 
	int frame1 = int(time) % numFrames;
	float interpolationFactor = time - (int)time;

	// Create vertex array object
	m_vao = new globjects::VertexArray;
	m_vao->bind();

	//bind indices
	m_indices->bind(gl::GL_ELEMENT_ARRAY_BUFFER);
	
	//bind frame1
	auto vertexBinding = m_vao->binding(0);
	vertexBinding->setAttribute(0);
	vertexBinding->setBuffer(frame_vertices[frame1], 0, sizeof(glm::vec3));
	vertexBinding->setFormat(3, gl::GL_FLOAT);
	m_vao->enable(0);

	if (!frame_normals.empty())
	{
		vertexBinding = m_vao->binding(1);
		vertexBinding->setAttribute(1);
		vertexBinding->setBuffer(frame_normals[frame1], 0, sizeof(glm::vec3));
		vertexBinding->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE);
		m_vao->enable(1);
	}

	//bind frame2
	vertexBinding = m_vao->binding(2);
	vertexBinding->setAttribute(2);
	vertexBinding->setBuffer(frame_vertices[frame1 + 1], 0, sizeof(glm::vec3));
	vertexBinding->setFormat(3, gl::GL_FLOAT);
	m_vao->enable(2);

	if (!frame_normals.empty())
	{
		vertexBinding = m_vao->binding(3);
		vertexBinding->setAttribute(3);
		vertexBinding->setBuffer(frame_normals[frame1 + 1], 0, sizeof(glm::vec3));
		vertexBinding->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE);
		m_vao->enable(3);
	}

	m_vao->drawElements(gl::GL_TRIANGLES, frame_sizes[frame1] + frame_sizes[frame1 + 1], gl::GL_UNSIGNED_INT, nullptr);

	m_vao->unbind();

}

