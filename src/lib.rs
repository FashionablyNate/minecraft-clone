use winit::{
    event::*,
    event_loop::{ControlFlow, EventLoop},
    window::{Window, WindowBuilder},
};

#[cfg(target_arch = "wasm32")]
use wasm_bindgen::prelude::*;

struct State {
    surface: wgpu::Surface,
    device: wgpu::Device,
    queue: wgpu::Queue,
    config: wgpu::SurfaceConfiguration,
    size: winit::dpi::PhysicalSize<u32>,
    clear_color: wgpu::Color,
}

impl State {
    async fn new(window: &Window) -> Self {

        // the size of Surface set to size of window
        let size = window.inner_size();

        // The instance is a handle to our GPU
        // Backends::all => Vulkan + Metal + DX12 + Browser WebGPU
        let instance = wgpu::Instance::new(wgpu::Backends::all());
        let surface = unsafe { instance.create_surface(window) };

        // handle to graphics card, used to get information about it
        let adapter = instance.request_adapter(
            &wgpu::RequestAdapterOptions {

                // Two variants for power_preference; LowPower and HighPerformance
                // WGPU favors LowPower if there's no HighPerformance option.
                power_preference: wgpu::PowerPreference::default(),

                // Tells WGPU to find an adaptor to project to supplied surface
                compatible_surface: Some(&surface),

                // force_fallback_adapter forces WGPU to pick and adapter that
                // will work on all hardware, usually means backend will use a
                // software system instead of hardware (like the GPU)
                force_fallback_adapter: false,
            },
        ).await.unwrap();

        let (device, queue) = adapter.request_device(
            &wgpu::DeviceDescriptor {

                // specify any special/extra features to turn on
                features: wgpu::Features::empty(),

                // limits specifies limits on certain types of resources that the
                // gpu can use
                limits: if cfg!(target_arch = "wasm32") {
                    
                    // WebGL doesn't support all of wgpu's features, so we turn them
                    // off when building for the web
                    wgpu::Limits::downlevel_webgl2_defaults()
                } else {
                    wgpu::Limits::default()
                },
                label: None,
            },
            None, // Trace Path
        ).await.unwrap();

        // defines how the surface creates it's surface textures
        let config = wgpu::SurfaceConfiguration {

            // usage describes how surface textures will be used, RENDER_ATTACHMENT
            // specifies that the textures will be used to write to the screen
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,

            // format defines how surface textures will be stored on the gpu,
            // we use get preferred format to get the preferred format for
            // displaying on the current display
            format: surface.get_preferred_format(&adapter).unwrap(),
            
            // width and height in pixels of the SurfaceTexture, usually just the
            // width and height of the window
            width: size.width,
            height: size.height,

            // present_mode uses and enum to sync the surface with the display.
            // FIFO will cap the display rate at the display's framerate.
            present_mode: wgpu::PresentMode::Fifo,
        };
        surface.configure(&device, &config);

        let clear_color = wgpu::Color {
            r: 0.1,
            g: 0.2,
            b: 0.3,
            a: 1.0,
        };

        // load all the above values into the surface's fields
        Self {
            surface,
            device,
            queue,
            config,
            size,
            clear_color,
        }
    }

    fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {

        // if the new size isn't zero
        if new_size.width > 0 && new_size.height > 0 {

            // we assign the new size to the various surface fields
            self.size = new_size;
            self.config.width = new_size.width;
            self.config.height = new_size.height;
            self.surface.configure(&self.device, &self.config);
        }
    }

    fn input(&mut self, event: &WindowEvent) -> bool {
        match event {
            WindowEvent::CursorMoved { position, .. } => {
                self.clear_color = wgpu::Color {
                    r: position.x as f64 / self.size.width as f64,
                    g: position.y as f64 / self.size.height as f64,
                    b: (position.x * position.y) as f64 /
                        (self.size.width * self.size.height) as f64,
                    a: 1.0,
                };
                true
            }
            _ => false,
        }
    }

    fn update(&mut self) {
        
    }

