use std::sync::Arc;

use winit::event::{Event, WindowEvent};
use winit::event_loop::ControlFlow;
use winit::event_loop::EventLoop;
use winit::window::WindowBuilder;

use crate::engine::vk_engine::VulkanEngine;
use vulkano::buffer::BufferContents;
use vulkano::buffer::{Buffer, BufferCreateInfo, BufferUsage};
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::device::{Device, DeviceCreateInfo, DeviceExtensions, QueueCreateInfo, QueueFlags};
use vulkano::image::ImageUsage;
use vulkano::instance::debug::{
    DebugUtilsMessenger, DebugUtilsMessengerCallback, DebugUtilsMessengerCreateInfo,
};
use vulkano::instance::{Instance, InstanceCreateInfo, InstanceExtensions};
use vulkano::memory::allocator::StandardMemoryAllocator;
use vulkano::memory::allocator::{AllocationCreateInfo, MemoryTypeFilter};
use vulkano::swapchain::{Surface, SurfaceApi};
use vulkano::swapchain::{Swapchain, SwapchainCreateInfo};
use vulkano::{Handle, VulkanLibrary, VulkanObject};
// So this is going to be an attempt to re-write the updated 'vkguide.dev' project in Rust.
// The previous iteration of this project used the previous version of the guide, so I thought
// that I should probably use the new version of the guide to get the best possible framework.

mod engine;

// Basically just the entry point to the program
// Probably won't do much other than that
fn main() {
    let library = VulkanLibrary::new().expect("No local Vulkan library/DLL");
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
    let surface = Surface::from_window(instance.clone(), window.clone()).unwrap();

    let (physical_device, queue_family_index) =
        engine::vk_engine::select_physical_device(&instance, &surface, &device_extensions);

    let (device, mut queues) = Device::new(
        physical_device.clone(),
        DeviceCreateInfo {
            queue_create_infos: vec![QueueCreateInfo {
                queue_family_index,
                ..Default::default()
            }],
            enabled_extensions: device_extensions,
            ..Default::default()
        },
    )
    .expect("Failed to create device!");
    let queue = queues.next().unwrap();

    let caps = physical_device
        .surface_capabilities(&surface, Default::default())
        .expect("failed to get surface capabilities");

    let dimensions = window.inner_size();
    let composite_alpha = caps.supported_composite_alpha.into_iter().next().unwrap();
    let image_format = Some(
        physical_device
            .surface_formats(&surface, Default::default())
            .unwrap()[0]
            .0,
    )
    .unwrap();

    let (mut swapchain, images) = Swapchain::new(
        device.clone(),
        surface.clone(),
        SwapchainCreateInfo {
            min_image_count: caps.min_image_count + 1,
            image_format,
            image_extent: dimensions.into(),
            image_usage: ImageUsage::COLOR_ATTACHMENT,
            composite_alpha,
            ..Default::default()
        },
    )
    .unwrap();

    let mut game_engine = engine::vk_engine::VulkanEngine::new(engine_info, instance, device);

    event_loop.run(|event, _, control_flow| match event {
        Event::WindowEvent {
            event: WindowEvent::CloseRequested,
            ..
        } => {
            //*control_flow = ControlFlow::Exit;
        }
        _ => (),
    });

    println!("Bye :3");
    return;
}
