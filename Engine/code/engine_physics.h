#ifndef H_ENGINE_PHYSICS
#define H_ENGINE_PHYSICS

#include "engine_math.h"
#include "engine_chunk.h"

typedef struct
PhysicsBody
{
    vec3f_t Position;
    vec3f_t Velocity;
    vec3f_t Gravity;
} PhysicsBody;

typedef struct
PhysicsAABB
{
    vec3f_t Min;
    vec3f_t Max;
} PhysicsAABB;

typedef struct
PhysicsStaticGeometry
{
    Chunk** ChunkList;
    PhysicsAABB* AABBList;
    int AABBCount;
} PhysicsStaticGeometry;

void
UpdatePhysicsBody(PhysicsBody* Body, float DeltaTime);

int
CheckAABBCollision(PhysicsAABB* AABB, PhysicsAABB* Other);

int
CheckStaticChunkCollisions(PhysicsAABB* AABB, 
			   PhysicsStaticGeometry* Geometry,
			   int ChunkDimension, int CubeSize,
			   vec3f_t* PenetrationVector);

int
HandleAABBCollision(PhysicsAABB* A, PhysicsAABB* B, PhysicsAABB* MDiff);

#endif
