#version 450

// Input
layout(location = 0) in vec3 inColour;


// Output
layout(location = 0) out vec4 outColour;

layout (set = 0, binding = 1) uniform SceneData {
    vec4 fogColour;
    vec4 fogDistances;
    vec4 ambientColour;
    vec4 sunlightDirection;
    vec4 sunlightColour;
} sceneData;

void main() {
    // Colours are vec 4
    // R G B A
    //outColour = texture(texSampler, fragTexCoord);
    outColour = vec4(inColour + sceneData.ambientColour.xyz, 1.0f);
}