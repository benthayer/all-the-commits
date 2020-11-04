mod gen_commits;

use gen_commits::gen_hash;
use std::fs::File;

/// Cargo compiles main.rs first, so...
/// Remember to run `cargo run main.rs` for the program to run properly!
fn main() {
  let (mut file, mut err) = (File::create("./commits"), File::create("./commits"));
  // check()
  /// For the following line below, review mutex guards in Rust, see link below
  /// for more info (make sure to CLT + F "defer" on the page):
  /// https://www.reddit.com/r/golang/comments/5plu38/re_esrs_comparison_of_concurrency_in_go_and_rust/
  // defer f.close()

}
