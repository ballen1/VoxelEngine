#ifndef H_ENGINE_PLAYER
#define H_ENGINE_PLAYER

#include "engine_physics.h"
#include "engine_camera.h"
#include "engine_sdl.h"

typedef struct
VEPlayer
{
    PhysicsBody Body;
    PhysicsAABB Hitbox;
    float HitboxWidth;
    float HitboxHeight;
    FirstPersonCamera Camera;
    vec3f_t CameraPosition;
    int FlyMode;
} VEPlayer;

void
CreatePlayer(VEPlayer* Player,
	     float StartX, float StartY, float StartZ,
	     float HitboxWidth, float HitboxHeight);

void
AttachFirstPersonCamera(VEPlayer* Player,
			float StartPitch, float StartYaw,
			vec3f_t CameraPosition);

void
HandlePlayerInput(VEPlayer* Player, InputSDL* Inputs, float DeltaTime);

void
UpdatePlayerHitbox(VEPlayer* Player);

void
UpdateVEPlayer(VEPlayer* Player, PhysicsStaticGeometry* VoxelGeography, float DeltaTime);

#endif
