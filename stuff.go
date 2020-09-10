package main

import "crypto/sha1"
import "encoding/hex"
import "fmt"

func main() {
    data := []byte("hello\n")
    shasum := sha1.Sum(data)
    hash := shasum[:]
    fmt.Println(hex.EncodeToString(hash))
}
