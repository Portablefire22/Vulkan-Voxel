use cgmath::{prelude::VectorSpace, Matrix4, Vector3};

struct Camera {
    position: Vector3<f32>,
    yaw: f32,
    pitch: f32,
    speed: f32,
    sensitivity: f32,
    zoom: f32,
    data: GPUCameraData,
}

// Data to be sent to the shaders
struct GPUCameraData {
    view: Matrix4<f32>,
    proj: Matrix4<f32>,
    view_proj: Matrix4<f32>,
}
