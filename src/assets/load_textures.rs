use bevy::{
    asset::{AssetEvent, AssetServer, Assets, Handle},
    ecs::{
        event::EventReader,
        system::{Commands, Res, ResMut, Resource},
    },
    log::info,
    pbr::StandardMaterial,
    render::texture::{Image, ImageSampler},
    utils::default,
};

#[derive(Resource)]
pub struct TextureAtlasMaterial {
    pub block_atlas: Handle<StandardMaterial>,
    pub block_atlas_image: Handle<Image>,
}

pub fn load_minecraft_atlas() {}
pub fn load_texture_atlas(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    let texture_handle = asset_server.load("Atlas.png");
    let material_handle = materials.add(StandardMaterial {
        base_color_texture: Some(texture_handle.clone()),
        alpha_mode: bevy::pbr::AlphaMode::Opaque,
        unlit: true,
        ..default()
    });
    commands.insert_resource(TextureAtlasMaterial {
        block_atlas: material_handle,
        block_atlas_image: texture_handle,
    });
}

pub fn texture_fixer(
    mut ev_assets: EventReader<AssetEvent<Image>>,
    mut assets: ResMut<Assets<Image>>,
) {
    for ev in ev_assets.read() {
        match ev {
            AssetEvent::Added { id } => {
                if let Some(texture) = assets.get_mut(*id) {
                    texture.sampler = ImageSampler::nearest()
                }
            }
            _ => {}
        }
    }
}
