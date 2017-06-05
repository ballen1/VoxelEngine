#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

out vec3 objectColor;
out vec3 vertexNormal;
out vec3 worldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform bool apocalypse;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    if (apocalypse)
    {
	gl_Position.x += sin(time);
	gl_Position.y += cos(time);
    }

    objectColor = color;
    worldPosition = vec3(model * vec4(position, 1.0f));
    vertexNormal = normal;
}