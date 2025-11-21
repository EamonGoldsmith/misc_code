#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;

uniform float Time;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 62);
    
    // apply material
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
    vec3 emission = vec3(0.0);
    vec2 tex_offset = vec2(0.0, Time / 1000.0) + TexCoords;
    if (length(texture(material.specular, TexCoords).rgb) < 0.1) {
        emission = texture(material.emission, tex_offset).rgb;
    }
            
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}