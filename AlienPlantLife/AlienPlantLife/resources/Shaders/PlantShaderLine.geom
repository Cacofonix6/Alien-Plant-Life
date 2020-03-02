#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 100) out;


in vec3 Normal[];
in vec3 NextNorm[];
in float ID[];
out vec4 Colour;

uniform mat4 world;

const float PI = 3.1415926;


void main()
{
	float red = ID[0] / 5;
	Colour = vec4(red, 0, 0, 0);

	//////////// pos ////////////

	gl_Position = world * gl_in[0].gl_Position;
	EmitVertex();

	//////////// Next pos ////////////
	
	vec4 pos = gl_in[0].gl_Position + vec4(Normal[0], 0);
	gl_Position = world * pos;
	EmitVertex();	

	
	
}
