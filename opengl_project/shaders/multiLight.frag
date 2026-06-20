#version 330 core

// supports 16 point lights 1 directional light and 1 spotlight
// supports textures 

struct Material
{
	
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

};


struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// constant linear and quadratic terms for calculating light attenuation
	float constant;
	float linear;
	float quadratic;
	
};

/*
struct SpotLight;
{
	vec3 position;
	vec3 direction;

	float cutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};
*/

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

// pre processing for 16 max lights in program
#define MAX_NR_LIGHTS 16 
uniform PointLight pointLights[MAX_NR_LIGHTS];
uniform int pointLightCount; // number of point lights being used
uniform Material material;
uniform vec3 viewPos;
uniform DirLight dirLight;


vec3 CalculateDirectional(DirLight light, vec3 norm, vec3 viewDir)
{
	// normalize direction of light
	vec3 lightDir = normalize(-light.direction); 


	// ambient light 
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;


	// diffuse light
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	

	// specular light 
	vec3 reflectDir = reflect(-lightDir, norm); // direction of reflected light (using normalized normal)
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;


	return(specular + ambient + diffuse); // returns light values as vec3

}


// maybe calculate point light and calculate directional light can be made into one function 
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos)
{
	// normalized light direction based on light and Frag position
	vec3 lightDir = normalize(light.position - fragPos);

	// ambient light
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	// diffuse light
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

	// specular light
	vec3 reflectDir = reflect(-lightDir, norm); // direction of reflected light (using normalized normal)
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

	// attenuation values
	float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  				light.quadratic * (distance * distance)); 

	// changes strength of light based of attenuation (distance from point source)
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;


	return(specular + ambient + diffuse); // returns light values as vec3


}

/*
vec3 CalculateSpotlight()
{

}
*/


void main()
{
	
	
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalculateDirectional(dirLight, norm, viewDir);

	for( int i = 0; i < pointLightCount; i++)
	{
		result += CalculatePointLight(pointLights[i], norm, viewDir, FragPos);
	}

	FragColor = vec4(result, 1.0);

}