use std::f32::consts::TAU;

use bevy::{
    asset::{AssetServer, Assets},
    ecs::{
        component::Component,
        system::{Commands, Query, Res, ResMut},
    },
    hierarchy::BuildChildren,
    math::primitives::{Plane3d, Sphere},
    pbr::{PbrBundle, PointLight, PointLightBundle, StandardMaterial},
    render::{
        color::Color,
        mesh::{Mesh, Meshable},
    },
    scene::SceneBundle,
    time::Time,
    transform::components::Transform,
    utils::default,
};

pub fn spawn_gltf(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
    ass: Res<AssetServer>,
) {
    // ground plane
    commands.spawn((
        PbrBundle {
            mesh: meshes.add(Plane3d::default().mesh().size(10.0, 10.0)),
            transform: Transform::from_xyz(0.0, -1.0, 0.0),
            material: materials.add(StandardMaterial {
                base_color: Color::WHITE,
                perceptual_roughness: 1.0,
                ..default()
            }),
            ..default()
        },
        Rotatable { speed: 0.01 },
    ));

    commands.spawn(PbrBundle {
        mesh: meshes.add(crate::mesh::chunk::generate_chunk_mesh()),
        material: materials.add(StandardMaterial {
            base_color: Color::WHITE,
            perceptual_roughness: 1.0,
            ..default()
        }),
        transform: Transform::from_xyz(0.0, 0.0, -1.0),
        ..default()
    });

    commands
        .spawn(PointLightBundle {
            transform: Transform::from_xyz(0.0, -0.9, 0.5),
            point_light: PointLight {
                intensity: 4000.0, // lumens
                color: Color::BLUE,
                shadows_enabled: true,
                ..default()
            },
            ..default()
        })
        .with_children(|builder| {
            builder.spawn(PbrBundle {
                mesh: meshes.add(Sphere::new(0.1).mesh().uv(32, 18)),
                material: materials.add(StandardMaterial {
                    base_color: Color::GREEN,
                    emissive: Color::rgba_linear(0.0, 0.0, 7.13, 0.0),
                    ..default()
                }),
                ..default()
            });
        });
}

// Define the component & designate a rotation speed to it
#[derive(Component)]
pub struct Rotatable {
    speed: f32,
}

pub fn rotate_cube(mut cubes: Query<(&mut Transform, &Rotatable)>, timer: Res<Time>) {
    for (mut transform, cube) in &mut cubes {
        transform.rotate_y(cube.speed * TAU * timer.delta_seconds());
    }
}
