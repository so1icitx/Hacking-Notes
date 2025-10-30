# Understanding the Process API in Unix-like Operating Systems



## What is the Process API?
The **Process API** provides system calls that enable users to create, manage, and terminate processes, which are running programs. Processes virtualize the CPU, allowing multiple programs to run on limited hardware. The API abstracts low-level OS functionality, making it accessible to programmers.

### Key System Calls
1. **fork**: Creates a new process by duplicating the calling process.
2. **exec**: Replaces the current process image with a new program.
3. **wait**: Allows a parent process to wait for a child process to terminate.
4. **Others**: `kill` (send signals), `pipe` (inter-process communication), `getpid` (get process ID), `getuid` (get user ID).

## Fork System Call
The `fork` system call creates a new process (child) by duplicating the calling process (parent). The child inherits a copy of the parent’s address space, registers, and program counter, but they are distinct.

### Characteristics
- **Return Value**:
  - **< 0**: Fork failed (e.g., insufficient memory or system limits).
  - **0**: In the child process.
  - **> 0**: In the parent process, returns the child’s PID.
- **Child Process**: Starts execution at the instruction after `fork`, not at `main`.
- **Non-deterministic Execution**: The scheduler determines whether the parent or child runs first.

### Example: `p1.c`
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Hello, world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello, I am child (pid:%d)\n", (int)getpid());
    } else {
        printf("Hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    }
    return 0;
}
```

- **Output Example**:
  ```
  Hello, world (pid:4110)
  Hello, I am parent of 4111 (pid:4110)
  Hello, I am child (pid:4111)
  ```
- **Explanation**:
  - The parent process (e.g., PID 4110) prints “Hello, world” and calls `fork`.
  - `fork` creates a child (e.g., PID 4111).
  - The child executes the `rc == 0` block, printing its PID.
  - The parent executes the `rc > 0` block, printing the child’s PID.
  - Execution order (parent vs. child) is non-deterministic due to scheduling.

### Debugging with `gdb`
Compile with debug info:
```bash
gcc -g -o p1.elf p1.c
gdb p1.elf
(gdb) break main
(gdb) run
(gdb) next
# Steps to fork()
(gdb) next
# After fork(), gdb may detach from child
(gdb) info proc
# Shows parent PID (e.g., 5581)
cat /proc/5581/status
# State: t (tracing, in debugger)
ls /proc | grep 5582
# Child process (5582) may be zombie (Z state)
```

- **Zombie Process**: If the child finishes before the parent and the parent doesn’t call `wait`, the child becomes a zombie (state `Z`), retaining its process table entry until reaped.

## Wait System Call
The `wait` system call makes the parent process pause until a child process changes state (e.g., terminates). This prevents zombie processes by ensuring the parent reaps the child’s exit status.

### Example: `p2.c`
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Hello, world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello, I am child (pid:%d)\n", (int)getpid());
    } else {
        int wc = wait(NULL);
        printf("Hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int)getpid());
    }
    return 0;
}
```

- **Output Example**:
  ```
  Hello, world (pid:4120)
  Hello, I am child (pid:4121)
  Hello, I am parent of 4121 (wc:4121) (pid:4120)
  ```
- **Explanation**:
  - The parent calls `wait(NULL)`, pausing until the child (PID 4121) terminates.
  - The child prints and exits, allowing the parent to resume.
  - `wait` returns the child’s PID (4121), preventing a zombie state.
  - The child always finishes before the parent, making execution deterministic.

## Exec System Call
The `exec` family (e.g., `execve`) replaces the current process image with a new program, changing its code, data, and address space while keeping the same PID.

### Example: `p3.c`
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Hello, world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello, I am child (pid:%d)\n", (int)getpid());
        char *myargs[3];
        myargs[0] = "wc";           // Program: word count
        myargs[1] = "p3.c";         // Argument: file to count
        myargs[2] = NULL;           // End of arguments
        execvp(myargs[0], myargs); // Run wc
        printf("This shouldn't print\n");
    } else {
        int wc = wait(NULL);
        printf("Hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int)getpid());
    }
    return 0;
}
```

- **Output Example**:
  ```
  Hello, world (pid:4120)
  Hello, I am child (pid:4121)
  10 20 300 p3.c
  Hello, I am parent of 4121 (wc:4121) (pid:4120)
  ```
- **Explanation**:
  - The parent (PID 4120) forks a child (PID 4121).
  - The child calls `execvp` to run `wc p3.c`, replacing its image with `wc`.
  - `wc` outputs word count (e.g., 10 lines, 20 words, 300 bytes).
  - The `printf("This shouldn't print\n")` is not executed because `execvp` overwrites the child’s image.
  - The parent waits and resumes after `wc` finishes.

### Debugging with `gdb`
```bash
gcc -g -o p3.elf p3.c
gdb p3.elf
(gdb) break 16
(gdb) run
(gdb) next
# Steps to fork()
cat /proc/<child_pid>/status
# Shows wc.elf in zombie state (Z)
(gdb) continue
# Parent resumes after wc finishes
```

## I/O Redirection
Unix separates `fork` and `exec` to allow the parent to modify the child’s environment (e.g., file descriptors) before executing a new program. This enables **I/O redirection**, such as redirecting `stdout` to a file.

### Example: `p4.c`
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    printf("Hello, world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello, I am child (pid:%d)\n", (int)getpid());
        close(STDOUT_FILENO); // Close stdout (fd 1)
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, 0644); // New fd 1
        char *myargs[3];
        myargs[0] = "wc";
        myargs[1] = "p4.c";
        myargs[2] = NULL;
        execvp(myargs[0], myargs); // Run wc
    } else {
        int wc = wait(NULL);
        printf("Hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int)getpid());
    }
    return 0;
}
```

