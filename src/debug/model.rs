use std::f32::consts::{PI, TAU};

use bevy::{
    asset::{AssetServer, Assets},
    ecs::{
        component::Component,
        system::{Commands, Query, Res, ResMut},
    },
    hierarchy::BuildChildren,
    math::{
        primitives::{Plane3d, Sphere},
        IVec2, Quat,
    },
    pbr::{
        light_consts, CascadeShadowConfigBuilder, DirectionalLight, DirectionalLightBundle,
        PbrBundle, PointLight, PointLightBundle, StandardMaterial,
    },
    render::{
        color::Color,
        mesh::{Mesh, Meshable},
        texture::{Image, ImageSampler},
    },
    scene::SceneBundle,
    time::Time,
    transform::components::Transform,
    utils::default,
};

use crate::{assets::load_textures::TextureAtlasMaterial, world::region::Region};

pub fn spawn_gltf(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: Res<TextureAtlasMaterial>,
    mut assets: ResMut<Assets<Image>>,
) {
    for z in -10..=10 {
        for x in -10..=10 {
            let mut tmp = Region::new(IVec2 {
                x: x as i32,
                y: z as i32,
            });
            for y in -2..=5 {
                tmp.create_chunk(y);
                let chunk = tmp.get_chunk(y).unwrap();
                chunk.spawn_chunk(&tmp, &mut commands, &mut meshes, &mut materials)
            }
        }
    }
    // Band-aid fix
    if let Some(texture) = assets.get_mut(materials.block_atlas_image.clone()) {
        texture.sampler = ImageSampler::nearest()
    }
    // directional 'sun' light
    commands.spawn(DirectionalLightBundle {
        directional_light: DirectionalLight {
            illuminance: light_consts::lux::OVERCAST_DAY,
            shadows_enabled: true,
            ..default()
        },
        transform: Transform {
            translation: bevy::math::Vec3::new(0.0, 2.0, 0.0),
            rotation: Quat::from_rotation_x(-PI / 4.),
            ..default()
        },
        // The default cascade config is designed to handle large scenes.
        // As this example has a much smaller world, we can tighten the shadow
        // bounds for better visual quality.
        cascade_shadow_config: CascadeShadowConfigBuilder {
            first_cascade_far_bound: 4.0,
            maximum_distance: 10.0,
            ..default()
        }
        .into(),
        ..default()
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
