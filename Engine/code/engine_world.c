#include "engine_world.h"
#include "engine_generation.h"
#include "engine_camera.h"

#include <stdlib.h>
#include <math.h>

global VEAudioClip ReseedSound;

void
InitializeWorld(CubeWorld* World,
		CubeType* CubeTypes,
		int X, int Z, int Height,
                int ChunkSize,
                float PerlinXSeed, float PerlinZSeed, float Inc,
		int Octaves, float Persistence)
{
    VELoadAudioClip(&ReseedSound, "../data/assets/music/Drilling.wav", 4);

    float* RenderVertexBuffer;
    int RVBSize;

    RVBSize = sizeof(float) * RENDER_FLOATS_PER_CUBE_VERTEX
	                     * ChunkSize * ChunkSize * ChunkSize
	                     * 6 * 6;

    RenderVertexBuffer = malloc(RVBSize);
    
    World->WorldChunks = malloc(sizeof(Chunk)*X*Z*Height);
    World->ChunkDirtyList = malloc(sizeof(int)*X*Z*Height);

    World->WorldIsDirty = 0;
    World->TotalRebuild = 0;
    World->PhysicsModelDirty = 0;

    World->CubeTypes = CubeTypes;

    World->ChunkCount = X*Z*Height;
    World->ChunkDimension = ChunkSize;

    World->XLength = X;
    World->ZLength = Z;
    World->StackHeight = Height;

    World->PerlinXSeed = PerlinXSeed;
    World->PerlinZSeed = PerlinZSeed;
    World->Increment = Inc;
    World->Octaves = Octaves;
    World->Persistence = Persistence;

    World->DayNightCycle = 0;
    World->Apocalypse = 0;
    World->Spheres = 0;

    for (int s = 0; s < SPHERE_DEMO_COUNT; s++)
    {
	InitializeChunk(&World->Sphere[s], ChunkSize,
			Vec3f(s*ChunkSize,
			      0,
			      -ChunkSize));

	GenerateSphereChunk(&World->Sphere[s]);

	GreedyMesh(&World->Sphere[s], World->CubeTypes, RenderVertexBuffer, &RVBSize);
	InitializeChunkRenderBuffers(&World->Sphere[s], RenderVertexBuffer);
    }

    int i = 0;
    
    for (int y = 0; y < Height; y++)
    {
	for (int z = 0; z < Z; z++)
	{
	    for (int x = 0; x < X; x++)
	    {
		World->ChunkDirtyList[i] = 0;

		InitializeChunk(&World->WorldChunks[i], ChunkSize,
				Vec3f(x*ChunkSize,
				      y*ChunkSize,
				      z*ChunkSize));

		GenerateOctavedPerlinChunk(PerlinXSeed+(x*Inc*ChunkSize),
					   PerlinZSeed+(z*Inc*ChunkSize),
					   Inc, 
					   &World->WorldChunks[i], y, Height,
					   Octaves, Persistence);
	    
		ChunkEarthBiomeWash(&World->WorldChunks[i]);

		GreedyMesh(&World->WorldChunks[i], World->CubeTypes, RenderVertexBuffer, &RVBSize);
	    
		InitializeChunkRenderBuffers(&World->WorldChunks[i], RenderVertexBuffer);

		i++;
	    }
	}
    }
    
    free(RenderVertexBuffer);
}

