#ifndef H_ENGINE_GENERATION
#define H_ENGINE_GENERATION

#include "engine_chunk.h"
#include "engine_cube.h"

void
GenerateSphereChunk(Chunk* Chunk);

void
GenerateRandomHeightMap(Chunk* Chunk);

float
PerlinNoise(float X, float Y);

float
PerlinInterpolate(float Val);

float
Blend(float a, float b, float t);

float
OctavePerlinNoise(float X, float Y, int Octaves, float Persistence);

void
GeneratePerlinChunk(float XSeed, float ZSeed, float Inc, Chunk* Chunk);

void
GenerateOctavedPerlinChunk(float XSeed, float ZSeed, float Inc, 
			   Chunk* Chunk, int YLevel, int MaxHeight,
			   int Octaves, float Persistence);

void
LandscapeEarthBiomeWash(Chunk* Chunk);

#endif
