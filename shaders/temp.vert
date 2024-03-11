#version 460 

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec4 vert_colour;

layout(location = 0) out vec4 out_colour;

void main() {
  gl_Position = vec4(vert_position, 1);
  out_colour = vert_colour;
}
