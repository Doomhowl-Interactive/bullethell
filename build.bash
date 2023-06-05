#!/bin/bash
git fetch --recurse-submodules

pushd vendor/basalt
git merge
popd

cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build