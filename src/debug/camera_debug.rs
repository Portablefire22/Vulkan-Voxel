use bevy::{
    ecs::{
        query::With,
        system::{Query, Res},
    },
    render::camera::{Camera, Projection},
    time::Time,
    transform::components::Transform,
};

pub fn debug_projection(query_camera: Query<&Projection, With<crate::camera::PlayerCamera>>) {
    let projection = query_camera.single();
    match projection {
        Projection::Perspective(persp) => {}
        Projection::Orthographic(ortho) => {}
    }
}
