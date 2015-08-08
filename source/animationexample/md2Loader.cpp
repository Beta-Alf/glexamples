#include "md2Loader.h"
#include <algorithm>
#include <globjects/Program.h>
#include <FrameDrawable.h>

	//store the normal vectors in an array
	glm::vec3 anorms[NUMVERTEXNORMALS] =
	{	
#include <anorms.h>
	};

	md2Loader::md2Loader() : m_model(nullptr){
	}

	md2Loader::~md2Loader(){
		if (m_model != nullptr)
		{
			fclose(m_model);
		}
	}

void md2Loader::loadModel(const char* filename){
	m_model = fopen(filename, "rb");
	fread(&m_header, sizeof(md2_header), 1, m_model); // Read header where all info about model is stored

	char* frameData = new char[m_header.num_frames * m_header.framesize]; 
	fseek(m_model, m_header.offset_frames, SEEK_SET); // Find the beginning of the frame data
	fread(frameData, sizeof(char), m_header.num_frames * m_header.framesize, m_model); // Put the frame Data into the buffer

	m_FrameVertices.resize(m_header.num_frames, std::vector<glm::vec3>(m_header.num_vertices)); // Allocate space for vertices
	m_FrameNormals.resize(m_header.num_frames, std::vector<glm::vec3>(m_header.num_vertices)); // And normals

	// Extract vertices and normals from frame data (uncompress Vertex-data)
	for (int i = 0; i < m_header.num_frames; i++)
	{
		VertexKeyframe* frame = (VertexKeyframe*)&frameData[m_header.framesize * i]; // Find Frame in frameData and cast into Frame-Type. 

		for (int j = 0; j < m_header.num_vertices; j++)
		{
			m_FrameVertices[i][j].x = frame->translate[0] + (float(frame->vertices[j].position[0]) * frame->scale[0]);
			m_FrameVertices[i][j].z = frame->translate[1] + (float(frame->vertices[j].position[1]) * frame->scale[1]); // z and y need to be switched so the model stands upright
			m_FrameVertices[i][j].y = frame->translate[2] + (float(frame->vertices[j].position[2]) * frame->scale[2]);

			m_FrameNormals[i][j] = anorms[frame->vertices[j].normalIndex];
		}
	}
}

gloperate::PolygonalGeometry md2Loader::createFrame(int number){

	gloperate::PolygonalGeometry Frame;

// Read OpenGL rendering commands
	m_glCommands.resize(m_header.num_glcmds);
	fseek(m_model, m_header.offset_glcmds, SEEK_SET);
	fread(&m_glCommands[0], sizeof(int), m_header.num_glcmds, m_model); // Write the Gl-Commands into array
	
	unsigned int i = 0;

	while (m_glCommands[i] != 0){ // We're finished when the number of vertives to draw equals 0
		// Push vertices so the model is drawn correctliy with GL_triangle	
		if (m_glCommands[i] < 0){ // Supposed to be drawn with TriangleFan
				int numVertices = -(m_glCommands[i]);
				i += 3; // Jump over the Texture Coordinates (only need every third number)
				unsigned int firstIndex = m_glCommands[i];
				i += 3;
				unsigned int lastPushed = m_glCommands[i];
				for (int j = 2; j < numVertices; j++){ 
					m_VaoIndices.push_back(firstIndex);
					m_VaoIndices.push_back(lastPushed);
					i += 3; // Get next Vertex
					m_VaoIndices.push_back(m_glCommands[i]);
					lastPushed = m_glCommands[i];
				}
			}
			else { // Supposed to be drawn with TriangleStrips
				int numVertices = m_glCommands[i];
				i += 3; 
				unsigned int first = m_glCommands[i];
				i += 3;
				unsigned int second = m_glCommands[i];
				for (int j = 2; j < numVertices; j++){
					m_VaoIndices.push_back(first);
					m_VaoIndices.push_back(second); //push back the last two vertices as start of new triangle
					i += 3;
					m_VaoIndices.push_back(m_glCommands[i]);
					first = second;
					second = m_glCommands[i];
				}
			}
			i++; // Go to the next glCommand
		}

	Frame.setIndices(m_VaoIndices);
	Frame.setVertices(m_FrameVertices[number]);
	Frame.setNormals(m_FrameNormals[number]);

	return Frame;
}

FrameDrawable* md2Loader::modelToGPU(){
	for (int i = 0; i < m_header.num_frames; i++){
		m_Frames.push_back(createFrame(i));
	}
	return new FrameDrawable(m_Frames);
}

