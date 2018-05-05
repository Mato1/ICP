#!/bin/bash

FORMAT_ATTRIBUTES=(
    "--style=break" "--indent=spaces=4" "--break-blocks" 
  	"--pad-comma" "--pad-header" "--align-pointer=middle" "--align-reference=name" 
    "--suffix=none" "--indent-continuation=3" "--min-conditional-indent=0"
)



format_all() {

	SOURCES="$(find ./src -name '*.cpp' -o -name '*.h')"

	for i in ${SOURCES[@]}; do
		format_file "$i"
	done
}

format_file() {
	astyle "${FORMAT_ATTRIBUTES[@]}" "$1"
}

#main

if [ $# -gt 2 ]; then
	echo "Too many arguments. Max 2 arguments are allowed" >&2
	exit 1
fi

while getopts "a f:" opt; do
	case $opt in
		a)
			format_all >&2
			;;
		f)
			format_file "$OPTARG"
			;;
	esac
done
