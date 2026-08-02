#version 330 core

// The color output of our fragment shader
out vec4 FragColor;

// The color input received from the vertex shader

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


uniform vec3 lightColor;
uniform vec3 lightPos;  


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;


uniform sampler2D ourTexture;

void main() {
   
    //FragColor = vec4(ourColor, 1.0);
   // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);  
    vec3 norm = normalize(Normal);
    
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient + diffuse;
    FragColor = texture(ourTexture, TexCoord) * vec4(result,1.0);
}