
# Nmap Live Host Discovery


## What is Live Host Discovery?

Before scanning for open ports or services, you need to know which hosts are actually online. Nmap’s host discovery (aka ping scan) checks if systems are up without diving into full port scans. It’s like knocking on doors to see who’s home—saving you time and effort.

**Key Idea**: Nmap uses different protocols (ARP, ICMP, TCP, UDP) to detect live hosts, depending on the network and your permissions (root vs. non-root).

---

## Specifying Targets in Nmap

Nmap needs to know what to scan. You can specify targets in a few ways:

- **List**: Individual IPs or hostnames, e.g., `10.10.10.1 scanme.nmap.org example.com` (scans 3 hosts).
- **Range**: A sequence of IPs, e.g., `10.11.12.15-20` (scans 6 IPs: 15, 16, ..., 20).
- **Subnet**: A CIDR block, e.g., `10.10.10.1/30` (scans 4 IPs).
- **File Input**: A text file with targets, e.g., `nmap -iL list_of_hosts.txt`.

### Checking Targets Without Scanning (`-sL`)

Want to preview what Nmap will scan? Use `-sL` to list targets without actually scanning them. Note: Nmap tries reverse-DNS resolution unless you add `-n` to skip it.

#### Example
```bash
nmap -sL 10.10.10.1/30 -n
```
- **Output**: Lists the 4 IPs in `10.10.10.1/30` without scanning or DNS lookups.

---

## How Nmap Discovers Live Hosts

Nmap uses protocols from different TCP/IP layers to find live hosts:

- **Link Layer**: ARP (Address Resolution Protocol)
- **Network Layer**: ICMP (Internet Control Message Protocol)
- **Transport Layer**: TCP (Transmission Control Protocol)
- **Transport Layer**: UDP (User Datagram Protocol)

Let’s break down each method, how it works, and when to use it.

---

## ARP Scan (`-PR`)

**ARP (Address Resolution Protocol)** maps IP addresses to MAC addresses on the same subnet. It’s super reliable for local networks (Ethernet or Wi-Fi) because devices must respond to ARP requests if they’re online.

- **How it Works**: Nmap sends ARP requests to the broadcast address, asking, “Who has this IP?” Live hosts reply with their MAC address.
- **When to Use**: Only works on the same subnet; requires root privileges (`sudo`).
- **Nmap Option**: `-PR -sn` (ARP scan without port scanning).

### Example
```bash
sudo nmap -PR -sn 10.10.210.6/24
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-02 07:12 BST
  Nmap scan report for ip-10-10-210-75.eu-west-1.compute.internal (10.10.210.75)
  Host is up (0.00013s latency).
  MAC Address: 02:83:75:3A:F2:89 (Unknown)
  Nmap scan report for ip-10-10-210-100.eu-west-1.compute.internal (10.10.210.100)
  Host is up (-0.100s latency).
  MAC Address: 02:63:D0:1B:2D:CD (Unknown)
  ...
  Nmap done: 256 IP addresses (4 hosts up) scanned in 3.12 seconds
  ```
- **What it does**: Finds 4 live hosts on the `10.10.210.0/24` subnet using ARP.

**Bonus Tool**: Try `arp-scan` for focused ARP scanning (not on AttackBox by default; install with `apt install arp-scan`).
```bash
sudo arp-scan 10.10.210.6/24
```

---

## ICMP Scans

**ICMP (Internet Control Message Protocol)** is great for checking if hosts are alive, but firewalls often block it. Nmap supports three ICMP methods.

### ICMP Echo Scan (`-PE`)

- **How it Works**: Sends an ICMP Type 8 (Echo) request and expects a Type 0 (Echo Reply). It’s like the classic `ping` command.
- **When to Use**: Good for remote networks, but many systems block ICMP Echo.
- **Nmap Option**: `-PE -sn`

