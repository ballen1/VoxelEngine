#include "engine_player.h"

global VEAudioClip PlayerJump;
global VEAudioClip PlayerFall;

void
CreatePlayer(VEPlayer* Player,
	     float StartX, float StartY, float StartZ,
	     float HitboxWidth, float HitboxHeight)
{
    VELoadAudioClip(&PlayerJump, "../data/assets/music/Jump.wav", 1);
    VELoadAudioClip(&PlayerFall, "../data/assets/music/Fall.wav", 2);

    Player->Body.Position.x = StartX;
    Player->Body.Position.y = StartY;
    Player->Body.Position.z = StartZ;

    Player->Body.Velocity.x = 0.0f;
    Player->Body.Velocity.y = 0.0f;
    Player->Body.Velocity.z = 0.0f;

    Player->HitboxWidth = HitboxWidth;
    Player->HitboxHeight = HitboxHeight;

    UpdatePlayerHitbox(Player);

    Player->FlyMode = 1;
}

void
AttachFirstPersonCamera(VEPlayer* Player,
			float StartPitch, float StartYaw,
			vec3f_t CameraPosition)
{
    Player->Camera.Position = Player->Body.Position;
    Player->Camera.Pitch = StartPitch;
    Player->Camera.Yaw = StartYaw;

    Player->CameraPosition = CameraPosition;
    Player->Camera.Position = AddVec3f(Player->Camera.Position, Player->CameraPosition);
}

void
HandlePlayerInput(VEPlayer* Player, InputSDL* Inputs, float DeltaTime)
{
    if (WasKeyPressed(Inputs, K_F))
    {
	Player->FlyMode = !(Player->FlyMode);

	if (!Player->FlyMode)
	{
	    Player->Body.Gravity.y = -0.0005f;
	}
	else
	{
	    Player->Body.Gravity.y = 0.0f;
	}
    }

    if (Player->FlyMode)
    {
	if (Inputs->Keys[K_W])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    Forward = MultiplyVec3f(Forward, 0.15f*DeltaTime);
	    Player->Body.Position = AddVec3f(Player->Body.Position, Forward);
	}
	if (Inputs->Keys[K_S])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    Forward = MultiplyVec3f(Forward, 0.15f*DeltaTime);
	    Player->Body.Position = SubtractVec3f(Forward, Player->Body.Position);
	}
	if (Inputs->Keys[K_D])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    vec3f_t Right = FPCamRight(Forward);
	    Right = MultiplyVec3f(Right, 0.15f*DeltaTime);
	    Player->Body.Position = AddVec3f(Player->Body.Position, Right);
	}
	if (Inputs->Keys[K_A])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    vec3f_t Right = FPCamRight(Forward);
	    Right = MultiplyVec3f(Right, 0.15f*DeltaTime);
	    Player->Body.Position = SubtractVec3f(Right, Player->Body.Position);
	}
    }

    if (!Player->FlyMode)
    {
	if (Inputs->Keys[K_W])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    Forward = MultiplyVec3f(Forward, 0.02f*DeltaTime);
	    Player->Body.Position = AddVec3f(Player->Body.Position, Forward);
	}
	if (Inputs->Keys[K_S])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    Forward = MultiplyVec3f(Forward, 0.02f*DeltaTime);
	    Player->Body.Position = SubtractVec3f(Forward, Player->Body.Position);
	}
	if (Inputs->Keys[K_D])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    vec3f_t Right = FPCamRight(Forward);
	    Right = MultiplyVec3f(Right, 0.02f*DeltaTime);
	    Player->Body.Position = AddVec3f(Player->Body.Position, Right);
	}
	if (Inputs->Keys[K_A])
	{
	    vec3f_t Forward = FPCamFront(&Player->Camera);
	    vec3f_t Right = FPCamRight(Forward);
	    Right = MultiplyVec3f(Right, 0.02f*DeltaTime);
	    Player->Body.Position = SubtractVec3f(Right, Player->Body.Position);
	}
	if (WasKeyPressed(Inputs, K_SPACE))
	{
	    VEPlayAudioClip(&PlayerJump, 0);
	    Player->Body.Velocity.y = 0.02f;
	}
    }

    int X, Y;

    SDL_GetRelativeMouseState(&X, &Y);

    Player->Camera.Yaw += X * 0.05f;
    Player->Camera.Pitch += Y * 0.05f;
}

void
UpdatePlayerHitbox(VEPlayer* Player)
{
    Player->Hitbox.Min.x = Player->Body.Position.x - (Player->HitboxWidth/2.0);
    Player->Hitbox.Min.y = Player->Body.Position.y - (Player->HitboxHeight/2.0);
    Player->Hitbox.Min.z = Player->Body.Position.z - (Player->HitboxWidth/2.0);

    Player->Hitbox.Max.x = Player->Body.Position.x + (Player->HitboxWidth/2.0);
    Player->Hitbox.Max.y = Player->Body.Position.y + (Player->HitboxHeight/2.0);
    Player->Hitbox.Max.z = Player->Body.Position.z + (Player->HitboxWidth/2.0);
}

void
UpdateVEPlayer(VEPlayer* Player, PhysicsStaticGeometry* VoxelGeography, float DeltaTime)
{
    if (!Player->FlyMode)
    {
	UpdatePhysicsBody(&Player->Body, DeltaTime);
	UpdatePlayerHitbox(Player);

	if (Player->Body.Velocity.y < -0.09 && !VEIsChannelPlaying(&PlayerFall))
	{
	    VEPlayAudioClip(&PlayerFall, 0);
	}
	if (Player->Body.Velocity.y > -0.006 && VEIsChannelPlaying(&PlayerFall))
	{
	    VEFadeAudioClip(&PlayerFall, 500);
	}

	vec3f_t PenVec;
	
	int ResolutionCount = 0;

	while (CheckStaticCollisions(&Player->Hitbox, VoxelGeography, 32, 1, &PenVec))
	{
	    Player->Body.Position = AddVec3f(Player->Body.Position, PenVec);
	    Player->Hitbox.Min = AddVec3f(Player->Hitbox.Min, PenVec);
	    Player->Hitbox.Max = AddVec3f(Player->Hitbox.Max, PenVec);

	    if (PenVec.y != 0)
	    {
		Player->Body.Velocity.y = 0.0f;
	    }

	    ResolutionCount++;

	    if (ResolutionCount >= 50)
	    {
		Player->FlyMode = 1;
		break;
	    }
	}
    }
    Player->Camera.Position = AddVec3f(Player->Body.Position, Player->CameraPosition);
}
