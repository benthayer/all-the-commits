mod gen_commits;
use gen_commits::check;
use gen_commits::gen_hash;
use gen_commits::sum_to_int;

fn main() {
    // unimplemented!                   // ---------------- see Golang example below
    println!("Testing main.rs; success!");
    //     // Track the commits we're making
    //     f, err := os.Create("./commits")
    //     check(err)
    //     defer f.Close()
    //     w := bufio.NewWriter(f)

    //     // 1<<28 == 16^7
    //     const num_total_hashes = 1<<28

    //     // progress tracking
    //     var expected float64 = 0
    //     var progress float64 = 0

    //     for commit_number := 0; commit_number < (num_total_hashes); commit_number++ {
    //         expected += float64(num_total_hashes)/float64(num_total_hashes-commit_number)
    //     }

    //     // variables
    //     var commit_generated [num_total_hashes]bool
    //     var sha_sum [20]byte
    //     var int_hash uint32

    //     // set up first commit
    //     parent_hash := "f9a17849fe28dff34647f698a392be2a9ce3617b"
    //     hash_decoded, err := hex.DecodeString(parent_hash[:8])
    //     check(err)
    //     copy(sha_sum[:], hash_decoded)
    //     int_hash = sum_to_int(sha_sum)
    //     commit_generated[int_hash] = true

    //     // start generating commits
    //     for commit_number := 1; commit_number < (num_total_hashes); commit_number++ {
    //         // generating commits
    //         salt := 0
    //         for commit_generated[int_hash] {
    //             salt++
    //             sha_sum = gen_hash(parent_hash, strconv.Itoa(salt))
    //             int_hash = sum_to_int(sha_sum)
    //         }
    //         commit_generated[int_hash] = true
    //         parent_hash = hex.EncodeToString(sha_sum[:])

    //         // progress tracking
    //         w.WriteString(parent_hash + " " + strconv.Itoa(salt) + "\n")
    //         progress += float64(num_total_hashes)/float64(num_total_hashes-commit_number)
    //         if commit_number > (num_total_hashes - 12000) {
    //             w.Flush()
    //         }
    //         fmt.Print(strconv.FormatFloat(progress/expected, 'f', 8, 64) + "   " + strconv.Itoa(commit_number) + "\r")
    //     }
    // }
}
