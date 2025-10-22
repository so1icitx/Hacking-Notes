# Inodes in linux 

## Overview
Inodes are a fundamental concept in Unix-like file systems (e.g., Linux, macOS), representing the metadata about files and directories.

## What is an Inode?
An inode (index node) stores metadata about a file or directory, excluding the file name and content. Metadata includes:
- **File Type**: Regular file, directory, symbolic link, etc.
- **Permissions**: Read, write, execute permissions for owner, group, and others.
- **Owner and Group**: User ID (UID) and Group ID (GID).
- **Size**: File size in bytes.
- **Timestamps**: Access time (`atime`), modification time (`mtime`), change time (`ctime`), and sometimes birth time.
- **Link Count**: Number of hard links to the inode.
- **Block Pointers**: Locations of data blocks on disk where content is stored.
- **Inode Number**: A unique identifier for the inode within a file system.

**Key Point**: The inode does *not* store the file name or content. File names are stored in directories, and content is stored in data blocks referenced by the inode.

### Why Inodes Are Confusing
Inodes seem simple, metadata about files, but their complexity arises from:
- **File Name Separation**: File names are stored in directories, not inodes, enabling multiple names (hard links) for the same inode.
- **System-Dependent Variations**: Different file systems (e.g., ext4, ZFS) implement inodes differently (e.g., ZFS uses "dnodes").
- **Interactions with Links**: Hard links and symbolic links create complex relationships.
- **Kernel Abstractions**: Inodes interact with file descriptor tables, open file tables, and inode tables, which can be overwhelming.

## Inode Structure and System Calls
Inodes are accessed via system calls like `stat`, which retrieves metadata into a `struct stat` buffer. Here’s an example in C:

```c
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    struct stat buf;
    if (stat("file.ext", &buf) == 0) {
        printf("Inode: %llu\n", (unsigned long long)buf.st_ino);
        printf("Size: %lld bytes\n", (long long)buf.st_size);
        printf("Links: %u\n", (unsigned)buf.st_nlink);
        printf("Permissions: %o\n", buf.st_mode & 0777);
        printf("UID: %u, GID: %u\n", buf.st_uid, buf.st_gid);
    } else {
        perror("stat");
    }
    return 0;
}
```

- **Output Example** (for `file.ext` with content "foo"):
  ```
  Inode: 123456
  Size: 4 bytes
  Links: 1
  Permissions: 755
  UID: 501, GID: 20
  ```
- **Explanation**:
  - `stat("file.ext", &buf)` fills `buf` with inode metadata.
  - `st_ino`: Inode number.
  - `st_size`: File size (4 bytes for "foo" plus newline).
  - `st_nlink`: Number of hard links.
  - `st_mode`: Permissions and file type.
  - `st_uid`, `st_gid`: Owner and group IDs.

### Accessing Inodes in Other Languages
- **Python**:
  ```python
  import os
  stats = os.stat("file.ext")
  print(f"Inode: {stats.st_ino}")
  print(f"Size: {stats.st_size} bytes")
  print(f"Links: {stats.st_nlink}")
  ```
- **Node.js**:
  ```javascript
  const fs = require("fs");
  const stats = fs.statSync("file.ext");
  console.log(`Inode: ${stats.ino}`);
  console.log(`Size: ${stats.size} bytes`);
  console.log(`Links: ${stats.nlink}`);
  ```

These languages access the same `stat` system call, retrieving inode metadata.

## File System Components
To understand inodes, you need to know how they interact with kernel structures:

1. **File Descriptor Table** (per-process):
   - Maps file descriptors (integers) to open file table entries.
   - Standard descriptors: 0 (stdin), 1 (stdout), 2 (stderr).
   - Each process has its own table, so file descriptor 3 in Process A differs from 3 in Process B.

2. **Open File Table** (system-wide):
   - Tracks open files, including:
     - **File Position (Offset)**: Current read/write position.
     - **Access Mode**: Read, write, or both (e.g., `O_WRONLY`, `O_RDWR`).
     - **Reference Count**: Number of file descriptors pointing to this entry.
     - **Inode Pointer**: Links to the inode table entry.
   - Shared across processes; multiple file descriptors can point to the same entry.

3. **Inode Table** (system-wide):
   - Manages inodes for active files, including:
     - Metadata (type, permissions, size, etc.).
     - Inode state (locked, dirty, in use).
     - Reference count (number of processes referencing the inode).
   - Differs from the **inode array** on disk, which is a fixed-size structure storing all inodes persistently.

4. **Inode Array** (on disk):
   - Part of the file system, storing metadata for all files and directories.
   - Each entry has a unique inode number.
   - Directory entries map file names to inode numbers.


## Disclaimer
Inodes vary across file systems (e.g., ext4, ZFS). POSIX defines the inode number (`st_ino`) but not the inode structure itself, leading to implementation differences. Always test code and check file system specifics.
