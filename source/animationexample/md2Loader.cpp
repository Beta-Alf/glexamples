#include "md2Loader.h"
#include <algorithm>
#include <globjects/Program.h>

	//store the normal vectors in an array
	glm::vec3 anorms[NUMVERTEXNORMALS] =
	{	
#include <anorms.h>
	};

	md2Loader::md2Loader() : model(nullptr){
	}

	md2Loader::~md2Loader(){
		if (model != nullptr)
		{
			fclose(model);
		}
	}

void md2Loader::loadModel(const char* filename){
	model = fopen(filename, "rb");
	fread(&header, sizeof(md2_header), 1, model); // Read header where all info about model is stored

	char* frameData = new char[header.num_frames * header.framesize]; 
	fseek(model, header.offset_frames, SEEK_SET); // Find the beginning of the frame data
	fread(frameData, sizeof(char), header.num_frames * header.framesize, model); // Put the frame Data into the buffer

	FrameVertices.resize(header.num_frames, std::vector<glm::vec3>(header.num_vertices)); // Allocate space for vertices
	FrameNormals.resize(header.num_frames, std::vector<glm::vec3>(header.num_vertices)); // And normals

	// Extract vertices and normals from frame data (uncompress Vertex-data)
	for (int i = 0; i < header.num_frames; i++)
	{
		VertexKeyframe* frame = (VertexKeyframe*)&frameData[header.framesize * i]; // Find Frame in frameData and cast into Frame-Type. 

		for (int j = 0; j < header.num_vertices; j++)
		{
			FrameVertices[i][j].x = frame->translate[0] + (float(frame->vertices[j].position[0]) * frame->scale[0]);
			FrameVertices[i][j].z = frame->translate[1] + (float(frame->vertices[j].position[1]) * frame->scale[1]); // z and y need to be switched so the model stands upright
			FrameVertices[i][j].y = frame->translate[2] + (float(frame->vertices[j].position[2]) * frame->scale[2]);

			FrameNormals[i][j] = anorms[frame->vertices[j].normalIndex];
		}
	}
}

gloperate::PolygonalGeometry md2Loader::createFrame(int number){

	gloperate::PolygonalGeometry Frame;

// Read OpenGL rendering commands
	glCommands.resize(header.num_glcmds);
	fseek(model, header.offset_glcmds, SEEK_SET);
	fread(&glCommands[0], sizeof(int), header.num_glcmds, model); // Write the Gl-Commands into array
	
	unsigned int i = 0;
while (glCommands[i] != 0){ // We're finished when the number of vertives to draw equals 0
	// Push vertices so the model is drawn correctliy with GL_triangle	
	if (glCommands[i] < 0){ // Supposed to be drawn with TriangleFan
			int numVertices = -(glCommands[i]);
			i += 3; // Jump over the Texture Coordinates (only need every third number)
			unsigned int firstIndex = glCommands[i];
			i += 3;
			unsigned int lastPushed = glCommands[i];
			for (int j = 2; j < numVertices; j++){ 
				VaoIndices.push_back(firstIndex);
				VaoIndices.push_back(lastPushed);
				i += 3; // Get next Vertex
				VaoIndices.push_back(glCommands[i]);
				lastPushed = glCommands[i];
			}
		}
		else { // Supposed to be drawn with TriangleStrips
			int numVertices = glCommands[i];
			i += 3; 
			unsigned int first = glCommands[i];
			i += 3;
			unsigned int second = glCommands[i];
			for (int j = 2; j < numVertices; j++){
				VaoIndices.push_back(first);
				VaoIndices.push_back(second); //push back the last two vertices as start of new triangle
				i += 3;
				VaoIndices.push_back(glCommands[i]);
				first = second;
				second = glCommands[i];
			}
		}
		i++; // Go to the next glCommand
	}

	Frame.setIndices(VaoIndices);
	Frame.setVertices(FrameVertices[number]);
	Frame.setNormals(FrameNormals[number]);

	return Frame;
}


FrameDrawable* md2Loader::modelToGPU(){
	for (int i = 0; i < header.num_frames; i++){
		m_Frames.push_back(createFrame(i));
	}
	return new FrameDrawable(m_Frames);
}

