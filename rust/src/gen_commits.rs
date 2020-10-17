use byteorder::{BigEndian, ByteOrder};
use crossbeam_channel as channel;
use ring::digest;

// 1<<28 == 16^7
const NUM_TOTAL_HASHES: usize = (1 << 28) as usize;

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
pub fn salt_mine(
  parent: &str,
  commit_generated: [bool; NUM_TOTAL_HASHES],
  salt_chan: channel::unbounded(),
  result_chan: CommitInfo,
) {
  for salt in salt_chan {
    /// Finish `get_next_commit` function first!
  }
}

pub fn get_next_commit(
  parent: &str,
  commit_generated: [bool; NUM_TOTAL_HASHES],
  commit_number: i64,
) -> CommitInfo {
  let numWorkers: i64;

  if commit_number <= (NUM_TOTAL_HASHES as i64) / 2 {
    numWorkers = 1
  } else if commit_number <= (NUM_TOTAL_HASHES as i64) / 4 {
    numWorkers = 2
  } else if commit_number <= (NUM_TOTAL_HASHES as i64) / 8 {
    numWorkers = 4
  } else {
    numWorkers = 8
  }

  let salt = 1;
  let (tx_salt_chan, rx_salt_chan) = channel::unbounded();
  let (tx_result_chan, rx_result_chan) = channel::bounded(numWorkers);

  // Left off on the two for loops

  return result;
}
