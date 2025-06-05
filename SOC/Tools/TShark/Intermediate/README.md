# TShark Intermediate Guide for Network Traffic Analysis

This guide provides an intermediate-level overview of **TShark**, the command-line network traffic analyzer, focusing on Wireshark-like features, advanced statistics, stream analysis, object extraction, credential hunting, and specialized filtering. 

## Introduction

**TShark** is a powerful command-line tool developed by the Wireshark team, offering most Wireshark features for packet analysis, statistics, and automation. Its text-based interface enables deep packet inspection, integration with CLI tools (e.g., `awk`, `uniq`), and scripting for large-scale investigations. This intermediate guide covers TShark’s advanced capabilities, including protocol hierarchy, endpoint/conversation statistics, stream following, object/credential extraction, and advanced filtering with `contains` and `matches` operators. It includes practical applications for detecting malicious activities like C2 communication, malspam, and file downloads.

## Prerequisites

- **System**: Linux-based OS (e.g., Ubuntu, Kali) with TShark installed.
- **Permissions**: Superuser privileges (`sudo`) for live sniffing.
- **Tools**: TShark (v3.0+), `awk`, `sort`, `uniq`, `grep`, `capinfos`.
- **Files**: Sample PCAPs (e.g., `demo.pcapng`, `colour.pcap`, `credentials.pcap`).
- **Knowledge**: Familiarity with TShark basics, Wireshark filters, and protocols (HTTP, DNS, TCP, SMTP).
- **Safety**: Analyze in a sandboxed VM; avoid interacting with malicious domains/IPs.

## Installation

Install TShark:
```bash
sudo apt update
sudo apt install tshark
```

Verify version:
```bash
tshark -v
```

## Key Features

- **Statistics**: Protocol hierarchy, packet lengths, endpoints, conversations, expert info.
- **Stream Analysis**: Follow TCP, UDP, HTTP streams for payload inspection.
- **Object Extraction**: Export files from HTTP, SMB, TFTP, etc.
- **Credential Hunting**: Detect cleartext credentials in FTP, HTTP, SMTP.
- **Advanced Filtering**: Use `contains`, `matches`, and field extraction for precise analysis.
- **Integration**: Pipe output to `awk`, `sort`, `uniq` for data processing.

## Command-Line Wireshark Features

TShark shares Wireshark’s display filters and offers features like colorized output, statistics, and stream analysis. These apply to all packets unless filtered with `-Y`.

### 1. Colorized Output

Enable Wireshark-style packet highlighting:
```bash
tshark -r colour.pcap --color
```

**Use Case**: Spot anomalies (e.g., HTTP errors, retransmissions) quickly in terminal output.

### 2. Statistics (`-z`)

List available statistics options:
```bash
tshark -z help
```

Suppress packet output with `-q` to focus on statistics:
```bash
tshark -r demo.pcapng -z io,phs -q
```

#### Protocol Hierarchy (`-z io,phs`)

View protocol distribution:
```bash
tshark -r demo.pcapng -z io,phs -q
```
**Example Output**:
```
===================================================================
Protocol Hierarchy Statistics
Filter: 
  eth                                    frames:43 bytes:25091
    ip                                   frames:43 bytes:25091
      tcp                                frames:41 bytes:24814
        http                             frames:4 bytes:2000
          data-text-lines                frames:1 bytes:214
          xml                            frames:1 bytes:478
      udp                                frames:2 bytes:277
        dns                              frames:2 bytes:277
===================================================================
```

Filter specific protocol:
```bash
tshark -r demo.pcapng -z io,phs,udp -q
```
**Output**:
```
===================================================================
Protocol Hierarchy Statistics
Filter: udp
  eth                                    frames:2 bytes:277
    ip                                   frames:2 bytes:277
      udp                                frames:2 bytes:277
        dns                              frames:2 bytes:277
===================================================================
```

**Use Case**: Identify dominant protocols (e.g., HTTP, DNS) to focus investigations.

#### Packet Lengths (`-z plen,tree`)

