#! /bin/bash

make ./bin/$1

./bin/$1 < ./input.txt


make clean
