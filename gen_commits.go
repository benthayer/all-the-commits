package main

import (
	"bufio"
	"crypto/sha1"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var tree = "tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6"
var author = "author Ben Thayer <ben@benthayer.com> 1599715620 -0500"
var committer = "committer Ben Thayer <ben@benthayer.com> 1599715620 -0500"
var message = "Initial commit"

// 1<<28 == 16^7
const num_total_hashes = 1 << 28

func gen_hash(parent_hash string, salt_int int) [20]byte {
	parent := "parent " + parent_hash
	salt := strconv.Itoa(salt_int)
	commit := strings.Join(
		[]string{
			tree,
			parent,
			author,
			committer,
			"",
			message,
			"",
			salt,
			""},
		"\n")

	prefix := "commit " + strconv.Itoa(len(commit)) + "\x00"
	object := prefix + commit
	data := []byte(object)
	sha_sum := sha1.Sum(data)
	return sha_sum
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func sum_to_int(sha_sum [20]byte) uint32 {
	hash := make([]byte, 4)
	copy(hash, sha_sum[:4])
	hash[3] = hash[3] - (hash[3] % 16)
	return binary.BigEndian.Uint32(hash) / 16
}

type CommitInfo struct {
	sha_sum [20]byte
	salt    int
}

func saltMine(parent string, commit_generated *[num_total_hashes]bool, salt_chan chan int, result_chan chan CommitInfo) {
	for salt := range salt_chan {
		sha_sum := gen_hash(parent, salt)

		// Try again if it's unique
		if commit_generated[sum_to_int(sha_sum)] {
			continue
		}

		// Tell the others or die trying!
		select {
		case result_chan <- CommitInfo{sha_sum, salt}:
			return
		}
	}
}

func get_next_commit(parent string, commit_generated *[num_total_hashes]bool, commit_number int) CommitInfo {

	var numWorkers int
	if commit_number <= num_total_hashes/2 {
		numWorkers = 1
	} else if commit_number <= num_total_hashes/4 {
		numWorkers = 2
	} else if commit_number <= num_total_hashes/8 {
		numWorkers = 4
	} else {
		numWorkers = 8
	}

	salt := 1
	salt_chan := make(chan int)
	result_chan := make(chan CommitInfo, numWorkers)

	for i := 0; i < numWorkers; i++ {
		go saltMine(parent, commit_generated, salt_chan, result_chan)
	}

	for {
		select {
		case salt_chan <- salt:
			salt++
		case result := <-result_chan:
			commit_generated[sum_to_int(result.sha_sum)] = true
			close(salt_chan)
			return result
		}
	}
}

func main() {
	// Track the commits we're making
	f, err := os.Create("./commits")
	check(err)
	defer f.Close()
	w := bufio.NewWriter(f)

	// progress tracking
	var expected float64 = 0
	var progress float64 = 0

	for commit_number := 0; commit_number < (num_total_hashes); commit_number++ {
		expected += float64(num_total_hashes) / float64(num_total_hashes-commit_number)
	}

	// variables
	var commit_generated [num_total_hashes]bool

	// set up first commit
	parent_hash := "f9a17849fe28dff34647f698a392be2a9ce3617b"
	hash_decoded, err := hex.DecodeString(parent_hash[:])
	check(err)
	var sha_sum [20]byte
	copy(sha_sum[:], hash_decoded)
	commit_generated[sum_to_int(sha_sum)] = true

	// start generating commits
	for commit_number := 1; commit_number < (num_total_hashes); commit_number++ {
		commit_info := get_next_commit(parent_hash, &commit_generated, commit_number)

		parent_hash = hex.EncodeToString(commit_info.sha_sum[:])

		// progress tracking
		w.WriteString(parent_hash + " " + strconv.Itoa(commit_info.salt) + "\n")
		progress += float64(num_total_hashes) / float64(num_total_hashes-commit_number)
		if commit_number > (num_total_hashes - 12000) {
			w.Flush()
		}
		fmt.Print(strconv.FormatFloat(progress/expected, 'f', 8, 64) + "   " + strconv.Itoa(commit_number) + "\r")
	}
}
