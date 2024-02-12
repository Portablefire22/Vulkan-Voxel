use std::sync::Arc;
use vulkano::buffer::BufferContents;
use vulkano::buffer::{Buffer, BufferCreateInfo, BufferUsage};
use vulkano::device::{Device, DeviceCreateInfo, QueueCreateInfo, QueueFlags};
use vulkano::instance::debug::{
    DebugUtilsMessenger, DebugUtilsMessengerCallback, DebugUtilsMessengerCreateInfo,
};
use vulkano::instance::{Instance, InstanceCreateInfo, InstanceExtensions};
use vulkano::memory::allocator::StandardMemoryAllocator;
use vulkano::memory::allocator::{AllocationCreateInfo, MemoryTypeFilter};
use vulkano::swapchain::{Surface, SurfaceApi};
use vulkano::VulkanLibrary;
use winit::event::{Event, WindowEvent};
use winit::event_loop::ControlFlow;
use winit::window::WindowBuilder;

use crate::engine::vk_engine::VulkanEngine;
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::device::{DeviceCreateInfo, DeviceExtensions, QueueCreateInfo, QueueFlags};
use vulkano::{Handle, VulkanLibrary, VulkanObject};

use wkinit::event_loop::EventLoop;
// So this is going to be an attempt to re-write the updated 'vkguide.dev' project in Rust.
// The previous iteration of this project used the previous version of the guide, so I thought
// that I should probably use the new version of the guide to get the best possible framework.

mod engine;

// Basically just the entry point to the program
// Probably won't do much other than that
fn main() {
    let engine_info = engine::vk_engine::create_engine_info();
    let window_info = engine::vk_engine::WindowInfo {
        extent: (800, 400),
        title: "Vulkan Voxel".to_string(),
    };

    let event_loop = EventLoop::new();

    let required_extensions = Surface::required_extensions(&event_loop);

    let instance = engine::vk_engine::create_instance(required_extensions);

    let device_extensions = DeviceExtensions {
        khr_swapchain: true,
        khr_ray_tracing_pipeline: true, // >:3
        ..DeviceExtensions::empty()
    };

    let window = Arc::new(WindowBuilder::new().build(&event_loop).unwrap());
    let surface = Surface::from_window(instance.clone(), window.clone());

    let (device, queue, physical_device) =
        engine::vk_engine::select_physical_device(&instance, &surface, &device_extensions);

    let caps = physical_device
        .surface_capabilities(&surface, Default::default())
        .expect("failed to get surface capabilities");

    let mut game_engine =
        engine::vk_engine::VulkanEngine::new(engine_info, window, instance, device);

    event_loop.run(|event, _, control_flow| match event {
        Event::WindowEvent {
            event: WindowEvent::CloseRequested,
            ..
        } => {
            *control_flow = ControlFlow::Exit;
        }
        _ => (),
    });

    println!("Bye :3");
    return;
}
