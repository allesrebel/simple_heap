# Super Simple Heap Manager in C
Designed to be use in the absence of a OS or a baremetal system.
Ideally, a Board Support Package would contain the RAM information
or maybe a linker script to define in the nessesary memory map.
Still needs a c runtime environment, but otherwise, it works :)

## Testing
The included makefile shows how to build for none baremetal OSes
Just define DEBUG and the platform's stdlib will take care
of the initial malloc of HEAP (instead of directly modifying RAM)
