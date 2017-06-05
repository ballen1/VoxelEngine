#include "engine_main.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "engine_sdl.h"
#include "engine_shader.h"
#include "engine_camera.h"
#include "engine_math.h"
#include "engine_cube.h"
#include "engine_chunk.h"
#include "engine_renderer.h"
#include "engine_clock.h"
#include "engine_generation.h"
#include "engine_world.h"
#include "engine_assets.h"
#include "engine_text.h"
#include "engine_ui.h"
#include "engine_physics.h"
#include "engine_console.h"

global GLuint program;

global PerspectiveProjection Projection;
global mat4f_t ProjectionMatrix;
global CubeWorld World;
global DirectionLight Light0;
global DirectionLight Light1;
global GameClock UClock;
global CubeType CubeTypes[128];

global RenderConfig RConfig;
global SkyBox WorldSkyBox;

global VEPlayer Player;
global PhysicsStaticGeometry* VoxelGeo;

global VEConsole Console;

internal void
draw_basic(SDL_Window* window, float DeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UpdateVEPlayer(&Player, VoxelGeo, DeltaTime);

    RenderSkyBox(&WorldSkyBox, &RConfig);
    RenderChunks(&RConfig, World.WorldChunks, World.ChunkCount,
		 UClock.Time, World.Apocalypse);
    if (World.Spheres)
    {
	RenderChunks(&RConfig, World.Sphere, SPHERE_DEMO_COUNT,
		     UClock.Time, World.Apocalypse);
    }
    UpdateTextElements(&UClock, &Player, &Console, GetActiveCubeCount(&World));
    RenderUI();

    SDL_GL_SwapWindow(window);

}

internal void
SetupCameraAndProjection()
{
    Projection.Width = 1280.0f;
    Projection.Height = 800.0f;
    Projection.VerticalFov = 45.0f;
    Projection.Near = 0.3f;
    Projection.Far = 10000.0f;

    ProjectionMatrix = GetPerspectiveProjection(&Projection);

    AttachFirstPersonCamera(&Player, 0.0f, 0.0f,
			    Vec3f(0.0f, 0.8f, 0.0f));
}

internal void
createScene()
{

    GLuint SkyBoxShaderProgram;
    CreateShaderProgram(&SkyBoxShaderProgram, "../data/shaders/skybox_vertex.vert",
			"../data/shaders/skybox_fragment.frag");
    WorldSkyBox.SkyBoxShader = SkyBoxShaderProgram;

    char* SkyBoxTextures[] = {
	"../data/assets/textures/right.png",
	"../data/assets/textures/left.png",
	"../data/assets/textures/top.png",
	"../data/assets/textures/bottom.png",
	"../data/assets/textures/front.png",
	"../data/assets/textures/back.png"
    };

    LoadSkyBox(&WorldSkyBox, SkyBoxTextures);

    CreateShaderProgram(&program, "../data/shaders/vertex.vert", "../data/shaders/fragment.frag");

    LoadCubeTypeDefinitions("../data/assets/cubetypes.vxe", CubeTypes);
    
    StartClock(&UClock);
}

int main(int argc, char* argv[])
{

    Light0.AmbientStrength = 0.1f;
    Light0.LightColor = Vec3f(1.0f, 1.0f, 1.0f);
    Light0.LightDirection = Vec3f(0.5f, -0.3f, 0.0f);
    Light1.AmbientStrength = 0.1f;
    Light1.LightColor = Vec3f(0.2f, 0.2f, 0.4f);
    Light1.LightDirection = Vec3f(-0.5f, 0.6f, 1.0f);

    WindowSDL Window;
    InputSDL Inputs;
    GameState g_state;

    g_state.mode = RUNNING;

    CreateWindowAndGLContext(&Window, "Engine", 1280, 800);

    createScene();

    CreatePlayer(&Player,
		 -5.0f, 32.0f, -5.0f,
		 0.8f, 0.8f);

    SetupCameraAndProjection();

    InitializeWorld(&World, CubeTypes, 18, 18, 4, DEFAULT_CHUNK_SIZE, 1.5, 1.5, 0.01,
		    6, 0.3f);

    LayoutTextElements(&Window, GetActiveCubeCount(&World));

    RConfig.FPSCamera = &Player.Camera;
    RConfig.ProjectionMatrix = ProjectionMatrix;
    RConfig.ChunkShaderProgram = program;
    RConfig.DirLight0 = &Light0;
    RConfig.DirLight1 = &Light1;

    VoxelGeo = GetStaticChunkGeometryInformation(&World);

    InitializeInputs(&Inputs);
    InitializeVEConsole(&Console, &World, CubeTypes);

    VESong Sita;
    VELoadSong(&Sita, "../data/assets/music/SitaMixTape.wav");
    VEPlayMusic(&Sita, 1);
    VEPauseMusic();

    while (!g_state.mode == SHOULD_EXIT)
    {
	UpdateClock(&UClock);
	float DeltaTime = UClock.DeltaTime;
	PollForInput(&Inputs);
	
	if (Inputs.Quit)
	{
	    g_state.mode = SHOULD_EXIT;
	}
	UpdateWorld(&World);

	if (World.PhysicsModelDirty)
	{
	    RebuildStaticChunkGeometryInformation(&World, VoxelGeo);
	    World.PhysicsModelDirty = 0;
	}

	UpdateVEConsole(&Console, &Inputs);
	
	if (!Console.Enabled)
	{
	    if (WasKeyPressed(&Inputs, K_U))
	    {
		ReseedWorld(&World, CubeTypes, 1.5, 1.5, 0.01, 6, 0.8f);
	    }
	    HandlePlayerInput(&Player, &Inputs, DeltaTime);
	}

	if (World.DayNightCycle)
	{
	    mat4f_t Light0Rot = RotationMat4f(0.1*DeltaTime, 0, 0, 1);
	    vec4f_t VResult = MultiplyVec4fByMat4f(Vec4f(Light0.LightDirection.x,
							 Light0.LightDirection.y,
							 Light0.LightDirection.z,
							 1),
						   Light0Rot);
	    Light0.LightDirection.x = VResult.x;
	    Light0.LightDirection.y = VResult.y;
	    Light0.LightDirection.z = VResult.z;
	}

	if (World.Apocalypse &&
	    (Light0.LightColor.y > 0.0f ||
	     Light0.LightColor.z > 0.0f))
	{
	    Light0.LightColor.y -= 0.005f;
	    Light0.LightColor.z -= 0.005f;
	    
	    if (Light0.LightColor.y < 0.0f ||
		Light0.LightColor.z < 0.0f)
	    {
		Light0.LightColor.y = 0.0f;
		Light0.LightColor.z = 0.0f;
	    }
	}
	
	if (!World.Apocalypse &&
	    (Light0.LightColor.y < 1.0f ||
	     Light0.LightColor.z < 1.0f))
	{
	    Light0.LightColor.y += 0.005f;
	    Light0.LightColor.z += 0.005f;
	    
	    if (Light0.LightColor.y > 1.0f ||
		Light0.LightColor.z > 1.0f)
	    {
		Light0.LightColor.y = 1.0f;
		Light0.LightColor.z = 1.0f;    
	    }

	}

	draw_basic(Window.Window, DeltaTime);
    }

    return(0);
}















