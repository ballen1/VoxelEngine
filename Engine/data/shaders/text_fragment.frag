#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D fontTexture;

void main()
{
    vec4 TexColor = texture(fontTexture, texCoord);

    if (TexColor.a < 0.1)
       discard;

    color = TexColor;
}