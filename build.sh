#!/bin/bash

export PATH=/home/ece449/gcc-5.4.0/bin:$PATH
export LD_LIBRARY_PATH=/home/ece449/gcc-5.4.0/lib64:$LD_LIBRARY_PATH

g++ src/*.cpp -Wall -std=c++11 -g -lm -o myevl
