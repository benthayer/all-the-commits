mod gen_commits;
// use gen_commits::check;
// use gen_commits::gen_hash;
// use gen_commits::sum_to_int;

fn main() {
    println!("Testing main.rs; success!");

    // Track the commits we're making
    unimplemented!
    // all `check(err)` calls should be made with `panic
    
 
    //     // Track the commits we're making
    //     f, err := os.Create("./commits")
    //     check(err)
    //     defer f.Close()
    //     w := bufio.NewWriter(f)

    //     // progress tracking
    //     var expected float64 = 0
    //     var progress float64 = 0

    //     for commit_number := 0; commit_number < (num_total_hashes); commit_number++ {
    //         expected += float64(num_total_hashes) / float64(num_total_hashes-commit_number)
    //     }

    //     // variables
    //     var commit_generated [num_total_hashes]bool

    //     // set up first commit
    //     parent_hash := "f9a17849fe28dff34647f698a392be2a9ce3617b"
    //     hash_decoded, err := hex.DecodeString(parent_hash[:])
    //     check(err)
    //     var sha_sum [20]byte
    //     copy(sha_sum[:], hash_decoded)
    //     commit_generated[sum_to_int(sha_sum)] = true

    //     // start generating commits
    //     for commit_number := 1; commit_number < (num_total_hashes); commit_number++ {
    //         commit_info := get_next_commit(parent_hash, &commit_generated, commit_number)

    //         parent_hash = hex.EncodeToString(commit_info.sha_sum[:])

    //         // progress tracking
    //         w.WriteString(parent_hash + " " + strconv.Itoa(commit_info.salt) + "\n")
    //         progress += float64(num_total_hashes) / float64(num_total_hashes-commit_number)
    //         if commit_number > (num_total_hashes - 12000) {
    //             w.Flush()
    //         }
    //         fmt.Print(strconv.FormatFloat(progress/expected, 'f', 8, 64) + "   " + strconv.Itoa(commit_number) + "\r")
    //     }
}
