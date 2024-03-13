use bevy::{diagnostic::FrameTimeDiagnosticsPlugin, prelude::*};
use bevy_inspector_egui::quick::WorldInspectorPlugin;

mod camera;
mod debug;
mod mesh;

fn main() {
    App::new()
        .add_plugins((
            DefaultPlugins,
            FrameTimeDiagnosticsPlugin::default(),
            WorldInspectorPlugin::new(),
        ))
        .add_systems(
            Startup,
            (
                camera::setup_camera,
                debug::fps_counter::setup_fps_counter,
                debug::model::spawn_gltf,
                camera::initial_capture_cursor,
                debug::debug_ui::create_debug_ui,
            ),
        )
        .add_systems(
            Update,
            (
                debug::fps_counter::fps_text_update_system,
                debug::fps_counter::fps_counter_showhide,
                debug::model::rotate_cube,
                camera::camera_controller,
                debug::debug_ui::debug_ui_text_update,
            ),
        )
        .run();
}
