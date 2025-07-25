#![allow(
    dead_code,
    unused_variables,
    clippy::too_many_arguments,
    clippy::unnecessary_wraps
)]

extern crate sdl3;

use log::*;

use imgui::*;

use winit::keyboard::*;
use winit::dpi::LogicalSize;
use winit::event::{Event, WindowEvent};
use winit::event_loop::EventLoop;
use winit::window::{Window, WindowBuilder};

use anyhow::{anyhow, Result};

use vulkanalia::Version;
use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::window as vk_window;
use vulkanalia::prelude::v1_0::*;

static WIDTH: u32 = 1500;
static HEIGHT: u32 = 900;
static TITLE: &str = "judas";

const PORTABILITY_LINUX_VERSION: Version = Version::new(1, 3, 216);

pub fn main() -> Result<(), Box<dyn std::error::Error>>
{
    pretty_env_logger::init();

    /* WINDOW */
    // -------------
    let event_loop = EventLoop::new()?;
    
    let window = WindowBuilder::new()
        .with_title(TITLE)
        .with_inner_size(LogicalSize::new(WIDTH, HEIGHT))
        .build(&event_loop)?;
    // ----------------

    let mut world = unsafe
    { 
        Works::create(&window).unwrap() 
    };

    let mut value = 0;
    
    event_loop.run(move |event, elwt| 
    {
        match event {
            // Request a redraw when all events were processed.
            Event::AboutToWait => window.request_redraw(),
            Event::WindowEvent { event, .. } => match event {
                // Render a frame if our Vulkan app is not being destroyed.
                WindowEvent::RedrawRequested if !elwt.exiting() => unsafe { world.render(&window) }.unwrap(),
                // Destroy our Vulkan app.
                WindowEvent::CloseRequested => {
                    elwt.exit();
                    unsafe { world.destroy(); }
                }
                _ => {}
            }
            _ => {}
        }
    })?;

    Ok(())
}

#[derive(Clone, Debug)]
struct Works
{
    entry: Entry,
    instance: Instance,
}

impl Works
{
    /// Creates our Vulkan app.
    unsafe fn create(window: &Window) -> Result<Self> 
    {
        let loader = LibloadingLoader::new(LIBRARY)?;
        let entry = Entry::new(loader).map_err(|b| anyhow!("{}", b))?;
        let instance = create_instance(window, &entry)?;
        Ok(Self {entry, instance})
    }

    /// Renders a frame for our Vulkan app.
    unsafe fn render(&mut self, window: &Window) -> Result<()> 
    {
        Ok(())
    }

    /// Destroys our Vulkan app.
    unsafe fn destroy(&mut self) 
    {
        self.instance.destroy_instance(None);
    }
}

/// The Vulkan handles and associated properties used by our Vulkan app.
#[derive(Clone, Debug, Default)]
struct AppData {}

// INSTANCE
unsafe fn create_instance(window: &Window, entry: &Entry) -> Result<Instance> {
    // Application Info
    let application_info = vk::ApplicationInfo::builder()
        .application_name(b"judas\0")
        .application_version(vk::make_version(1, 0, 0))
        .engine_name(b"No Engine\0")
        .engine_version(vk::make_version(1, 0, 0))
        .api_version(vk::make_version(1, 0, 0));

    // Extensions

    let mut extensions = vk_window::get_required_instance_extensions(window)
        .iter()
        .map(|e| e.as_ptr())
        .collect::<Vec<_>>();

    
    // Required by Vulkan SDK on macOS since 1.3.216.
    let flags = if cfg!(target_os = "mac") && entry.version()? >= PORTABILITY_LINUX_VERSION 
    {
        info!("enabling extensions for mac portability.");
        extensions.push(vk::KHR_GET_PHYSICAL_DEVICE_PROPERTIES2_EXTENSION.name.as_ptr());
        extensions.push(vk::KHR_PORTABILITY_ENUMERATION_EXTENSION.name.as_ptr());
        vk::InstanceCreateFlags::ENUMERATE_PORTABILITY_KHR
    } else {
        vk::InstanceCreateFlags::empty()
    };

    // Create
    let info = vk::InstanceCreateInfo::builder()
        .application_info(&application_info)
        .enabled_extension_names(&extensions)
        .flags(flags);

    Ok(entry.create_instance(&info, None)?)
}
