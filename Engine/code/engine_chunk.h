#ifndef H_ENGINE_CHUNK
#define H_ENGINE_CHUNK

#include <GL/glew.h>

#include "engine_main.h"
#include "engine_cube.h"

global int DEFAULT_CHUNK_SIZE = 32;

global int RENDER_FLOATS_PER_CUBE_VERTEX = 9;

typedef struct
Chunk
{

    Cube*** Grid;
    int ChunkDimension;
    int ActiveCubes;

    vec3f_t Transform;

    int MeshVertexCount;

    GLuint VAO;
    GLuint VBO;

    float PerlinXSeed;
    float PerlinZSeed;

    int YLevel;

} Chunk;

enum CubeFaces
{
    LEFT,
    RIGHT,
    FORWARD,
    BACK,
    TOP,
    BOTTOM
};

void
InitializeChunk(Chunk* Chk, int size, vec3f_t Transform);

void
FreeChunk(Chunk* Chk);

void
GreedyMesh(Chunk* Chk, CubeType* CubeTypes,
	   float* RenderBuffer, int* VertexCount);

void
ChunkHeightFill(int x, int z, int height, Chunk* Chk);

void
AddQuad(Chunk* Chk, int Side,
        vec3f_t P1, vec3f_t P2,
        vec3f_t P3, vec3f_t P4,
        int CubeID, CubeType* Types,
	float* RenderBuffer, int* VertexCount);

Cube*
GetCubeFromPosition(Chunk* Chk, vec3f_t Position);

int
GetChunkHeight(Chunk* Chk, int X, int Z);

#endif
