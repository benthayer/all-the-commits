// mod gen_commits;
//
///// Cargo compiles main.rs first, so...
///// Remember to run `cargo run main.rs` for the program to run properly!
// fn main() {
//     unimplemented!
// }

// ----------- Save above; below is scratch work ---------

// use crossbeam_channel as channel;

// fn main() {
//     let (tx, rx) = channel::unbounded();

//     tx.send(1);
//     tx.send(2);

//     // Explicityly `drop` this sender allowing the iterator to close.
//     drop(tx);

//     for item in rx {
//         println!("{:?}", item);
//     }
// }
