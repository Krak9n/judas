#![cfg_attr(not(debug_assertions), linux_subsystem = "linux")]
#![allow(clippy::single_match)]

extern crate glfw;
extern crate glad_gl;
// include the OpenGL type aliases
use gl::types::*;

use glad_gl::gl;
use glfw::{Action, Context, Key};

static TITLE: &str = "agonia";
static WIDTH: u32 = 1500;
static HEIGHT: u32 = 800;

fn main() 
{
    use glfw::fail_on_errors;
    let mut glfw = glfw::init(fail_on_errors!()).unwrap();

    // Create a windowed mode window and its OpenGL context
    let (mut window, events) = glfw.create_window(
        WIDTH, 
        HEIGHT, 
        TITLE,
        glfw::WindowMode::Windowed)         
        .expect("Failed to create GLFW window.");

    // Make the window's context current
    window.make_current();
    window.set_key_polling(true);
    
    gl::load(|e| glfw.get_proc_address_raw(e) as *const std::os::raw::c_void);
    
    while !window.should_close() {
        glfw.poll_events();
        for (_, event) in glfw::flush_messages(&events) {
            handle_window_event(&mut window, event);
        }

        unsafe {
            gl::ClearColor(0.7, 0.9, 0.1, 1.0);
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }

        window.swap_buffers();
    }
}

fn handle_window_event(window: &mut glfw::Window, event: glfw::WindowEvent) {
    match event {
        glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => {
            window.set_should_close(true)
        }
        _ => {}
    }
}

