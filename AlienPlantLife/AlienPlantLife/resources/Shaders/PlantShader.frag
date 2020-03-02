#version 330 core
out vec4 FragColor;

in vec4 Colour;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 LightPos;
uniform sampler2D texture1;

void main()
{        
	vec3 lightColour = vec3(1,1,1);

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColour;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3(0,1,0));

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;

	vec3 result = (ambient + diffuse);

    //FragColor = vec4(result, 1.0);
    //FragColor = Colour;
    //FragColor = vec4(TexCoords, 0, 0);

    FragColor = texture(texture1, TexCoords) * vec4(result, 1);
}
