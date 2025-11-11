#!/usr/bin/env bash

somewhat_path() {
	SOMEWHAT_REALDIR="$(realpath $0)"
	FILENAME="${SOMEWHAT_REALDIR##*/}"
	PATHTO="${SOMEWHAT_REALDIR/$FILENAME/}"
	return "$PATHTO"
}

#dir_name() {
#	return "$(cd $(dirname $0) && echo $PWD)"
#}

echo "$(cd $(dirname $0) && echo $PWD)"
