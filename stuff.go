package main

import "crypto/sha1"
import "encoding/hex"
import "encoding/binary"

import "strings"
import "strconv"
import "fmt"

import "os"

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
    fmt.Println(object)
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
    first_commit_hash := "f9a17849fe28dff34647f698a392be2a9ce3617b"
    hash_decoded, err := hex.DecodeString(first_commit_hash[:8])
    check(err)

    f, err := os.Create("./commits")
    check(err)
    defer f.Close()

    // 1<<28 == 16^7
    var commit_generated [1<<28]bool

    int_hash := binary.BigEndian.Uint32(hash_decoded) / 16
    commit_generated[int_hash] = true

    salt := 0
    var shasum [20]byte
    for commit_generated[int_hash] {
        shasum = gen_hash(first_commit_hash, strconv.Itoa(salt))
        hash := shasum[:4]
        hash[3] = hash[3] - (hash[3] % 16)
        int_hash = binary.BigEndian.Uint32(hash) / 16
    }
    h := fmt.Sprintf("%x", int_hash)
    fmt.Println(h)
    f.Write([]byte(hex.EncodeToString(shasum[:]) + "\n"))
    f.Sync()
    check(err)
}
