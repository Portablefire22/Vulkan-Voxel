use std::sync::Arc;
use vulkano::buffer::BufferContents;
use vulkano::buffer::{Buffer, BufferCreateInfo, BufferUsage};
use vulkano::device::{Device, DeviceCreateInfo, QueueCreateInfo, QueueFlags};
use vulkano::instance::{Instance, InstanceCreateInfo};
use vulkano::memory::allocator::StandardMemoryAllocator;
use vulkano::memory::allocator::{AllocationCreateInfo, MemoryTypeFilter};
use vulkano::VulkanLibrary;

use crate::engine::vk_engine::VulkanEngine;

// So this is going to be an attempt to re-write the updated 'vkguide.dev' project in Rust.
// The previous iteration of this project used the previous version of the guide, so I thought
// that I should probably use the new version of the guide to get the best possible framework.

mod engine;

// Basically just the entry point to the program
// Probably won't do much other than that
fn main() {
    let mut game_engine = engine::vk_engine::VulkanEngine::new();
    game_engine.init();
    game_engine.run();
    game_engine.cleanup();
    println!("Bye :3");
    return;
}
