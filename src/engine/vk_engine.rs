use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use std::sync::Arc;
use std::time::Instant;
use vulkano::device::{DeviceCreateInfo, QueueCreateInfo, QueueFlags};
use vulkano::instance::debug::{
    DebugUtilsMessenger, DebugUtilsMessengerCallback, DebugUtilsMessengerCreateInfo,
};
use vulkano::instance::{Instance, InstanceCreateInfo, InstanceExtensions};
use vulkano::swapchain::{Surface, SurfaceApi};
use vulkano::{Handle, VulkanLibrary, VulkanObject};

pub struct WindowInfo {
    pub extent: (u32, u32),
    pub title: String,
}

pub struct WindowVk {
    pub window: sdl2::video::Window,
    pub is_initialised: bool,
    sdl_context: sdl2::Sdl,
    info: WindowInfo,
}

pub struct EngineInfo {}

pub struct VulkanEngine {
    pub info: EngineInfo,
    pub window_vk: WindowVk,

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
        window_vk: WindowVk,
        instance: Arc<vulkano::instance::Instance>,
        chosen_gpu: Arc<vulkano::device::Device>,
    ) -> Self {
        VulkanEngine {
            info,
            window_vk,
            framenumber: 0,
            fps: 0,
            fps_time: Instant::now(),

            instance,
            chosen_gpu,
        }
    }

    pub fn init(&mut self) {
        println!("Engine is starting!");

        let surface_handle = self
            .window_vk
            .window
            .vulkan_create_surface(self.instance.handle().as_raw() as _)
            .unwrap();

        // Don't drop window before the 'Surface' or Vulkan 'Swapchain'
        let surface = unsafe {
            Surface::from_handle(
                Arc::clone(&self.instance),
                <_ as Handle>::from_raw(surface_handle),
                SurfaceApi::Xlib,
                None,
            )
        };

        /*let mut canvas = self
            .window_vk
            .window
            .into_canvas()
            .build()
            .map_err(|e| e.to_string())
            .expect("canvas error");
        canvas.set_draw_color(Color::RGB(255, 0, 0));
        canvas.clear();
        canvas.present();*/

        self.window_vk.is_initialised = true;

        let mut event_pump = self.window_vk.sdl_context.event_pump().unwrap();
        println!("Window has been created");
        'running: loop {
            for event in event_pump.poll_iter() {
                match event {
                    Event::Quit { .. }
                    | Event::KeyDown {
                        keycode: Some(Keycode::Escape),
                        ..
                    } => {
                        break 'running;
                    }
                    _ => {}
                }
            }
            if self.window_vk.window.is_minimized() {
                ::std::thread::sleep(::std::time::Duration::new(0, 250_000_000u32)); // Sleep for
                                                                                     // 0.25s
                continue;
            }
            // ::std::thread::sleep(::std::time::Duration::new(0, 1_000_000_000u32 / 60));
            // ^ Sleeps to get 60 fps, this is basically useless because Vulkan should handle it
            // with the mailbox shit
            self.draw();
        }
    }

    pub fn run(&mut self) {}

    pub fn cleanup(&mut self) {}

    pub fn draw(&mut self) {}
}

pub fn create_window(info: WindowInfo) -> WindowVk {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();
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
}

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
) -> (Arc<vulkano::device::Device>, Arc<vulkano::device::Queue>) {
    let physical_device = instance
        .enumerate_physical_devices()
        .expect("Could not enumerate physical devices")
        .next()
        .expect("No devices availaable!");
    let queue_family_index = physical_device
        .queue_family_properties()
        .iter()
        .enumerate()
        .position(|(_queue_family_index, queue_family_properties)| {
            queue_family_properties
                .queue_flags
                .contains(QueueFlags::GRAPHICS)
        })
        .expect("Could not find a queue capable of graphics!");

    let (device, mut queues) = vulkano::device::Device::new(
        physical_device,
        DeviceCreateInfo {
            queue_create_infos: vec![QueueCreateInfo {
                queue_family_index: queue_family_index.try_into().unwrap(),
                ..Default::default()
            }],

            ..Default::default()
        },
    )
    .expect("Failed to create a device!");
    print_type_of(&queues);
    return (device, queues.next().unwrap());
}

pub fn create_swapchain(window_info: &WindowInfo) -> vulkano::swapchain::Swapchain {}
