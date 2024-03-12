use bevy::{
    core_pipeline::core_3d::Camera3dBundle,
    ecs::query::With,
    ecs::{component::Component, system::Commands, system::Query},
    math::Vec3,
    prelude::default,
    render::camera::{PerspectiveProjection, Projection},
    transform::components::Transform,
};

#[derive(Component)]
pub struct PlayerCameraMarker;

pub fn setup_camera(mut commands: Commands) {
    commands.spawn((
        Camera3dBundle {
            projection: PerspectiveProjection {
                fov: 60.0_f32.to_radians(),
                ..default()
            }
            .into(),
            transform: Transform::from_xyz(10.0, 12.0, 16.0).looking_at(Vec3::ZERO, Vec3::Y),
            ..default()
        },
        PlayerCameraMarker,
    ));
}
