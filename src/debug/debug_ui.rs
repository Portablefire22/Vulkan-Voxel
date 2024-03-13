use bevy::{
    ecs::{
        component::Component,
        query::With,
        system::{Commands, Query},
    },
    hierarchy::BuildChildren,
    log::warn,
    render::color::Color,
    text::{Text, TextSection, TextStyle},
    transform::components::Transform,
    ui::{
        node_bundles::{NodeBundle, TextBundle},
        BackgroundColor, PositionType, Style, UiRect, Val, ZIndex,
    },
    utils::default,
};

use crate::camera::PlayerCamera;

#[derive(Component)]
pub struct DebugUiRoot;

#[derive(Component)]
pub struct DebugUiText;

pub fn create_debug_ui(mut commands: Commands) {
    let root = commands
        .spawn((
            DebugUiRoot,
            NodeBundle {
                background_color: BackgroundColor(Color::BLACK.with_a(0.5)),
                // always on top
                z_index: ZIndex::Global(i32::MAX),
                style: Style {
                    position_type: PositionType::Absolute,
                    left: Val::Percent(1.0),
                    top: Val::Percent(1.0),
                    bottom: Val::Auto,
                    right: Val::Auto,

                    padding: UiRect::all(Val::Px(4.0)),
                    ..Default::default()
                },
                ..Default::default()
            },
        ))
        .id();
    let text_pos = commands
        .spawn((
            DebugUiText,
            TextBundle {
                text: Text::from_sections([
                    TextSection {
                        value: format!("{} {}", env!("CARGO_PKG_NAME"), env!("CARGO_PKG_VERSION"))
                            .into(),
                        style: TextStyle {
                            font_size: 16.0,
                            color: Color::WHITE,
                            // Custom font, remove the comment
                            // font: my_font_handle
                            ..default()
                        },
                    },
                    TextSection {
                        value: "\nX: N/A".into(),
                        style: TextStyle {
                            font_size: 16.0,
                            color: Color::WHITE,
                            ..default()
                        },
                    },
                    TextSection {
                        value: "\nY: N/A".into(),
                        style: TextStyle {
                            font_size: 16.0,
                            color: Color::WHITE,
                            ..default()
                        },
                    },
                    TextSection {
                        value: "\nZ: N/A".into(),
                        style: TextStyle {
                            font_size: 16.0,
                            color: Color::WHITE,
                            ..default()
                        },
                    },
                ]),
                ..Default::default()
            },
        ))
        .id();
    commands.entity(root).push_children(&[text_pos]);
}

pub fn debug_ui_text_update(
    mut query: Query<&mut Text, With<DebugUiText>>,
    camera_trans: Query<&Transform, With<PlayerCamera>>,
) {
    match camera_trans.get_single() {
        Ok(trans) => {
            for mut text in &mut query {
                text.sections[1].value = format!("\nX: {:.2}", trans.translation.x);
                text.sections[2].value = format!("\nY: {:.2}", trans.translation.y);
                text.sections[3].value = format!("\nZ: {:.2}", trans.translation.z);
            }
        }
        _ => warn!("Failed to get camera transform for position UI!"),
    }
}
