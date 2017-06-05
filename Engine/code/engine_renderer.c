#include "engine_renderer.h"
#include "stb_image.h"
#include <stdlib.h>

void
CreateTriangleVertexList(float* vertices,
                         vec3f_t v1,
			 vec3f_t v2,
			 vec3f_t v3,
                         vec3f_t normal,
                         vec3f_t color)
{

    vertices[0] = v1.x;
    vertices[1] = v1.y;
    vertices[2] = v1.z;

    vertices[3] = normal.x;
    vertices[4] = normal.y;
    vertices[5] = normal.z;

    vertices[6] = color.x;
    vertices[7] = color.y;
    vertices[8] = color.z;

    vertices[9] = v2.x;
    vertices[10] = v2.y;
    vertices[11] = v2.z;

    vertices[12] = normal.x;
    vertices[13] = normal.y;
    vertices[14] = normal.z;

    vertices[15] = color.x;
    vertices[16] = color.y;
    vertices[17] = color.z;

    vertices[18] = v3.x;
    vertices[19] = v3.y;
    vertices[20] = v3.z;

    vertices[21] = normal.x;
    vertices[22] = normal.y;
    vertices[23] = normal.z;    

    vertices[24] = color.x;
    vertices[25] = color.y;
    vertices[26] = color.z;

}

vec3f_t GetAmbientLightingDL(DirectionLight* Light)
{
    return MultiplyVec3f(Light->LightColor, Light->AmbientStrength);
}

void InitializeChunkRenderBuffers(Chunk* Chunk, float* RenderBuffer)
{

    int RenderByteSize = (Chunk->MeshVertexCount) * RENDER_FLOATS_PER_CUBE_VERTEX * sizeof(float);

    glGenVertexArrays(1, &Chunk->VAO);
    glGenBuffers(1, &Chunk->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, Chunk->VBO);
    glBufferData(GL_ARRAY_BUFFER, RenderByteSize,
		 RenderBuffer, GL_STATIC_DRAW);

    glBindVertexArray(Chunk->VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			  9*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

void
FreeChunkRenderBuffers(Chunk* Chunk)
{
    glDeleteVertexArrays(1, &Chunk->VAO);
    glDeleteBuffers(1, &Chunk->VBO);
}

void
UpdateChunkRenderBuffers(Chunk* Chunk, float* RenderBuffer)
{
    int RenderByteSize = (Chunk->MeshVertexCount) * RENDER_FLOATS_PER_CUBE_VERTEX * sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, Chunk->VBO);
    glBufferData(GL_ARRAY_BUFFER, RenderByteSize,
		 RenderBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
RenderChunks(RenderConfig* Config, Chunk* Chunks, int ChunkCount,
	     int Time, int ApocalypseMode)
{

    glUseProgram(Config->ChunkShaderProgram);

    mat4f_t Model, View;

    GLint ModelLoc = glGetUniformLocation(Config->ChunkShaderProgram, "model");
    GLint ViewLoc = glGetUniformLocation(Config->ChunkShaderProgram, "view");
    GLint ProjLoc = glGetUniformLocation(Config->ChunkShaderProgram, "projection");
    GLint LightColor0Loc = glGetUniformLocation(Config->ChunkShaderProgram, "lightColor0");
    GLint LightDir0Loc = glGetUniformLocation(Config->ChunkShaderProgram, "lightDirection0");
    GLint LightColor1Loc = glGetUniformLocation(Config->ChunkShaderProgram, "lightColor1");
    GLint LightDir1Loc = glGetUniformLocation(Config->ChunkShaderProgram, "lightDirection1");
    GLint AmbientLightLoc = glGetUniformLocation(Config->ChunkShaderProgram, "ambientLight");
    GLint TimeLoc = glGetUniformLocation(Config->ChunkShaderProgram, "time");
    GLint ApocalypseLoc = glGetUniformLocation(Config->ChunkShaderProgram, "apocalypse");

    glUniform1f(TimeLoc, Time);
    glUniform1i(ApocalypseLoc, ApocalypseMode);

    View = GetFirstPersonView(Config->FPSCamera);

    glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, View.mat);
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, Config->ProjectionMatrix.mat);

    glUniform3f(LightColor0Loc,
		Config->DirLight0->LightColor.x,
		Config->DirLight0->LightColor.y,
		Config->DirLight0->LightColor.z);

    glUniform3f(LightDir0Loc, 
		Config->DirLight0->LightDirection.x,
		Config->DirLight0->LightDirection.y,
		Config->DirLight0->LightDirection.z);

    glUniform3f(LightColor1Loc,
		Config->DirLight1->LightColor.x,
		Config->DirLight1->LightColor.y,
		Config->DirLight1->LightColor.z);

    glUniform3f(LightDir1Loc, 
		Config->DirLight1->LightDirection.x,
		Config->DirLight1->LightDirection.y,
		Config->DirLight1->LightDirection.z);

    vec3f_t Lighting = GetAmbientLightingDL(Config->DirLight0);

    glUniform3f(AmbientLightLoc, Lighting.x, Lighting.y, Lighting.z);

    for (int i = 0; i < ChunkCount; i++)
    {
  
       Model = TranslationMat4f(Chunks[i].Transform.x,
				Chunks[i].Transform.y,
				Chunks[i].Transform.z);
  
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, Model.mat);

	glBindVertexArray(Chunks[i].VAO);
	glDrawArrays(GL_TRIANGLES, 0, Chunks[i].MeshVertexCount);
	glBindVertexArray(0);

    }

}

