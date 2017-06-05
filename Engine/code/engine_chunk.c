#include "engine_chunk.h"

#include <stdlib.h>
#include <math.h>

void
InitializeChunk(Chunk* Chk, int size, vec3f_t Transform)
{
    Chk->Transform.x = Transform.x;
    Chk->Transform.y = Transform.y;
    Chk->Transform.z = Transform.z;

    Chk->ChunkDimension = size;

    Chk->Grid = (Cube***)malloc(size * sizeof(Cube**));
    Chk->ActiveCubes = 0;

    Cube** YData = (Cube**)malloc(size * size * sizeof(Cube*));
    Cube* ZData = (Cube*)malloc(size * size * size * sizeof(Cube));

    int CubeID = rand() % 3;

    for (int x = 0; x < size; x++)
    {
	Chk->Grid[x] = YData + (x * size);
	
	for (int y = 0; y < size; y++)
	{
	    Chk->Grid[x][y] = ZData + (x*size*size) + (y*size);
	    
	    for (int z = 0; z < size; z++)
	    {
		//SetActive(&Chk->Grid[x][y][z], (rand() <= 1500));
		SetActive(&Chk->Grid[x][y][z], 1);
		SetType(&Chk->Grid[x][y][z], CubeID);

		if (GetActive(&Chk->Grid[x][y][z]))
		{
		    Chk->ActiveCubes++;
		}
	    }
	}
    }
}

void
FreeChunk(Chunk* Chk)
{
    free(**Chk->Grid);
    free(*Chk->Grid);
    free(Chk->Grid);
}

void
GreedyMesh(Chunk* Chk, CubeType* CubeTypes,
	   float* RenderBuffer, int* VertexCount)
{
    *VertexCount = 0;
    Chk->MeshVertexCount = 0;

    int CubeFace;

    int FrontFace, BackFace,
	Front, Back;

    int u, v;

    int index[] = {0, 0, 0};
    int disp[] = {0, 0, 0};

    int MaskSize = (Chk->ChunkDimension * Chk->ChunkDimension);
    int* Mask = malloc(sizeof(int)*MaskSize);

    int MaskIndex = 0;

    int QWidth, QHeight;

    for (FrontFace = 1, BackFace = 0; !(FrontFace && BackFace); )
    {
	for (int dim = 0; dim < 3; dim++)
	{

	    // Determine CubeFace
	    if (BackFace)
	    {
		switch (dim)
		{
		    case 0:
			CubeFace = LEFT;
			break;
		    case 1:
			CubeFace = BOTTOM;
			break;
		    case 2:
			CubeFace = BACK;
			break;
		    default:
			break;
		}
	    }
	    else if (FrontFace)
	    {
		switch (dim)
		{
		    case 0:
			CubeFace = RIGHT;
			    break;
		    case 1:
			CubeFace = TOP;
			    break;
		    case 2:
			CubeFace = FORWARD;
			    break;
		    default:
			break;
		}
	    }

	    u = (dim+1) % 3;
	    v = (dim+2) % 3;

	    index[0] = 0;
	    index[1] = 0;
	    index[2] = 0;

	    disp[0] = 0;
	    disp[1] = 0;
	    disp[2] = 0;
	    disp[dim] = 1;

	    for (index[dim] = -1; index[dim] < Chk->ChunkDimension; )
	    {      
		// Reset the mask
		MaskIndex = 0;
				
		// Create a mask for each two dimensional plane
		for (index[u] = 0; index[u] < Chk->ChunkDimension; index[u]++)
		{
		    for (index[v] = 0; index[v] < Chk->ChunkDimension; index[v]++)
		    {
			Front = (index[dim] >= 0) ? 
			    GetActive(&Chk->Grid[index[0]][index[1]][index[2]]) : 0;

			Back = (index[dim] < Chk->ChunkDimension - 1) ?
			    GetActive(&Chk->Grid[index[0]+disp[0]]
				      [index[1]+disp[1]]
				      [index[2]+disp[2]]) : 0;
		       	
			int FrontType = -1;
			int BackType = -1;

			if (Front && index[dim] >= 0)
			{
			    FrontType = GetType(&Chk->Grid[index[0]][index[1]][index[2]]);
			}
			if (Back && index[dim] < Chk->ChunkDimension - 1)
			{
			    BackType = GetType(&Chk->Grid[index[0]+disp[0]][index[1]+disp[1]][index[2]+disp[2]]);
			}

			Mask[MaskIndex++] = (Front != 0 && Back != 0 && FrontType == BackType) ? -1 :
			    BackFace ? BackType : FrontType;

		    }
		}
		
		index[dim]++;

		// Generate mesh
		MaskIndex = 0;

		for (int i = 0; i < Chk->ChunkDimension; i++)
		{
		    for (int j = 0; j < Chk->ChunkDimension; )
		    {
			if (Mask[MaskIndex] != -1)
			{
			    
			    // Width of Quad
			    for (QWidth = 1;
				 (Mask[MaskIndex + QWidth] != -1 && Mask[MaskIndex + QWidth] == Mask[MaskIndex] && j + QWidth < Chk->ChunkDimension);
				 ++QWidth) {}

			    // Height of Quad
			    int Done = 0;

			    for (QHeight = 1; i + QHeight < Chk->ChunkDimension; QHeight++)
			    {
				for (int k = 0; k < QWidth; k++)
				{
				    if (Mask[MaskIndex + k + QHeight*Chk->ChunkDimension] == -1 || Mask[MaskIndex + k + QHeight*Chk->ChunkDimension] != Mask[MaskIndex])
				    {
					Done = 1;
					break;
				    }
				}

				if (Done)
				{
				    break;
				}
			    }

			    index[u] = i;
			    index[v] = j;

			    int index_Du[] = {0, 0, 0};
			    int index_Dv[] = {0, 0, 0};

			    index_Du[u] = QHeight;
			    index_Dv[v] = QWidth;
			    
			    AddQuad(Chk, CubeFace,
				    Vec3f(index[0], index[1], index[2]),
				    Vec3f(index[0] + index_Dv[0],
					  index[1] + index_Dv[1],
					  index[2] + index_Dv[2]),
				    Vec3f(index[0] + index_Du[0] + index_Dv[0],
					  index[1] + index_Du[1] + index_Dv[1],
					  index[2] + index_Du[2] + index_Dv[2]),
				    Vec3f(index[0] + index_Du[0],
					  index[1] + index_Du[1],
					  index[2] + index_Du[2]),
				    Mask[MaskIndex], CubeTypes,
				    RenderBuffer, VertexCount);

			    // Munch away at the part of the mask that we just consumed
			    for (int t = 0; t < QHeight; t++)
			    {
				for (int s = 0; s < QWidth; s++)
				{
				    Mask[MaskIndex + s + t*Chk->ChunkDimension] = -1;
				}
			    }

			    MaskIndex += QWidth;
			    j += QWidth;
			}
			else
			{
			    MaskIndex++;
			    j++;
			}
		    }
		}
	    }
	}

	if (FrontFace)
	{
	    BackFace = 1;
	    FrontFace = 0;
	}
	else if (BackFace)
	{
	    FrontFace = 1;
	}
    }

    free(Mask);
}

