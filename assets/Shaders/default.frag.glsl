#version 330 core

// The color output of our fragment shader
out vec4 FragColor;

// The color input received from the vertex shader

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material {
    vec4 baseColor;
    int hasTexture;
    int hasSpecular;
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};
uniform Material material;

uniform vec3 viewPos;

void main() {
    // 1. Decide de onde vem a cor base (Textura ou Cor Sólida do glTF)
    vec4 finalBaseColor;
    if(material.hasTexture == 1) {
        finalBaseColor = texture(material.texture_diffuse1, TexCoord);
    } else {
        finalBaseColor = material.baseColor;
    }

    vec3 albedo = vec3(finalBaseColor);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 specularMapColor = vec3(0.5);
    if(material.hasSpecular == 1) {
        specularMapColor = vec3(texture(material.texture_specular1, TexCoord));
    }

    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * specularMapColor; 
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, finalBaseColor.a);
}