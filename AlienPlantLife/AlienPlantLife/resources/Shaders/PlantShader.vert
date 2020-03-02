#version 330 core
layout (location = 0) in float aID;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aDirection;
layout (location = 3) in vec3 aNextDir;
layout (location = 4) in vec3 aPrevDir;
layout (location = 5) in float aSize;
layout (location = 6) in float aNextSize;
//layout (location = 5) in vec3 aForward;
//layout (location = 6) in vec3 aNextForward;
//layout (location = 7) in vec3 aPrevForward;

out float ID;
out vec3 Direction;
out vec3 NextDir;
out vec3 PrevDir;
out float Size;
out float NextSize;
//out vec3 Forward;
//out vec3 NextForward;
//out vec3 PrevForward;

void main()
{
	ID = aID;

    Direction = aDirection;
    NextDir = aNextDir;  
    PrevDir = aPrevDir; 

    Size = aSize;
    NextSize = aNextSize;
    //Forward = aForward;
    //NextForward = aNextForward;  
    //PrevForward = aPrevForward;  

    
	gl_Position = vec4(aPos, 1.0);    
}