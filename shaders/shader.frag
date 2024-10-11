#version 450

// Input
layout(location = 0) in vec4 inColour;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 colMod;
layout(set = 2, binding = 0) uniform sampler2D texture1;

// Output
layout(location = 0) out vec4 outColour;

layout(set = 0, binding = 1) uniform SceneData {
    vec4 fogColour;
    vec4 fogDistances;
    vec4 ambientColour;
    vec4 sunlightDirection;
    vec4 sunlightColour;
} sceneData;

void main() {
    // Colours are vec 4
    // R G B A
    vec2 v_texCoord = texCoord * vec2(1.0, -1.0);
    vec3 colour = texture(texture1, texCoord).xyz;
    outColour = vec4(colour.xyz * colMod.yyy, 1.0f);
}