Analyze packet size distribution:
```bash
tshark -r demo.pcapng -z plen,tree -q
```
**Output**:
```
=========================================================================================================================
Packet Lengths:
Topic / Item       Count     Average       Min val       Max val     Rate (ms)     Percent     Burst rate    Burst start  
-------------------------------------------------------------------------------------------------------------------------
Packet Lengths     43        583.51        54            1484        0.0014        100         0.0400        2.554        
 40-79             22        54.73         54            62          0.0007        51.16       0.0200        0.911        
 1280-2559         15        1440.67       1434          1484        0.0005        34.88       0.0200        2.554        
 ...
-------------------------------------------------------------------------------------------------------------------------
```

**Use Case**: Detect anomalous packet sizes (e.g., large HTTP payloads indicating file downloads).

#### Endpoints (`-z endpoints,<protocol>`)

List unique endpoints:
```bash
tshark -r demo.pcapng -z endpoints,ip -q
```
**Output**:
```
================================================================================
IPv4 Endpoints
Filter:
                       |  Packets  | |  Bytes  | | Tx Packets | | Tx Bytes | | Rx Packets | | Rx Bytes |
145.254.160.237               43         25091         20            2323          23           22768   
65.208.228.223                34         20695         18           19344          16            1351   
 ...
================================================================================
```

**Supported Protocols**: `eth`, `ip`, `ipv6`, `tcp`, `udp`, `wlan`.

**Use Case**: Identify active hosts, useful for spotting unauthorized IPs in malware traffic.

#### Conversations (`-z conv,<protocol>`)

View traffic flows between endpoints:
```bash
tshark -r demo.pcapng -z conv,ip -q
```
**Output**:
```
================================================================================
IPv4 Conversations
Filter:
                                           |       <-      | |       ->      | |     Total     |    Relative    |   Duration
                                           | Frames  Bytes | | Frames  Bytes | | Frames  Bytes |      Start     |             
65.208.228.223   <-> 145.254.160.237           16      1351      18     19344      34     20695     0.000000000        30.3937
145.254.160.237  <-> 216.239.59.99              4      3236       3       883       7      4119     2.984291000         1.7926
 ...
================================================================================
```

**Use Case**: Detect frequent C2 communications (e.g., Cobalt Strike in Carnage scenario).

#### Expert Info (`-z expert`)

View Wireshark’s automatic alerts:
```bash
tshark -r demo.pcapng -z expert -q
```
**Output**:
```
Notes (3)
=============
   Frequency      Group           Protocol  Summary
           1   Sequence                TCP  This frame is a (suspected) spurious retransmission
           1   Sequence                TCP  This frame is a (suspected) retransmission
           1   Sequence                TCP  Duplicate ACK (#1)
Chats (8)
=============
   Frequency      Group           Protocol  Summary
           1   Sequence                TCP  Connection establish request (SYN): server port 80
           1   Sequence               HTTP  GET /download.html HTTP/1.1\r\n
 ...
```

**Use Case**: Identify protocol anomalies (e.g., retransmissions, HTTP errors).

#### IPv4/IPv6 Statistics (`-z ptype,tree`)

Summarize protocol types:
```bash
tshark -r demo.pcapng -z ptype,tree -q
```
**Output**:
```
==========================================================================================================================
IPv4 Statistics/IP Protocol Types:
Topic / Item       Count         Average       Min val       Max val Rate (ms)     Percent       Burst rate    Burst start  
--------------------------------------------------------------------------------------------------------------------------
IP Protocol Types  43                                                0.0014        100          0.0400        2.554        
 TCP               41                                                0.0013        95.35        0.0300        0.911        
 UDP               2                                                 0.0001        4.65         0.0100        2.554        
--------------------------------------------------------------------------------------------------------------------------
```

#### IP Hosts (`-z ip_hosts,tree`)

