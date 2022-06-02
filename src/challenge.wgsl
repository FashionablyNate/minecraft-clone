// vertex shader

// stores the output of the vertex shader
struct VertexOutput {
    
    // this is the value to use as the clip coordinates which help determine
    // what should be visible by the user
    [[builtin(position)]] clip_position: vec4<f32>;
    [[location(0)]] color: vec2<f32>;
};


// main function, [[stage(vertex)]] marks this as a valid entry point for a 
// vertex shader
[[stage(vertex)]]
fn vs_main(
    [[builtin(vertex_index)]] in_vertex_index: u32,
) -> VertexOutput {

    // we declare a VertexOutput object named out, which we will return
    var out: VertexOutput;

    // here we declare two of the coordinates for the triangle
    let x = f32(1 - i32(in_vertex_index)) * 0.5;
    let y = f32(i32(in_vertex_index & 1u) * 2 - 1) * 0.5;

    // assign coordinates to clip_position of out and return it
    out.clip_position = vec4<f32>(x, y, 0.0, 1.0);
    out.color = vec2<f32>(x, y);
    return out;
}

// fragment shader

// [[stage(fragment)]] marks this as a valid entry point for the fragment shader
[[stage(fragment)]]

// main fragment function takes in the vertex output of the vertex shader, and
// outputs a color in the first color target (location(0))
fn fs_main(in: VertexOutput) -> [[location(0)]] vec4<f32> {
    return vec4<f32>(
        in.color.x,
        in.color.y,
        0.5,
        1.0
    );
}