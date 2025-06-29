
# Nmap Basics

## What is Nmap?

**Nmap (Network Mapper)** is a free, open-source tool for discovering devices on a network and auditing their security. It tells you what’s running, which ports are open, and even guesses operating systems and service versions. Imagine it as a network detective, uncovering clues about what’s out there and how it’s configured.

**Note**: We’ll often run Nmap with `sudo` or as root to unlock its full potential. Without root privileges, you’re limited to basic scans like TCP connect scans—more on that later!

---

## Specifying Targets in Nmap

Nmap needs to know what to scan, and it’s super flexible about how you tell it:

- **IP Range with `-`**: Scan a range of IPs, e.g., `192.168.0.1-10` (scans 192.168.0.1 to 192.168.0.10).
- **IP Subnet with `/`**: Scan a whole subnet, e.g., `192.168.0.1/24` (scans 192.168.0.0 to 192.168.0.255).
- **Hostname**: Target a specific hostname, e.g., `google.com`.

## Discovering Live Hosts with Ping Scan (`-sn`)

### Scanning a Local Network

#### Description
- Identifies active devices in a local network using ARP requests.
- Displays MAC addresses of detected devices.

#### Command
```bash
sudo nmap -sn 192.168.66.0/24
```

#### Example Output
```
Starting Nmap 7.92 ( https://nmap.org ) at 2024-08-07 13:49 EEST
Nmap scan report for XiaoQiang (192.168.66.1)
Host is up (0.0069s latency).
MAC Address: 44:DF:65:D8:FE:6C (Unknown)
Nmap scan report for S190023240007 (192.168.66.88)
Host is up (0.090s latency).
MAC Address: 7C:DF:A1:D3:8C:5C (Espressif)
...
Nmap done: 256 IP addresses (7 hosts up) scanned in 2.64 seconds
```

### Scanning a Remote Network

#### Description
- Detects live hosts in remote networks using ICMP echo requests and TCP/UDP probes.

#### Command
```bash
sudo nmap -sn 192.168.11.0/24
```

#### Example Output
```
Starting Nmap 7.92 ( https://nmap.org ) at 2024-08-07 14:05 EEST
Nmap scan report for 192.168.11.1
Host is up (0.018s latency).
Nmap scan report for 192.168.11.151
Host is up (0.0013s latency).
...
Nmap done: 256 IP addresses (5 hosts up) scanned in 10.67 seconds
```

---

## Listing Targets Without Scanning (`-sL`)

#### Description
- Lists target IPs without scanning them.

#### Command
```bash
nmap -sL 192.168.0.1/24
```

---

## Scanning for Services: TCP and UDP Scans

### TCP Connect Scan (`-sT`)

#### Description
- Completes a full TCP handshake to detect open ports.
- Works without root privileges.
- Noisy and easily logged.

#### Command
```bash
nmap -sT 192.168.124.211
```

### SYN Scan (`-sS`)

#### Description
- Sends SYN packets and waits for SYN-ACK responses.
- Does not complete the handshake, making it stealthier.
- Requires root privileges.

#### Command
```bash
sudo nmap -sS 192.168.124.211
```

### UDP Scan (`-sU`)

#### Description
- Sends UDP packets to detect open services.
- Slower and less reliable than TCP scans.

#### Command
```bash
sudo nmap -sU 192.168.124.211
```

---

## Controlling Which Ports to Scan

#### Common Options
- `-F`: Fast mode, scans 100 most common ports.
- `-p [range]`: Specifies a port range (e.g., `-p 1-100`).
- `-p-`: Scans all 65,535 ports.

#### Example Commands
```bash
sudo nmap -sS -p 1-100 192.168.124.211  # Scan ports 1-100
sudo nmap -sS -p- 192.168.124.211       # Scan all ports
```

---

## OS Detection (`-O`)

#### Description
- Identifies the target’s operating system based on network behavior.

#### Command
```bash
sudo nmap -sS -O 192.168.124.211
```

#### Example Output
```
OS details: Linux 4.15 - 5.8
```

---

## Service and Version Detection (`-sV`)

#### Description
- Determines the versions of services running on open ports.

#### Command
```bash
sudo nmap -sS -sV 192.168.124.211
```

#### Example Output
```
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.10
```

---

## Comprehensive Scans (`-A`)

#### Description
- Enables OS detection, version detection, traceroute, and script scanning.

#### Command
```bash
sudo nmap -A 192.168.124.211
```

---

## Forcing a Scan (`-Pn`)

#### Description
- Assumes all hosts are up, even if they don’t respond to ping.

#### Command
```bash
sudo nmap -sS -Pn 192.168.124.211
```

---

## Controlling Scan Speed (`-T`)

| Option  | Speed  | Description |
|---------|--------|------------|
| `-T0`   | Slowest | Super stealthy |
| `-T1`   | Slow   | Quiet scan |
| `-T2`   | Moderate | Polite scan |
| `-T3`   | Default | Normal speed |
| `-T4`   | Fast   | Aggressive scan |
| `-T5`   | Fastest | Very noisy |

#### Example Command
```bash
sudo nmap -sS -T0 192.168.124.211  # Slowest scan
```

---

## Verbose Output (`-v`)

#### Description
- Provides detailed output during scanning.

#### Command
```bash
sudo nmap -sS -v 192.168.139.1/24
```

---

## Saving Scan Results

| Option  | Format |
|---------|--------|
| `-oN <file>` | Normal text output |
| `-oX <file>` | XML format |
| `-oG <file>` | Grepable format |
| `-oA <basename>` | Saves in all three formats |

#### Example Command
```bash
sudo nmap -sS -oA scan_results 192.168.139.1
```

---



