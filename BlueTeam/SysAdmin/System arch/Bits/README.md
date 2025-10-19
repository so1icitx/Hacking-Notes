# Bitwise Operations in C

## Overview
Bitwise operations allow programmers to manipulate individual bits within data, offering memory-efficient solutions for tasks like setting flags, storing compact numerical values, and treating bit sequences as arrays.

## Why Bitwise Operations?
Computers store data in binary (1s and 0s), but programmers typically work with higher-level abstractions (e.g., integers). Bitwise operations let you interact directly with bits, which is useful for:
- **Space Efficiency**: Packing multiple options or small values into fewer bytes (e.g., flags in a single integer).
- **Performance**: Fast bit-level checks for settings or conditions.
- **Applications**: Flag systems (e.g., file open modes), compact data storage (e.g., grades), and bit arrays (e.g., network protocols).

Example: An `int` (32 bits) is treated as a single value, but bitwise operations let you manipulate its individual bits for specific meanings.

## Bitwise Operators
Bitwise operators work on binary representations of numbers, bit by bit. Below are the five key operators discussed, with examples using:
```c
unsigned int x = 6;  // Binary: 00000110
unsigned int y = 12; // Binary: 00001100
unsigned int z = 0;  // Binary: 00000000
```

### 1. Bitwise AND (&)
- **Operation**: Sets a bit to 1 if both corresponding bits are 1.
- **Truth Table**:
  ```
  0 & 0 = 0
  0 & 1 = 0
  1 & 0 = 0
  1 & 1 = 1
  ```
- **Example**:
  ```c
  #include <stdio.h>
  int main() {
      unsigned int x = 6;  // 00000110
      unsigned int y = 12; // 00001100
      unsigned int z = x & y; // 00000100 (4)
      printf("AND: %u\n", z); // Output: 4
      return 0;
  }
  ```
  - **Explanation**: Only the third bit (from right) is 1 in both `x` and `y`, so `z` = 4 (binary `00000100`).

### 2. Bitwise OR (|)
- **Operation**: Sets a bit to 1 if at least one corresponding bit is 1.
- **Truth Table**:
  ```
  0 | 0 = 0
  0 | 1 = 1
  1 | 0 = 1
  1 | 1 = 1
  ```
- **Example**:
  ```c
  z = x | y; // 00001110 (14)
  printf("OR: %u\n", z); // Output: 14
  ```
  - **Explanation**: Bits are 1 where either `x` or `y` has a 1, resulting in `14` (binary `00001110`).

### 3. Bitwise XOR (^)
- **Operation**: Sets a bit to 1 if exactly one corresponding bit is 1 (exclusive OR).
- **Truth Table**:
  ```
  0 ^ 0 = 0
  0 ^ 1 = 1
  1 ^ 0 = 1
  1 ^ 1 = 0
  ```
- **Example**:
  ```c
  z = x ^ y; // 00001010 (10)
  printf("XOR: %u\n", z); // Output: 10
  ```
  - **Explanation**: Bits are 1 where `x` and `y` differ, resulting in `10` (binary `00001010`).

### 4. Left Shift (<<)
- **Operation**: Shifts bits left, filling with 0s on the right. Each shift doubles the value (if no overflow).
- **Example**:
  ```c
  z = x << 1; // 00001100 (12)
  printf("Left Shift: %u\n", z); // Output: 12
  z = x << 2; // 00011000 (24)
  printf("Left Shift x2: %u\n", z); // Output: 24
  ```
  - **Explanation**: `6 << 1` = `12` (shifts `00000110` to `00001100`). `6 << 2` = `24` (shifts two positions).

### 5. Right Shift (>>)
- **Operation**: Shifts bits right, truncating bits on the right. Each shift halves the value (integer division).
- **Example**:
  ```c
  z = x >> 1; // 00000011 (3)
  printf("Right Shift: %u\n", z); // Output: 3
  z = x >> 2; // 00000001 (1)
  printf("Right Shift x2: %u\n", z); // Output: 1
  ```
  - **Explanation**: `6 >> 1` = `3` (shifts `00000110` to `00000011`). `6 >> 2` = `1` (shifts two positions, truncating).

