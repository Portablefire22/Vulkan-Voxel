#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec3 vertColour;

layout(location = 0) out vec3 outColour;

layout(set = 0, binding = 0) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
    mat4 viewproj;
} cameraData;

layout( push_constant ) uniform constants {
    vec4 data;
    mat4 renderMatrix;
} PushConstants;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(vertPosition, 1.0);
    mat4 transformMatrix = (cameraData.viewproj * PushConstants.renderMatrix);
    gl_Position = transformMatrix * vec4(vertPosition, 1.0f);
    outColour = vertColour;
}