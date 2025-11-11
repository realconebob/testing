#!/usr/bin/env bash

# Check for file
    # If file exists and was modified less than TIME ago
        # exit
    # else
        # execute script
        # modify file

# The last time I used GNU find, this was fine, but I might have to do cd wrapping again

TIMESTAMP_NAME="check.ts"

# To stop find from freaking out and finding the wrong files, cd to the local dir
somewhat_realpath="$(realpath $0)"
PATHTO="${somewhat_realpath/check.bash/}"
if [[ "$(pwd)/" != "$PATHTO" ]]; then
	(cd $PATHTO && bash check.bash)
	exit
fi

CHECKTIME="$((60*24))"
	# CHECKTIME is in minutes. Math works inside the $(()). 60*24 is the same as 1 day

COM_FIND="find . -maxdepth 1 -iname $TIMESTAMP_NAME -mmin +$(($CHECKTIME))"
	# Finds a file with the name TIMESTAMP_NAME that's greater than 1 minute old and is in the dir this script is executed in

if [ -n "$($COM_FIND)" ]; then
	echo "$(date)" > $TIMESTAMP_NAME
	echo "Timestamp Updated"
fi
