
# License Key Exploit: Reverse Engineering with GDB

[![C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![gdb](https://img.shields.io/badge/tool-gdb-green.svg)](https://www.gnu.org/software/gdb/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Welcome to my first reverse engineering project! I'm **so1icitx**, and this repository showcases a logical vulnerability in a C program (`vulnerable_license.c`) that checks a license key. Using `gdb` with `pwndbg`, I analyzed the program, uncovered a hidden flaw, and crafted an exploit to bypass the license check without the correct key (`AAAA-KEY12345`). This project is purely educational, demonstrating binary analysis and exploit development for learning purposes.

## Table of Contents
- [Project Overview](#project-overview)
- [Prerequisites](#prerequisites)
- [Setup and Compilation](#setup-and-compilation)
- [Vulnerability Analysis with GDB](#vulnerability-analysis-with-gdb)
- [Exploiting the Vulnerability](#exploiting-the-vulnerability)
- [Lessons Learned](#lessons-learned)
- [Responsible Use](#responsible-use)
- [Contributing](#contributing)

## Project Overview
The `vulnerable_license.c` program validates a license key, expecting `AAAA-KEY12345`. A logical flaw in the validation logic allows unauthorized access. I used `gdb` to:
- Dissect the `check_license` function’s assembly.
- Manipulate registers and memory to bypass checks.
- Discover a simpler input-based exploit.

This project highlights my journey into binary exploitation, from debugging to crafting a working exploit, making it a great starting point for anyone interested in CTF challenges or vulnerability research.

## Prerequisites
- **Environment**: Linux (e.g., Ubuntu in a VM like VirtualBox)
- **Tools**:
  - `gcc`: `sudo apt install build-essential`
  - `gdb`: `sudo apt install gdb`
  - `pwndbg` (optional, for enhanced debugging): Follow [pwndbg installation](https://github.com/pwndbg/pwndbg)
- **Skills**: Basic C, Linux terminal, and `gdb` commands (e.g., `break`, `run`, `print`).

## Setup and Compilation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/so1icitx/license-key-exploit.git
   cd license-key-exploit
   ```
2. **Compile the Program**:
   ```bash
   gcc -o license vulnerable_license.c
   ```
   Creates the `license` binary.

3. **Test the Program**:
   - Correct key:
     ```bash
     ./license AAAA-KEY12345
     ```
     Output: `Access granted! Welcome to the system.`
   - Incorrect key:
     ```bash
     ./license AAAA-KEY99999
     ```
     Output: `Invalid license key. Access denied.`

## Vulnerability Analysis with GDB
I analyzed the `check_license` function using `gdb` with `pwndbg` to uncover the vulnerability. My process:
- **Started Debugging**:
  ```bash
  gdb ./license
  break check_license
  run AAAA-@##!
  ```
- **Key Breakpoints**:
  - `check_license+109` (`0x5555555551c6`): Length check (`cmp rbx, rax`).
  - `check_license+208` (`0x555555555229`): Validation check (`cmp dword ptr [rbp - 0x14], 0x2f3`).
- **Inspected State**:
  - `rbx`: Input length (e.g., 9 for `AAAA-@##!`).
  - `sum` (`[rbp - 0x14]`): Sum of ASCII values.
  - `special` (`[rbp - 0x1c]`): Flag for `@` character.
  - Used `print sum`, `print special`, `x/s $rdi` to monitor values.
- **Manipulated Execution**:
  - Set `rbx = 13` to pass the length check (9 ≠ 13).
  - Set `sum = 0x2f3` (755, the ASCII sum of `AAAA-KEY12345`) to bypass validation.

This revealed a flaw in the validation logic, allowing access via memory manipulation or a crafted input.

## Exploiting the Vulnerability
### GDB-Based Exploit
I initially bypassed the check by altering program state:
- **Bypass Length Check**:
  At `check_license+109`, `rbx` (input length) was 9, but the correct key’s length is 13. I set:
  ```gdb
  set $rbx = 13
  ```
- **Force Validation**:
  At `check_license+208`, `sum` (`[rbp - 0x14]`) was incorrect. I set:
  ```gdb
  set *(int *)($rbp - 0x14) = 0x2f3
  ```
- **Result**: `Access granted!` because `sum == 0x2f3`.

### Input-Based Exploit
Static analysis revealed a simpler exploit:
- The program grants access if `sum == 0x2f3` **or** the input contains an `@` (sets `special = 1`).
- Any 13-character input with `@` works.

**Exploit Command**:
```bash
./license AAAAAAAAA@AAA
```
**Output**:
```
Checking license key: AAAAAAAAA@AAA
Access granted! Welcome to the system.
```

**Verification in GDB**:
```bash
gdb ./license
break check_license
run AAAAAAAAA@AAA
next
print special
```
When `@` is checked (`cmp al, 0x40`), `special` becomes 1, bypassing the check.

### Why It Works
- The input has 13 characters, passing the length check.
- The `@` character triggers `special = 1`, satisfying the validation condition (`sum == 0x2f3 || special`).

## Lessons Learned
As a beginner in binary exploitation, I learned:
- **GDB Proficiency**: Setting breakpoints, inspecting registers, and modifying memory (`set $rbx`, `set *(int *)`).
- **Assembly Basics**: Understanding comparisons (`cmp`) and jumps (`jne`, `je`) in `check_license`.
- **Logical Vulnerabilities**: How flawed conditions (e.g., `|| special`) create bypasses.
- **Static Analysis**: Crafting inputs like `AAAAAAAAA@AAA` without `gdb` is faster but requires code review.
- **Patience Pays Off**: Debugging took time, but persistence revealed the `@` bypass.

## Responsible Use
⚠️ **Educational Use Only**: This code is intentionally vulnerable for learning. Do **not** use it to harm systems or networks you don’t own. Unauthorized testing may violate laws (e.g., CFAA in the U.S.).

- **Safe Testing**: Use a virtual machine (e.g., Ubuntu in VirtualBox).
- **Ethical Hacking**: Explore certifications like OSCP or try CTF platforms (OverTheWire, Pwnable.kr).
- **Responsible Disclosure**: Report real-world vulnerabilities to vendors or via platforms like HackerOne.

