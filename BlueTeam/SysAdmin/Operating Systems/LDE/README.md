# Understanding Limited Direct Execution in Operating Systems

## What is Limited Direct Execution (LDE)?
**Limited Direct Execution (LDE)** is a technique where the OS runs user processes directly on the CPU to maximize performance, but imposes restrictions to maintain control. It addresses two main challenges in CPU virtualization:
1. **Performance**: Minimize overhead when switching between processes.
2. **Control**: Ensure the OS can regain control from user processes to enforce security, protection, and scheduling.

### CPU Virtualization 
CPU virtualization allows multiple processes to share a physical CPU through **time sharing** and **multiprogramming**:
- **Multiprogramming**: Multiple processes reside in memory simultaneously.
- **Time Sharing**: The CPU switches between processes, creating the illusion of concurrent execution.


### Memory and CPU Setup
Textual diagram of memory and CPU:
```
[Memory]                        [CPU]
+---------------------+         +-------+
| OS Kernel           |         |       |
| Process 1 (Job 1)   |         |  P1   |
| Process 2 (Job 2)   |         |       |
| Process 3 (Job 3)   |         |       |
+---------------------+         +-------+
```
- **Multiprogramming**: Jobs 1, 2, and 3 are loaded into memory.
- **Time Sharing**: The OS selects a process (e.g., P1) to run on the CPU, while others wait in memory.

## Direct Execution Protocol
In **direct execution**, the OS runs a process directly on the CPU without restrictions, but this lacks control. The steps are:
1. Create a process list.
2. Allocate memory for the process.
3. Load the program into memory.
4. Set up the stack with `argc` and `argv` (command-line arguments).
5. Clear registers.
6. Jump to the program’s `main` function.
7. After `main` returns, free memory and remove the process from the list.

### Timeline
```
Time: | OS | Load P1 | Run P1 | P1 Returns | OS | Free P1 |
CPU:  | OS |        | P1     |           | OS |         |
```
- **Issue**: If the process doesn’t return control (e.g., infinite loop), the OS becomes a mere library, unable to manage other processes.

## Limited Direct Execution (LDE)
LDE enhances direct execution by introducing:
1. **User and Kernel Modes**: Separate execution modes to restrict process actions.
2. **System Calls**: Allow processes to request privileged operations from the kernel.
3. **Timer Interrupts**: Enable the OS to regain control periodically.
4. **Context Switching**: Switch between processes efficiently.

### User and Kernel Modes
- **User Mode (Ring 3)**: Processes run with limited privileges, unable to access hardware directly.
- **Kernel Mode (Ring 0)**: The OS kernel runs with full hardware access.
- **Device Drivers**: Run in Rings 1 or 2 (less privileged than the kernel but more than user processes).

Textual diagram of privilege levels (x86):
```
[Ring 0: Kernel]
  [Ring 1: Device Drivers]
    [Ring 2: Device Drivers]
      [Ring 3: User Processes]
```
- **Control Registers**: Set privilege levels (e.g., x86’s `CR0` or `CR3`).

### System Calls
System calls allow user processes to request privileged operations (e.g., I/O, process creation) from the kernel. They use **trap instructions** to switch from user mode to kernel mode.

#### Trap Instructions
- **Trap**: Jumps to the kernel, raises privilege to kernel mode (e.g., `syscall` in x86_64, `int 0x80` in x86 32-bit).
- **Return-from-Trap**: Returns to the user process, lowers privilege to user mode (e.g., `sysret` in x86_64, `iret` in x86).

#### System Call Example: `write`
```c
#include <unistd.h>
#include <stdlib.h>

int main() {
    char *buf = "Hello, world!\n";
    write(1, buf, 14); // Write to stdout (fd=1)
    exit(0);
}
```

Assembly equivalent:
```nasm
section .data
    msg db 'Hello, world!', 0xa ; newline
    len equ $ - msg          ; length=14

section .text
    global _start
_start:
    mov rax, 1    ; sys_write service number
    mov rdi, 1    ; fd=1 (stdout)
    mov rsi, msg  ; buffer
    mov rdx, len  ; length
    syscall       ; Trap to kernel
    mov rax, 60   ; sys_exit service number
    xor rdi, rdi  ; status=0
    syscall       ; Trap to kernel
```

- **Explanation**:
  - `syscall` traps to the kernel, passing the service number (`1` for `write`) in `rax`.
  - Parameters: `rdi` (file descriptor), `rsi` (buffer), `rdx` (length).
  - The kernel writes to `stdout` and returns control via `sysret`.


### Trap Table (Interrupt Descriptor Table, IDT)
The **trap table** (or IDT in x86 protected mode) maps interrupt numbers to **trap handlers** (interrupt service routines, ISRs). The kernel sets it up at boot time.

#### Setup
- **Boot Time**: The kernel, running in Ring 0, initializes the IDT with addresses of trap handlers.
- **Interrupt Numbers**: Each number (e.g., `0x80` for system calls in x86 32-bit) points to a handler.
- **Protection**: User processes specify a service number (e.g., `1` for `write`), not a memory address, preventing unauthorized access.

#### Example: IDT Setup (Simplified)
```c
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
};

struct idt_entry idt[256]; // 256 interrupt vectors

void idt_set_gate(uint8_t num, uint64_t handler_addr) {
    idt[num].offset_low = handler_addr & 0xFFFF;
    idt[num].offset_high = (handler_addr >> 16) & 0xFFFF;
    idt[num].selector = 0x08; // Kernel code segment
    idt[num].type_attr = 0x8E; // Interrupt gate
    idt[num].zero = 0;
}
```

