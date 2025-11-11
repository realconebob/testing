package main

import (
	"math/rand"
	"fmt"
)

func main() {
	const ALPHABET string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	res := []rune(ALPHABET)

	rand.Shuffle(len(res), func(i int, j int) {
		tmp := res[i]
		res[i] = res[j]
		res[j] = tmp
	})
	fmt.Printf("%v\n", string(res));

	// Forgot that strings are immutable in Go, so while you can cast a string to a rune slice and modify the index - without go
	// 	complaining about this - that doesn't mean it'll actually change the value. You need to be manipulating a slice, not a
	// 	slice cast
}