void
GenerateQuadFace(float* vertices,
		 vec3f_t v1,
		 vec3f_t v2,
		 vec3f_t v3,
		 vec3f_t v4,
                 vec3f_t normal,
                 vec3f_t color)
{
    
    CreateTriangleVertexList(vertices, v1, v2, v3, normal, color);

    CreateTriangleVertexList(vertices+(3*RENDER_FLOATS_PER_CUBE_VERTEX), 
			     v1, v3, v4, normal, color);
   
}

void
InitializeFontRenderer(FontRenderConfig* RenderConfig,
		       TextureAtlas* Image,
		       int VBOCount,
                       int BufferSize,
		       GLuint Shader)
{
    
    RenderConfig->FontShader = Shader;

    glGenTextures(1, &RenderConfig->FontTexture);
    glBindTexture(GL_TEXTURE_2D, RenderConfig->FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		 Image->Width,
		 Image->Height,
		 0,
		 GL_RGBA, GL_UNSIGNED_BYTE,
		 Image->ImageFile);

    glBindTexture(GL_TEXTURE_2D, 0);

    RenderConfig->TextRenderBuffer = malloc(BufferSize * sizeof(float));
    RenderConfig->RenderIndexBuffer = malloc(BufferSize * sizeof(int));
    RenderConfig->GlyphCounts = malloc(VBOCount * sizeof(int));
    RenderConfig->VBOS = malloc(VBOCount * sizeof(GLuint*));
    RenderConfig->VAOS = malloc(VBOCount * sizeof(GLuint*));
    RenderConfig->EBOS = malloc(VBOCount * sizeof(GLuint*));

    for (int i = 0; i < VBOCount; i++)
    {
	glGenVertexArrays(1, &RenderConfig->VAOS[i]);
	glGenBuffers(1, &RenderConfig->VBOS[i]);
	glGenBuffers(1, &RenderConfig->EBOS[i]);
    }

    RenderConfig->BufferCount = VBOCount;
    RenderConfig->BufferLengths = BufferSize;

}

void
FreeFontRenderBuffers(FontRenderConfig* RenderConfig)
{
    free(RenderConfig->TextRenderBuffer);
    free(RenderConfig->RenderIndexBuffer);
}

