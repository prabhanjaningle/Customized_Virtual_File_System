CVFS (Custom Virtual File System)

CVFS is a user-space virtual file system implemented in C, designed to simulate the internal working of a UNIX/Linux file system.
The project provides a custom shell interface that allows users to perform basic file operations such as creating, reading, writing, deleting files, and viewing file metadata.

All file operations are performed in memory, without interacting with the host operating system’s actual file system.

Features

Custom command-line shell (CVFS >)

File creation with permissions (Read / Write / Read + Write)

Read and write operations using file descriptors

File deletion (unlink)

File metadata display (stat)

Directory listing (ls)

Manual pages for supported commands (man)

Error handling using predefined error codes

File System Components

Boot Block – Stores boot-related information

Super Block – Maintains total and free inode count

Inode List (DILB) – Linked list of inodes

File Table – Tracks file offsets and access modes

UFDT (User File Descriptor Table) – Manages open files

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

Language: C

Platform: Linux

Concepts: Operating Systems, File Systems, Memory Management

Notes

This is a virtual (in-memory) file system

No real files are created on the disk

Intended for learning and academic purposes

Learning Outcome

This project helps in understanding:

Inode-based file system architecture

File descriptor and permission handling

Memory management in system programming

Error handling in C

Author

Prabhanjan Sanjay Ingle
