use winit::event_loop::EventLoop;

use crate::engine::{self, render::RenderLoop};

struct Game {
    render_loop: RenderLoop,
}

impl Game {
    pub fn new(event_loop: EventLoop<()>) -> Self {
        let render_loop = RenderLoop::new(&event_loop);
        Self { render_loop }
    }

    pub fn update(&mut self) {
        self.render_loop.update();
    }

    pub fn handle_window_resize(&mut self) {
        self.render_loop.handle_window_resize();
    }
}
