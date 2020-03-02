#version 330 core
out vec4 color;

in vec2 texCoords;

uniform sampler2D particleTex;

void main()
{
    color = texture(particleTex, texCoords);
    //color = vec4(texCoords.xy,1,1);
} 

