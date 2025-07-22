#!/usr/bin/env -S bash

SOMEWHAT_REALDIR="$(realpath $0)"
FILENAME="${SOMEWHAT_REALDIR##*/}"
PATHTO="${SOMEWHAT_REALDIR/$FILENAME/}"

DUSER="root"

function setup () {
	# Move to this directory if it's not the working dir
	if [[ "$(pwd)/" != "$PATHTO" ]]; then
		(cd "$PATHTO" && bash "$FILENAME" "$@")
		exit $?
	fi

	# Try to run this as root if not already
	if [[ "$(whoami)" != "$DUSER" ]]; then
		sudo -u "$DUSER" bash "$FILENAME" "$@"
		exit $?
	fi

	return 0
}

function testing () {
	things=("$@")
	if [ ${#things[@]} -gt 0 ]; then
		for i in $(seq 0 ${#things[@]}); do
			echo "${things[i]}"
		done
	fi

	return 0
}

#testing "$@"
# "Array-ness" of arguments is preserved when passed in quoted

setup "$@" && exit $?
