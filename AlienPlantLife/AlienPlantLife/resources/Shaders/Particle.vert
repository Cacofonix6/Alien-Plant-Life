#version 330 core
layout (location = 0) in vec3 aPos; // <vec2 position, vec2 texCoords>


void main()
{
	gl_Position = vec4(aPos, 1.0);    

    //float scale = 10.0f;
    //TexCoords = vertex.zw;
    //ParticleColor = color;
    //gl_Position = projection * vec4((vertex.xy * scale) + offset.xy, 0.0, 1.0);
}
