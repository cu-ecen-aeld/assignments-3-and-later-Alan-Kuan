#!/bin/sh

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
fi

mkdir -p `dirname "$1"`

if ! [ -f "$1" ]; then
    touch "$1"
fi

echo "$2" > "$1"