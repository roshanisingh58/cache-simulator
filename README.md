**Project Overview**

This project implements a Cache Simulator in C++.
It simulates a 256-set, 4-way set-associative cache with a 64-byte block size.
The simulator processes 32-bit memory addresses by dividing them into:

- Tag

 - Index

 - Offset

The goal is to simulate cache behavior including hits, misses, and block replacement without storing actual data.

**Features**

- 4-way Set Associative Cache

- LRU (Least Recently Used) Replacement Policy

- Parallel Tag Comparison

- Miss handling using MSHR (Miss Status Holding Register)



The simulator reads memory access traces from:

tracefile.txt

dram_tracefile.txt

Each line represents a memory access address used to simulate cache operations.
