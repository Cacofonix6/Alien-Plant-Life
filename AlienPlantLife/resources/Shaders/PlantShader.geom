#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 100) out;

in float ID[];
in vec3 Direction[];
in vec3 NextDir[];
in vec3 PrevDir[];
in float Size[];
in float NextSize[];
//in vec3 Forward[];
//in vec3 NextForward[];
//in vec3 PrevForward[];

out vec4 Colour;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 world;
uniform mat4 model;
uniform float growthFactor;
uniform float mutator;

const float PI = 3.1415926;

// A rotation matrix is required to create the cylindrical trianlge strip
mat4 createRotationMatrix(vec3 _axis, float _angle)
{
	vec3 axis = normalize(_axis);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	float co = cos(_angle);
	float si = sin(_angle);

	return mat4(co + (x*x)*(1-co) , x*y*(1-co)-z*si  , x*z*(1-co)+y*si, 0.0,
				y*x*(1-co)+z*si   , co +(y*y)*(1-co) , y*z*(1-co)-x*si, 0.0,
				z*x*(1-co)-y*si   , z*y*(1-co)+x*si  , co+(z*z)*(1-co), 0.0,
				0.0				  , 0.0 			 , 0.0			  , 1.0);
}

vec3 rotate(vec3 _vec, vec3 _axis, float _angle)
{
	mat4 rMat = createRotationMatrix(_axis, _angle);

	return (rMat * vec4(_vec, 1.0)).xyz;
}

