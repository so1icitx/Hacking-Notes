# Netstat: Your Network’s Telescope

**Netstat** (Network Statistics) is a powerful command-line tool available on virtually every operating system, including Windows, Linux, and macOS. 

## What is Netstat?

Netstat displays detailed information about your computer’s network connections, including:
- **Active Connections**: Connections your computer is making or maintaining with other devices (e.g., web servers, printers, or network-attached storage).
- **Listening Ports**: Ports open and waiting for incoming connections (e.g., a web server listening on port 80).
- **Routing Tables**: How your computer routes network traffic to different destinations.
- **Network Interfaces**: Details about your network adapters (e.g., Ethernet, Wi-Fi).

Netstat is pre-installed on Windows, Linux, and macOS, making it a universal tool for network monitoring. It’s especially useful for:
- Identifying which programs are opening connections (e.g., a browser connecting to `example.com`).
- Detecting unauthorized connections that might indicate malware.
- Checking which ports are open and what services are listening.

## Core Concepts

### Connection Types
Netstat primarily deals with two transport protocols:
- **TCP (Transmission Control Protocol)**: Connection-oriented, reliable, used for web browsing (HTTP/HTTPS), email, and file transfers. Shows states like ESTABLISHED or LISTEN.
- **UDP (User Datagram Protocol)**: Connectionless, faster but less reliable, used for DNS, streaming, or NetBIOS. Typically lacks state information.

### Connection States
Netstat’s output includes a **State** column for TCP connections, indicating their lifecycle:
- **ESTABLISHED**: An active, open connection (e.g., your browser connected to a web server on port 443 for HTTPS).
- **LISTEN**: A service is waiting for incoming connections (e.g., a web server listening on port 80).
- **TIME_WAIT**: A temporary state after a connection closes, ensuring all packets are processed.
- **SYN_SENT**: The computer has initiated a connection and is waiting for a response.
- **CLOSE_WAIT**: The remote host has closed the connection, but the local application hasn’t yet.
- **FIN_WAIT**: The local host is closing the connection, waiting for acknowledgment.

UDP connections don’t have states due to their connectionless nature.

### Output Structure
Running `netstat` without options displays a table with:
- **Proto**: Protocol (TCP, UDP, TCPv6, UDPv6).
- **Local Address**: Your computer’s IP address and port (e.g., `192.168.1.100:12345`).
- **Foreign Address**: The remote host’s IP address and port (e.g., `93.184.216.34:443` for `example.com`).
- **State**: The connection’s status (TCP only).
- **PID/Program Name** (with certain options): The process ID or executable responsible for the connection.

For example:
```
Proto  Local Address         Foreign Address       State
TCP    192.168.1.100:51234   93.184.216.34:443    ESTABLISHED
```
This shows a TCP connection to `example.com` (port 443) from a local port, actively connected.

## Why Netstat Matters

Imagine your computer as a busy airport with flights (connections) constantly taking off and landing. Netstat is the air traffic control tower, showing you:
- **Who’s Connected**: Identify remote hosts (e.g., a web server or a suspicious IP).
- **What’s Listening**: Check which ports are open, potentially exposing services to attackers.
- **What’s Running**: Link connections to specific programs or processes, helping diagnose issues or detect malware.

Use cases include:
- **Troubleshooting**: Identify why a service isn’t connecting (e.g., wrong port).
- **Security**: Spot unauthorized connections (e.g., an unknown IP on port 8888).
- **Performance**: Monitor network load from active connections.

## Using Netstat

### Basic Command
Run netstat without options to see active connections:
```bash
netstat
```
This may be slow due to DNS lookups resolving IP addresses to hostnames.

### Key Considerations
- **Speed**: DNS resolution can slow output. Use options to skip it (see platform-specific READMEs).
- **Permissions**: Some commands require administrator/root privileges to show all connections or processes.
- **Output Customization**: Options vary by OS (e.g., `-n` for numeric IPs, `-a` for all connections). See `windows` and `linux` READMEs for details.
- **Alternatives**: Tools like `ss` (Linux) or `Get-NetTCPConnection` (Windows PowerShell) are modern replacements but less universal.

### Checking Connection States
To understand what’s happening:
- **ESTABLISHED**: Your browser connecting to a website (port 443) or a printer on the network.
- **LISTEN**: A service like SSH (port 22) waiting for incoming connections.
- **TIME_WAIT**: Normal after closing a connection, but excessive instances may indicate network issues.

### Example Scenario
You open `example.com` in a browser, which connects to its web server on port 443. Running netstat shows:
```
Proto  Local Address         Foreign Address       State
TCP    192.168.1.100:51234   93.184.216.34:443    ESTABLISHED
```
This confirms an active HTTPS connection to `example.com`.

## Further Reading
- [Microsoft: Netstat Command](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/netstat)
- [Linux Man Page: netstat](https://man7.org/linux/man-pages/man8/netstat.8.html)
- [Arch Linux Wiki: Network Tools](https://wiki.archlinux.org/title/Network_tools)
- [Notion for Note-Taking](https://www.notion.so/)

nd `linux` subdirectories for platform-specific commands, example outputs, and step-by-step explanations. Happy network sleuthing! 🌐
