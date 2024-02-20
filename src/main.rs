use crate::engine::vk_engine;
use lazy_static::lazy_static;

use winit::event::{Event, WindowEvent};
use winit::event_loop::{ControlFlow, EventLoop};

mod camera;
mod debug;
// So this is going to be an attempt to re-write the updated 'vkguide.dev' project in Rust.
// The previous iteration of this project used the previous version of the guide, so I thought
// that I should probably use the new version of the guide to get the best possible framework.
mod engine;
mod game;
// Basically just the entry point to the program
// Probably won't do much other than that

fn main() {
    let event_loop = EventLoop::new();
    let mut render_loop = engine::render::RenderLoop::new(&event_loop);
    event_loop.run(move |event, _, control_flow| match event {
        Event::WindowEvent {
            event: WindowEvent::CloseRequested,
            ..
        } => {
            *control_flow = ControlFlow::Exit;
        }
        Event::WindowEvent {
            event: WindowEvent::Resized(_),
            ..
        } => {}
    });
}
