#!/bin/bash

export PATH=/home/ece449/gcc-5.4.0/bin:$PATH
export LD_LIBRARY_PATH=/home/ece449/gcc-5.4.0/lib64:$LD_LIBRARY_PATH

g++ src/*.h src/*.cpp  -Wall -std=c++11 -g -lm -Wl,-rpath=/home/ece449/gcc-5.4.0/lib64 -o myevl
