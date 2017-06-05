#include "engine_cube.h"
#include "engine_renderer.h"

void
SetActive(Cube* Cube, unsigned int ActiveFlag)
{
    uint8_t InfoFill;
    
    InfoFill = (Cube->Info >> 1);
    InfoFill = (Cube->Info << 1);
    InfoFill = (InfoFill | ActiveFlag);

    Cube->Info = InfoFill;
}

void
SetType(Cube* Cube, unsigned int TypeFlag)
{
    unsigned int ActiveFlag;

    ActiveFlag = (Cube->Info & 1);
    Cube->Info = TypeFlag;
    Cube->Info = ((Cube->Info << 1) | ActiveFlag);
}

int
GetActive(Cube *Cube)
{
    return (Cube->Info & 1);
}

int
GetType(Cube *Cube)
{
    return (Cube->Info >> 1);
}

void
GenerateCube(float* vertices,
	     vec3f_t* position,
             float dimension)
{

    vec3f_t P1 = Vec3f(dimension*position->x,
		       dimension*position->y,
		       dimension*position->z);

    vec3f_t P2 = Vec3f(dimension*position->x,
	               dimension*position->y + dimension,
	               dimension*position->z);

    vec3f_t P3 = Vec3f(dimension*position->x + dimension,
	               dimension*position->y + dimension,
	               dimension*position->z);
    vec3f_t P4 = Vec3f(dimension*position->x + dimension,
	               dimension*position->y,
	               dimension*position->z);

    vec3f_t P5 = Vec3f(dimension*position->x,
	               dimension*position->y,
	               dimension*position->z + dimension);

    vec3f_t P6 = Vec3f(dimension*position->x,
	               dimension*position->y + dimension,
	               dimension*position->z + dimension);

    vec3f_t P7 = Vec3f(dimension*position->x + dimension,
	               dimension*position->y + dimension,
	               dimension*position->z + dimension);

    vec3f_t P8 = Vec3f(dimension*position->x + dimension,
	               dimension*position->y,
	               dimension*position->z + dimension);

    vec3f_t Normal = Vec3f(0.0f, 0.0f, -1.0f);

    GenerateQuadFace(vertices,
		     P1, P2, P3, P4,
	             Normal,
	             Vec3f(1.0, 0.0, 0.0));
   
    Normal = Vec3f(1.0f, 0.0f, 0.0f);

    GenerateQuadFace(vertices + 36,
		     P4, P3, P7, P8, 
		     Normal,
	             Vec3f(1.0, 0.0, 0.0));

    Normal = Vec3f(0.0f, -1.0f, 0.0f);

    GenerateQuadFace(vertices + 72,
		     P1, P5, P8, P4, 
		     Normal,
		     Vec3f(1.0, 0.0, 0.0));

    Normal = Vec3f(0.0f, 1.0f, 0.0f);

    GenerateQuadFace(vertices + 108,
		     P2, P6, P7, P3,
	             Normal,
		     Vec3f(1.0, 0.0, 0.0));

    Normal = Vec3f(-1.0f, 0.0f, 0.0f);

    GenerateQuadFace(vertices + 144,
		     P5, P6, P2, P1,
	             Normal,
		     Vec3f(1.0, 0.0, 0.0));

    Normal = Vec3f(0.0f, 0.0f, 1.0f);

    GenerateQuadFace(vertices + 180,
		     P8, P7, P6, P5, 
		     Normal,
		     Vec3f(1.0, 0.0, 0.0));

}
