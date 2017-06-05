#include <engine_shader.h>

#include <stdlib.h>

void
CreateShaderProgram(GLuint* Program, char* VertexShaderPath, char* FragmentShaderPath)
{
    FILE *VertexShaderFile, *FragmentShaderFile;
    char *VertexShader, *FragmentShader;
    int fileSize, c;

    VertexShaderFile = fopen(VertexShaderPath, "rb");
    
    if (!VertexShaderFile)
    {
	printf("Vertex shader file does not exist");
    }

    fseek(VertexShaderFile, 0, SEEK_END);
    fileSize = ftell(VertexShaderFile);
    fseek(VertexShaderFile, 0, SEEK_SET);

    VertexShader = malloc(fileSize+1);

    if (!VertexShader)
    {
	printf("Could not allocate memory for vertex shader");
    }

    fread(VertexShader, 1, fileSize, VertexShaderFile);
    VertexShader[fileSize] = '\0';

    fclose(VertexShaderFile);

    FragmentShaderFile = fopen(FragmentShaderPath, "rb");

    if (!FragmentShaderFile)
    {
	printf("Fragment shader file does not exist");
    }

    fseek(FragmentShaderFile, 0, SEEK_END);
    fileSize = ftell(FragmentShaderFile);
    fseek(FragmentShaderFile, 0, SEEK_SET);

    FragmentShader = malloc(fileSize+1);

    if (!FragmentShader)
    {
	printf("Could not allocate memory for fragment shader");
    }

    fread(FragmentShader, 1, fileSize, FragmentShaderFile);
    FragmentShader[fileSize] = '\0';

    fclose(FragmentShaderFile);
  
    GLuint GL_VertexShader, GL_FragmentShader;
    GLint Success;
    GLchar InfoLog[512];

    GL_VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(GL_VertexShader, 1, &VertexShader, 0);
    glCompileShader(GL_VertexShader);

    glGetShaderiv(GL_VertexShader, GL_COMPILE_STATUS, &Success);

    if (!Success)
    {
	glGetShaderInfoLog(GL_VertexShader, 512, 0, InfoLog);
	printf("ERROR->SHADER->VERTEX->COMPILATION: SEE LOG BELOW\n");
	printf(InfoLog);
    }

    GL_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(GL_FragmentShader, 1, &FragmentShader, 0);
    glCompileShader(GL_FragmentShader);

    glGetShaderiv(GL_FragmentShader, GL_COMPILE_STATUS, &Success);

    if (!Success)
    {
	glGetShaderInfoLog(GL_FragmentShader, 512, 0, InfoLog);
	printf("ERROR->SHADER->FRAGMENT->COMPILATION: SEE LOG BELOW\n");
	printf(InfoLog);
    }

    *Program = glCreateProgram();
    
    glAttachShader(*Program, GL_VertexShader);
    glAttachShader(*Program, GL_FragmentShader);
    glLinkProgram(*Program);

    glGetProgramiv(*Program, GL_LINK_STATUS, &Success);

    if (!Success)
    {
	glGetProgramInfoLog(*Program, 512, 0, InfoLog);
	printf("ERROR->PROGRAM->LINKING: SEE LOG BELOW\n");
	printf(InfoLog);
    }

    glDeleteShader(GL_VertexShader);
    glDeleteShader(GL_FragmentShader);
    
    free(VertexShader);
    free(FragmentShader);

}
