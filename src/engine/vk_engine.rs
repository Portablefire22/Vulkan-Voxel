use std::sync::Arc;
use std::time::Instant;
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::device::{DeviceCreateInfo, DeviceExtensions, QueueCreateInfo, QueueFlags};
use vulkano::instance::debug::{
    DebugUtilsMessenger, DebugUtilsMessengerCallback, DebugUtilsMessengerCreateInfo,
};
use vulkano::instance::{Instance, InstanceCreateInfo, InstanceExtensions};
use vulkano::swapchain::{Surface, SurfaceApi};
use vulkano::{Handle, VulkanLibrary, VulkanObject};

use winit::event_loop::EventLoop;

pub struct WindowInfo {
    pub extent: (u32, u32),
    pub title: String,
}

pub struct WindowVk {
    pub is_initialised: bool,
    info: WindowInfo,
}

pub struct EngineInfo {}

pub struct VulkanEngine {
    pub info: EngineInfo,
    //pub window_vk: WindowVk,
    fps: usize,
    fps_time: Instant,
    framenumber: usize,

    instance: Arc<vulkano::instance::Instance>,
    chosen_gpu: Arc<vulkano::device::Device>,
}

fn print_type_of<T>(_: &T) {
    println!("{}", std::any::type_name::<T>())
}

impl VulkanEngine {
    pub fn new(
        info: EngineInfo,
        //window_vk: WindowVk,
        instance: Arc<vulkano::instance::Instance>,
        chosen_gpu: Arc<vulkano::device::Device>,
    ) -> Self {
        VulkanEngine {
            info,
            //window_vk,
            framenumber: 0,
            fps: 0,
            fps_time: Instant::now(),

            instance,
            chosen_gpu,
        }
    }

    pub fn init(&mut self) {
        println!("Engine is starting!");

        //self.window_vk.is_initialised = true;

        println!("Window has been created");
    }

    pub fn run(&mut self) {}

    pub fn cleanup(&mut self) {}

    pub fn draw(&mut self) {}
}

/*pub fn create_window(info: WindowInfo, event_loop: winit::event_loop::EventLoop<>) -> WindowVk {
    let window = Arc::new(WindowBuilder::new().build()(&event_loop).unwrap());

    let window = video_subsystem
        .window(&info.title, info.extent.0, info.extent.1)
        .vulkan()
        .build()
        .unwrap();
    WindowVk {
        window,
        is_initialised: false,
        sdl_context,
        info,
    }
}*/

pub fn create_instance(
    instance_extensions: vulkano::instance::InstanceExtensions,
) -> Arc<vulkano::instance::Instance> {
    Instance::new(VulkanLibrary::new().unwrap(), {
        let mut instance_info = InstanceCreateInfo::application_from_cargo_toml();
        instance_info.enabled_extensions = instance_extensions;
        instance_info
    })
    .unwrap()
}

pub fn create_engine_info() -> EngineInfo {
    EngineInfo {}
}

pub fn select_physical_device(
    instance: &Arc<vulkano::instance::Instance>,
    surface: &Arc<Surface>,
    device_extensions: &DeviceExtensions,
) -> (Arc<PhysicalDevice>, u32) {
    instance // Only find devices with the requested extensions
        .enumerate_physical_devices()
        .expect("Could not enumerate physical devices")
        .filter(|p| p.supported_extensions().contains(&device_extensions))
        .filter_map(|p| {
            p.queue_family_properties()
                .iter()
                .enumerate()
                .position(|(i, q)| {
                    q.queue_flags.contains(QueueFlags::GRAPHICS)
                        && p.surface_support(i as u32, &surface).unwrap_or(false)
                })
                .map(|q| (p, q as u32))
        })
        .min_by_key(|(p, _)| match p.properties().device_type {
            // Rank the available GPUs by their type, lowest is best
            PhysicalDeviceType::DiscreteGpu => 0,
            PhysicalDeviceType::IntegratedGpu => 1,
            PhysicalDeviceType::VirtualGpu => 2,
            PhysicalDeviceType::Cpu => 3,
            _ => 4,
        })
        .expect("Supported device not found!")
}

//pub fn create_swapchain(window_info: &WindowInfo) -> vulkano::swapchain::Swapchain {}

pub fn destroy_swapchain() {}
