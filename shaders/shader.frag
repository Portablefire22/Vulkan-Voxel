#version 450

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    // Colours are vec 4
    // R G B A
    //outColour = texture(texSampler, fragTexCoord);
    outColour = vec4(fragColour, 1.0f);
}