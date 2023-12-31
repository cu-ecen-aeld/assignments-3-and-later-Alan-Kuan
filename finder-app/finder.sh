#!/bin/sh

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi

if ! [ -d "$1" ]; then
    echo "'$1' is not a directory!"
    exit 1
fi

file_num=`find "$1" -type f | wc -l`
matched_line_num=`grep "$2" -r "$1" | wc -l`

echo "The number of files are ${file_num} and the number of matching lines are ${matched_line_num}"