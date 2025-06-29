
#  Nmap Basic Port Scans

Hey there, it’s so1icitx! Welcome back to my "Hacking Notes" repo, where I’m documenting my cybersecurity journey with TryHackMe. Today, we’re exploring **Nmap Basic Port Scans**—the art of finding which services are running on a host by checking TCP and UDP ports. This guide covers the essentials, from TCP connect and SYN scans to UDP scans, with examples and tips to make it all clear. Let’s dive in!

---

## What Are Ports and Why Scan Them?

Ports are like doors on a host—each one leads to a specific **network service** (e.g., a web server or email service). **TCP** and **UDP** ports range from 1 to 65,535, and no two services can share the same port on the same IP.

- **Examples**:
  - **HTTP**: TCP port 80 (default for web servers).
  - **HTTPS**: TCP port 443 (secure web).
  - **SSH**: TCP port 22 (remote access).

**Port States**:
- **Open**: A service is listening (e.g., a web server on port 80).
- **Closed**: No service is listening, but the port is reachable.
- **Filtered**: Nmap can’t tell if it’s open or closed (likely blocked by a firewall).
- **Unfiltered**: Accessible, but Nmap can’t confirm open or closed (used in ACK scans).
- **Open|Filtered**: Could be open or filtered (common in UDP scans).
- **Closed|Filtered**: Could be closed or filtered.

**Why Scan?** To find services you can interact with—or exploit—during pentesting.

---

## Understanding TCP Flags

Port scans rely on **TCP headers**, especially the **flags** that control connections. Here’s a quick rundown:

- **SYN**: Starts a TCP connection (part of the 3-way handshake).
- **ACK**: Acknowledges received data.
- **RST**: Resets a connection (e.g., sent by closed ports).
- **FIN**: Signals no more data to send.
- **PSH**: Pushes data to the application quickly.
- **URG**: Marks urgent data (rarely used).

**TCP 3-Way Handshake**:
1. Client sends **SYN**.
2. Server replies with **SYN/ACK** (if port is open).
3. Client sends **ACK** to complete the connection.

Nmap uses these flags to probe ports without always finishing the handshake.

---

## TCP Connect Scan (`-sT`)

The **TCP Connect Scan** completes the full 3-way handshake to check if a port is open. It’s straightforward but noisy, as it creates loggable connections.

- **How It Works**:
  - Sends **SYN** to a port.
  - Open port replies with **SYN/ACK**; Nmap sends **ACK** then **RST/ACK** to tear down.
  - Closed port sends **RST/ACK** or nothing.
- **Pros**:
  - Works for unprivileged users (no root needed).
  - Reliable for basic scans.
- **Cons**:
  - Noisy—likely to be logged.
  - Slower due to full handshakes.

### Example
```bash
nmap -sT 10.10.10.1
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 09:53 BST
  Nmap scan report for 10.10.10.1
  Host is up (0.0024s latency).
  Not shown: 995 closed ports
  PORT    STATE SERVICE
  22/tcp  open  ssh
  25/tcp  open  smtp
  80/tcp  open  http
  111/tcp open  rpcbind
  143/tcp open  imap
  993/tcp open  imaps
  995/tcp open  pop3s
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 0.40 seconds
  ```
- **What it does**: Finds 7 open TCP ports by completing handshakes.

---

## TCP SYN Scan (`-sS`)

The **TCP SYN Scan** (aka stealth scan) is Nmap’s default for privileged users. It’s faster and less likely to be logged since it doesn’t complete the handshake.

- **How It Works**:
  - Sends **SYN** to a port.
  - Open port replies with **SYN/ACK**; Nmap sends **RST** to stop.
  - Closed port sends **RST/ACK** or nothing.
- **Pros**:
  - Stealthier—avoids full connections.
  - Faster than connect scan.
- **Cons**:
  - Requires root privileges (`sudo`).
  - May still be detected by advanced firewalls.

### Example
```bash
sudo nmap -sS 10.10.10.1
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 09:53 BST
  Nmap scan report for 10.10.10.1
  Host is up (0.0073s latency).
  Not shown: 994 closed ports
  PORT    STATE SERVICE
  22/tcp  open  ssh
  25/tcp  open  smtp
  80/tcp  open  http
  110/tcp open  pop3
  111/tcp open  rpcbind
  143/tcp open  imap
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1.60 seconds
  ```
- **What it does**: Finds 6 open TCP ports without full connections.

---

