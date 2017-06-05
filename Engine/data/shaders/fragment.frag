#version 330 core

in vec3 objectColor;
in vec3 vertexNormal;
in vec3 worldPosition;

out vec4 color;

uniform vec3 ambientLight;

uniform vec3 lightColor0;
uniform vec3 lightDirection0;

uniform vec3 lightColor1;
uniform vec3 lightDirection1;

void main()
{ 
    vec3 fragNormal = normalize(vertexNormal);

    vec3 fragLight0Dir = normalize(-(lightDirection0));
    vec3 fragLight1Dir = normalize(-(lightDirection1));

    vec3 diffuseLight0 = max(dot(fragNormal, fragLight0Dir), 0.0) * lightColor0;
    vec3 diffuseLight1 = max(dot(fragNormal, fragLight1Dir), 0.0) * lightColor1;

    color = vec4((diffuseLight0 + diffuseLight1 + ambientLight) * objectColor, 1.0f);
}