### 6. Bitwise NOT (~) 
- **Operation**: Flips all bits (0 to 1, 1 to 0).
- **Example**:
  ```c
  unsigned int a = 5; // 00000101
  z = ~a; // 11111010 (large number or negative in signed int)
  printf("NOT: %u\n", z); // Output: Large number (e.g., 4294967290 for unsigned)
  ```
  - **Note**: For a 32-bit `int`, `~5` flips all 32 bits, often resulting in a large or negative number due to sign extension.

## Bit Fields
Bit fields are sequences of bits where individual bits or groups represent specific meanings. They are used for:
1. **Flags/Options**: Packing multiple boolean options into one integer (e.g., file open modes).
2. **Compact Numbers**: Storing small values in fewer bits (e.g., grades from 0-12).
3. **Bit Arrays**: Treating bits as an array of 1s and 0s (e.g., for network data).

### Scenario 1: Flags/Options
Bit fields are ideal for passing multiple options to functions, like the `open` function's flags:
```c
open("file.txt", O_RDONLY | O_CREAT); // Combines read-only and create flags
```
- Each flag (e.g., `O_RDONLY`) is a bit position set to 1, combined using `|`.

**Example: Logging Function with Flags**
Create a logging function where users choose what to include (time, date, username, counter) using a bit field.

```c
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define LOG_TIME    0b00000001 // Bit 0: Include time
#define LOG_DATE    0b00000010 // Bit 1: Include date
#define LOG_USER    0b00000100 // Bit 2: Include user ID
#define LOG_COUNT   0b00001000 // Bit 3: Include counter
#define LOG_ALL     (LOG_TIME | LOG_DATE | LOG_USER | LOG_COUNT) // All options
#define LOG_DATE_TIME (LOG_DATE | LOG_TIME) // Date and time

void log_message(const char *message, uint8_t options, int counter) {
    if (options & LOG_TIME) {
        time_t now = time(NULL);
        printf("Time: %s", ctime(&now));
    }
    if (options & LOG_DATE) {
        time_t now = time(NULL);
        printf("Date: %s", ctime(&now));
    }
    if (options & LOG_USER) {
        printf("User: test_user\n");
    }
    if (options & LOG_COUNT) {
        printf("Counter: %d\n", counter);
    }
    printf("Message: %s\n", message);
}

int main() {
    log_message("No options", 0, 1); // No flags
    log_message("User and Date", LOG_USER | LOG_DATE, 2);
    log_message("Time, User, Count", LOG_TIME | LOG_USER | LOG_COUNT, 3);
    log_message("Count only", LOG_COUNT, 4);
    log_message("All options", LOG_ALL, 5);
    log_message("Date and Time", LOG_DATE_TIME, 6);
    return 0;
}
```

- **Explanation**:
  - Uses `uint8_t` (8 bits) for up to 8 flags.
  - Macros define bit positions (e.g., `LOG_TIME = 0b00000001`).
  - `&` checks if a bit is set (e.g., `options & LOG_TIME`).
  - `|` combines flags (e.g., `LOG_USER | LOG_DATE`).
  - Output varies based on flags, saving space by packing options into one byte.

### Scenario 2: Compact Numbers
Use bit fields to store small numbers (e.g., school grades 0-12) in fewer bits than a full `int`.

```c
#include <stdio.h>

struct counters {
    signed int little : 2; // 2 bits (-2 to 1)
    signed int medium : 4; // 4 bits (-8 to 7)
    signed int large : 6;  // 6 bits (-32 to 31)
};

int main() {
    struct counters c = {0, 0, 0};
    printf("Size of struct: %zu bytes\n", sizeof(c)); // May be 4 bytes (aligned)

    for (int i = 0; i < 10; i++) {
        c.little++;
        c.medium++;
        c.large++;
        printf("Little: %d, Medium: %d, Large: %d\n", c.little, c.medium, c.large);
    }
    return 0;
}
```

- **Output (Signed)**:
  ```
  Size of struct: 4 bytes
  Little: 0, Medium: 1, Large: 1
  Little: 1, Medium: 2, Large: 2
  Little: -2, Medium: 3, Large: 3
  Little: -1, Medium: 4, Large: 4
  ...
  ```
  - **Note**: `little` wraps to negative due to 2-bit signed range (-2 to 1).

- **Unsigned Version**:
  ```c
  struct counters {
      unsigned int little : 2; // 0 to 3
      unsigned int medium : 4; // 0 to 15
      unsigned int large : 6;  // 0 to 63
  };
  ```
  - **Output**:
    ```
    Size of struct: 4 bytes
    Little: 0, Medium: 1, Large: 1
    Little: 1, Medium: 2, Large: 2
    Little: 2, Medium: 3, Large: 3
    Little: 3, Medium: 4, Large: 4
    Little: 0, Medium: 5, Large: 5
    ...
    ```
    - Wraps to 0 after max value (e.g., 3 for 2 bits).