void
CreateTextBuffer(char* Text, int Buffer,
		 FontRenderConfig* RenderConfig,
		 FontTextureAtlas* Atlas,
		 vec2f_t Origin,
		 int FontSize,
		 WindowSDL* Window)
{

    //vec2f_t GLOrigin = ScreenCoordToGLCoord(Origin.x, Origin.y, Window);
    vec2f_t GLOrigin;
    float XPos = 0.0f;
    float YOffset = 0.0f;

    float GLBaseWidth = 0.03f * FontSize;
    float GLBaseHeight = 0.03f * FontSize;
    float GLBaseSpace = 0.0001 * FontSize;

    float GLWidth, GLHeight;

    int i = 0;
    
    while (Text[i] != '\0')
    {
	vec2f_t TexOrigin;
	float TexWidth, TexHeight;

	CharacterGetGLDimensions(&Atlas->Characters[Text[i]],
				 &Atlas->AtlasImage,
				 &TexOrigin,
				 &TexWidth, &TexHeight, &YOffset);
	
	GLWidth = TexWidth * GLBaseWidth;
	GLHeight = TexHeight * GLBaseHeight;

	float TexX1 = TexOrigin.x;
	float TexX2 = TexOrigin.x + TexWidth;
	float TexY1 = TexOrigin.y;
	float TexY2 = TexOrigin.y + TexHeight;

	GLOrigin = ScreenCoordToGLCoord(Origin.x,
					Origin.y,
					Window);

	GLOrigin.x += XPos;

	GLOrigin.y -= YOffset * GLBaseHeight;

	RenderConfig->TextRenderBuffer[(i*20)] = GLOrigin.x + GLWidth;
	RenderConfig->TextRenderBuffer[(i*20)+1] = GLOrigin.y - GLHeight;
	RenderConfig->TextRenderBuffer[(i*20)+2] = 0.0f;
	RenderConfig->TextRenderBuffer[(i*20)+3] = TexX2;
	RenderConfig->TextRenderBuffer[(i*20)+4] = TexY2;

	RenderConfig->TextRenderBuffer[(i*20)+5] = GLOrigin.x + GLWidth;
	RenderConfig->TextRenderBuffer[(i*20)+6] = GLOrigin.y;
	RenderConfig->TextRenderBuffer[(i*20)+7] = 0.0f;
	RenderConfig->TextRenderBuffer[(i*20)+8] = TexX2;
	RenderConfig->TextRenderBuffer[(i*20)+9] = TexY1;

	RenderConfig->TextRenderBuffer[(i*20)+10] = GLOrigin.x;
	RenderConfig->TextRenderBuffer[(i*20)+11] = GLOrigin.y;
	RenderConfig->TextRenderBuffer[(i*20)+12] = 0.0f;
	RenderConfig->TextRenderBuffer[(i*20)+13] = TexX1;
	RenderConfig->TextRenderBuffer[(i*20)+14] = TexY1;

	RenderConfig->TextRenderBuffer[(i*20)+15] = GLOrigin.x;
	RenderConfig->TextRenderBuffer[(i*20)+16] = GLOrigin.y - GLHeight;
	RenderConfig->TextRenderBuffer[(i*20)+17] = 0.0f;
	RenderConfig->TextRenderBuffer[(i*20)+18] = TexX1;
	RenderConfig->TextRenderBuffer[(i*20)+19] = TexY2;

	RenderConfig->RenderIndexBuffer[(i*6)] = (i*4);
	RenderConfig->RenderIndexBuffer[(i*6)+1] = (i*4)+1;
	RenderConfig->RenderIndexBuffer[(i*6)+2] = (i*4)+3;
	RenderConfig->RenderIndexBuffer[(i*6)+3] = (i*4)+1;
	RenderConfig->RenderIndexBuffer[(i*6)+4] = (i*4)+2;
	RenderConfig->RenderIndexBuffer[(i*6)+5] = (i*4)+3;

	XPos += GLWidth + GLBaseSpace;

	i++;
    }

    glBindVertexArray(RenderConfig->VAOS[Buffer]);
    glBindBuffer(GL_ARRAY_BUFFER, RenderConfig->VBOS[Buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*i*20,
		 RenderConfig->TextRenderBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderConfig->EBOS[Buffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*i*6,
		 RenderConfig->RenderIndexBuffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    RenderConfig->GlyphCounts[Buffer] = i;
}

void
RenderText(int Buffer,
	   FontRenderConfig* RenderConfig)
{

    glUseProgram(RenderConfig->FontShader);

    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, RenderConfig->FontTexture);

    glBindVertexArray(RenderConfig->VAOS[Buffer]);
    glDrawElements(GL_TRIANGLES, (6*RenderConfig->GlyphCounts[Buffer]), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);

}

vec2f_t
ScreenCoordToGLCoord(float X, float Y, WindowSDL* Window)
{

    float GLX, GLY;

    float PercentX = X / (float)Window->ScreenWidth;
    GLX = (2*PercentX) - 1.0f;

    float PercentY = Y / (float)Window->ScreenHeight;
    GLY = (2*PercentY) - 1.0f;

    return (Vec2f(GLX, GLY));
}

void
LoadSkyBox(SkyBox* RSkybox, char** SkyBoxImages)
{
    glGenTextures(1, &RSkybox->SkyBoxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, RSkybox->SkyBoxTexture);

    int ImageWidth[6], ImageHeight[6], BPP;
    unsigned char* Image[6];

    for (int i = 0; i < 6; i++)
    {
	Image[i] = stbi_load(SkyBoxImages[i],
			     &ImageWidth[i],
			     &ImageHeight[i],
			     &BPP, 0);

	if (!Image[i])
	{
	    printf("Loading of skybox texture failed\n");
	    exit(1);
	}
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA,
		 ImageWidth[0], ImageHeight[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[0]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA,
		 ImageWidth[1], ImageHeight[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[1]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA,
		 ImageWidth[2], ImageHeight[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[2]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA,
		 ImageWidth[3], ImageHeight[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[3]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA,
		 ImageWidth[4], ImageHeight[4], 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[4]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA,
		 ImageWidth[5], ImageHeight[5], 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[5]);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    GLfloat Vertices[] = {
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
  
	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,
  
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
   
	-10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,
  
	-10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,
  
	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f
    };

    glGenVertexArrays(1, &RSkybox->VAO);
    glGenBuffers(1, &RSkybox->VBO);
    glBindVertexArray(RSkybox->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, RSkybox->VBO);
    glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    for (int i = 0; i < 6; i++)
    {
	stbi_image_free(Image[i]);
    }

}

void
RenderSkyBox(SkyBox* RSkybox, RenderConfig* RConfig)
{
    glDepthMask(GL_FALSE);
    glUseProgram(RSkybox->SkyBoxShader);

    GLint ViewLoc = glGetUniformLocation(RSkybox->SkyBoxShader, "view");
    GLint ProjLoc = glGetUniformLocation(RSkybox->SkyBoxShader, "projection");

    mat4f_t View = GetFirstPersonView(RConfig->FPSCamera);

    View.mat[12] = 0;
    View.mat[13] = 0;
    View.mat[14] = 0;

    glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, View.mat);
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, RConfig->ProjectionMatrix.mat);

    glBindVertexArray(RSkybox->VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, RSkybox->SkyBoxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}
