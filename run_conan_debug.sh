#!/usr/bin/env bash

conan install . -s build_type=Debug -if=cmake-build-debug -o with_bullet=True -b missing
