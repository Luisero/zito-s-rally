#version 330 core

// The color output of our fragment shader
out vec4 FragColor;

// The color input received from the vertex shader
in vec4 vColor;

void main() {
    // Set the final output color of the pixel to the color passed from the vertex shader
    FragColor = vColor;
}