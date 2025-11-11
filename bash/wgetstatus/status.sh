#!/usr/bin/env -S bash

function test_wget() {
	export WGETRES="$(wget -4 example.com 2>&1 | grep response)"
}

test_wget && echo "$WGETRES"
