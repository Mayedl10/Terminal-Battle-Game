#!/usr/bin/bash
rm main
g++ src/*.cpp -I include -I util -std=c++23 -o main