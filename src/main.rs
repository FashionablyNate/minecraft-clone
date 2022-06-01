use mc_clone::run;

fn main() {
    // awaits future before running
    pollster::block_on(run());
}