void
ReseedWorld(CubeWorld* World,
	    CubeType* CubeTypes,
	    float PerlinXSeed, float PerlinZSeed, float Inc,
	    int Octaves, float Persistence)
{
    VEPlayAudioClip(&ReseedSound, 0);

    World->WorldIsDirty = 1;

    World->PerlinXSeed = PerlinXSeed;
    World->PerlinZSeed = PerlinZSeed;
    World->Increment = Inc;
    World->Octaves = Octaves;
    World->Persistence = Persistence;

    int RVBSize;

    RVBSize = sizeof(float) * RENDER_FLOATS_PER_CUBE_VERTEX
	* World->ChunkDimension * World->ChunkDimension * World->ChunkDimension
	* 6 * 6;

    World->RenderVertexBuffer = malloc(RVBSize);

    int i = 0;

    for (int y = 0; y < World->StackHeight; y++)
    {
	for (int z = 0; z < World->ZLength; z++)
	{
	    for (int x = 0; x < World->XLength; x++)
	    {
		World->ChunkDirtyList[i] = 1;
		World->WorldChunks[i].PerlinXSeed = PerlinXSeed + (x*Inc*World->ChunkDimension);
		World->WorldChunks[i].PerlinZSeed = PerlinZSeed + (z*Inc*World->ChunkDimension);
		World->WorldChunks[i].YLevel = y;
		i++;
	    }
	}
    }
}

void
UpdateWorld(CubeWorld* World)
{
    if (World->WorldIsDirty)
    {
	if (!VEIsChannelPlaying(&ReseedSound))
	{
	    VEPlayAudioClip(&ReseedSound, 0);
	}

	int ChkIndex = 0;
    
	while (ChkIndex < World->ChunkCount && !World->ChunkDirtyList[ChkIndex])
	{
	    ChkIndex++;
	}

	if (ChkIndex < World->ChunkCount && World->ChunkDirtyList[ChkIndex])
	{
	    int RVBSize;

	    RVBSize = sizeof(float) * RENDER_FLOATS_PER_CUBE_VERTEX
		* World->ChunkDimension * World->ChunkDimension * World->ChunkDimension
		* 6 * 6;

	    GenerateOctavedPerlinChunk(World->WorldChunks[ChkIndex].PerlinXSeed,
				       World->WorldChunks[ChkIndex].PerlinZSeed,
				       World->Increment, 
				       &World->WorldChunks[ChkIndex],
				       World->WorldChunks[ChkIndex].YLevel,
				       World->StackHeight,
				       World->Octaves, World->Persistence);
	    
	    ChunkEarthBiomeWash(&World->WorldChunks[ChkIndex]);

	    GreedyMesh(&World->WorldChunks[ChkIndex], World->CubeTypes,
		       World->RenderVertexBuffer, &RVBSize);
	    
	    if (!World->TotalRebuild)
	    {
		UpdateChunkRenderBuffers(&World->WorldChunks[ChkIndex], World->RenderVertexBuffer);
	    }
	    else
	    {
		InitializeChunkRenderBuffers(&World->WorldChunks[ChkIndex],
					     World->RenderVertexBuffer);
	    }

	    World->ChunkDirtyList[ChkIndex] = 0;

	}
	else
	{
	    if (VEIsChannelPlaying(&ReseedSound))
	    {
		VEFadeAudioClip(&ReseedSound, 1000);
	    }
	    World->WorldIsDirty = 0;
	    World->TotalRebuild = 0;
	    free(World->RenderVertexBuffer);
	}
    }
}

void
InitializePlayer(CubeWorld* World, vec3f_t Position,
		 float CameraPitch, float CameraYaw)
{
    World->Player.Body.Position = Position;

    World->Player.Camera.Position = World->Player.Body.Position;
    World->Player.Camera.Pitch = CameraPitch;
    World->Player.Camera.Yaw = CameraYaw;
}

Chunk*
GetChunkFromPosition(CubeWorld* World, vec3f_t Position)
{
    
    if (Position.x < 0 || Position.x >= World->XLength*World->ChunkDimension) return (0);
    if (Position.y < 0 || Position.y >= World->StackHeight*World->ChunkDimension) return (0);
    if (Position.z < 0 || Position.z >= World->ZLength*World->ChunkDimension) return (0);

    int LowerBoundX = floor(Position.x / World->ChunkDimension);
    int LowerBoundZ = floor(Position.z / World->ChunkDimension);

    return (&World->WorldChunks[LowerBoundZ*World->ZLength + LowerBoundX]);
}

