#version 330 core
layout (location = 0) in float aID;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aNextNorm;
layout (location = 4) in vec3 aForward;
layout (location = 5) in vec3 aNextForward;
layout (location = 6) in vec3 aPrevForward;

out vec3 Normal;
out vec3 NextNorm;
out float ID;

void main()
{
	ID = aID;

    Normal = aNormal;
    NextNorm = aNextNorm;  

    
	gl_Position = vec4(aPos, 1.0);    
}