#include "engine_assets.h"

#include <stdio.h>

void
LoadCubeTypeDefinitions(char* Path, CubeType* Types)
{

    FILE *CubeTypeFile;
   
    CubeTypeFile = fopen(Path, "rb");

    if (!CubeTypeFile)
    {
	printf("Cube Type file does not exist");
    }

    char Buffer[1000];
    char Line[1000];

    int ID;
    float rgb[3];

    while (fgets(Buffer, 1000, CubeTypeFile) != 0)
    {
	int index = 0;
	int i = 0;

	// Get the ID

	for (; Buffer[index] != ' '; index++, i++)
	{
	    Line[i] = Buffer[index];
	}
	
	Line[i] = ' ';

	ID = atoi(Line);
	
	// Get the color vector

	for (int k = 0; k < 3; k++)
	{
	    for (index++, i = 0;
		 Buffer[index] != ',' && Buffer[index] != '\n'
		     && Buffer[index] != ' ' && Buffer[index] != '\0';
		 i++, index++)
	    {
		Line[i] = Buffer[index];
	    }

	    Line[i] = ' ';

	    rgb[k] = atof(Line);
	}

	Types[ID].Color = Vec3f(rgb[0], rgb[1], rgb[2]);
    }

}

