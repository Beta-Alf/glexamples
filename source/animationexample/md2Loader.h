#pragma once

#include <globjects/base/ref_ptr.h>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>

namespace globjects
{
	class Program;
	class PolygonalGeometry;
	class PolygonalDrawable;
}

struct VertexKeyframe
{
	glm::vec3 scale;				/* scale factor */
	glm::vec3 translate;			/* translation vector */
	char name[16];					/* frame name */
	struct md2_vertex_t *verts;		/* list of frame's vertices */
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
	md2Loader() = default;
	~md2Loader();

	void loadModel(const char* filename);
	void modelToGPU();

	std::vector<VertexKeyframe> m_keyframes;
	globjects::ref_ptr<globjects::Program> m_program;

	std::vector< std::vector< glm::vec3 > > FrameVertices; // Vertices extracted for every frame
	std::vector< std::vector< int > > FrameNormals; // Normal indices extracted for every frame
	std::vector< int > glCommands; // Rendering OpenGL commands

	
};

