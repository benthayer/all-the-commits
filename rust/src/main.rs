mod gen_commits;

use std::fs::File;
use gen_commits::gen_hash;

/// Cargo compiles main.rs first, so...
/// Remember to run `cargo run main.rs` for the program to run properly!
fn main() {
  let mut file = File::create("./commits");
  println!("Main function prints.");
}
