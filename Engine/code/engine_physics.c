#include "engine_physics.h"

void
UpdatePhysicsBody(PhysicsBody* Body, float DeltaTime)
{
    Body->Velocity = AddVec3f(Body->Gravity, Body->Velocity);
    
    vec3f_t Vel = MultiplyVec3f(Body->Velocity, DeltaTime);

    Body->Position = AddVec3f(Body->Position, Vel);
}

int
CheckAABBCollision(PhysicsAABB* AABB, PhysicsAABB* Other)
{
    if (AABB->Max.x < Other->Min.x || AABB->Min.x > Other->Max.x) return (0);
    if (AABB->Max.y < Other->Min.y || AABB->Min.y > Other->Max.y) return (0);
    if (AABB->Max.z < Other->Min.z || AABB->Min.z > Other->Max.z) return (0);

    return (1);
}

int
CheckStaticCollisions(PhysicsAABB* AABB, 
		      PhysicsStaticGeometry* Geometry,
		      int ChunkDimension, int CubeSize,
		      vec3f_t* PenetrationVector)
{
    int HitCount = 0;

    for (int i = 0; i < Geometry->AABBCount; i++)
    {
	// Check if there is any overlap between the AABB and the entire cube chunk
	if (CheckAABBCollision(AABB, &Geometry->AABBList[i]))
	{

	    PhysicsAABB CubeAABB;
	    PhysicsAABB MDiff;

	    // Now check for specific cube collisions
	    for (int X = 0; X < ChunkDimension; X++)
	    {
		CubeAABB.Min.x = Geometry->AABBList[i].Min.x + X*CubeSize;

		for (int Z = 0; Z < ChunkDimension; Z++)
		{
		    CubeAABB.Min.z = Geometry->AABBList[i].Min.z + Z*CubeSize;
		    for (int Y = 0; Y < ChunkDimension; Y++)
		    {
			if (GetActive(&(*Geometry->ChunkList)[i].Grid[X][Y][Z]))
			{
			    CubeAABB.Min.y = Geometry->AABBList[i].Min.y + Y*CubeSize;
			    CubeAABB.Max.x = CubeAABB.Min.x + CubeSize;
			    CubeAABB.Max.y = CubeAABB.Min.y + CubeSize;
			    CubeAABB.Max.z = CubeAABB.Min.z + CubeSize;

			    if (HandleAABBCollision(AABB, &CubeAABB, &MDiff))
			    {
				float MinDistX, MaxDistX,
				      MinDistY, MaxDistY,
				      MinDistZ, MaxDistZ;

				vec3f_t PenVec;

				MinDistX = VE_Fabs(MDiff.Min.x);
				MaxDistX = VE_Fabs(MDiff.Max.x);
				MinDistY = VE_Fabs(MDiff.Min.y);
				MaxDistY = VE_Fabs(MDiff.Max.y);
				MinDistZ = VE_Fabs(MDiff.Min.z);
				MaxDistZ = VE_Fabs(MDiff.Max.z);

				float Minimum = MinDistX;
				PenVec = Vec3f(MDiff.Min.x, 0.0f, 0.0f);

				if (MaxDistX < Minimum)
				{
				    Minimum = MaxDistX;
				    PenVec = Vec3f(MDiff.Max.x, 0.0f, 0.0f);
				}
				if (MinDistY < Minimum)
				{
				    Minimum = MinDistY;
				    PenVec = Vec3f(0.0f, MDiff.Min.y, 0.0f);   
				}
				if (MaxDistY < Minimum)
				{
				    Minimum = MaxDistY;
				    PenVec = Vec3f(0.0f, MDiff.Max.y, 0.0f);
				}
				if (MinDistZ < Minimum)
				{
				    Minimum = MinDistZ;
				    PenVec = Vec3f(0.0f, 0.0f, MDiff.Min.z);
				}
				if (MaxDistZ < Minimum)
				{
				    Minimum = MaxDistZ;
				    PenVec = Vec3f(0.0f, 0.0f, MDiff.Max.z);
				}
				
				*PenetrationVector = PenVec;
				return (1);
			    }
			}
		    }
		}
	    }
	    
	}
    }
    return(0);
}

int
HandleAABBCollision(PhysicsAABB* A, PhysicsAABB* B, PhysicsAABB* MDiff)
{
    float MDiffX, MDiffY, MDiffZ,
	  SumX, SumY, SumZ;

    MDiffX = B->Min.x - A->Max.x;
    MDiffY = B->Min.y - A->Max.y;
    MDiffZ = B->Min.z - A->Max.z;

    SumX = (A->Max.x - A->Min.x) + (B->Max.x - B->Min.x);
    SumY = (A->Max.y - A->Min.y) + (B->Max.y - B->Min.y);
    SumZ = (A->Max.z - A->Min.z) + (B->Max.z - B->Min.z);
    
    // If it encompasses the origin, there is a collision
    if (MDiffX < 0 && (MDiffX + SumX) > 0 &&
	MDiffY < 0 && (MDiffY + SumY) > 0 &&
	MDiffZ < 0 && (MDiffZ + SumZ) > 0)
    {
	MDiff->Min.x = MDiffX;
	MDiff->Min.y = MDiffY;
	MDiff->Min.z = MDiffZ;
	
	MDiff->Max.x = MDiffX + SumX;
	MDiff->Max.y = MDiffY + SumY;
	MDiff->Max.z = MDiffZ + SumZ;
	return (1);
    }
    else
    {
	return (0);
    }
}