PhysicsStaticGeometry*
GetStaticChunkGeometryInformation(CubeWorld* World)
{
    PhysicsStaticGeometry* Geometry = malloc(sizeof(PhysicsStaticGeometry));
    Geometry->ChunkList = &(World->WorldChunks);
    Geometry->AABBList = malloc(sizeof(PhysicsAABB)*World->ChunkCount);
    Geometry->AABBCount = World->ChunkCount;

    for (int i = 0; i < World->ChunkCount; i++)
    {
	Geometry->AABBList[i].Min = World->WorldChunks[i].Transform;
	Geometry->AABBList[i].Max = AddVec3f(World->WorldChunks[i].Transform,
	    Vec3f(World->ChunkDimension, World->ChunkDimension, World->ChunkDimension));
    }

    return (Geometry);
}

void
RebuildStaticChunkGeometryInformation(CubeWorld* World,
				      PhysicsStaticGeometry* Geo)
{
    Geo->ChunkList = &(World->WorldChunks);
    free(Geo->AABBList);
    Geo->AABBList = malloc(sizeof(PhysicsAABB)*World->ChunkCount);
    Geo->AABBCount = World->ChunkCount;

    for (int i = 0; i < World->ChunkCount; i++)
    {
	Geo->AABBList[i].Min = World->WorldChunks[i].Transform;
	Geo->AABBList[i].Max = AddVec3f(World->WorldChunks[i].Transform,
	    Vec3f(World->ChunkDimension, World->ChunkDimension, World->ChunkDimension));
    }

}

int
GetActiveCubeCount(CubeWorld* World)
{
    int Sum = 0;
    for (int i = 0; i < World->ChunkCount; i++)
    {
	Sum += World->WorldChunks[i].ActiveCubes;
    }
    return (Sum);
}

void
RedoWorld(CubeWorld* World,
	  int X, int Z, int Height,
	  int ChunkSize,
	  float PerlinXSeed, float PerlinZSeed, float Inc,
	  int Octaves, float Persistence)
{
    for (int i = 0; i < World->ChunkCount; i++)
    {
	FreeChunk(&World->WorldChunks[i]);
	FreeChunkRenderBuffers(&World->WorldChunks[i]);
    }

    free(World->WorldChunks);
    free(World->ChunkDirtyList);

    int RVBSize;
    
    RVBSize = sizeof(float) * RENDER_FLOATS_PER_CUBE_VERTEX
	* ChunkSize * ChunkSize * ChunkSize
	* 6 * 6;

    World->RenderVertexBuffer = malloc(RVBSize);
    
    World->WorldChunks = malloc(sizeof(Chunk)*X*Z*Height);
    World->ChunkDirtyList = malloc(sizeof(int)*X*Z*Height);

    World->WorldIsDirty = 1;
    World->TotalRebuild = 1;

    World->ChunkCount = X*Z*Height;
    World->ChunkDimension = ChunkSize;

    World->XLength = X;
    World->ZLength = Z;
    World->StackHeight = Height;

    World->PerlinXSeed = PerlinXSeed;
    World->PerlinZSeed = PerlinZSeed;
    World->Increment = Inc;
    World->Octaves = Octaves;
    World->Persistence = Persistence;

    World->DayNightCycle = 0;
    World->Apocalypse = 0;
    World->Spheres = 0;

    World->PhysicsModelDirty = 1;

    int i = 0;

    for (int y = 0; y < World->StackHeight; y++)
    {
	for (int z = 0; z < World->ZLength; z++)
	{
	    for (int x = 0; x < World->XLength; x++)
	    {
		InitializeChunk(&World->WorldChunks[i], ChunkSize,
				Vec3f(x*ChunkSize,
				      y*ChunkSize,
				      z*ChunkSize));

		World->ChunkDirtyList[i] = 1;
		World->WorldChunks[i].PerlinXSeed = PerlinXSeed + (x*Inc*World->ChunkDimension);
		World->WorldChunks[i].PerlinZSeed = PerlinZSeed + (z*Inc*World->ChunkDimension);
		World->WorldChunks[i].YLevel = y;
		i++;
	    }
	}
    }
}
