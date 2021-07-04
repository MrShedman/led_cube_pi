#!/usr/bin/env bash

sudo apt install cmake libyaml-cpp-dev libglm-dev wiringpi

mkdir -p build/debug
mkdir -p build/release
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cd ../release
cmake -DCMAKE_BUILD_TYPE=Release ../..
