#version 330 core

// The color output of our fragment shader
out vec4 FragColor;

// The color input received from the vertex shader
in vec3 ourColor;

void main() {
   
    FragColor = vec4(ourColor, 1.0);
  
}