
# `/proc` 

## 1. Overview: The Matrix of Linux
The `/proc` directory is not a standard directory saved to your hard drive. It is a **Virtual Filesystem** (also known as a pseudo-filesystem) generated on the fly by the Linux kernel,.
*   **Purpose:** It acts as an interface between the kernel and user space. It allows you to view the state of the kernel and running processes in real-time,.
*   **Storage:** The files within `/proc` exist **only in RAM**. They take up zero space on the disk.
*   **Dynamic Nature:** If you list the files (using `ls -l`), most will show a file size of **0 bytes**. This is because the content is generated dynamically at the exact moment you read the file,,.
*   **Persistence:** Since it lives in memory, the contents of `/proc` disappear when the system is powered down.

**Key Concept:** When you run tools like `ps`, `top`, `htop`, `free`, or `mount`, they are actually just parsing text files inside `/proc` and formatting them for you,.

---

## 2. Directory Structure
The root of `/proc` contains two main types of entries:
1.  **Numbered Directories:** Each number corresponds to a **Process ID (PID)** of a currently running process,.
2.  **System Files:** Text files that provide hardware and kernel configuration information.

### A. The Process Directories (`/proc/[PID]/`)
Every running process has its own directory named after its PID (e.g., `/proc/1/` for the `init` or `systemd` process),. These directories are created when a process starts and vanish instantly when the process is killed.

**Critical Files Inside Each PID Directory:**

*   **`cmdline`**
    *   **What it is:** The complete command string used to start the process, including arguments,.
    *   **Format:** Arguments are separated by null bytes (not spaces), so it may look like one long string or have no spaces when viewed with `cat`.
    *   **Tip:** Use `xxd` or `tr` to view the null delimiters properly. This is a primary place to look for passwords or flags passed as arguments.
*   **`cwd`** (Link)
    *   **What it is:** A symbolic link to the **Current Working Directory** of the process,.
    *   **Usage:** Tells you exactly where in the filesystem the process is operating from. Useful for finding hidden directories used by malware.
*   **`exe`** (Link)
    *   **What it is:** A symbolic link to the actual binary executable on the disk.
    *   **Forensic Magic:** If a malicious actor starts a program and then deletes the binary file to hide their tracks, this link remains active. You can sometimes recover the deleted binary from here because it is still loaded in memory.
*   **`environ`**
    *   **What it is:** Contains the environment variables set for the process at runtime (e.g., `SHELL`, `PATH`, `USER`),.
    *   **Format:** Binary/Null-delimited file.
    *   **Usage:** Hackers check this for sensitive data like API keys or passwords stored in environment variables.
*   **`fd/`** (Directory)
    *   **What it is:** Contains the **File Descriptors**. These are symbolic links to all files the process has open.
    *   **Standard Descriptors:**
        *   `0`: Standard Input (stdin)
        *   `1`: Standard Output (stdout)
        *   `2`: Standard Error (stderr)
    *   **Usage:** You can see exactly where a process is writing its output or errors (e.g., to `/dev/null` or a specific log file). Useful for tracking malware activity.
*   **`maps`**
    *   **What it is:** Displays the memory regions mapped by the process, including executable code, libraries, and stack/heap,.
    *   **Usage:** Crucial for exploit development (buffer overflows) to identify writable and executable memory segments.
*   **`io`**
    *   **What it is:** Statistics on the Input/Output usage of the process.
    *   **Usage:** Helps identify which processes are hammering your disk, even if they aren't using much CPU.
*   **`statm`**
    *   **What it is:** Memory status information.
    *   **Format:** Raw numbers meant to be parsed by tools, not humans.

---

### B. System Information Files (Root of `/proc/`)
These files provide a global view of the Linux system.

*   **`cpuinfo`**
    *   **Details:** Processor type, model, speed, core count, and flags (features),.
    *   **Usage:** Verify if you are in a Virtual Machine (e.g., seeing "QEMU Virtual CPU").
*   **`meminfo`**
    *   **Details:** Total RAM, free memory, buffers, and cache.
    *   **Usage:** This is exactly where the `free` command gets its data.
*   **`mounts`**
    *   **Details:** Lists all currently mounted filesystems, their mount points, and types,.
    *   **Usage:** Similar to the `mount` or `df` commands. Hackers check this for remote storage (NFS shares) that might contain sensitive data.
*   **`partitions`**
    *   **Details:** Block sizes and names of major and minor partitions (e.g., `sda`, `sr0`).
*   **`version`**
    *   **Details:** The exact Linux kernel version, GCC compiler version, and build date.
    *   **Usage:** Used by attackers to find specific kernel vulnerabilities (exploits) for that exact version.
*   **`devices`**
    *   **Details:** Lists available character and block devices.
*   **`mdstat`**
    *   **Details:** Current status of RAID (Redundant Array of Independent Disks) configurations.

---

## 3. The `/proc/net/` Subdirectory
This directory is a goldmine for network reconnaissance without using standard tools like `netstat` or `lsof`.

*   **`tcp`**
    *   **Contents:** A list of open TCP sockets.
    *   **Usage:** You can identify open ports here even if `netstat` is not installed.
    *   **decoding:** The ports are often displayed in **Hexadecimal**. You must convert them to decimal to read the port number (e.g., a hex value corresponds to port 9176).
*   **`arp`**
    *   **Contents:** The ARP table (Address Resolution Protocol).
    *   **Usage:** Shows the MAC addresses and IPs of other devices on the local network. Useful for lateral movement and identifying targets for Man-in-the-Middle attacks.
*   **`dev`**
    *   **Contents:** Network interface statistics (packets sent/received, errors).
    *   **Usage:** Can reveal if the system is running containers (e.g., spotting a `docker0` bridge interface).

---

