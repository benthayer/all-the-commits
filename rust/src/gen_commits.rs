use encoding;
use std::ptr::copy;
use std::convert::TryInto;
use ring::{digest};
use byteorder::{ByteOrder, BigEndian};



const NUM_TOTAL_HASHES: i32 = 1 << 28;

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


pub fn check(error: ) {
  // func check(e error) {
  //   if e != nil {
  //       panic(e)
  //   }
  // }
}


pub fn sum_to_int(sha_sum: Vec<u8>) -> u32 {
  let mut dst = [0, 0, 0, 0];
  // for reference, see the link below to understand why this works:
  // https://stackoverflow.com/questions/28219231/how-to-idiomatically-copy-a-slice 
  dst.clone_from_slice(&sha_sum[0..4]);
  
  dst[3] = dst[3] - (dst[3] % 16);

  let int_to_return = BigEndian::read_u32(&dst) / 16;

  return int_to_return;
}


struct CommitInfo {
  sha_sum: Vec<u8>,
  salt: i32
}
// type CommitInfo struct {
// 	sha_sum [20]byte
// 	salt    int
// }


pub fn salt_mine() {
  // for elem in iter {
    
  // }
}
// func saltMine(parent string, commit_generated *[num_total_hashes]bool, salt_chan chan int, result_chan chan CommitInfo) {
  // 	for salt := range salt_chan {
  // 		sha_sum := gen_hash(parent, salt)

  // 		// Try again if it's unique
  // 		if commit_generated[sum_to_int(sha_sum)] {
  // 			continue
  // 		}

  // 		// Tell the others or die trying!
  // 		select {
  // 		case result_chan <- CommitInfo{sha_sum, salt}:
  // 			return
  // 		}
  // 	}
// }


pub fn get_next_commit() {
  unimplemented!
}
// func get_next_commit(parent string, commit_generated *[num_total_hashes]bool, commit_number int) CommitInfo {

  // 	var numWorkers int
  // 	if commit_number <= num_total_hashes/2 {
  // 		numWorkers = 1
  // 	} else if commit_number <= num_total_hashes/4 {
  // 		numWorkers = 2
  // 	} else if commit_number <= num_total_hashes/8 {
  // 		numWorkers = 4
  // 	} else {
  // 		numWorkers = 8
  // 	}

  // 	salt := 1
  // 	salt_chan := make(chan int)
  // 	result_chan := make(chan CommitInfo, numWorkers)

  // 	for i := 0; i < numWorkers; i++ {
  // 		go saltMine(parent, commit_generated, salt_chan, result_chan)
  // 	}

  // 	for {
  // 		select {
  // 		case salt_chan <- salt:
  // 			salt++
  // 		case result := <-result_chan:
  // 			commit_generated[sum_to_int(result.sha_sum)] = true
  // 			close(salt_chan)
  // 			return result
  // 		}
  // 	}
// }