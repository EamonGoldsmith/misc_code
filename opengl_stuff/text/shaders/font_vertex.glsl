#version 330 core
layout (location = 0) in vec4 vertex; // (x, y, u, v)

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    // Fix: Change Z from 0.0 to 0.1 (or any value between 0.1 and 100.0)
    // to put the geometry inside the Ortho viewing frustum.
    gl_Position = projection * vec4(vertex.xy, 0.1, 1.0); 
    TexCoords = vertex.zw;
}