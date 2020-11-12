mod gen_commits;

use gen_commits::{gen_hash, NUM_TOTAL_HASHES_i32, NUM_TOTAL_HASHES_usize};
use std::convert::TryFrom;
use std::fs::File;
use std::io::BufWriter;
use std::path::Path;

/// A great reference for error handling in Rust:
/// http://web.mit.edu/rust-lang_v1.25/arch/amd64_ubuntu1404/share/doc/rust/html/book/first-edition/error-handling.html
fn main() {
  let path = Path::new("./commits");
  let display = path.display();

  // Open a file in write-only mode, returns `io::Result<File>`
  let mut f = match File::create(&path) {
    Err(why) => panic!("could'nt create {}: {}", display, why),
    Ok(f) => f,
  };

  let mut w = BufWriter::new(f);

  // progress tracking
  let (mut expected, mut progress) = (0.0, 0.0);

  for commit_number in 0..NUM_TOTAL_HASHES_i32 {
    expected += f64::try_from(NUM_TOTAL_HASHES_i32).unwrap()
      / f64::try_from(NUM_TOTAL_HASHES_i32 - commit_number).unwrap()
  }

  // variables
  let mut commit_generated: [bool; NUM_TOTAL_HASHES_usize];

  // set up first commit
  let parent_hash: &str = "f9a17849fe28dff34647f698a392be2a9ce3617b";
  /// TODO: slice parent hash and figure out if you are decoding string correctly.
  let mut hash_decoded = match hex::decode(parent_hash..)
}
