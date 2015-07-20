#include "md2Loader.h"
#include <algorithm>
#include <globjects/Program.h>

typedef VertexKeyframe Frame;

	//store the normal vectors in an array
	glm::vec3 anorms[NUMVERTEXNORMALS] =
	{	//anorms hier reinkopiert...
		{ -0.525731, 0.000000, 0.850651 },
		{ -0.442863, 0.238856, 0.864188 },
		{ -0.295242, 0.000000, 0.955423 },
		{ -0.309017, 0.500000, 0.809017 },
		{ -0.162460, 0.262866, 0.951056 },
		{ 0.000000, 0.000000, 1.000000 },
		{ 0.000000, 0.850651, 0.525731 },
		{ -0.147621, 0.716567, 0.681718 },
		{ 0.147621, 0.716567, 0.681718 },
		{ 0.000000, 0.525731, 0.850651 },
		{ 0.309017, 0.500000, 0.809017 },
		{ 0.525731, 0.000000, 0.850651 },
		{ 0.295242, 0.000000, 0.955423 },
		{ 0.442863, 0.238856, 0.864188 },
		{ 0.162460, 0.262866, 0.951056 },
		{ -0.681718, 0.147621, 0.716567 },
		{ -0.809017, 0.309017, 0.500000 },
		{ -0.587785, 0.425325, 0.688191 },
		{ -0.850651, 0.525731, 0.000000 },
		{ -0.864188, 0.442863, 0.238856 },
		{ -0.716567, 0.681718, 0.147621 },
		{ -0.688191, 0.587785, 0.425325 },
		{ -0.500000, 0.809017, 0.309017 },
		{ -0.238856, 0.864188, 0.442863 },
		{ -0.425325, 0.688191, 0.587785 },
		{ -0.716567, 0.681718, -0.147621 },
		{ -0.500000, 0.809017, -0.309017 },
		{ -0.525731, 0.850651, 0.000000 },
		{ 0.000000, 0.850651, -0.525731 },
		{ -0.238856, 0.864188, -0.442863 },
		{ 0.000000, 0.955423, -0.295242 },
		{ -0.262866, 0.951056, -0.162460 },
		{ 0.000000, 1.000000, 0.000000 },
		{ 0.000000, 0.955423, 0.295242 },
		{ -0.262866, 0.951056, 0.162460 },
		{ 0.238856, 0.864188, 0.442863 },
		{ 0.262866, 0.951056, 0.162460 },
		{ 0.500000, 0.809017, 0.309017 },
		{ 0.238856, 0.864188, -0.442863 },
		{ 0.262866, 0.951056, -0.162460 },
		{ 0.500000, 0.809017, -0.309017 },
		{ 0.850651, 0.525731, 0.000000 },
		{ 0.716567, 0.681718, 0.147621 },
		{ 0.716567, 0.681718, -0.147621 },
		{ 0.525731, 0.850651, 0.000000 },
		{ 0.425325, 0.688191, 0.587785 },
		{ 0.864188, 0.442863, 0.238856 },
		{ 0.688191, 0.587785, 0.425325 },
		{ 0.809017, 0.309017, 0.500000 },
		{ 0.681718, 0.147621, 0.716567 },
		{ 0.587785, 0.425325, 0.688191 },
		{ 0.955423, 0.295242, 0.000000 },
		{ 1.000000, 0.000000, 0.000000 },
		{ 0.951056, 0.162460, 0.262866 },
		{ 0.850651, -0.525731, 0.000000 },
		{ 0.955423, -0.295242, 0.000000 },
		{ 0.864188, -0.442863, 0.238856 },
		{ 0.951056, -0.162460, 0.262866 },
		{ 0.809017, -0.309017, 0.500000 },
		{ 0.681718, -0.147621, 0.716567 },
		{ 0.850651, 0.000000, 0.525731 },
		{ 0.864188, 0.442863, -0.238856 },
		{ 0.809017, 0.309017, -0.500000 },
		{ 0.951056, 0.162460, -0.262866 },
		{ 0.525731, 0.000000, -0.850651 },
		{ 0.681718, 0.147621, -0.716567 },
		{ 0.681718, -0.147621, -0.716567 },
		{ 0.850651, 0.000000, -0.525731 },
		{ 0.809017, -0.309017, -0.500000 },
		{ 0.864188, -0.442863, -0.238856 },
		{ 0.951056, -0.162460, -0.262866 },
		{ 0.147621, 0.716567, -0.681718 },
		{ 0.309017, 0.500000, -0.809017 },
		{ 0.425325, 0.688191, -0.587785 },
		{ 0.442863, 0.238856, -0.864188 },
		{ 0.587785, 0.425325, -0.688191 },
		{ 0.688191, 0.587785, -0.425325 },
		{ -0.147621, 0.716567, -0.681718 },
		{ -0.309017, 0.500000, -0.809017 },
		{ 0.000000, 0.525731, -0.850651 },
		{ -0.525731, 0.000000, -0.850651 },
		{ -0.442863, 0.238856, -0.864188 },
		{ -0.295242, 0.000000, -0.955423 },
		{ -0.162460, 0.262866, -0.951056 },
		{ 0.000000, 0.000000, -1.000000 },
		{ 0.295242, 0.000000, -0.955423 },
		{ 0.162460, 0.262866, -0.951056 },
		{ -0.442863, -0.238856, -0.864188 },
		{ -0.309017, -0.500000, -0.809017 },
		{ -0.162460, -0.262866, -0.951056 },
		{ 0.000000, -0.850651, -0.525731 },
		{ -0.147621, -0.716567, -0.681718 },
		{ 0.147621, -0.716567, -0.681718 },
		{ 0.000000, -0.525731, -0.850651 },
		{ 0.309017, -0.500000, -0.809017 },
		{ 0.442863, -0.238856, -0.864188 },
		{ 0.162460, -0.262866, -0.951056 },
		{ 0.238856, -0.864188, -0.442863 },
		{ 0.500000, -0.809017, -0.309017 },
		{ 0.425325, -0.688191, -0.587785 },
		{ 0.716567, -0.681718, -0.147621 },
		{ 0.688191, -0.587785, -0.425325 },
		{ 0.587785, -0.425325, -0.688191 },
		{ 0.000000, -0.955423, -0.295242 },
		{ 0.000000, -1.000000, 0.000000 },
		{ 0.262866, -0.951056, -0.162460 },
		{ 0.000000, -0.850651, 0.525731 },
		{ 0.000000, -0.955423, 0.295242 },
		{ 0.238856, -0.864188, 0.442863 },
		{ 0.262866, -0.951056, 0.162460 },
		{ 0.500000, -0.809017, 0.309017 },
		{ 0.716567, -0.681718, 0.147621 },
		{ 0.525731, -0.850651, 0.000000 },
		{ -0.238856, -0.864188, -0.442863 },
		{ -0.500000, -0.809017, -0.309017 },
		{ -0.262866, -0.951056, -0.162460 },
		{ -0.850651, -0.525731, 0.000000 },
		{ -0.716567, -0.681718, -0.147621 },
		{ -0.716567, -0.681718, 0.147621 },
		{ -0.525731, -0.850651, 0.000000 },
		{ -0.500000, -0.809017, 0.309017 },
		{ -0.238856, -0.864188, 0.442863 },
		{ -0.262866, -0.951056, 0.162460 },
		{ -0.864188, -0.442863, 0.238856 },
		{ -0.809017, -0.309017, 0.500000 },
		{ -0.688191, -0.587785, 0.425325 },
		{ -0.681718, -0.147621, 0.716567 },
		{ -0.442863, -0.238856, 0.864188 },
		{ -0.587785, -0.425325, 0.688191 },
		{ -0.309017, -0.500000, 0.809017 },
		{ -0.147621, -0.716567, 0.681718 },
		{ -0.425325, -0.688191, 0.587785 },
		{ -0.162460, -0.262866, 0.951056 },
		{ 0.442863, -0.238856, 0.864188 },
		{ 0.162460, -0.262866, 0.951056 },
		{ 0.309017, -0.500000, 0.809017 },
		{ 0.147621, -0.716567, 0.681718 },
		{ 0.000000, -0.525731, 0.850651 },
		{ 0.425325, -0.688191, 0.587785 },
		{ 0.587785, -0.425325, 0.688191 },
		{ 0.688191, -0.587785, 0.425325 },
		{ -0.955423, 0.295242, 0.000000 },
		{ -0.951056, 0.162460, 0.262866 },
		{ -1.000000, 0.000000, 0.000000 },
		{ -0.850651, 0.000000, 0.525731 },
		{ -0.955423, -0.295242, 0.000000 },
		{ -0.951056, -0.162460, 0.262866 },
		{ -0.864188, 0.442863, -0.238856 },
		{ -0.951056, 0.162460, -0.262866 },
		{ -0.809017, 0.309017, -0.500000 },
		{ -0.864188, -0.442863, -0.238856 },
		{ -0.951056, -0.162460, -0.262866 },
		{ -0.809017, -0.309017, -0.500000 },
		{ -0.681718, 0.147621, -0.716567 },
		{ -0.681718, -0.147621, -0.716567 },
		{ -0.850651, 0.000000, -0.525731 },
		{ -0.688191, 0.587785, -0.425325 },
		{ -0.587785, 0.425325, -0.688191 },
		{ -0.425325, 0.688191, -0.587785 },
		{ -0.425325, -0.688191, -0.587785 },
		{ -0.587785, -0.425325, -0.688191 },
		{ -0.688191, -0.587785, -0.425325 }
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

	char* frameData = new char[header.num_frames * header.framesize]; // Read all frame data to one big buffer
	fseek(model, header.offset_frames, SEEK_SET); //find the beginning of the frame data
	fread(frameData, sizeof(char), header.num_frames * header.framesize, model); //put the frame Data into the buffer

	FrameVertices.resize(header.num_frames, std::vector<glm::vec3>(header.num_vertices)); // Allocate space for vertices
	FrameNormals.resize(header.num_frames, std::vector<glm::vec3>(header.num_vertices)); // And normals

	// Extract vertices and normals from frame data (uncompress Vertex-data)
	for (int i = 0; i < header.num_frames; i++)
	{
		Frame* frame = (Frame*)&frameData[header.framesize * i]; // Convert frameData to Frame-Array

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

// Now let's read OpenGL rendering commands
	glCommands.resize(header.num_glcmds);
	fseek(model, header.offset_glcmds, SEEK_SET);
	fread(&glCommands[0], sizeof(int), header.num_glcmds, model); //write the Gl-Commands into array
	
	unsigned int i = 0;
	while (glCommands[i] != 0){ //ist die Anzahl der zu malenden Vertices 0, sind wir fertig
		if (glCommands[i] < 0){ //draw with TriangleFan
			int numVertices = -(glCommands[i]);
			i += 3; //for now I am leaving out the textureCoordinates
			unsigned int firstIndex = glCommands[i];
			i += 3;
			unsigned int lastPushed = glCommands[i];
			for (int j = 2; j < numVertices; j++){
				VaoIndices.push_back(firstIndex);
				VaoIndices.push_back(lastPushed);
				i += 3; //get next Vertex
				VaoIndices.push_back(glCommands[i]);
				lastPushed = glCommands[i];
			}
			i++; //get to the next glCommand
		}
		else { //draw with TriangleStrips
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
			i++;
		}


	}

	Frame.setIndices(VaoIndices);
	Frame.setVertices(FrameVertices[number]);
	Frame.setNormals(FrameNormals[number]);

	return Frame;
}


FrameDrawable md2Loader::modelToGPU(){
	for (int i = 0; i < header.num_frames; i++){
		Frames.push_back(createFrame(i));
	}
	return FrameDrawable(Frames);
}

