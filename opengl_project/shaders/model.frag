#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform DirLight dirLight;
uniform vec3 viewPos;
uniform Material material;


vec3 CalculateDirectional(DirLight light, vec3 norm, vec3 viewDir)
{
    // normalize direction of light
    vec3 lightDir = normalize(-light.direction);

    // ambient light
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;

    // diffuse light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb;

    // specular light 
	vec3 reflectDir = reflect(-lightDir, norm); // direction of reflected light (using normalized normal)
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb;


	return(specular + ambient + diffuse); // returns light values as vec3
}



void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalculateDirectional(dirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
    
}