#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec3 vertColour;

layout(location = 0) out vec3 outColour;

layout( push_constant ) uniform constants {
    vec4 data;
    mat4 renderMatrix;
} PushConstants;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(vertPosition, 1.0);
    gl_Position = PushConstants.renderMatrix * vec4(vertPosition, 1.0f);
    outColour = vertColour;
}