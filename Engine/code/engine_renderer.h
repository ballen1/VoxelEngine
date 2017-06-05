#ifndef H_ENGINE_RENDERER
#define H_ENGINE_RENDERER

#include "engine_math.h"
#include "engine_camera.h"
#include "engine_chunk.h"
#include "engine_text.h"
#include "engine_sdl.h"

#include <GL/glew.h>

typedef struct TextureAtlas TextureAtlas;

typedef struct
DirectionLight
{

    vec3f_t LightColor;
    vec3f_t LightDirection;
    float AmbientStrength;
    
} DirectionLight;

typedef struct
RenderConfig
{

    FirstPersonCamera* FPSCamera;
    mat4f_t ProjectionMatrix;
    GLuint ChunkShaderProgram;
    DirectionLight* DirLight0;
    DirectionLight* DirLight1;

} RenderConfig;

typedef struct
FontRenderConfig
{
    GLuint FontTexture;
    GLuint FontShader;
    float* TextRenderBuffer;
    int* RenderIndexBuffer;
    int BufferLengths;
    int BufferCount;
    int* GlyphCounts;
    GLuint* VBOS;
    GLuint* VAOS;
    GLuint* EBOS;
} FontRenderConfig;

typedef struct
SkyBox
{
    GLuint SkyBoxShader;
    GLuint SkyBoxTexture;
    GLuint VAO;
    GLuint VBO;
} SkyBox;

void
CreateTriangleVertexList(float* vertices,
                         vec3f_t v1,
			 vec3f_t v2,
			 vec3f_t v3,
			 vec3f_t normal,
                         vec3f_t color);

vec3f_t
GetAmbientLightingDL(DirectionLight* Light);

void
InitializeChunkRenderBuffers(Chunk* Chunk, float* RenderBuffer);

void
UpdateChunkRenderBuffers(Chunk* Chunk, float* RenderBuffer);

void
RenderChunks(RenderConfig* Config, Chunk* Chunks, int ChunkCount,
	     int Time, int ApocalypseMode);

void
GenerateQuadFace(float* vertices,
		 vec3f_t v1,
		 vec3f_t v2,
		 vec3f_t v3,
		 vec3f_t v4,
                 vec3f_t normal,
                 vec3f_t color);

void
InitializeFontRenderer(FontRenderConfig* RenderConfig,
		       TextureAtlas* Image,
		       int VBOCount,
		       int BufferSize,
		       GLuint Shader);

void
FreeFontRenderBuffers(FontRenderConfig* RenderConfig);

void
CreateTextBuffer(char* Text, int Buffer,
		 FontRenderConfig* RenderConfig,
		 FontTextureAtlas* Atlas,
		 vec2f_t Origin,
		 int FontSize,
		 WindowSDL* Window);

void
RenderText(int Buffer,
	   FontRenderConfig* RenderConfig);

vec2f_t
ScreenCoordToGLCoord(float X, float Y, WindowSDL* Window);

void
LoadSkybox(SkyBox* RSkybox, char** SkyBoxImages);

void
RenderSkyBox(SkyBox* RSkybox, RenderConfig* RConfig);

#endif
