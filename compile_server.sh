#!/bin/bash

cd server/
cmake .
make clean
make
cd ../
