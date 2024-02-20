use crate::engine::vk_engine;
use lazy_static::lazy_static;
mod camera;
mod debug;
// So this is going to be an attempt to re-write the updated 'vkguide.dev' project in Rust.
// The previous iteration of this project used the previous version of the guide, so I thought
// that I should probably use the new version of the guide to get the best possible framework.
mod engine;

// Basically just the entry point to the program
// Probably won't do much other than that

fn main() {
    let mut game_engine = engine::vk_engine::VulkanEngine::new();
    game_engine.run();
}
