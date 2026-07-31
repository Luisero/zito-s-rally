#version 330 core

// The color output of our fragment shader
out vec4 FragColor;

// The color input received from the vertex shader
in vec3 pos;

void main() {
   
    FragColor = vec4(1.0f,0.f,0.f,1.f);
    FragColor.r += sin(pos.x);
    FragColor.g += sin(pos.z);
}