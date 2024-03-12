use bevy::prelude::*;

mod camera;
mod debug;

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .add_systems(
            Startup,
            (
                camera::setup_camera,
                debug::fps_counter::setup_fps_counter,
                debug::model::spawn_gltf,
            ),
        )
        .add_systems(
            Update,
            (
                debug::fps_counter::fps_text_update_system,
                debug::fps_counter::fps_counter_showhide,
                debug::model::rotate_cube,
            ),
        )
        .run();
}