- **Packing to Save Space**:
  - Default struct size may be 4 bytes due to alignment.
  - Use `#pragma pack(push, 1)` to pack tightly:
    ```c
    #pragma pack(push, 1)
    struct counters {
        unsigned int little : 2;
        unsigned int medium : 4;
        unsigned int large : 6;
    };
    #pragma pack(pop)
    ```
    - **Output**: `Size of struct: 2 bytes` (12 bits = 1.5 bytes, rounded to 2).

### Scenario 3: Bit Arrays
Treat an integer as an array of bits, manipulating individual bits with macros.

```c
#include <stdio.h>
#include <stdint.h>

#define SET_BIT(field, n)   ((field) |= (1ULL << (n)))
#define CLEAR_BIT(field, n) ((field) &= ~(1ULL << (n)))
#define IS_BIT_SET(field, n) (((field) >> (n)) & 1)

int main() {
    uint64_t bitfield = 0;
    SET_BIT(bitfield, 0);  // Set bit 0
    SET_BIT(bitfield, 3);  // Set bit 3
    SET_BIT(bitfield, 7);  // Set bit 7
    CLEAR_BIT(bitfield, 3); // Clear bit 3

    for (int i = 0; i < 64; i++) {
        printf("%c", IS_BIT_SET(bitfield, i) ? '+' : '.');
    }
    printf("\n");
    return 0;
}
```

- **Output**: `+......+........................................................`
  - `+` for set bits (0 and 7), `.` for unset bits.
- **Explanation**:
  - `SET_BIT`: Sets bit `n` to 1 using `|`.
  - `CLEAR_BIT`: Sets bit `n` to 0 using `&` and `~`.
  - `IS_BIT_SET`: Checks if bit `n` is 1 by shifting and masking.

## Flag System with Bitwise Operators
Bitwise operators enable a memory-efficient flag system, storing multiple boolean states in one variable.

**Problem**: Store states of multiple features (e.g., enabled/disabled) without using separate variables.

**Naive Approach**:
```c
int x = 1; // Feature 1 enabled
int y = 0; // Feature 2 disabled
if (x) printf("First feature enabled\n");
if (y) printf("Second feature enabled\n");
```
- **Issue**: Uses 8 bytes (4 per `int`) for two flags, wasting space.

**Bitwise Flag System**:
```c
#include <stdio.h>

#define FIRST_FLAG  0b1        // Bit 0
#define SECOND_FLAG 0b10       // Bit 1
#define THIRD_FLAG  0b10000000 // Bit 7 (128)

int main() {
    unsigned int flags = FIRST_FLAG | SECOND_FLAG | THIRD_FLAG; // Enable all three
    flags &= ~SECOND_FLAG; // Disable second flag
    flags |= SECOND_FLAG;  // Re-enable second flag

    if (flags & FIRST_FLAG)  printf("First feature enabled\n");
    if (flags & SECOND_FLAG) printf("Second feature enabled\n");
    if (flags & THIRD_FLAG)  printf("Third feature enabled\n");

    return 0;
}
```

- **Output**:
  ```
  First feature enabled
  Second feature enabled
  Third feature enabled
  ```
- **Explanation**:
  - **Set Flags**: `flags = FIRST_FLAG | SECOND_FLAG | THIRD_FLAG` (binary `10000011` = 131).
  - **Check Flags**: `flags & FIRST_FLAG` checks if bit 0 is 1.
  - **Disable Flag**: `flags &= ~SECOND_FLAG` clears bit 1.
  - **Enable Flag**: `flags |= SECOND_FLAG` sets bit 1.
  - **Efficiency**: Uses 4 bytes for up to 32 flags vs. 4 bytes per flag in the naive approach.

## Tools for Bitwise Analysis
- **Compiler**: GCC/Clang to test bitwise code.
- **Debugger**: GDB to inspect binary values.
- **Online Converters**: Binary-to-decimal tools for quick checks.

## Installation
- **GCC**: `sudo apt install gcc` (Linux) or download MinGW (Windows).
- **GDB**: `sudo apt install gdb` (Linux).


