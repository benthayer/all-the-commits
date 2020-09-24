use encoding;
use ring::{digest, test};
use serde::{Serialize, Deserialize};

pub fn gen_hash(parent_hash: &str, salt: &str) -> i32 {
  let tree = r"tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6";
  let author = r"author Ben Thayer <ben@benthayer.com> 1599715620 -0500";
  let committer = r"committer Ben Thayer <ben@benthayer.com> 1599715620 -0500";
  let message = r"Initial commit";

  let parent = "parent ".to_owned() + parent_hash;
  let commit = unimplemented!   // ---------------- see Golang example below
  // commit := strings.Join(
  //   []string{
  //       tree,
  //       parent,
  //       author,
  //       committer,
  //       "",
  //       message,
  //       "",
  //       salt,
  //       ""},
  //   "\n")
  let prefix = unimplemented!    // ---------------- see Golang example below
  // prefix := "commit " + strconv.Itoa(len(commit)) + "\x00"
  let object = unimplemented!    // ---------------- see Golang example below
  // object := prefix + commit
  let data = unimplemented!      // ---------------- see Golang example below
  // data := []byte(object)
  let sha_sum = unimplemented!   // ---------------- see Golang example below
  /// Use SHA256 like so:
  /// let _actual_digest = digest::digest(&digest::SHA256, _data_as_bytes)
  // sha_sum := sha1.Sum(data)
  return sha_sum;
}

/// -------------------- Remaining Golang implementation -----------------------
/// General error check
pub fn check() {
  unimplemented!                  // ---------------- see Golang example below
  // func check(e error) {
  //   if e != nil {
  //       panic(e)
  //   }
  // }
}

/// Sum array of bytes, return as binary
pub fn sum_to_int() {
  unimplemented!                  // ---------------- see Golang example below
  // func sum_to_int (sha_sum [20]byte) {
  //   hash := make([]byte, 4)
  //   copy(hash, sha_sum[:4])
  //   hash[3] = hash[3] - (hash[3] % 16)
  //   return binary.BigEndian.Uint32(hash) / 16
// }
}