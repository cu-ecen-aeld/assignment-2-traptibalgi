#!/bin/bash

filesdir=$1
searchstr=$2

if [ $# -ne 2 ]
then
	echo "Expecting 2 parameters"
	exit 1
elif [ ! -d $filesdir ] 
then
	echo "filesdir is not a directory!"
	exit 1
fi

num_files=$(ls "$filesdir" | wc -l)

count=$(grep -r "$searchstr" "$filesdir" | wc -l)

echo "The number of files are $num_files and the number of matching lines are $count"


