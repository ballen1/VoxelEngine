#include "engine_generation.h"

#include <stdlib.h>
#include <math.h>

void
GenerateSphereChunk(Chunk* Chunk)
{

    Chunk->ActiveCubes = 0;
    
    int radius = Chunk->ChunkDimension / 2;

    for (int x = 0; x < Chunk->ChunkDimension; x++)
    {
	for (int y = 0; y < Chunk->ChunkDimension; y++)
	{
	    for (int z = 0; z < Chunk->ChunkDimension; z++)
	    {

		SetType(&Chunk->Grid[x][y][z], CT_SNOW);

		if (((x - radius)*(x - radius) + (y - radius)*(y - radius)
		     + (z - radius)*(z - radius)) < (radius*radius))
		{
		    SetActive(&Chunk->Grid[x][y][z], 1);
		    Chunk->ActiveCubes++;
		}
		else
		{
		    SetActive(&Chunk->Grid[x][y][z], 0);
		}
	    }
	}
    }

}

void
GenerateRandomHeightMap(Chunk* Chunk)
{

    time_t t;
    srand((unsigned) time(&t));

    for (int x = 0; x < Chunk->ChunkDimension; x++)
    {
	for (int z = 0; z < Chunk->ChunkDimension; z++)
	{
	    int Height = (rand() % (Chunk->ChunkDimension)) + 1;
	    
	    ChunkHeightFill(x, z, Height, Chunk);
	}
    }

}

