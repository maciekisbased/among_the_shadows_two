#version 330 core
struct Material
{
    sampler2D texture_diffuse1;
};
in vec2 TexCoord;
out vec4 FragColor;
uniform Material material;
void main()
{
    vec3 result = texture(material.texture_diffuse1, TexCoord).rgb;
    FragColor = vec4(result, 1.0);
}