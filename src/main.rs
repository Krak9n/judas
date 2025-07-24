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

use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::window as vk_window;
use vulkanalia::prelude::v1_0::*;

static WIDTH: u32 = 1500;
static HEIGHT: u32 = 900;
static TITLE: &str = "judas";

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

    let app = unsafe
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
                WindowEvent::RedrawRequested if !elwt.exiting() => unsafe { Works::render(&mut Works {}, &window) }.unwrap(),
                // Destroy our Vulkan app.
                WindowEvent::CloseRequested => {
                    elwt.exit();
                    unsafe { Works::destroy(&mut Works {}); }
                }
                _ => {}
            }
            _ => {}
        }
    })?;

    Ok(())
}

#[derive(Clone, Debug)]
struct Works {}

impl Works
{
    /// Creates our Vulkan app.
    unsafe fn create(window: &Window) -> Result<Self> 
    {
        Ok(Self {})
    }

    /// Renders a frame for our Vulkan app.
    unsafe fn render(&mut self, window: &Window) -> Result<()> 
    {
        Ok(())
    }

    /// Destroys our Vulkan app.
    unsafe fn destroy(&mut self) {}
}

/// The Vulkan handles and associated properties used by our Vulkan app.
#[derive(Clone, Debug, Default)]
struct AppData {}
