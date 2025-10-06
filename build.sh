#!/usr/bin/env bash
set -e
if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi
g++ -g -std=c++17 -O0 -I . -o bin/interrupts.exe interrupts.cpp
echo "OK."