// growingCircularGeometry is the guts of the shader
void growingCircularGeometry(vec4 _position)
{
	///////////////////////////////////////////////////////////////////////
	// "Normal" in this shader is the growth direction. it is an antequated
	// name but I have yet to change it

	// verticalGrowthSeg and nextVerticalGrowthSeg use the ID's of the vertices to determine 
	// when it should start and stop growing 
	float verticalGrowthSeg = clamp(growthFactor - ID[0], 0, 1);
	float nextVerticalGrowthSeg = clamp(growthFactor - (ID[0] + 1), 0, 1);

	// this is a logarithmic function that determines the witdh of the segment, it is logarithmic so that
	// the growth plateaus as it time goes on
	//mutator = 50;
	//float growthSeg = max(log2(     ((growthFactor - (ID[0] + 1)) + mutator) / mutator   ), 0);
	//float nextGrowthSeg = max(log2(    ((growthFactor - (ID[0] + 2)) + mutator) / mutator   ), 0);

	float size = Size[0] / 2;
	float nextSize = NextSize[0] / 2;

	float growthSeg = max((growthFactor - (ID[0] + 1)) / (mutator - size), 0);
	float nextGrowthSeg = max((growthFactor - (ID[0] + 2)) / (mutator - nextSize), 0);

	// This looks really wierd but I read that "if" statements aren't very efficient in 
	// shaders so this is the result.
	// basically the last vertex will know its the last one because the growth direction 
	// will be a zero vector. so this checks if the vector is zero by adding the x, y and z
	// getting the absolute value, rounding it up and clamping it between 1 and 0. This essentially
	// gives us a boolean value of 1 or 0
	int hasNext = int(clamp(ceil(abs(NextDir[0].x + NextDir[0].y + NextDir[0].z)), 0, 1));

	vec3 forward = cross(Direction[0],  normalize(Direction[0] + vec3(0,1,1)));
	vec3 prevForward = cross(PrevDir[0], normalize(PrevDir[0] + vec3(0,1,1)));
	vec3 nextForward = cross(NextDir[0], normalize(NextDir[0] + vec3(0,1,1)));

	// these calculations basically give us the plane in which the segments join
	// bottom of the segment is a plane between the forward and right of the previous node
	// and the forward and right of the current node
	vec3 prevSharedNorm = normalize(Direction[0] + PrevDir[0]);	
	vec3 prevSharedForward = normalize(forward + prevForward);
	vec3 prevSharedRight = normalize(cross(prevSharedNorm, prevSharedForward));

	// and the top of the segment is between the current and next planes.
	vec3 nextSharedNorm = normalize(Direction[0] + NextDir[0]);
	vec3 nextSharedForward = normalize(forward + nextForward);
	vec3 nextSharedRight = normalize(cross(nextSharedNorm, nextSharedForward));

	// this calculates how far the tip has made it between the nodes	
	vec4 growthPoint = vec4(Direction[0] * nextVerticalGrowthSeg, 0);

	// this stops the last node growing out so the limb finishes at a point using the 
	// has next pseudo boolean explained above
	nextGrowthSeg = nextGrowthSeg * hasNext;

	vec4 pos;	
	vec4 offset;
	vec4 projection;

	// this is the amount of edges the circumference will have
	int edgeCount = 6;

	
	// now we loop through and create all the vertices of the triangle strip.
	// We will emit the bottom vertex of the segment and then the top vertex 
	// then rotate and do it again. this creates a strip of triangles the axis 
	// that is the growth direction
	for (int i = 0; i < edgeCount; i++)
	{
		// first we get the angle of each segment
		float angle = PI * 2.0 * i / edgeCount;

		// create the offset at the bottom of the circumference which will start
		// at the direction between the forwards of the previous node and this node,
		// explained above
		//                   vec to rotate      axis            angle
		offset = vec4(rotate(prevSharedForward, prevSharedNorm, angle), 0);		

		// then set the position of the vertex to be the position of the node
		// plus the offset multiplied by the scaling factor of the growth witdth
		pos = _position + (offset * growthSeg);

		// convert the position to world space
		gl_Position = world * pos;

		// send the model space position to the frag shader for lighting
		FragPos = vec3(model * pos);

		// send the normal of the vertex to the fragment shader for lighting
		// (which is conveniently just the offset, which should be a unit vector but normalize incase)
		Normal = normalize(offset.xyz);

		TexCoords = vec2(float(i) / float(edgeCount),1);
		//TexCoords = vec2(mod(i+1, 2),1);

		Colour = vec4(mod(i, 2), 0, 0, 1);
		EmitVertex();


		// We then do basically the same thing for the top vertex except we use the
		// plane between this node and the next node...		
		offset = vec4(rotate(nextSharedForward, nextSharedNorm, angle), 0);

		// ...and we offset the plane to where the growth point is
		pos = _position + (offset * nextGrowthSeg) + growthPoint;
		gl_Position = world * pos;
		FragPos = vec3(model * pos);
		Normal = normalize(offset.xyz);

		TexCoords = vec2(float(i) / float(edgeCount),0);
		//TexCoords = vec2(mod(i+1, 2),0);
		Colour = vec4(mod(i, 2), 0, 0, 1);
		EmitVertex();
	}

	// we finish the triangle strip by adding the first 2 points again
	offset = vec4(rotate(prevSharedForward, prevSharedNorm, 0), 0);
	pos = _position + (offset * growthSeg);
	gl_Position = world * pos;
	FragPos = vec3(model * pos);
	Normal = normalize(offset.xyz);
	Colour = vec4(0, 0, 0, 1);
	TexCoords = vec2(1,1);
	EmitVertex();

	offset = vec4(rotate(nextSharedForward, nextSharedNorm, 0), 0);
	pos = _position + (offset * nextGrowthSeg) + growthPoint;
	gl_Position = world * pos;
	FragPos = vec3(model * pos);
	Normal = normalize(offset.xyz);
	Colour = vec4(0, 0, 0, 1);
	TexCoords = vec2(1,0);
	EmitVertex();

    EndPrimitive();	

}



void main()
{
	float red = 70;
	float green = 30;
	float blue = 9;
	//Colour = vec4(red/256, green/256, blue/256, 1);

	growingCircularGeometry(gl_in[0].gl_Position);

}

