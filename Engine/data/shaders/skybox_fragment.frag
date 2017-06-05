#version 330 core

in vec3 textureCoordinates;

out vec4 color;

uniform samplerCube skyBoxTexture;

void main()
{
    color = texture(skyBoxTexture, textureCoordinates);
}
