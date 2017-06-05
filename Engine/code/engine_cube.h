#ifndef H_ENGINE_CUBE
#define H_ENGINE_CUBE

#include "engine_main.h"
#include "engine_math.h"

#include <stdint.h>

global float DEFAULT_CUBE_SIZE = 1.0f;
global int CUBE_BUFFER_INDICES = 216;

#define CT_DEEP_EARTH 0
#define CT_STONE 1
#define CT_GRASS 2
#define CT_SNOW 3
#define CT_DEEP_EARTH_SEC 4
#define CT_STONE_SEC 5
#define CT_GRASS_SEC 6
#define CT_SNOW_SEC 7

typedef struct
Cube
{
    uint8_t Info;
} Cube;

typedef struct
CubeType
{
    vec3f_t Color;
} CubeType;

void
SetActive(Cube *Cube, unsigned int ActiveFlag);

void
SetType(Cube *Cube, unsigned int TypeFlag);

int
GetActive(Cube *Cube);

int
GetType(Cube *Cube);

void
GenerateCube(float* vertices,
	     vec3f_t* position,
	     float dimension);

#endif
