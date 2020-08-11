#!/bin/bash
ehco "building"
cmake .
make
rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
ehco "complete"
