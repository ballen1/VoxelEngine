#include "engine_text.h"

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void
LoadCharacterMapping(FontTextureAtlas* Atlas,
		     char* TexturePath,
		     char* MappingPath)
{
    FILE *MappingFile;

    MappingFile = fopen(MappingPath, "rb");
    
    if (!MappingFile)
    {
	printf("Mapping file does not exist");
    }

    char Buffer[1000];
    char Line[1000];

    // Skip first 3 lines for now
    for (int i = 0; i < 3; i++)
    {
	fgets(Buffer, 1000, MappingFile);
    }
    
    // Get char count
    int CharCount;
    int Index = 0, LineIndex = 0;

    fgets(Buffer, 1000, MappingFile);

    for (; Buffer[Index] != '='; Index++) {};

    Index++;
    for (; Buffer[Index] != '\n' && Buffer[Index] != ' '; Index++, LineIndex++)
    {
	Line[LineIndex] = Buffer[Index];
    }

    Line[LineIndex] = '\n';

    CharCount = atoi(Line);

    for (int i = 0; i < CharCount; i++)
    {
	if (fgets(Buffer, 1000, MappingFile) != 0)
	{
	    Index = 0;

	    // Get ASCII Character ID
      	    int ID = ParseIntValue(Buffer, &Index);

	    Atlas->Characters[ID].ID = ID;

	    // Get X position
	    Atlas->Characters[ID].X = ParseIntValue(Buffer, &Index);

	    // Get Y position
	    Atlas->Characters[ID].Y = ParseIntValue(Buffer, &Index);
	    
	    // Get Width
	    Atlas->Characters[ID].Width = ParseIntValue(Buffer, &Index);

	    // Get Height
	    Atlas->Characters[ID].Height = ParseIntValue(Buffer, &Index);

	    // X Offset
	    Atlas->Characters[ID].XOffset = ParseIntValue(Buffer, &Index);

	    // Y Offset
	    Atlas->Characters[ID].YOffset = ParseIntValue(Buffer, &Index);
	}
    }

    Atlas->AtlasImage.ImageFile = stbi_load("../data/font/TestFont_0.tga",
					   &(Atlas->AtlasImage.Width),
					   &(Atlas->AtlasImage.Height),
					   &(Atlas->AtlasImage.BPP), 0);
    if (!(Atlas->AtlasImage.ImageFile))
    {
	printf("Loading of Bitmap Font File failed\n");
	exit(1);
    }
}

void
FreeCharacterMapping(FontTextureAtlas* Atlas)
{
    stbi_image_free(Atlas->AtlasImage.ImageFile);
}

int
ParseIntValue(char* Buffer, int* Index)
{
    char Line[1000];
    memset(Line, '\n', sizeof(Line));
    int LineIndex = 0;

    for (; Buffer[*Index] != '='; (*Index)++) {};
    (*Index)++;

    for (; Buffer[(*Index)] != ' '; (*Index)++, LineIndex++)
    {
	Line[LineIndex] = Buffer[(*Index)];
    }

    Line[LineIndex] = '\n';
   
    return (atoi(Line));
}

void
CharacterGetGLDimensions(CharacterMapping* Character,
			 TextureAtlas* Image,
			 vec2f_t* Origin,
                         float* Width, float* Height,
			 float* YOffset)
{

    Origin->x = ((float)Character->X) / ((float)Image->Width);
    Origin->y = ((float)Character->Y) / ((float)Image->Height);
    *Width = (((float)Character->Width) / ((float)Image->Width));
    *Height = (((float)Character->Height) / ((float)Image->Height));
    *YOffset = (((float)Character->YOffset) / ((float)Image->Height));
}
