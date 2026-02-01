CVFS – Custom Virtual File System

CVFS (Custom Virtual File System) is a user-space, in-memory file system implemented in C, designed to demonstrate the internal working of a UNIX/Linux-like file system.
The project provides a custom command-line shell through which users can perform fundamental file operations and observe how a file system manages files internally.

This project does not interact with the host operating system’s real file system and is intended purely for learning and academic purposes.

Key Features

Custom interactive shell (CVFS >)

File creation with permission control (Read / Write / Read + Write)

Read and write operations using file descriptors

File deletion (unlink)

File metadata display (stat)

Directory listing (ls)

Command help and manual pages (help, man)

Structured error handling using predefined error codes

File System Architecture

CVFS simulates the following core file system components:

Boot Block – Stores basic boot and system information

Super Block – Maintains total and free inode count

Inode List (DILB) – Linked list representing all inodes

File Table – Maintains file offsets and access modes

UFDT (User File Descriptor Table) – Tracks opened files per process

Supported Commands
creat <filename> <permission>
read <fd> <size>
write <fd>
unlink <filename>
stat <filename>
ls
man <command>
clear
exit

Permission Values

1 → Read

2 → Write

3 → Read + Write

Example Usage
CVFS > creat demo.txt 3
CVFS > write 0
Hello World
CVFS > read 0 5
CVFS > stat demo.txt
CVFS > ls
CVFS > unlink demo.txt

Technologies Used

Programming Language: C

Platform: Linux

Core Concepts:

Operating Systems

File System Design

Inodes and File Descriptors

Memory Management

Important Notes

CVFS is a virtual (in-memory) file system

No real files or directories are created on disk

Designed for conceptual clarity, not production use

Learning Outcomes

This project helped in understanding:

Internal design of inode-based file systems

File descriptor and permission handling

Memory allocation and cleanup in C

Error handling in system-level programs

Designing low-level systems with structured components

Author

Prabhanjan Sanjay Ingle
