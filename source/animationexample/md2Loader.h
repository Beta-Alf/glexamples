#pragma once

#include <globjects/base/ref_ptr.h>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>
#include <gloperate/primitives/PolygonalGeometry.h>
#include <gloperate/primitives/PolygonalDrawable.h>

class FrameDrawable;

// number of vertex normals in anorms.h file used for decompressing normals in a md2 file
#define NUMVERTEXNORMALS		162


namespace globjects
{
	class Program;
}

struct md2_vertex
{
	unsigned char position[3];			/* position */
	unsigned char normalIndex;			/* normal vector index */
};

struct VertexKeyframe
{
	glm::vec3 scale;					/* scale factor */
	glm::vec3 translate;				/* translation vector */
	char name[16];						/* frame name */
	struct md2_vertex vertices[0];		/* list of frame's vertices */
};


/* MD2 header, source: http://tfc.duke.free.fr/coding/md2-specs-en.html */
struct md2_header
{
	int ident;                  /* magic number: "IDP2" */
	int version;                /* version: must be 8 */

	int skinwidth;              /* texture width */
	int skinheight;             /* texture height */

	int framesize;              /* size in bytes of a frame */

	int num_skins;              /* number of skins */
	int num_vertices;           /* number of vertices per frame */
	int num_st;                 /* number of texture coordinates */
	int num_tris;               /* number of triangles */
	int num_glcmds;             /* number of opengl commands */
	int num_frames;             /* number of frames */

	int offset_skins;           /* offset skin data */
	int offset_st;              /* offset texture coordinate data */
	int offset_tris;            /* offset triangle data */
	int offset_frames;          /* offset frame data */
	int offset_glcmds;          /* offset OpenGL command data */
	int offset_end;             /* offset end of file */
};

class md2Loader
{
public:
	md2Loader();
	~md2Loader();

	void loadModel(const char* filename);
	FrameDrawable* modelToGPU();

protected:
	gloperate::PolygonalGeometry createFrame(int number);

	md2_header m_header; // Header of the object-file
	FILE* m_model; // Object-file

	// Member-variables
	std::vector<gloperate::PolygonalGeometry> m_Frames;
	
	std::vector< std::vector< glm::vec3 > > m_FrameVertices; // Vertices extracted for every frame
	std::vector< std::vector< glm::vec3 > > m_FrameNormals; // Normal indices extracted for every vertex in every frame
	std::vector< int > m_glCommands; // OpenGL rendering commands
	std::vector< unsigned int > m_VaoIndices; // Indices to give to the PolygonalGeometry Object for drawing in GL_Triangle

	
};