## UDP Scan (`-sU`)

**UDP Scans** target connectionless UDP ports, which are trickier since open ports don’t always respond.

- **How It Works**:
  - Sends a UDP packet to a port.
  - Open port: Usually no reply (marked `open|filtered`).
  - Closed port: Sends **ICMP Port Unreachable** (marked `closed`).
- **Pros**:
  - Finds UDP services like DNS (port 53) or RPC (port 111).
  - Essential for complete network mapping.
- **Cons**:
  - Slow—UDP responses are unreliable.
  - Requires root privileges (`sudo`).

### Example
```bash
sudo nmap -sU 10.10.10.1
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 09:54 BST
  Nmap scan report for 10.10.10.1
  Host is up (0.00061s latency).
  Not shown: 998 closed ports
  PORT    STATE         SERVICE
  68/udp  open|filtered dhcpc
  111/udp open          rpcbind
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1085.05 seconds
  ```
- **What it does**: Finds port 111 open and port 68 possibly open (or filtered).

---

## Controlling Which Ports to Scan

Nmap defaults to scanning the 1,000 most common ports, but you can customize:

- **Port List**: `-p22,80,443` (scans ports 22, 80, 443).
- **Port Range**: `-p1-1023` (scans ports 1 to 1023).
- **All Ports**: `-p-` (scans all 65,535 ports—slow!).
- **Fast Mode**: `-F` (scans top 100 ports).
- **Top Ports**: `--top-ports 10` (scans 10 most common ports).

### Examples
```bash
sudo nmap -sS -p22,80,443 10.10.10.1   # Scans specific ports
sudo nmap -sU -p1-1023 10.10.10.1       # Scans UDP ports 1-1023
nmap -sT -F 10.10.10.1                  # Scans top 100 TCP ports
```

---

## Scan Timing (`-T`)

Timing controls how fast Nmap scans, balancing speed and stealth:

- **`-T0` (Paranoid)**: Slowest, stealthiest (5-minute gaps).
- **`-T1` (Sneaky)**: Quiet, good for real engagements.
- **`-T2` (Polite)**: Moderate pace.
- **`-T3` (Normal)**: Default, balanced.
- **`-T4` (Aggressive)**: Fast, good for CTFs.
- **`-T5` (Insane)**: Fastest, but risks packet loss.

### Example
```bash
sudo nmap -sS -T1 10.10.10.1   # Stealthy SYN scan
```

**Tip**: Use `-T0` or `-T1` to avoid IDS alerts; `-T4` for quick lab scans.

---

## Advanced Timing Options

For precise control:

- **Packet Rate**:
  - `--min-rate <number>`: Minimum packets per second.
  - `--max-rate <number>`: Maximum packets per second (e.g., `--max-rate 10`).
- **Parallel Probes**:
  - `--min-parallelism <num>`: Minimum simultaneous probes.
  - `--max-parallelism <num>`: Maximum simultaneous probes (e.g., `--min-parallelism 512`).

### Example
```bash
sudo nmap -sS --max-rate 10 --min-parallelism 512 10.10.10.1
```
- **What it does**: Limits to 10 packets/second with at least 512 probes active.

---

## Other Useful Options

- **Fast Mode**: `-F` (scans 100 ports instead of 1,000).
- **Sequential Scanning**: `-r` (scans ports in order, not random—good for testing).
- **Combining Scans**: Mix TCP and UDP, e.g., `sudo nmap -sS -sU 10.10.10.1`.

---

## Summary of Nmap Port Scan Options

| **Option** | **Type** | **Description**                          | **Privileges** | **Best For**                     |
|------------|----------|------------------------------------------|----------------|----------------------------------|
| `-sT`      | TCP      | Full handshake connect scan              | None needed    | Unprivileged users, noisy scans  |
| `-sS`      | TCP      | SYN (stealth) scan, no handshake         | Root required  | Fast, stealthy scans             |
| `-sU`      | UDP      | Probes UDP ports for ICMP replies        | Root required  | Finding UDP services             |
| `-F`       | Any      | Scans top 100 ports                      | Optional       | Quick scans                      |
| `-p<ports>`| Any      | Specifies ports (list, range, all)       | Optional       | Targeted scanning                |
| `-T<0-5>`  | Any      | Timing template (0=slow, 5=fast)         | Optional       | Balancing speed vs. stealth      |
| `-r`       | Any      | Scans ports sequentially                 | Optional       | Consistent testing               |

---
