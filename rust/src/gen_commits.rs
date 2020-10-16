use byteorder::{BigEndian, ByteOrder};
// use crossbeam_channel as channel; this is used to illustrate how to send values
// on a channel
use ring::digest;

/// Sending values on a channel
/// 
/// Create a non-buffered channel.
/// let (tx, rx) = channel::unbounded();
/// 
/// Create a buffered channel with a capacity of one.
/// let (s, r) = channel::bounded(1);
/// 
/// for i in 0..100 {
///   // We can send an "infinite" amount of items into the unbounded channel
///   // without blocking.
///   // This is different from Go as we don't need a receiver ready.
///   tx.send(i);
/// }
/// 
/// // Try and receive one of the values in a blocking fashion.
/// println!("{:?}", rx.recv());
/// 
/// s.send(1);
/// // This would block until the receiving half read the value!
/// s.send(2);
///
/// 
///                              YOU LEFT OFF HERE 
/// 
/// Continue referring to this example of Channels in Rust
/// https://gsquire.github.io/static/post/a-rusty-go-at-channels/#:~:text=Channels%20in%20Rust,can%20be%20shared%20across%20threads


// 1<<28 == 16^7
const NUM_TOTAL_HASHES: i64 = 1 << 28;

pub fn gen_hash(parent_hash: &str, salt_int: i64) -> Vec<u8> {
  let tree = "tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6";
  let author = "author Ben Thayer <ben@benthayer.com> 1599715620 -0500";
  let committer = "committer Ben Thayer <ben@benthayer.com> 1599715620 -0500";
  let message = "Initial commit";

  let parent = "parent ".to_string() + parent_hash;
  let salt = salt_int.to_string();
  let commit = [tree, &parent, author, committer, "", message, "", &salt, ""].join("\n");

  let prefix = "commit ".to_string() + &commit.len().to_string() + "\x00";
  let object = prefix + &commit;
  let _data = object.as_bytes();
  let sha_sum = digest::digest(&digest::SHA1_FOR_LEGACY_USE_ONLY, _data);

  return sha_sum.as_ref().to_vec();
}

/// Double check if this is needed.
pub fn check() {}

pub fn sum_to_int(sha_sum: Vec<u8>) -> u64 {
  let mut dst = [0, 0, 0, 0];
  // for reference, see the link below to understand why this works:
  // https://stackoverflow.com/questions/28219231/how-to-idiomatically-copy-a-slice
  dst.clone_from_slice(&sha_sum[0..4]);
  dst[3] = dst[3] - (dst[3] % 16);

  let int_to_return = BigEndian::read_u64(&dst) / 16;

  return int_to_return;
}

struct CommitInfo {
  sha_sum: Vec<u8>,
  salt: i64,
}

/// Channel inputs, `salt_chan` and `result_chan`
pub fn salt_mine(parent: &str, commit_generated: &[bool], salt_chan: i64, result_chan: i64) {
  // From examples in blog post, probs need something like this:  
  tx.send(CommitInfo{sha_sum, salt});
}

pub fn get_next_commit() {}