void
ChunkHeightFill(int x, int z, int height, Chunk* Chk)
{
    for (int y = 0; y < height; y++)
    {
	if (!GetActive(&Chk->Grid[x][y][z]))
	{
	    (Chk->ActiveCubes)++;
	    SetActive(&Chk->Grid[x][y][z], 1);
	}
    }
    for (int y = height; y < Chk->ChunkDimension; y++)
    {
	if (GetActive(&Chk->Grid[x][y][z]))
	{
	    (Chk->ActiveCubes)--;
	    SetActive(&Chk->Grid[x][y][z], 0);
	}
    }
}

void
AddQuad(Chunk* Chk, int Side,
        vec3f_t P1, vec3f_t P2,
        vec3f_t P3, vec3f_t P4,
	int CubeID, CubeType* Types,
	float* RenderBuffer, int* VertexCount)
{

    vec3f_t Normal;

    switch(Side)
    {
	case LEFT:
	    Normal = Vec3f(-1.0f, 0.0f, 0.0f);
	    break;
	case RIGHT:
	    Normal = Vec3f(1.0f, 0.0f, 0.0f);
	    break;
	case BACK:
	    Normal = Vec3f(0.0f, 0.0f, -1.0f);
	    break;
	case FORWARD:
	    Normal = Vec3f(0.0f, 0.0f, 1.0f);
	    break;
	case TOP:
	    Normal = Vec3f(0.0f, 1.0f, 0.0f);
	    break;
	case BOTTOM:
	    Normal = Vec3f(0.0f, -1.0f, 0.0f);
	    break;
	default:
	    break;
    }

    GenerateQuadFace(RenderBuffer+(RENDER_FLOATS_PER_CUBE_VERTEX*
					  *VertexCount),
		     P1, P2, P3, P4,
		     Normal,
	             Types[CubeID].Color);

   *VertexCount += 6;
   Chk->MeshVertexCount += 6;

}

Cube*
GetCubeFromPosition(Chunk* Chk, vec3f_t Position)
{

    vec3f_t Max = Vec3f(Chk->Transform.x + Chk->ChunkDimension,
			Chk->Transform.y + Chk->ChunkDimension,
			Chk->Transform.z + Chk->ChunkDimension);

    if (Position.x < Chk->Transform.x || Position.x >= Max.x) return(0);
    if (Position.y < Chk->Transform.y || Position.y >= Max.y) return(0);
    if (Position.z < Chk->Transform.z || Position.z >= Max.z) return(0);

    return (&Chk->Grid[(int)floor(Position.x)][(int)floor(Position.y)][(int)floor(Position.z)]);
}


// TODO: Get the height of the chunk at a certain X, Z position
int
GetChunkHeight(Chunk* Chk, int X, int Z)
{
    int Height;
    for (Height = 0; Height < Chk->ChunkDimension; Height++)
    {
	if (!GetActive(&Chk->Grid[X][Height][Z]))
	{
	    break;
	}
    }
    return (Height);
}
