use crossbeam_channel::{bounded, unbounded, Select, select, Receiver, Sender};
use threadpool::ThreadPool;

use byteorder::{BigEndian, ByteOrder};
use ring::digest;
use std::convert::TryFrom;

// 1<<28 == 16^7
const NUM_TOTAL_HASHES_usize: usize = 1 << 28;
const NUM_TOTAL_HASHES_i32: i32 = 1 << 28;

pub fn gen_hash(parent_hash: &str, salt_int: i32) -> Vec<u8> {
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

pub fn sum_to_int(sha_sum: Vec<u8>) -> u32 {
  let mut dst = [0; 4];

  // for reference, see the link below to understand why this works:
  // https://stackoverflow.com/questions/28219231/how-to-idiomatically-copy-a-slice
  dst.clone_from_slice(&sha_sum[0..4]);
  dst[3] = dst[3] - (dst[3] % 16);

  let int_to_return: u32 = BigEndian::read_u32(&dst) / 16;

  return int_to_return;
}

struct CommitInfo {
  sha_sum: Vec<u8>,
  salt: i32,
}

pub fn salt_mine(
  parent: &str,
  commit_generated: [bool; NUM_TOTAL_HASHES_usize],
  r_salt_chan: Receiver<i32>,
  s_result_chan: Sender<CommitInfo>,
) {
  for salt in r_salt_chan {
    let sha_sum = gen_hash(parent, salt);
    let index: usize = usize::try_from(sum_to_int(sha_sum)).unwrap();

    if commit_generated[index] {
      continue;
    }

    // Tell the others, or die trying!
    select! {
      s_result_chan.send(CommitInfo{sha_sum, salt})
    }
  }
}

pub fn get_next_commit(
  parent: &str,
  commit_generated: [bool; NUM_TOTAL_HASHES_usize],
  commit_number: i32,
) -> CommitInfo {
  let numWorkers: i32;
  if commit_number <= (NUM_TOTAL_HASHES_i32) / 2 {
    numWorkers = 1;
  } else if commit_number <= (NUM_TOTAL_HASHES_i32) / 4 {
    numWorkers = 2;
  } else if commit_number <= (NUM_TOTAL_HASHES_i32) / 8 {
    numWorkers = 4;
  } else {
    numWorkers = 8;
  }

  // Create a new thread pool capable of executing four jobs concurrently.
  let pool = ThreadPool::new(4);

  let salt: i32 = 1;
  let (s_salt_chan, r_salt_chan) = unbounded();
  let (s_result_chan, r_result_chan) = bounded(numWorkers as usize);

  for i in 0..numWorkers {
    salt_mine(parent, commit_generated, r_salt_chan, s_result_chan);
  }

  /// Reference: https://docs.rs/crossbeam-channel/0.5.0/crossbeam_channel/struct.Select.html
  let mut sel = Select::new();

  /// Implement thread pools...
  for i in salt.. {
    select! {
      s_salt_chan.send(salt),
      salt += 1,
      // s_result_chan.send(r_result_chan),
      // recv(r_result_chan, result) => return result,
    }
  }

  return CommitInfo;
  /// Figure out how to have a loop so that `s_salt_chan` is not prematurely
  /// dropped during the chain of channel send and receive operations (and how
  /// to implement the increasing `salt` counter).
  // select! {
  //   s_result_chan.send(result_chan),
  //   commit_generated[sum_to_int(r_result_chan.sha_sum)] = true
  //   drop(s_salt_chan),
  //   recv(r_result_chan) return result,
  // }
}
