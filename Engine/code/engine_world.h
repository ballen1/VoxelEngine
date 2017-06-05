#ifndef H_ENGINE_WORLD
#define H_ENGINE_WORLD

#define SPHERE_DEMO_COUNT 3

#include "engine_math.h"
#include "engine_chunk.h"
#include "engine_cube.h"
#include "engine_player.h"
#include "engine_physics.h"

typedef struct
CubeWorld
{
    int ChunkCount;
    int ChunkDimension;

    int XLength;
    int ZLength;
    int StackHeight;

    Chunk* WorldChunks;
    CubeType* CubeTypes;

    int* ChunkDirtyList;
    int WorldIsDirty;
    int TotalRebuild;
    int PhysicsModelDirty;

    float* RenderVertexBuffer;

    float PerlinXSeed;
    float PerlinZSeed;
    float Increment;
    int Octaves;
    float Persistence;

    VEPlayer Player;

    int DayNightCycle;
    int Apocalypse;
    int Spheres;

    Chunk Sphere[SPHERE_DEMO_COUNT];

} CubeWorld;

void
InitializeWorld(CubeWorld* World,
		CubeType* CubeTypes,
		int X, int Z, int Height,
                int ChunkSize,
		float PerlinXSeed, float PerlinZSeed, float Inc,
		int Octaves, float Persistence);

void
InitializePlayer(CubeWorld* World, vec3f_t Position,
		 float CameraPitch, float CameraYaw);

void
ReseedWorld(CubeWorld* World,
	    CubeType* CubeTypes,
	    float PerlinXSeed, float PerlinZSeed, float Inc,
	    int Octaves, float Persistence);

void
UpdateWorld(CubeWorld* World);

void
RebuildStaticChunkGeometryInformation(CubeWorld* World,
				      PhysicsStaticGeometry* Geo);

Chunk*
GetChunkFromPosition(CubeWorld* World, vec3f_t Position);

PhysicsStaticGeometry*
GetStaticChunkGeometryInformation(CubeWorld* World);

int
GetActiveCubeCount(CubeWorld* World);

void
RedoWorld(CubeWorld* World,
	  int X, int Z, int Height,
	  int ChunkSize,
	  float PerlinXSeed, float PerlinZSeed, float Inc,
	  int Octaves, float Persistence);

#endif
