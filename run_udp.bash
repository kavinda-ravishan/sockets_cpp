#!/bin/bash

g++ ./client/main_udp_linux.cpp -o ./client/main_udp_linux.out
g++ ./server/main_udp_linux.cpp -o ./server/main_udp_linux.out
./server/main_udp_linux.out &
./client/main_udp_linux.out &