float
PerlinNoise(float X, float Y)
{

    vec2f_t Gradients[] = {
	                    Vec2f(1, 0),   Vec2f(1, 1),
                            Vec2f(0, 1),   Vec2f(-1, 0),
                            Vec2f(-1, -1), Vec2f(0, -1),
                            Vec2f(1, -1),  Vec2f(-1, 1)
                          };

    int Permutations[] = {
	151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
	151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

    int XUnit = floor(X);
    int YUnit = floor(Y);

    X = X - XUnit;
    Y = Y - YUnit;

    XUnit = XUnit & 255;
    YUnit = YUnit & 255;

    int GIndex_00 = Permutations[XUnit + Permutations[YUnit]] % 8;
    int GIndex_01 = Permutations[XUnit + Permutations[YUnit+1]] % 8;
    int GIndex_10 = Permutations[XUnit+1 + Permutations[YUnit]] % 8;
    int GIndex_11 = Permutations[XUnit+1 + Permutations[YUnit+1]] % 8;

    vec2f_t G00 = Gradients[GIndex_00];
    vec2f_t G01 = Gradients[GIndex_01];
    vec2f_t G10 = Gradients[GIndex_10];
    vec2f_t G11 = Gradients[GIndex_11];

    float Weight00 = DotProductVec2f(G00, Vec2f(X, Y));
    float Weight10 = DotProductVec2f(G10, Vec2f((X-1), Y));
    float Weight01 = DotProductVec2f(G01, Vec2f(X, (Y-1)));
    float Weight11 = DotProductVec2f(G11, Vec2f((X-1), (Y-1)));

    float XContribution = PerlinInterpolate(X);
    float YContribution = PerlinInterpolate(Y);

    // Blend Horizontally
    float MixY0 = Blend(Weight00, Weight10, XContribution);
    float MixY1 = Blend(Weight01, Weight11, XContribution);

    // Blend Vertically
    float TotalMix = Blend(MixY0, MixY1, YContribution);

    // Normalize the result to the range [0, 1] from the theoretical
    // possible range of [-1, 1]

    return ((TotalMix+1)/2.0);
}

float
PerlinInterpolate(float Val)
{
    return ((6*Val*Val*Val*Val*Val) - (15*Val*Val*Val*Val) + (10*Val*Val*Val));
}

float
Blend(float a, float b, float t)
{
    return ((1-t)*a + t*b);
}

float
OctavePerlinNoise(float X, float Y, int Octaves, float Persistence)
{
    float NoiseVal = 0;

    float Frequency = 1;
    float Amplitude = 1;

    float MaxNoiseVal = 0;
    
    for (int o = 0; o < Octaves; o++)
    {
	
	NoiseVal += PerlinNoise(X*Frequency, Y*Frequency) * Amplitude;

	MaxNoiseVal += Amplitude;

	Frequency *= 2;
	Amplitude *= Persistence;
    }
    
    return (NoiseVal/MaxNoiseVal);
}

void
GeneratePerlinChunk(float XSeed, float ZSeed, float Inc, Chunk* Chunk)
{
  
    float Noise, Height;

    for (int x = 0; x < Chunk->ChunkDimension; x++)
    {
	for (int z = 0; z < Chunk->ChunkDimension; z++)
	{
	    Noise = PerlinNoise(XSeed, ZSeed);

	    ZSeed += Inc;

	    Height = (int)(Noise * Chunk->ChunkDimension);
	    ChunkHeightFill(x, z, Height, Chunk);
	}

	ZSeed -= (Inc * Chunk->ChunkDimension);
	XSeed += Inc;
    }

}

void
GenerateOctavedPerlinChunk(float XSeed, float ZSeed, float Inc,
			   Chunk* Chunk, int YLevel, int MaxHeight,
			   int Octaves, float Persistence)
{
  
    float StackAdjustment = 1.0/MaxHeight;
    float PerlinLowerBound = YLevel*StackAdjustment;
    float PerlinUpperBound = PerlinLowerBound + StackAdjustment;

    float Noise, Height;

    for (int x = 0; x < Chunk->ChunkDimension; x++)
    {
	for (int z = 0; z < Chunk->ChunkDimension; z++)
	{
	    Noise = OctavePerlinNoise(XSeed, ZSeed, Octaves, Persistence);
	    
	    if (Noise < PerlinLowerBound)
	    {
		ChunkHeightFill(x, z, 0, Chunk);
	    }
	    if (Noise >= PerlinLowerBound && Noise < PerlinUpperBound)
	    {
		float AdjustedNoiseVal = (Noise - (StackAdjustment*YLevel)) / StackAdjustment;

		Height = (int)(AdjustedNoiseVal * Chunk->ChunkDimension);
		ChunkHeightFill(x, z, Height, Chunk);
	    }
	    if (Noise >= PerlinUpperBound)
	    {
		ChunkHeightFill(x, z, Chunk->ChunkDimension, Chunk);
	    }

	    ZSeed += Inc;
	}

	ZSeed -= (Inc * Chunk->ChunkDimension);
	XSeed += Inc;
    }

}

//TODO: implement heights
void
ChunkEarthBiomeWash(Chunk* Chunk)
{
    int StoneThresh, SnowThresh;

    StoneThresh = (int)(Chunk->ChunkDimension*0.2);
    SnowThresh = (int)(Chunk->ChunkDimension*0.6);

    for (int X = 0; X < Chunk->ChunkDimension; X++)
    {
	for (int Z = 0; Z < Chunk->ChunkDimension; Z++)
	{
	    int ChkHeight = GetChunkHeight(Chunk, X, Z);

	    int Y = 0;
	    while (Y < StoneThresh)
	    {
		SetType(&Chunk->Grid[X][Y][Z], CT_STONE);
		Y++;
	    }

	    while (Y < ChkHeight - 1)
	    {
		SetType(&Chunk->Grid[X][Y][Z], CT_DEEP_EARTH);
		Y++;
	    }
	    
	    if (Y < Chunk->ChunkDimension)
	    {
		SetType(&Chunk->Grid[X][Y][Z], CT_GRASS);
		Y++;
	    }

	    Y = SnowThresh;

	    while (Y >= SnowThresh && Y < Chunk->ChunkDimension)
	    {
		SetType(&Chunk->Grid[X][Y][Z], CT_SNOW);
		Y++;
	    } 
	}
    }
}