- **Explanation**: Sets an IDT entry for interrupt `num` to point to `handler_addr`.

#### Example: Custom Trap Handler
```c
#include <stdio.h>

void isr_handler(int num) {
    printf("Interrupt %d triggered\n", num);
}

int main() {
    asm volatile("int $3"); // Trigger interrupt 3
    asm volatile("int $4"); // Trigger interrupt 4
    return 0;
}
```

- **Output**:
  ```
  Interrupt 3 triggered
  Interrupt 4 triggered
  ```
- **Explanation**: Inline assembly triggers interrupts, invoking custom handlers if set in the IDT.

## Interrupts and Exceptions
Interrupts and exceptions allow the OS to handle events that require CPU attention:
- **Interrupts**: Asynchronous hardware events (e.g., I/O completion, timer interrupt).
- **Exceptions**: Synchronous errors or events:
  - **Traps**: Immediate, process continues (e.g., system calls, debugger breakpoints).
  - **Faults**: Handled with user intervention (e.g., page faults, division by zero).
  - **Aborts**: Unrecoverable errors (e.g., kernel panic, Windows blue screen).

### Example Events
- Division by zero: Triggers a fault.
- I/O completion: Signals the process via an interrupt.
- Page fault: Handled when accessing invalid memory.
- System call: Traps to the kernel (e.g., `write`).

## Timer Interrupts
Timer interrupts allow the OS to regain control periodically, preventing processes from monopolizing the CPU (e.g., in infinite loops).

### Programmable Interrupt Timer (PIT)
- **Setup**: The kernel configures the PIT at boot to interrupt every X milliseconds.
- **Operation**:
  - Timer counts down to zero.
  - Triggers an interrupt (e.g., IRQ 0 in x86).
  - Halts the current process, saves its state, and invokes the timer handler.

#### Example: Timer Handler
```c
#include <stdio.h>

void timer_callback() {
    printf("Timer interrupt: %d seconds elapsed\n", 10);
}

void init_timer(int freq) {
    // Register handler for IRQ 0
    register_interrupt_handler(0, timer_callback);
    // Set PIT frequency (simplified)
    outb(0x43, 0x36); // PIT command
    outb(0x40, freq & 0xFF); // Low byte
    outb(0x40, (freq >> 8) & 0xFF); // High byte
}

int main() {
    init_timer(100); // ~10ms intervals
    asm volatile("sti"); // Enable interrupts
    while (1); // Spin
}
```

- **Output** (every ~10 seconds):
  ```
  Timer interrupt: 10 seconds elapsed
  ```
- **Explanation**: The timer triggers IRQ 0, invoking `timer_callback`.

## Context Switching
**Context switching** is the mechanism to switch between processes, saving the state of the current process and restoring the state of the next process.

### Steps
1. Save registers (e.g., `rax`, `rip`, `rsp`) of the current process to its kernel stack.
2. Restore registers from the kernel stack of the next process.
3. Switch to the next process’s kernel stack.
4. Return to user mode, jumping to the restored program counter.

#### XV6 Context Switch Example
```nasm
# switch.S (XV6)
.text
.global switch
switch:
    # Save old context (struct context *old)
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    movl 20(%esp), %eax  # old context pointer
    movl %esp, (%eax)    # Save stack pointer
    # Load new context (struct context *new)
    movl 24(%esp), %eax  # new context pointer
    movl (%eax), %esp    # Restore stack pointer
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    ret
```

- **Explanation**:
  - Saves registers (`ebp`, `ebx`, `esi`, `edi`) to the old process’s kernel stack.
  - Switches to the new process’s kernel stack.
  - Restores registers for the new process.
  - Returns to the new process’s execution point.

### Timeline with Timer Interrupt
```
Time: | OS | Load P1 | Run P1 | Timer Interrupt | OS | Run P2 |
CPU:  | OS |        | P1     | OS             | OS | P2     |
```
- **Steps**:
  1. OS initializes trap table and timer.
  2. Loads and runs Process 1 (P1).
  3. Timer interrupt triggers, saving P1’s state to its kernel stack.
  4. OS runs, scheduler selects Process 2 (P2).
  5. Restores P2’s state from its kernel stack.
  6. P2 runs in user mode.

## Handling Concurrent Interrupts
During interrupt handling, another interrupt may occur, risking inconsistencies. Solutions:
- **Disable Interrupts**: Use `cli` (clear interrupts) and `sti` (set interrupts) in x86.
  ```nasm
  cli  ; Disable interrupts
  ; Critical section
  sti  ; Enable interrupts
  ```
- **Advanced Synchronization**: Use semaphores or mutexes (covered in later chapters).

## Cooperative vs. Non-Cooperative Approach
- **Cooperative**: The OS waits for processes to yield control via system calls (e.g., `yield`). Vulnerable to infinite loops.
  ```c
  #include <unistd.h>
  int main() {
      while (1); // Infinite loop, no yield
      return 0;
  }
  ```
- **Non-Cooperative**: Timer interrupts force the OS to regain control, ensuring fairness.

## Data Structures
- **Process Control Block (PCB)**: Stores process metadata (e.g., PID, state, kernel stack pointer). Managed by the OS.
- **Kernel Stack**: Per-process stack for saving registers during traps. Managed by hardware (e.g., x86 Task State Segment).
- **Trap Table (IDT)**: Maps interrupt numbers to handlers.


