#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 ourColor;

uniform mat4 model;
uniform mat4 projection;

void main() {
    // Manually move the cube back by 200 on the Z axis
    vec4 pos = model * vec4(aPos, 1.0);
    pos.z -= 200.0; 
    
    gl_Position = projection * pos;
    ourColor = aColor;
}