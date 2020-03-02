#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	mat4 world = projection * view * model;

	vec4 pos = gl_in[0].gl_Position;

   	vec4 newPos = pos + vec4(-0.5, -0.5, 0, 0);
    gl_Position = world * newPos;
   	texCoords = vec2(0,1);
    EmitVertex();

    newPos = pos + vec4(-0.5, 0.5, 0, 0);
    gl_Position = world * newPos;
   	texCoords = vec2(0,0);
    EmitVertex();

    newPos = pos + vec4(0.5, -0.5, 0, 0);
    gl_Position = world * newPos;
   	texCoords = vec2(1,1);
    EmitVertex();

    newPos = pos + vec4(0.5, 0.5, 0, 0);
    gl_Position = world * newPos;
   	texCoords = vec2(1,0);
    EmitVertex();

    EndPrimitive();	
}
