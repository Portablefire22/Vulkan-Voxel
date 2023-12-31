#version 460

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec3 vertColour;
layout(location = 3) in vec2 vertTexCoord;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 texCoord;

layout(set = 0, binding = 0) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
    mat4 viewproj;
} cameraData;

struct ObjectData {
    mat4 model;
    vec4 colour;
};

// Object Matrices
// std140 is to make the array match the C++ array layout
layout(std140, set = 1, binding = 0) readonly buffer ObjectBuffer{
    ObjectData objects[];
} objectBuffer;

layout( push_constant ) uniform constants {
    vec4 data;
    mat4 renderMatrix;
} PushConstants;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(vertPosition, 1.0);
    mat4 modelMatrix = objectBuffer.objects[gl_BaseInstance].model;
    mat4 transformMatrix = (cameraData.viewproj * modelMatrix);
    gl_Position = transformMatrix * vec4(vertPosition, 1.0f);
    outColour = objectBuffer.objects[gl_BaseInstance].colour;
    texCoord = vertTexCoord;
}