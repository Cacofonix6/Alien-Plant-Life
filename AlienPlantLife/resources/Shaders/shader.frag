#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos; 

uniform vec3 lightDir; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{    
	// ambient
	float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 dir = normalize(-lightDir);  
    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    FragColor = vec4((diffuse + ambient) * objectColor, 1.0);
}

