package main

import (
	"time"
	"fmt"
	"math"
)

func PseudoRandom(feed uint64) uint64 {
	return (feed * 10102007) ^ math.Float64bits(math.Exp(math.Float64frombits(feed))) ^ math.Float64bits(math.Pow(2, 0.5)) ^ feed
}

func main() {
	fmt.Printf("%v\n", time.Now().UnixNano() ^ (time.Now().UnixNano() + 31556952000000000))
	fmt.Printf("%v\n", PseudoRandom(uint64(time.Now().UnixNano())))
}
