use bevy::{
    app::AppExit,
    core_pipeline::core_3d::Camera3dBundle,
    ecs::{
        component::Component,
        event::{EventReader, Events, ManualEventReader},
        query::With,
        system::{Commands, Query, Res, ResMut, Resource},
    },
    input::{keyboard::*, mouse::MouseMotion, ButtonInput},
    log::warn,
    math::{Quat, Vec3},
    prelude::default,
    render::camera::{PerspectiveProjection, Projection},
    time::Time,
    transform::components::Transform,
    window::{CursorGrabMode, PrimaryWindow, Window},
};

#[derive(Component)]
pub struct PlayerCamera;

#[derive(Resource)]
pub struct MovementSettings {
    pub sensitivity: f32,
    pub speed: f32,
}

impl Default for MovementSettings {
    fn default() -> Self {
        Self {
            sensitivity: 0.0001,
            speed: 100.0,
        }
    }
}

#[derive(Resource, Default)]
pub struct InputState {
    pitch: f32,
    yaw: f32,
}

/// Capture/un-capture mouse cursor
fn toggle_capture_mouse(window: &mut Window) {
    match window.cursor.grab_mode {
        bevy::window::CursorGrabMode::None => {
            window.cursor.grab_mode = bevy::window::CursorGrabMode::Confined;
            window.cursor.visible = false;
        }
        _ => {
            window.cursor.grab_mode = bevy::window::CursorGrabMode::None;
            window.cursor.visible = true;
        }
    }
}

pub fn initial_capture_cursor(mut window: Query<&mut Window, With<PrimaryWindow>>) {
    if let Ok(mut window) = window.get_single_mut() {
        toggle_capture_mouse(&mut window);
    } else {
        warn!("initial_capture_cursor: Primary window not found, could not capture mouse!");
    }
}

pub fn camera_controller(
    keys: Res<ButtonInput<KeyCode>>,
    mut query_camera: Query<&mut Transform, With<crate::camera::PlayerCamera>>,
    mut input_state: ResMut<InputState>,
    movement_settings: Res<MovementSettings>,
    mut window: Query<&mut Window, With<PrimaryWindow>>, // Need this to capture and get mouse
    motion: Res<Events<MouseMotion>>,
    mut app_exit_events: ResMut<Events<AppExit>>,
    mut motion_event_reader: EventReader<MouseMotion>,
    // movement
    time: Res<Time>,
) {
    match window.get_single() {
        Ok(wind) => {
            let window_scale = wind.height().min(wind.width());
            match wind.cursor.grab_mode {
                CursorGrabMode::None => (),
                _ => {
                    for e in motion_event_reader.read() {
                        input_state.pitch -=
                            (movement_settings.sensitivity * e.delta.y * window_scale).to_radians();
                        input_state.yaw -=
                            (movement_settings.sensitivity * e.delta.x * window_scale).to_radians();
                    }
                }
            }
            input_state.pitch = input_state.pitch.clamp(-1.57, 1.57); // pi / 2
                                                                      // Really just assuming that
                                                                      // directly down is equal to
                                                                      // half of pi
            match query_camera.get_single_mut() {
                Ok(mut camera_trans) => {
                    let mut velocity = Vec3::ZERO;
                    let local_z = camera_trans.local_z();
                    let forward = -Vec3::new(local_z.x, 0.0, local_z.z);
                    let right = Vec3::new(local_z.z, 0.0, -local_z.x);

                    for key in keys.get_pressed() {
                        match wind.cursor.grab_mode {
                            CursorGrabMode::None => (), // Don't move if unfocused
                            _ => match key {
                                KeyCode::Space => velocity += Vec3::Y,
                                KeyCode::ShiftLeft | KeyCode::ShiftRight => velocity -= Vec3::Y,
                                KeyCode::KeyW => velocity += forward,
                                KeyCode::KeyS => velocity -= forward,
                                KeyCode::KeyA => velocity -= right,
                                KeyCode::KeyD => velocity += right,
                                KeyCode::Escape => {
                                    app_exit_events.send(AppExit);
                                }
                                _ => (),
                            },
                        }
                    }

                    velocity = velocity.normalize_or_zero();

                    camera_trans.translation +=
                        velocity * time.delta_seconds() * movement_settings.speed;

                    camera_trans.rotation = Quat::from_axis_angle(Vec3::Y, input_state.yaw)
                        * Quat::from_axis_angle(Vec3::X, input_state.pitch);
                }
                _ => {
                    warn!("Failed to get player camera in camera controller!");
                }
            }

            if keys.just_pressed(KeyCode::F10) {
                toggle_capture_mouse(&mut window.single_mut());
            }
        }
        _ => {
            warn!("Failed to get primary window in camera input!");
        }
    }
}
pub fn setup_camera(mut commands: Commands) {
    commands.insert_resource(InputState::default());
    commands.insert_resource(MovementSettings::default());
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
        PlayerCamera {},
    ));
}
