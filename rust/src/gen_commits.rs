/// Since use of `select!` macros are unstable, we MUST switch to using futures
/// instead.
// use crossbeam_channel::{bounded, select, unbounded, Receiver, Sender};
// ---------------------------
use tokio; // go through tutorial -> https://tokio.rs/tokio/tutorial

// use futures::{executor::block_on, future, select as fut_select};
use threadpool::ThreadPool;

use byteorder::{BigEndian, ByteOrder};
use ring::digest;
use std::convert::TryFrom;

// 1<<28 == 16^7
pub const NUM_TOTAL_HASHES_usize: usize = 1 << 28;
pub const NUM_TOTAL_HASHES_i32: i32 = 1 << 28;

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

type Err<T> = std::result::Result<T, Error>;

struct Error;

/// For unrecoverable errors -- perhaps need custom error types for each unique
/// contextual error with this program. See the link below for more details:
/// http://web.mit.edu/rust-lang_v1.25/arch/amd64_ubuntu1404/share/doc/rust/html/book/first-edition/error-handling.html
// pub fn check(option: Option<T>) {
//   if error != None {
//     panic!(error);
//   }
// }

pub fn sum_to_int(sha_sum: Vec<u8>) -> u32 {
  let mut dst = vec![0; 4];

  // for reference, see the link below to understand why this works:
  // https://stackoverflow.com/questions/28219231/how-to-idiomatically-copy-a-slice
  dst.clone_from_slice(&sha_sum[0..4]);
  dst[3] = dst[3] - (dst[3] % 16);

  let int_to_return: u32 = BigEndian::read_u32(&dst) / 16;

  return int_to_return;
}

pub struct CommitInfo {
  pub sha_sum: Vec<u8>,
  pub salt: i32,
}

pub fn salt_mine(
  parent: &str,
  commit_generated: [bool; NUM_TOTAL_HASHES_usize],
  salt_work_receiver: Receiver<i32>,
  result_sender: Sender<CommitInfo>,
) {
  for salt in salt_work_receiver {
    let sha_sum = gen_hash(parent, salt);
    let index: usize = usize::try_from(sum_to_int(sha_sum)).unwrap();

    if commit_generated[index] {
      continue;
    }

    // Tell the others, or die trying!
    // `select!` macro is unstable.
    // (see reference: https://stackoverflow.com/questions/52302371/what-is-the-equivalent-of-gos-select-case-paradigm-for-channels-in-rust)

    // select! {
    //   send(result_sender, CommitInfo { sha_sum, salt });
    //   // recv(result_receiver, res) -> res => return res,
    // }
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
  /// Use `futures` instead.
  // let (salt_work_sender, salt_work_receiver) = unbounded();
  // let (result_sender, result_receiver) = bounded(numWorkers as usize);
  let fut_salt_values = async {
    let fut_salt_mine_result = async move {
      for i in 0..numWorkers {
        let result_sender = result_sender.clone();

        pool.execute(move || {
          salt_mine(parent, commit_generated, salt_work_receiver, result_sender);
        });
      }
    };

    // Wait until salt has been mined before mining another.
    fut_salt_mine_result.await;
  };

  // Use of `select!` macro is unstable.

  // Select must operate concurrently with `salt_mine` so that `result_receiver`
  // is concurrently receiving input from the sending-channel output of
  // `saltMine()`.
  // select! {
  //   /// Learn how to properly use the `select!` macro in channels
  //   salt_work_sender.send(salt),
  //   salt += 1,
  //   block_on(fut_salt_values), // runs asynchronously, waiting for new salt

  //   commit_generated[sum_to_int(result_receiver.sha_sum)] = true,
  //   close(salt_chan),
  //   recv(result_receiver, result) => return result,
  // }
}
