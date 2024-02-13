#version 460
layout(location = 0) in vec4 vert_colour;
layout(location = 0) out vec4 f_colour;

void main() {
  f_colour = vert_colour; 
}
