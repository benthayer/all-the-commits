mod gen_commits;

use gen_commits::{
  gen_hash, get_next_commit, sum_to_int, NUM_TOTAL_HASHES_i32, NUM_TOTAL_HASHES_usize,
};
use std::io::Write;

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
    Err(why) => panic!("couldn't create {}: {}", display, why),
    Ok(f) => f,
  };

  let mut buffer = BufWriter::new(f);

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

  let mut hash_decoded = match hex::decode(&parent_hash) {
    Err(why) => panic!("couldn't decode `parent_hash`: {}", why),
    Ok(hash_decoded) => hash_decoded,
  };

  let sha_sum: Vec<u8>;
  sha_sum.clone_from_slice(&hash_decoded);
  commit_generated[usize::try_from(sum_to_int(sha_sum)).unwrap()] = true;

  // start generating commits
  for commit_number in 1..NUM_TOTAL_HASHES_i32 {
    let commit_info = get_next_commit(parent_hash, commit_generated, commit_number);

    let parent_hash = hex::encode(commit_info.sha_sum);

    // progress tracking
    let to_write = parent_hash + " " + &commit_info.salt.to_string() + "\n";
    buffer.write_all(&to_write.as_bytes());

    progress += f64::try_from(NUM_TOTAL_HASHES_i32).unwrap()
      / f64::try_from(NUM_TOTAL_HASHES_i32 - commit_number).unwrap();
    if commit_number > (NUM_TOTAL_HASHES_i32 - 12_000) {
      buffer.flush();
    }

    let eta: f64 = progress / expected; // no exponent

    println!(
      "{:.8}",
      format!("{}", eta) + "   " + &commit_number.to_string() + "\r"
    );
  }
}
