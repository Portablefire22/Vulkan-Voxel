use bevy::{diagnostic::FrameTimeDiagnosticsPlugin, prelude::*};
use bevy_inspector_egui::quick::WorldInspectorPlugin;
use std::env;

mod assets;
mod camera;
mod debug;
mod mesh;
mod world;

fn main() {
    let mut is_minecraft = false;
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        match args[1].as_str() {
            "--extract-textures" => assets::texture_extractor::rip_minecraft_textures(&args),
            "--minecraft" => is_minecraft = true,
            _ => println!("Unknown arg: {}", args[1]),
        }
    }
    App::new()
        .add_plugins((
            DefaultPlugins.set(ImagePlugin::default_nearest()),
            FrameTimeDiagnosticsPlugin::default(),
            WorldInspectorPlugin::new(),
        ))
        .add_systems(
            Startup,
            (
                camera::setup_camera,
                debug::fps_counter::setup_fps_counter,
                (
                    assets::load_textures::load_texture_atlas,
                    debug::model::spawn_gltf,
                )
                    .chain(),
                camera::initial_capture_cursor,
                debug::debug_ui::create_debug_ui,
            ),
        )
        .add_systems(
            Update,
            (
                assets::load_textures::texture_fixer,
                debug::fps_counter::fps_text_update_system,
                debug::fps_counter::fps_counter_showhide,
                camera::camera_controller,
                debug::debug_ui::debug_ui_text_update,
            ),
        )
        .run();
}