List all IP addresses:
```bash
tshark -r demo.pcapng -z ip_hosts,tree -q
```
**Output**:
```
===========================================================================================================================
IPv4 Statistics/All Addresses:
Topic / Item      Count         Average       Min val       Max val  Rate (ms)     Percent       Burst rate    Burst start  
---------------------------------------------------------------------------------------------------------------------------
All Addresses     43                                                 0.0014        100          0.0400        2.554        
 145.254.160.237  43                                                 0.0014        100.00       0.0400        2.554        
 65.208.228.223   34                                                 0.0011        79.07        0.0300        0.911            
 ...
---------------------------------------------------------------------------------------------------------------------------
```

#### Source/Destination IPs (`-z ip_srcdst,tree`)

Correlate source and destination IPs:
```bash
tshark -r demo.pcapng -z ip_srcdst,tree -q
```
**Output**:
```
==========================================================================================================================
IPv4 Statistics/Source and Destination Addresses:
Topic / Item                     Count         Average       Min val       Max val  Rate (ms)     Percent       Burst rate    Burst start  
--------------------------------------------------------------------------------------------------------------------------
Source IPv4 Addresses            43                                                 0.0014        100          0.0400              
 145.254.160.237                 20                                                 0.0007        46.51        0.0200               
 65.208.228.223                  18                                                 0.0006        41.86        0.0200
 ...
--------------------------------------------------------------------------------------------------------------------------
```

#### Destinations and Ports (`-z dests,tree`)

Analyze outgoing traffic:
```bash
tshark -r demo.pcapng -z dests,tree -q
```
**Output**:
```
=============================================================================================================================
IPv4 Statistics/Destinations and Ports:
Topic / Item            Count         Average       Min val       Max val       Rate (ms)     Percent       Burst rate    Burst start  
-----------------------------------------------------------------------------------------------------------------------------
Destinations and Ports  43                                                      0.0014        100          0.0400        2.554        
 145.254.160.237        23                                                      0.0008        53.49        0.0200        2.554        
  TCP                   22                                                      0.0007        95.65        0.0200        2.554        
   3372                 18                                                      0.0006        81.82        0.0200        2.554        
 ...
-----------------------------------------------------------------------------------------------------------------------------
```

#### DNS Statistics (`-z dns,tree`)

Summarize DNS traffic:
```bash
tshark -r demo.pcapng -z dns,tree -q
```
**Output**:
```
===========================================================================================================================
DNS:
Topic / Item                   Count         Average       Min val       Max val       Rate (ms)     Percent       Burst rate    Burst start  
---------------------------------------------------------------------------------------------------------------------------
Total Packets                  2                                             0.0055        100          0.0100        2.554        
 rcode                         2                                             0.0055        100.00       0.0100        2.554        
  No error                     2                                             0.0055        100.00       0.0100        2.554        
 ...
-------------------------------------------------------------------------------------------------------------------------
```

**Use Case**: Detect suspicious DNS queries (e.g., C2 domains).

#### HTTP Statistics

- Packet Counter (`-z http,tree`):
  ```bash
  tshark -r demo.pcapng -z http,tree -q
  ```
  **Output**:
  ```
  =============================================================================================================================
  HTTP/Packet Counter:
  Topic / Item            Count         Average       Min val       Max val       Rate (ms)     Percent     Burst rate  Burst start  
  ----------------------------------------------------------------------------------------------------------------------------
  Total HTTP Packets      4                                                       0.0010        100          0.0100     0.911        
   HTTP Response Packets  2                                                       0.0005        50.00        0.0100     3.956        
    2xx: Success          2                                                       0.0005        100.00       0.0100     3.956        
     200 OK               2                                                       0.0005        100.00       0.0100     3.956        
  ...
  -----------------------------------------------------------------------------------------------------------------------
  ```

- Requests (`-z http_req,tree`):
  ```bash
  tshark -r demo.pcapng -z http_req,tree -q
  ```

**Use Case**: Identify malicious HTTP requests (e.g., file downloads in Carnage).

### 3. Stream Analysis (`-z follow`)

Follow traffic streams for payload inspection:
```bash
tshark -r demo.pcapng -z follow,tcp,ascii,1 -q
```
**Output**:
```
===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 1
Node 0: 145.254.160.237:3371
Node 1: 216.239.59.99:80
GET /pagead/ads?client=ca-pub-2309191948673629&random=1084443430285...
Host: pagead2.googlesyndication.com
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.6)...
HTTP/1.1 200 OK
Content-Type: text/html; charset=ISO-8859-1
...
===================================================================
```