    fn render(&mut self) -> Result<(), wgpu::SurfaceError> {

        // get_current_texture waits for surface to return a new SurfaceTexture
        // to render to, we store this in output
        let output = self.surface.get_current_texture()?;

        // creates a TextureView with default settings to control how the
        // render code interacts with the texure
        let view = output.texture.create_view(&wgpu::TextureViewDescriptor::default());

        // creates an encoder that builds a buffer that can be sent to the gpu
        let mut encoder = self.device.create_command_encoder(&wgpu::CommandEncoderDescriptor {
            label: Some("Render Encoder"),
        });

        // the {} block creates a new scope where we can mutably borrow encoder,
        // so that way it gets released when we leave the block
        {

            let _render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {

                // label shows up in debuggers for easy identification
                label: Some("Render Pass"),
                
                // attachments for render pass
                color_attachments: &[wgpu::RenderPassColorAttachment {

                    // A TextureView object which describes a texture and the
                    // associated metadata
                    view: &view,

                    // the view that will receive the resolved output if multisampling
                    // is used. Multisampling is a process for reducing aliasing at the
                    // edges of rasterized primitives
                    resolve_target: None,

                    // A pair of load and store operations for an attachment aspect
                    ops: wgpu::Operations {

                        // Operation to perform to the output attachment at the start
                        // of a renderpass
                        load: wgpu::LoadOp::Clear(self.clear_color),
                        // whether data will be written through this attachment
                        store: true,
                    },
                }],
                // describes the depth or stencil buffers
                depth_stencil_attachment: None,
            });
        }

        // submit will accept anything that implements IntoIter
        self.queue.submit(std::iter::once(encoder.finish()));
        output.present();

        Ok(())
    }
}

#[cfg(target_arch="wasm32")]
use wasm_bindgen::prelude::*;

#[cfg_attr(target_arch="wasm32", wasm_bindgen(start))]
pub async fn run() {
    cfg_if::cfg_if! {
        if #[cfg(target_arch = "wasm32")] {
            std::panic::set_hook(Box::new(console_error_panic_hook::hook));
            console_log::init_with_level(log::Level::Warn).expect("Couldn't initialize logger");
        } else {
            env_logger::init();
        }
    }

    let event_loop = EventLoop::new();
    let window = WindowBuilder::new()
        .build(&event_loop)
        .unwrap();

    #[cfg(target_arch = "wasm32")]
    {
        // Winit prevents sizing with CSS, so we have to set
        // the size manually when on web.
        use winit::dpi::PhysicalSize;
        window.set_inner_size(PhysicalSize::new(450, 400));
        
        use winit::platform::web::WindowExtWebSys;
        web_sys::window()
            .and_then(|win| win.document())
            .and_then(|doc| {
                let dst = doc.get_element_by_id("wasm-example")?;
                let canvas = web_sys::Element::from(window.canvas());
                dst.append_child(&canvas).ok()?;
                Some(())
            })
            .expect("Couldn't append canvas to document body.");
    }

    // State::new uses async code, so we're going to wait for it to finish
    let mut state = State::new(&window).await;

    event_loop.run(move |event, _, control_flow| {
        match event {

            // when a redraw is requested
            Event::RedrawRequested(window_id) if window_id == window.id() => {

                // we call update on the state
                state.update();

                // then we call render, and error catch
                match state.render() {
                    Ok(_) => {}

                    // reconfigure the surface if lost
                    Err(wgpu::SurfaceError::Lost) => state.resize(state.size),

                    // The system is out of memory, give up
                    Err(wgpu::SurfaceError::OutOfMemory) => *control_flow = ControlFlow::Exit,

                    // All other errors
                    Err(e) => eprintln!("{:?}", e),
                }
            }

            Event::MainEventsCleared => {

                // RedrawRequested will only trigger once, unless we manually
                // request it
                window.request_redraw();
            }

            Event::WindowEvent {
                ref event,
                window_id,
            } if window_id == window.id() => {
                if !state.input(event) {
                    match event {
                        // A window close event or esc key press event
                        WindowEvent::CloseRequested
                        | WindowEvent::KeyboardInput {
                            input:
                                KeyboardInput {
                                    state: ElementState::Pressed,
                                    virtual_keycode: Some(VirtualKeyCode::Escape),
                                    ..
                                },
                            ..

                        // we exit the program
                        } => *control_flow = ControlFlow::Exit,

                        // if the window is resized
                        WindowEvent::Resized(physical_size) => {
                            // we call the resized method and pass a pointer to
                            // the updated sized
                            state.resize(*physical_size);
                        }

                        WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                            // new_inner_size is &&mut so we have to dereference it twice
                            state.resize(**new_inner_size);
                        }

                        _ => {}
                    }
                }
            }
            _ => {}
        }
    });
}

