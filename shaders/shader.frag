#version 450

layout(location = 0) in vec3 inColour;

layout(location = 0) out vec4 outColour;

//layout(binding = 1) uniform sampler2D texSampler;

void main() {
    // Colours are vec 4
    // R G B A
    //outColour = texture(texSampler, fragTexCoord);
    outColour = vec4(inColour, 1.0f);
}