**Supported Protocols**: `tcp`, `udp`, `http`, `http2`.

**Use Case**: Extract C2 payloads or HTTP GET/POST data.

### 4. Export Objects (`--export-objects`)

Extract files from protocols:
```bash
tshark -r demo.pcapng --export-objects http,/home/ubuntu/Desktop/extracted-by-tshark -q
```
**Output**:
```
$ ls -l /home/ubuntu/Desktop/extracted-by-tshark/
-rw-r--r-- 1 ubuntu ubuntu ads%3fclient=ca-pub-2309191948673629...
-rw-r--r-- 1 ubuntu ubuntu download.html
```

**Supported Protocols**: `dicom`, `http`, `imf`, `smb`, `tftp`.

**Use Case**: Recover malicious files (e.g., `documents[.]zip` in Carnage) for analysis.

### 5. Credential Hunting (`-z credentials`)

Detect cleartext credentials:
```bash
tshark -r credentials.pcap -z credentials -q
```
**Output**:
```
===================================================================
Packet     Protocol         Username         Info            
------     --------         --------         --------
72         FTP              admin            Username in packet: 37
167        FTP              administrator    Username in packet: 133
...
===================================================================
```

**Supported Protocols**: `ftp`, `http`, `imap`, `pop`, `smtp`.

**Use Case**: Identify compromised credentials in cleartext protocols.

### 6. Advanced Filtering

#### Contains Operator

Search for specific values (case-sensitive):
```bash
tshark -r demo.pcapng -Y 'http.server contains "Apache"'
```
**Output**:
```
38 4.846969 65.208.228.223 → 145.254.160.237 HTTP/XML HTTP/1.1 200 OK
```

Extract fields:
```bash
tshark -r demo.pcapng -Y 'http.server contains "Apache"' -T fields -e ip.src -e ip.dst -e http.server -E header=y
```
**Output**:
```
ip.src	ip.dst	http.server
65.208.228.223	145.254.160.237	Apache
```

**Use Case**: Find servers hosting malicious content.

#### Matches Operator

Search with regex (case-insensitive):
```bash
tshark -r demo.pcapng -Y 'http.request.method matches "(GET|POST)"'
```
**Output**:
```
4 0.911310 145.254.160.237 → 65.208.228.223 HTTP GET /download.html HTTP/1.1
18 2.984291 145.254.160.237 → 216.239.59.99 HTTP GET /pagead/ads?client...
```

Extract fields:
```bash
tshark -r demo.pcapng -Y 'http.request.method matches "(GET|POST)"' -T fields -e ip.src -e ip.dst -e http.request.method -E header=y
```
**Output**:
```
ip.src	ip.dst	http.request.method
145.254.160.237	65.208.228.223	GET
145.254.160.237	216.239.59.99	GET
```

**Use Case**: Detect specific HTTP methods used in C2 or file downloads.

#### Field Extraction (`-T fields`)

Extract specific packet fields:
```bash
tshark -r demo.pcapng -T fields -e ip.src -e ip.dst -E header=y -c 5
```
**Output**:
```
ip.src	ip.dst
145.254.160.237	65.208.228.223
65.208.228.223	145.254.160.237
...
```

**Use Case**: Correlate source/destination IPs for traffic mapping.

## Use Cases for Investigation

### 1. Extract Hostnames

Extract DHCP hostnames:
```bash
tshark -r hostnames.pcapng -T fields -e dhcp.option.hostname
```
**Output**:
```
92-rkd
T3400
60-alfb-sec2
...
```

Organize with CLI tools:
```bash
tshark -r hostnames.pcapng -T fields -e dhcp.option.hostname | awk NF | sort -r | uniq -c | sort -r
```
**Output**:
```
26 202-ac
18 92-rkd
14 93-sts-sec
...
```