- **Output Example**:
  ```
  Hello, world (pid:4130)
  Hello, I am child (pid:4131)
  Hello, I am parent of 4131 (wc:4131) (pid:4130)
  ```
  - File `p4.output` contains:
    ```
    12 25 350 p4.c
    ```
- **Explanation**:
  - The child closes `STDOUT_FILENO` (fd 1).
  - `open("./p4.output", ...)` assigns fd 1 to `p4.output`.
  - `execvp` runs `wc p4.c`, and its output goes to `p4.output` instead of the screen.
  - This mimics shell redirection: `wc p4.c > p4.output`.

## Pipes
Pipes enable **inter-process communication (IPC)** by connecting the `stdout` of one process to the `stdin` of another. The `pipe` system call creates a unidirectional data channel.

### Example: Shell Pipe
```bash
ps aux | grep cpu.elf
```
- **Explanation**: The `stdout` of `ps aux` is piped to the `stdin` of `grep`, filtering processes.

### C Example: Pipe Implementation
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pipe(pipefd); // Create pipe: pipefd[0] = read, pipefd[1] = write
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // Child: grep
        close(pipefd[1]); // Close write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read
        close(pipefd[0]);
        execlp("grep", "grep", "cpu.elf", NULL);
    } else { // Parent: ps
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write
        close(pipefd[1]);
        execlp("ps", "ps", "aux", NULL);
    }
    return 0;
}
```

- **Explanation**:
  - `pipe(pipefd)` creates a pipe with read (`pipefd[0]`) and write (`pipefd[1]`) ends.
  - The child redirects `stdin` to the pipe’s read end and runs `grep`.
  - The parent redirects `stdout` to the pipe’s write end and runs `ps`.
  - Mimics `ps aux | grep cpu.elf`.

## Signals and Process Control
The `kill` system call sends signals to processes or process groups to control their behavior.

### Common Signals
- **SIGTERM**: Terminate process (default for `kill`).
- **SIGSTOP**: Suspend process.
- **SIGCONT**: Resume suspended process.
- **SIGINT**: Interrupt (e.g., Ctrl+C).

### Example: Sending Signals
```bash
./cpu.elf 1000 &  # Run, assume PID=8356
kill -STOP 8356   # Suspends process
cat /proc/8356/status
# State: T (stopped)
kill -CONT 8356   # Resumes process
cat /proc/8356/status
# State: R (running)
kill -TERM 8356   # Terminates process
```

C code to handle signals:
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Received signal %d\n", sig);
}

int main() {
    signal(SIGINT, handler); // Handle Ctrl+C
    printf("Running (pid:%d)\n", (int)getpid());
    while (1) sleep(1); // Keep running
    return 0;
}
```

- **Output** (after pressing Ctrl+C):
  ```
  Running (pid:4140)
  Received signal 2
  ```

## Process Groups and Users
- **Process Groups**: Multiple processes can be grouped to receive signals collectively. Example:
  ```bash
  kill -TERM -1234  # Send SIGTERM to process group 1234
  ```
- **Users**: Processes are owned by users (identified by UID). Example:
  ```bash
  ps -u $USER  # Lists processes for current user (e.g., UID 125)
  ```
  ```c
  #include <unistd.h>
  #include <stdio.h>
  int main() {
      printf("User ID: %d\n", (int)getuid());
      return 0;
  }
  ```

## Why Separate Fork and Exec?
Separating `fork` and `exec` allows the parent to modify the child’s environment (e.g., file descriptors, signals) before executing a new program. This is critical for shell features like:
- **I/O Redirection**: Redirecting `stdout` to a file (as in `p4.c`).
- **Pipes**: Connecting processes (e.g., `ps | grep`).
- **Environment Setup**: Setting environment variables or signal handlers.

Contrast with systems like Windows, which use `CreateProcess` (akin to a combined `fork`+`exec`).


