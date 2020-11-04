mod gen_commits;

use gen_commits::gen_hash;
use std::fs::File;
use std::io::BufWriter;

fn main() /* -> std::io::Result<()> */
{
  /// I really need to properly learn about Rust's `Result` and `Option` types:
  /// http://web.mit.edu/rust-lang_v1.25/arch/amd64_ubuntu1404/share/doc/rust/html/book/first-edition/error-handling.html
  let mut f = File::create("./commits")?;
  Ok(());
  // check()

  // After thoroughly learning how to handle Error types, fix this by completing
  // this function's return type.
  let mut w = BufWriter::new(file);
}
