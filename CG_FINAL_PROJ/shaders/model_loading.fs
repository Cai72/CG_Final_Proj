#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec4 lightColor;

void main()
{    
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
	float ambientStrength = 0.3;
    vec4 ambient = ambientStrength * lightColor;

	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec4 diffuse = diff * lightColor;

	// specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;  

    vec4 result = (ambient + diffuse + specular) * textureColor;

    FragColor = result;
}