use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use std::sync::Arc;
use std::time::Instant;
use vulkano::instance::{Instance, InstanceCreateInfo, InstanceExtensions};
use vulkano::swapchain::{Surface, SurfaceApi};
use vulkano::{Handle, VulkanLibrary, VulkanObject};

pub struct EngineInfo {}

pub struct WindowInfo {}

pub struct Window {}

pub struct VulkanEngine {
    pub info: EngineInfo,
    pub window: Window,

    fps: usize,
    fps_time: Instant,
    framenumber: usize,
}

fn print_type_of<T>(_: &T) {
    println!("{}", std::any::type_name::<T>())
}

impl VulkanEngine {
    pub fn new(info: EngineInfo, window: Window) -> Self {
        VulkanEngine {
            info,
            window,
            framenumber: 0,
            fps: 0,
            fps_time: Instant::now(),
        }
    }

    pub fn init(&mut self) {
        println!("Engine is starting!");
        let sdl_context = sdl2::init().unwrap();
        let video_subsystem = sdl_context.video().unwrap();
        let window = video_subsystem
            .window("Vulkan Voxel", 1920, 1080)
            .vulkan() // Allows for a Vulkan context to be created
            .build()
            .unwrap();

        let instance_extensions =
            InstanceExtensions::from_iter(window.vulkan_instance_extensions().unwrap());

        let instance = Instance::new(VulkanLibrary::new().unwrap(), {
            let mut instance_info = InstanceCreateInfo::application_from_cargo_toml();
            instance_info.enabled_extensions = instance_extensions;
            instance_info
        })
        .unwrap();

        let surface_handle = window
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

        let mut canvas = window
            .into_canvas()
            .build()
            .map_err(|e| e.to_string())
            .expect("canvas error");
        canvas.set_draw_color(Color::RGB(255, 0, 0));
        canvas.clear();
        canvas.present();

        self.is_initialised = true;

        let mut event_pump = sdl_context.event_pump().unwrap();
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
