use std::sync::Arc;
use vulkano::{Validated, VulkanError};
use winit::event_loop::EventLoop;

use super::vk_engine::{self, Fence};

pub struct RenderLoop {
    engine: vk_engine::VulkanEngine,
    recreate_swapchain: bool,
    window_resized: bool,
    fences: Vec<Option<Arc<Fence>>>,
    previous_fence_i: u32,
}

impl RenderLoop {
    pub fn new(event_loop: &EventLoop<()>) -> Self {
        let engine = vk_engine::VulkanEngine::new(event_loop);
        let frames_in_flight = engine.get_image_count();
        let fences: Vec<Option<Arc<Fence>>> = vec![None; frames_in_flight];
        Self {
            engine,
            recreate_swapchain: false,
            window_resized: false,
            fences,
            previous_fence_i: 0,
        }
    }

    pub fn update(&mut self) {
        if self.window_resized {
            self.window_resized = false;
            self.recreate_swapchain = false;
            self.engine.handle_window_resize();
        }
        if self.recreate_swapchain {
            self.recreate_swapchain = false;
            self.engine.recreate_swapchain();
        }

        let (image_i, suboptimal, acquire_future) = match self
            .engine
            .acquire_swapchain_image()
            .map_err(Validated::unwrap)
        {
            Ok(r) => r,
            Err(VulkanError::OutOfDate) => {
                self.recreate_swapchain = true;
                return;
            }
            Err(e) => panic!("Failed to acquire next image {:?}", e),
        };
        if suboptimal {
            self.recreate_swapchain = true;
        }

        if let Some(image_fence) = &self.fences[image_i as usize] {
            image_fence.wait(None).unwrap();
        }

        // "Logic that uses the GPU resources that are currently not used (have been waited upon)"

        let something_needs_all_gpu_resources = false;
        let previous_future = match self.fences[self.previous_fence_i as usize].clone() {
            None => self.renderer.synchronize().boxed(),
            Some(fence) => {
                if something_needs_all_gpu_resources {
                    fence.wait(None).unwrap;
                }
                fence.boxed()
            }
        };

        if something_needs_all_gpu_resources {
            // If something needs every GPU resource (GPU is eepy sleepy)
        }

        let result = self
            .engine
            .flush_next_future(previous_future, acquire_future, image_i);

        self.fences[image_i as usize] = match result.map_err(Validated::unwrap) {
            Ok(fence) => Some(Arc::new(fence)),
            Err(VulkanError::OutOfDate) => {
                self.recreate_swapchain = true;
                None
            }
            Err(e) => {
                println!("Failed to flush future: {:?}", e);
                None
            }
        };

        self.previous_fence_i = image_i;
    }

    pub fn handle_window_resize(&mut self) {
        // Occurs on the next update
        self.window_resized = true;
    }
}
