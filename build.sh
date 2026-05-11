#!/usr/bin/bash
rm main
g++ src/*.cpp src/utility/*.cpp -I include -I include/utility -I util -std=c++23 -o main