#### Example
```bash
sudo nmap -PE -sn 10.10.68.220/24
```
- **Output** (local subnet):
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-02 10:16 BST
  Nmap scan report for ip-10-10-68-50.eu-west-1.compute.internal (10.10.68.50)
  Host is up (0.00017s latency).
  MAC Address: 02:95:36:71:5B:87 (Unknown)
  ...
  Nmap done: 256 IP addresses (8 hosts up) scanned in 2.11 seconds
  ```
- **Output** (remote subnet):
  ```
  Starting Nmap 7.92 ( https://nmap.org ) at 2021-09-02 12:16 EEST
  Nmap scan report for 10.10.68.50
  Host is up (0.12s latency).
  ...
  Nmap done: 256 IP addresses (8 hosts up) scanned in 8.26 seconds
  ```
- **What it does**: Finds 8 live hosts. On local subnets, ARP responses may kick in first, showing MAC addresses.

### ICMP Timestamp Scan (`-PP`)

- **How it Works**: Sends an ICMP Type 13 (Timestamp Request) and expects a Type 14 (Timestamp Reply).
- **When to Use**: Alternative to Echo when it’s blocked.
- **Nmap Option**: `-PP -sn`

#### Example
```bash
sudo nmap -PP -sn 10.10.68.220/24
```
- **Output**:
  ```
  Starting Nmap 7.92 ( https://nmap.org ) at 2021-09-02 12:06 EEST
  Nmap scan report for 10.10.68.50
  Host is up (0.13s latency).
  ...
  Nmap done: 256 IP addresses (8 hosts up) scanned in 10.93 seconds
  ```

### ICMP Address Mask Scan (`-PM`)

- **How it Works**: Sends an ICMP Type 17 (Address Mask Request) and expects a Type 18 (Address Mask Reply).
- **When to Use**: Rarely works—most systems block it.
- **Nmap Option**: `-PM -sn`

#### Example
```bash
sudo nmap -PM -sn 10.10.68.220/24
```
- **Output**:
  ```
  Starting Nmap 7.92 ( https://nmap.org ) at 2021-09-02 12:13 EEST
  Nmap done: 256 IP addresses (0 hosts up) scanned in 52.17 seconds
  ```
- **What it does**: No hosts found—likely blocked by firewalls.

**Key Insight**: ICMP Echo (`-PE`) is the most common, but try Timestamp (`-PP`) if it fails. Address Mask (`-PM`) is usually a last resort.

---

## TCP Scans

When ICMP is blocked, **TCP** scans probe specific ports to see if hosts respond.

### TCP SYN Ping (`-PS`)

- **How it Works**: Sends a TCP SYN packet (default: port 80). Open ports reply with SYN-ACK; closed ports send RST. Any response means the host is up.
- **When to Use**: Great for remote networks; requires root for stealth (no handshake completion).
- **Nmap Option**: `-PS[ports] -sn`

#### Example
```bash
sudo nmap -PS -sn 10.10.68.220/24
```
- **Output**:
  ```
  Starting Nmap 7.92 ( https://nmap.org ) at 2021-09-02 13:45 EEST
  Nmap scan report for 10.10.68.52
  Host is up (0.10s latency).
  ...
  Nmap done: 256 IP addresses (5 hosts up) scanned in 17.38 seconds
  ```
- **What it does**: Finds 5 live hosts by probing port 80.

### TCP ACK Ping (`-PA`)

- **How it Works**: Sends a TCP ACK packet (default: port 80). Any response (usually RST) indicates the host is up.
- **When to Use**: Works when SYN is filtered; requires root.
- **Nmap Option**: `-PA[ports] -sn`

#### Example
```bash
sudo nmap -PA -sn 10.10.68.220/24
```
- **Output**:
  ```
  Starting Nmap 7.92 ( https://nmap.org ) at 2021-09-02 13:46 EEST
  Nmap scan report for 10.10.68.52
  Host is up (0.11s latency).
  ...
  Nmap done: 256 IP addresses (5 hosts up) scanned in 29.89 seconds
  ```

**Port Flexibility**: Add ports like `-PS21-25,80,443` to target specific services (e.g., FTP, HTTP, HTTPS).

---

## UDP Ping (`-PU`)

**UDP** scans are trickier since open ports don’t reply, but closed ports trigger an **ICMP Port Unreachable** message, signaling the host is up.

- **How it Works**: Sends UDP packets to likely closed ports. An ICMP reply means the host is alive.
- **When to Use**: Useful when TCP and ICMP are blocked.
- **Nmap Option**: `-PU[ports] -sn`

### Example
```bash
sudo nmap -PU -sn 10.10.68.220/24
```
- **Output**:
  ```
  Starting Nmap 7.92 ( https://nmap.org ) at 2021-09-02 13:45 EEST
  Nmap scan report for 10.10.68.52
  Host is up (0.10s latency).
  ...
  Nmap done: 256 IP addresses (5 hosts up) scanned in 9.20 seconds
  ```

---

## Default Nmap Behavior

If you don’t specify a discovery method, Nmap chooses based on your setup:

- **Local Network (Privileged User)**: Uses ARP requests.
- **Remote Network (Privileged User)**: Sends ICMP Echo, TCP ACK (port 80), TCP SYN (port 443), and ICMP Timestamp.
- **Remote Network (Unprivileged User)**: Attempts TCP 3-way handshake on ports 80 and 443.

To disable port scanning and focus on discovery, always add `-sn`.

---

## Bonus: Masscan

**Masscan** is a faster, more aggressive alternative to Nmap for host discovery. It’s less stealthy but great for large networks.

### Examples
```bash
masscan 10.10.68.220/24 -p443
masscan 10.10.68.220/24 -p80,443
masscan 10.10.68.220/24 --top-ports 100
```

---

## Summary of Nmap Host Discovery Options

| **Option**      | **Protocol** | **Description**                              | **Best For**               |
|-----------------|--------------|----------------------------------------------|----------------------------|
| `-PR -sn`       | ARP          | Queries MAC addresses on local subnet        | Local networks (requires root) |
| `-PE -sn`       | ICMP Echo    | Sends ping requests                         | General use, but often blocked |
| `-PP -sn`       | ICMP Timestamp | Requests timestamp replies                | When Echo is blocked       |
| `-PM -sn`       | ICMP Address Mask | Requests subnet mask (rarely works)     | Last resort                |
| `-PS[ports] -sn`| TCP SYN      | Probes ports with SYN packets               | Remote networks (stealthy) |
| `-PA[ports] -sn`| TCP ACK      | Sends ACK packets, expects RST              | Firewalled networks        |
| `-PU[ports] -sn`| UDP          | Triggers ICMP replies from closed ports     | When TCP/ICMP fail         |

---