**Use Case**: Identify devices on the network, useful for mapping compromised hosts.

### 2. Extract DNS Queries

List DNS queries:
```bash
tshark -r dns-queries.pcap -T fields -e dns.qry.name | awk NF | sort -r | uniq -c | sort -r
```
**Output**:
```
96 connectivity-check.ubuntu.com.rhodes.edu
94 connectivity-check.ubuntu.com
8 3.57.20.10.in-addr.arpa
...
```

**Use Case**: Detect C2 domains or suspicious DNS activity (e.g., `maldivehost[.]net` in Carnage).

### 3. Extract User Agents

Identify HTTP user agents:
```bash
tshark -r user-agents.pcap -T fields -e http.user_agent | awk NF | sort -r | uniq -c | sort -r
```
**Output**:
```
6 Mozilla/5.0 (Windows; U; Windows NT 6.4; en-US)...
5 Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:100.0)...
4 sqlmap/1.4#stable (http://sqlmap.org)
3 Wfuzz/2.7
...
```

**Use Case**: Spot malicious tools (e.g., `sqlmap`, `Nmap`) or anomalous browser agents.

## Application to Carnage Scenario

Using TShark to analyze the TryHackMe "Carnage" PCAP (malicious zip download, Cobalt Strike C2, malspam):

1. **First HTTP Connection**:
   ```bash
   tshark -r carnage.pcap -Y "http" -T fields -e frame.time | head -n 1
   ```
   **Answer**: 2021-09-24 16:44:38

2. **Zip File Name**:
   ```bash
   tshark -r carnage.pcap -Y "http.request.method == GET" -T fields -e http.request.uri | grep ".zip"
   ```
   **Answer**: documents[.]zip

3. **Hosting Domain**:
   ```bash
   tshark -r carnage.pcap -Y "http.host contains attirenepal" -T fields -e http.host
   ```
   **Answer**: attirenepal[.]com

4. **File in Zip**:
   ```bash
   tshark -r carnage.pcap --export-objects http,/tmp/extracted -q
   ls /tmp/extracted | grep ".xls"
   ```
   **Answer**: chart-1530076591[.]xls

5. **Webserver and Version**:
   ```bash
   tshark -r carnage.pcap -Y "http.response" -T fields -e http.server -e http.x_powered_by
   ```
   **Answer**: LiteSpeed, PHP/7.2.34

6. **Malicious Domains**:
   ```bash
   tshark -r carnage.pcap -Y "tls.handshake.type == 1" -T fields -e tls.handshake.extensions_server_name | grep -E "finejewels|thietbiagt|americold"
   ```
   **Answer**: finejewels[.]com[.]au, thietbiagt[.]com, new[.]americold[.]com

7. **Cobalt Strike IPs**:
   ```bash
   tshark -r carnage.pcap -Y "http.request.method == POST" -T fields -e ip.dst | sort | uniq
   ```
   **Answer**: 185[.]106[.]96[.]158, 185[.]125[.]204[.]174

8. **Malspam Sender**:
   ```bash
   tshark -r carnage.pcap -Y "smtp and frame contains \"MAIL FROM\"" -T fields -e smtp.req.parameter | head -n 1
   ```
   **Answer**: farshin@mailfa[.]com

9. **SMTP Packet Count**:
   ```bash
   tshark -r carnage.pcap -Y "smtp" | nl | tail -n 1
   ```
   **Answer**: 1439

## Practical Tips

1. **Combine Filters and Statistics**:
   - Use `-Y` with `-z` for targeted analysis (e.g., `tshark -r demo.pcapng -Y "http" -z http,tree -q`).
2. **Manage Output**:
   - Pipe to `nl`, `uniq`, or `awk` to count or clean results.
3. **Focus on Anomalies**:
   - Check expert info (`-z expert`) for retransmissions or errors.
4. **Automate with Scripts**:
   - Save commands in a bash script for repetitive tasks.
5. **Test with Known Traffic**:
   - Use `curl` or `nc` to generate traffic for practice.
6. **Secure Analysis**:
   - Export objects only in a sandboxed environment.

