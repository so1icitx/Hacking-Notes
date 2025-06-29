# TShark Basics Guide for Network Traffic Analysis

This guide provides an introduction to **TShark**, a command-line network traffic analyzer developed by the Wireshark team.
## Introduction

**TShark** is a text-based network packet analyzer, serving as the command-line counterpart to Wireshark. It supports most Wireshark features, making it ideal for in-depth packet analysis, automation, and integration with other CLI tools (e.g., `grep`, `awk`). TShark can sniff live traffic like `tcpdump` or process PCAP files, offering flexibility for comprehensive network investigations. 

## Prerequisites

- **System**: Linux-based OS (e.g., Ubuntu, Kali) with TShark installed.
- **Permissions**: Superuser privileges (`sudo`) for live sniffing and interface listing.
- **Tools**: TShark (v3.0 or later), `capinfos`, `grep`, `cut`, `uniq`, `nl`, `sed`, `awk`.
- **Files**: Sample PCAP (e.g., `demo.pcapng`) for analysis.
- **Knowledge**: Basic understanding of networking protocols (TCP, HTTP, DNS) and Wireshark filters.

## Installation

Install TShark on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install tshark
```

Verify installation:
```bash
tshark -v
```
**Example Output**:
```
TShark (Wireshark) 3 (Git v3. packaged as 3.)
Copyright 1998-2020 Gerald Combs and contributors. License GPLv2+.
```

## Core Functionalities

TShark excels in:
- **Live Sniffing**: Capture traffic on specific interfaces.
- **PCAP Analysis**: Read and filter stored capture files.
- **Automation**: Pipe output to CLI tools for scripting.
- **Detailed Analysis**: Provide verbose packet details or hex/ASCII dumps.

## Key Parameters

| **Parameter** | **Purpose** | **Example** |
|---------------|-------------|-------------|
| `-h` | Display help page | `tshark -h` |
| `-v` | Show version info | `tshark -v` |
| `-D` | List sniffing interfaces | `sudo tshark -D` |
| `-i` | Select interface for capture | `tshark -i ens5` |
| `-r` | Read PCAP file | `tshark -r demo.pcapng` |
| `-c` | Limit packet count | `tshark -r demo.pcapng -c 10` |
| `-w` | Write to PCAP file | `tshark -w sample.pcap` |
| `-V` | Verbose output (Packet Details) | `tshark -r demo.pcapng -V` |
| `-q` | Silent mode (suppress output) | `tshark -q` |
| `-x` | Show hex/ASCII dump | `tshark -r demo.pcapng -x` |
| `-a` | Autostop conditions (duration, filesize, files) | `tshark -w test.pcap -a duration:2` |
| `-b` | Ring buffer (infinite loop) | `tshark -w test.pcap -b filesize:10 -b files:3` |
| `-f` | Capture filter (BPF syntax) | `tshark -f "host 10.10.10.10"` |
| `-Y` | Display filter (Wireshark syntax) | `tshark -r demo.pcapng -Y "http"` |

## TShark Workflow

### 1. Interface Management

List available interfaces:
```bash
sudo tshark -D
```
**Example Output**:
```
1. ens5
2. lo (Loopback)
3. any
4. bluetooth-monitor
5. nflog
```

Select an interface for sniffing:
```bash
tshark -i ens5
```
**Note**: Without `-i`, TShark defaults to the first interface (e.g., `ens5`).

### 2. Live Sniffing

Capture traffic on the default interface:
```bash
tshark
```
**Example Output**:
```
Capturing on 'ens5'
1 0.000000 aaa.aaa.aaa.aaa → bbb.bbb.bbb.bbb TCP 3372 → 80 [SYN] Seq=0 Win=8760 Len=0 MSS=1460
```

Capture on a specific interface with packet limit:
```bash
tshark -i lo -c 10
```
**Explanation**: Captures 10 packets on the Loopback interface (`lo`).

### 3. Reading PCAP Files

Read a PCAP file:
```bash
tshark -r demo.pcapng
```
**Example Output**:
```
1 0.000000 145.254.160.237 → 65.208.228.223 TCP 3372 → 80 [SYN] Seq=0 Win=8760 Len=0 MSS=1460
```

Limit to specific packets:
```bash
tshark -r demo.pcapng -c 2
```
**Output**:
```
1 0.000000 145.254.160.237 → 65.208.228.223 TCP 3372 → 80 [SYN] Seq=0 Win=8760 Len=0 MSS=1460
2 0.911310 65.208.228.223 → 145.254.160.237 TCP 80 → 3372 [SYN, ACK] Seq=0 Ack=1 Win=5840 Len=0 MSS=1380
```

### 4. Writing to PCAP Files

Save captured traffic to a file:
```bash
tshark -w sample-capture.pcap -c 100
```

Extract specific packets from a PCAP:
```bash
tshark -r demo.pcapng -c 1 -w write-demo.pcap
```
Verify output:
```bash
ls
# Output: demo.pcapng write-demo.pcap
```

### 5. Packet Details and Hex/ASCII Dumps

Show packet bytes:
```bash
tshark -r write-demo.pcap -x
```
**Example Output**:
```
0000  fe ff 20 00 01 00 00 00 01 00 00 00 08 00 45 00
0010  00 30 0f 41 40 00 80 06 91 eb 91 fe a0 ed 41 d0
```

Enable verbose output:
```bash
tshark -r demo.pcapng -c 1 -V
```
**Example Output**:
```
Frame 1: 62 bytes on wire (496 bits)
Ethernet II, Src: 00:00:01:00:00:00, Dst: fe:ff:20:00:01:00
Internet Protocol Version 4, Src: 145.254.160.237, Dst: 65.208.228.223
Transmission Control Protocol, Src Port: 3372, Dst Port: 80, Seq: 0, Len: 0
```

**Tip**: Use `-V` after filtering to reduce output complexity.

### 6. Capture Conditions

**Autostop (`-a`)**:
- Stop after 2 seconds:
  ```bash
  tshark -w test.pcap -a duration:2
  ```
- Stop after 10KB file size:
  ```bash
  tshark -w test.pcap -a filesize:10
  ```

**Ring Buffer (`-b`)**:
- Capture in 3 files, 10KB each:
  ```bash
  tshark -w test.pcap -b filesize:10 -b files:3
  ```

**Combined Example**:
```bash
tshark -w autostop-demo.pcap -a duration:2 -a filesize:5 -a files:5
```
**Output**:
Creates 5 files (`autostop-demo_...1_2022.pcap` to `...5_2022.pcap`).

### 7. Packet Filtering

TShark supports **capture filters** (live, BPF syntax) and **display filters** (post-capture, Wireshark syntax).

#### Capture Filters (`-f`)

| **Category** | **Example** | **Purpose** |
|--------------|-------------|-------------|
| Host | `tshark -f "host 10.10.10.10"` | Filter traffic to/from an IP. |
| Network | `tshark -f "net 10.10.10.0/24"` | Filter a subnet. |
| Port | `tshark -f "port 80"` | Filter specific port traffic. |
| Port Range | `tshark -f "portrange 80-100"` | Filter a range of ports. |
| Direction | `tshark -f "src host 10.10.10.10"` | Filter source IP. |
| Protocol | `tshark -f "tcp"` | Filter TCP traffic. |
| MAC | `tshark -f "ether host F8:DB:C5:A2:5D:81"` | Filter by MAC address. |
| IP Protocol | `tshark -f "ip proto 1"` | Filter ICMP (protocol 1). |

**Example**:
```bash
tshark -f "port 4444"
```
Captures traffic on port 4444, useful for detecting backdoors.

#### Display Filters (`-Y`)

| **Category** | **Example** | **Purpose** |
|--------------|-------------|-------------|
| IP | `tshark -Y "ip.addr == 10.10.10.10"` | Filter IP address. |
| IP Source | `tshark -Y "ip.src == 10.10.10.10"` | Filter source IP. |
| TCP Port | `tshark -Y "tcp.port == 80"` | Filter TCP port. |
| HTTP | `tshark -Y "http"` | Filter HTTP traffic. |
| HTTP Response | `tshark -Y "http.response.code == 200"` | Filter HTTP 200 responses. |
| DNS | `tshark -Y "dns"` | Filter DNS traffic. |
| DNS A Record | `tshark -Y "dns.qry.type == 1"` | Filter DNS A queries. |

**Example**:
```bash
tshark -r demo.pcapng -Y "ip.addr == 145.253.2.203"
```
**Output**:
```
13 2.55 145.254.160.237 → 145.253.2.203 DNS Standard query 0x0023 A
17 2.91 145.253.2.203 → 145.254.160.237 DNS Standard query response 0x0023 A
```

**Count Filtered Packets**:
```bash
tshark -r demo.pcapng -Y "http" | nl
```
**Output**:
```
1  4  0.911 145.254.160.237 → 65.208.228.223 HTTP GET /download.html HTTP/1.1
2 18  2.984 145.254.160.237 → 216.239.59.99 HTTP GET /pagead/ads?client...
3 27  3.955 216.239.59.99 → 145.254.160.237 HTTP HTTP/1.1 200 OK
4 38  4.846 65.208.228.223 → 145.254.160.237 HTTP/XML HTTP/1.1 200 OK
```

### 8. Integration with CLI Tools

Enhance TShark with tools for advanced analysis:
- **capinfos**: Summarize PCAP details:
  ```bash
  capinfos demo.pcapng
  ```
- **grep**: Search packet output:
  ```bash
  tshark -r demo.pcapng -Y "http" | grep "GET"
  ```
- **cut**: Extract fields:
  ```bash
  tshark -r demo.pcapng -T fields -e ip.src | cut -d' ' -f1
  ```
- **uniq**: Remove duplicates:
  ```bash
  tshark -r demo.pcapng -T fields -e ip.src | uniq
  ```
- **sed**: Modify output:
  ```bash
  tshark -r demo.pcapng -Y "http" | sed 's/HTTP/XML/HTTP/'
  ```
- **awk**: Process patterns:
  ```bash
  tshark -r demo.pcapng -Y "http" | awk '{print $3}'
  ```

## Practical Tips

1. **Start with Broad Filters**:
   - Use `tshark -f "tcp"` or `tshark -Y "http"` to scope traffic, then refine (e.g., `tshark -Y "http.response.code == 200"`).
2. **Limit Output**:
   - Combine `-c` or `nl` to manage large datasets.
3. **Use Verbosity Sparingly**:
   - Apply `-V` after filtering to avoid overwhelming output.
4. **Automate Analysis**:
   - Pipe to scripts for repetitive tasks (e.g., `tshark -r file.pcap -Y "dns" | grep "A" > dns.log`).
5. **Test Filters**:
   - Generate traffic with `curl` or `nc` to practice:
     ```bash
     curl tryhackme.com
     tshark -f "host tryhackme.com"
     ```
6. **Check PCAP Summary**:
   - Use `capinfos` before analysis to understand file scope.
7. **Secure Sniffing**:
   - Run in a sandboxed VM to avoid exposing live traffic.

## Example Workflow: Malicious Traffic Analysis

To apply TShark to the TryHackMe "Carnage" scenario (malicious zip download and Cobalt Strike C2), follow these steps:

1. **Read PCAP**:
   ```bash
   tshark -r carnage.pcap
   ```

2. **Find HTTP Downloads**:
   ```bash
   tshark -r carnage.pcap -Y "http.request.method == GET" | nl
   ```
   Identifies `documents[.]zip` in GET request.

3. **Extract Domain**:
   ```bash
   tshark -r carnage.pcap -Y "http.host contains attirenepal" -T fields -e http.host
   ```
   Outputs `attirenepal[.]com`.

4. **Check TLS Domains**:
   ```bash
   tshark -r carnage.pcap -Y "tls.handshake.type == 1" -T fields -e tls.handshake.extensions_server_name | uniq
   ```
   Lists `finejewels[.]com[.]au`, `thietbiagt[.]com`, `new[.]americold[.]com`.

5. **Identify C2 IPs**:
   ```bash
   tshark -r carnage.pcap -Y "http.request.method == POST" -T fields -e ip.dst | uniq
   ```
   Reveals `185[.]106[.]96[.]158`, `185[.]125[.]204[.]174`.

6. **Analyze Malspam**:
   ```bash
   tshark -r carnage.pcap -Y "smtp and frame contains \"MAIL FROM\"" -T fields -e smtp.req.parameter | nl
   ```
   Finds `farshin@mailfa[.]com`.

7. **Count SMTP Packets**:
   ```bash
   tshark -r carnage.pcap -Y "smtp" | nl | tail -n 1
   ```
   Outputs 1439 packets.

## Conclusion

TShark is a powerful tool for network traffic analysis, offering robust sniffing, filtering, and scripting capabilities. By mastering parameters like `-f`, `-Y`, `-a`, and `-b`, analysts can efficiently investigate live or captured traffic. Integration with CLI tools enhances automation, making TShark ideal for security investigations. Practice with sample PCAPs and traffic generators (e.g., `curl`, `nc`) to build proficiency.

## Citations

- TryHackMe TShark: The Basics Walkthrough
- Wireshark TShark Manual [wireshark.org](https://www.wireshark.org/docs/man-pages/tshark.html)
- Wireshark Capture Filter Reference [wireshark.org](https://www.wireshark.org/docs/man-pages/pcap-filter.html)
- Wireshark Display Filter Reference [wireshark.org](https://www.wireshark.org/docs/dfref/)
- GitLab Wireshark Capture Filters [gitlab.com](https://gitlab.com/wireshark/wireshark/-/wikis/CaptureFilters)

---
