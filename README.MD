# Super Simple Heap Manager in C
Designed to be use in the absence of a OS or a baremetal system.
Ideally, a Board Support Package would contain the RAM information
or maybe a linker script to define in the nessesary memory map.
Still needs a c runtime environment, but otherwise, it works :)

## Cost
* A static data location is needed for the pointer to the free_list

## Testing
The included makefile shows how to build for none baremetal OSes
Just define DEBUG and the platform's stdlib will take care
of the initial malloc of HEAP (instead of directly modifying RAM)

## Design
The heap manager is effectivly a simple linked list of blocks.
The linked list itself will keep track of free blocks. Malloc
traverses the free_list until it finds a block that could be split
or used immediately. The blocks themselves are also stored in the
heap, directly prior to the data.

## Limitations
The heap really doesn't have a way to defrag the allocations or 
restructure the heap. Will be added!
