package main

import "crypto/sha1"
import "encoding/hex"
import "encoding/binary"

import "fmt"
import "strings"
import "strconv"

import "os"
import "bufio"

var tree = "tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6"
var author = "author Ben Thayer <ben@benthayer.com> 1599715620 -0500"
var committer = "committer Ben Thayer <ben@benthayer.com> 1599715620 -0500"
var message = "Initial commit"

func gen_hash(parent_hash string, salt string) [20]byte {
    parent := "parent " + parent_hash
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
    shasum := sha1.Sum(data)
    return shasum
}


func check(e error) {
    if e != nil {
        panic(e)
    }
}


func main() {
    f, err := os.Create("./commits")
    check(err)

    defer f.Close()

    w := bufio.NewWriter(f)

    // 1<<28 == 16^7
    const num_total_hashes = 1<<28
    var commit_generated [num_total_hashes]bool

    parent_hash := "f9a17849fe28dff34647f698a392be2a9ce3617b"
    hash_decoded, err := hex.DecodeString(parent_hash[:8])
    check(err)
    int_hash := binary.BigEndian.Uint32(hash_decoded) / 16
    commit_generated[int_hash] = true

    var expected float64 = 0
    var progress float64 = 0

    for commit_number := 0; commit_number < (num_total_hashes); commit_number++ {
        expected += float64(num_total_hashes)/float64(num_total_hashes-commit_number)
    }

    var shasum [20]byte
    for commit_number := 1; commit_number < (num_total_hashes); commit_number++ {
        salt := 0
        for commit_generated[int_hash] {
            salt++
            shasum = gen_hash(parent_hash, strconv.Itoa(salt))
            hash := make([]byte, 4)
            copy(hash, shasum[:4])
            hash[3] = hash[3] - (hash[3] % 16)
            int_hash = binary.BigEndian.Uint32(hash) / 16
        }
        commit_generated[int_hash] = true
        parent_hash = hex.EncodeToString(shasum[:])
        w.WriteString(parent_hash + " " + strconv.Itoa(salt) + "\n")
        progress += float64(num_total_hashes)/float64(num_total_hashes-commit_number)
        if commit_number > (num_total_hashes - 12000) {
            w.Flush()
        }
        fmt.Print(strconv.FormatFloat(progress/expected, 'f', 8, 64) + "   " + strconv.Itoa(commit_number) + "\r")
    }
}
