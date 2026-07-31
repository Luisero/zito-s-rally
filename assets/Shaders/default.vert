#version 330 core

// The 3D position of the vertex coming from the application
layout (location = 0) in vec3 aPos; 

// The color variable we will send to the fragment shader
out vec4 vColor;

void main() {
    // gl_Position is a built-in variable to specify where the vertex is on the screen
    // We add 1.0 at the end to make it a 4D vector (vec4)
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    
    // Send a solid red color to the fragment shader (R, G, B, A)
    vColor = vec4(1.0, 0.0, 0.0, 1.0); 
}