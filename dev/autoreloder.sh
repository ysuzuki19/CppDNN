#!/bin/bash

while :;
do
	echo "[START]"
	./test.sh
	echo "[END]"
	res=$(fswatch -1 --event Updated . ../header)
done
