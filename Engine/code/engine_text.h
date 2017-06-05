#ifndef H_ENGINE_TEXT
#define H_ENGINE_TEXT

#include <GL/glew.h>

#include "engine_math.h"

typedef struct
CharacterMapping
{
    int ID;
    int X;
    int Y;
    int Width;
    int Height;
    int XOffset;
    int YOffset;
} CharacterMapping;

typedef struct
TextureAtlas
{
    int Width;
    int Height;
    int BPP;
    unsigned char* ImageFile;
} TextureAtlas;

typedef struct
FontTextureAtlas
{
    TextureAtlas AtlasImage;
    CharacterMapping Characters[256];
} FontTextureAtlas;

void
LoadCharacterMapping(FontTextureAtlas* Atlas,
		     char* TexturePath,
		     char* MappingPath);

void
FreeCharacterMapping(FontTextureAtlas* Atlas);

int
ParseIntValue(char* Buffer, int* Index); 

void
CharacterGetGLDimensions(CharacterMapping* Character,
                         TextureAtlas* Image,
                         vec2f_t* Origin,
			 float* Width, float* Height,
			 float* YOffset);

#endif
