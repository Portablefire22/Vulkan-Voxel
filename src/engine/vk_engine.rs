use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use std::sync::Arc;
use std::time::Instant;
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
}

fn print_type_of<T>(_: &T) {
    println!("{}", std::any::type_name::<T>())
}

impl VulkanEngine {
    pub fn new(info: EngineInfo, window_vk: WindowVk) -> Self {
        VulkanEngine {
            info,
            window_vk,
            framenumber: 0,
            fps: 0,
            fps_time: Instant::now(),
        }
    }

    pub fn init(&mut self) {
        println!("Engine is starting!");

        let instance_extensions = InstanceExtensions::from_iter(
            self.window_vk.window.vulkan_instance_extensions().unwrap(),
        );

        let instance = Instance::new(VulkanLibrary::new().unwrap(), {
            let mut instance_info = InstanceCreateInfo::application_from_cargo_toml();
            instance_info.enabled_extensions = instance_extensions;
            instance_info
        })
        .unwrap();

        let surface_handle = self
            .window_vk
            .window
            .vulkan_create_surface(instance.handle().as_raw() as _)
            .unwrap();

        // Don't drop window before the 'Surface' or Vulkan 'Swapchain'
        let surface = unsafe {
            Surface::from_handle(
                Arc::clone(&instance),
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
            ::std::thread::sleep(::std::time::Duration::new(0, 1_000_000_000u32 / 60));
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

pub fn create_engine_info() -> EngineInfo {
    EngineInfo {}
}
