#include "FrameDrawable.h"

#include <iostream>

#include <glm/glm.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/primitives/PolygonalGeometry.h>
#include <globjects/globjects.h>
#include <globjects/Program.h>

FrameDrawable::FrameDrawable(const std::vector<gloperate::PolygonalGeometry> & geometries)
{	
	// Save number of elements in index buffer
	m_size = static_cast<gl::GLsizei>(geometries[0].indices().size());
	
	//indices are the same for every frame
	m_indices = new globjects::Buffer;
	m_indices->setData(geometries[0].indices(), gl::GL_STATIC_DRAW); 

	m_numFrames = geometries.size();
	for (int i = 0; i < m_numFrames; i++){

		// Create and copy vertex buffer
		globjects::ref_ptr<globjects::Buffer> vertices = new globjects::Buffer;
		vertices->setData(geometries[i].vertices(), gl::GL_STATIC_DRAW);

		// Create and copy normal buffer
		globjects::ref_ptr<globjects::Buffer> normals;
		if (geometries[i].hasNormals())
		{
			normals = new globjects::Buffer;
			normals->setData(geometries[i].normals(), gl::GL_STATIC_DRAW);
		}

		//put Data into frame_vectors
		m_frame_normals.push_back(normals);
		m_frame_vertices.push_back(vertices);
	}

	m_program = new globjects::Program{};
	m_program->attach(
		globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/animationexample/vertexanimation.vert"),
		globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/animationexample/icosahedron.frag")
		);

	m_transformLocation = m_program->getUniformLocation("transform");
	m_interpolationLocation = m_program->getUniformLocation("interpol");
}


FrameDrawable::~FrameDrawable()
{
}

void FrameDrawable::draw(){
	assert(false); // you cannot draw the object without the transformation matrix
}

void FrameDrawable::draw(int firstFrame, int lastFrame, int fps, float time, const glm::mat4& transform){
	
	m_program->use();
	m_program->setUniform(m_transformLocation, transform);

	// calculate which frame to draw and how much to interpolate
	int numFramesAnim = lastFrame - firstFrame + 1; // number of frames in the animation	
	float temp = time * fps;
	int offset = floor(temp);
	float interpolationFactor = temp - (int)temp;
	
	int currentFrame = firstFrame + (offset % numFramesAnim);
	int nextFrame = firstFrame + ((offset + 1) % numFramesAnim);

	m_program->setUniform(m_interpolationLocation, interpolationFactor);

	// Create vertex array object
	m_vao = new globjects::VertexArray;
	m_vao->bind();

	//bind indices
	m_indices->bind(gl::GL_ELEMENT_ARRAY_BUFFER);
	
	//bind frame1
	auto vertexBinding = m_vao->binding(0);
	vertexBinding->setAttribute(0);
	vertexBinding->setBuffer(m_frame_vertices[currentFrame], 0, sizeof(glm::vec3));
	vertexBinding->setFormat(3, gl::GL_FLOAT);
	m_vao->enable(0);

	if (!m_frame_normals.empty())
	{
		vertexBinding = m_vao->binding(1);
		vertexBinding->setAttribute(1);
		vertexBinding->setBuffer(m_frame_normals[currentFrame], 0, sizeof(glm::vec3));
		vertexBinding->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE);
		m_vao->enable(1);
	}

	//bind frame2
	vertexBinding = m_vao->binding(2);
	vertexBinding->setAttribute(2);
	vertexBinding->setBuffer(m_frame_vertices[nextFrame], 0, sizeof(glm::vec3));
	vertexBinding->setFormat(3, gl::GL_FLOAT);
	m_vao->enable(2);

	if (!m_frame_normals.empty())
	{
		vertexBinding = m_vao->binding(3);
		vertexBinding->setAttribute(3);
		vertexBinding->setBuffer(m_frame_normals[nextFrame], 0, sizeof(glm::vec3));
		vertexBinding->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE);
		m_vao->enable(3);
	}

	m_vao->drawElements(gl::GL_TRIANGLES, m_size, gl::GL_UNSIGNED_INT, nullptr);
	m_vao->unbind();
	m_program->release();

	while (auto a = gl::glGetError() != gl::GL_NO_ERROR)
	{
		std::cout << "Error detected" << std::endl;
		std::cout << a << std::endl;
	}

}

