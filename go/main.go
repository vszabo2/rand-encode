package main

import "fmt"
import "math"

const seq = "heyvic"

func test(x int32) {
    var rand GlibcRand
    rand.Srand(x)
    for _, c := range seq {
        if 'a' + int(rand.Rand() % 26) != int(c) {
            return
        }
    }
    fmt.Printf("Found seed: %v\n", uint32(x))
}

func main() {
    for i := int64(math.MinInt32); i <= math.MaxInt32; i++ {
        go test(int32(i))
    }
}
