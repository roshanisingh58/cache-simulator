My code and aprroach involves designing and implementing a cache simulator in C++ with the following specifications: 
a cache with 256 sets, 4-way set-associative, and a 64-byte line/block size. 
The simulator is built to handle 32-bit physical/memory addresses by dividing them into three parts: 
Tag, Index, and Offset, with 18, 8, and 6 bits respectively. 
The primary focus of the project is to simulate the cache structure without storing actual data, ensuring efficient tag comparison, block access, and cache miss handling using the Miss Status Holding Register (MSHR).

The code supports following features:-
- Tag comparison and block access in the cache happened in parallel for all the blocks of the selected index.
- LRU replacement policy for cache block replacement.
- Cache miss under a cache miss using Miss Status Holding Register (MSHR). 
