# Hermit Newlib Port

This directory contains all the system dependent code for the Hermit Newlib port.

For the syscalls, see `syscall.h` in `include/hermit`. 

The port is based on some of Musl's libc files. The files are copied from the Musl libc 
repository and modified to work with Newlib. All original files are referenced in the header 
of the file.
