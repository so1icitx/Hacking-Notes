# Zeek for Beginners: A Comprehensive Guide to Network Security Monitoring

This guide is your go-to resource for mastering **Zeek** (formerly Bro), a powerful open-source and commercial network monitoring tool. I’ve crafted a **long, beginner-friendly** `README.md` to help you understand Zeek’s core concepts, installation, configuration, scripting, logging, and real-world applications, tailored to stick in your memory as you learn cybersecurity. It’s detailed, breaks down every command, and includes practical examples to make Zeek approachable for your notebook. This guide also compares Zeek vs. Snort, addressing your interest in Snort, and covers Zeek’s unique features like logs and scripting. Let’s dive in and make Zeek your ally in network security monitoring!

### Key Points
- **What Zeek Does**: Zeek is a passive network traffic analyzer focused on network security monitoring (NSM), generating detailed logs for investigating suspicious activity, performance, and troubleshooting.
- **Beginner-Friendly**: It’s versatile, open-source (with a commercial Corelight version), and script-driven, but requires learning its logs and scripting language.
- **Core Features**: Produces 50+ logs across 7 protocols, supports event-driven scripting, and excels at in-depth traffic analysis for threat hunting.
- **Zeek vs. Snort**: Zeek focuses on network analysis and logging for complex threat detection, while Snort is signature-based IDS/IPS for known threats.
- **Learning Curve**: Zeek’s scripting and log correlation require networking knowledge, but practice with examples builds confidence.

## 1. Introduction to Zeek

Zeek, originally called Bro, is an open-source and commercial **Network Intrusion Detection System (NIDS)** and traffic analysis framework developed by Lawrence Berkeley Labs and supported by a community and Corelight ([Zeek Official Site](https://zeek.org/)). It’s a passive network monitor that logs detailed traffic data, making it ideal for **Network Security Monitoring (NSM)** to investigate suspicious or malicious activity. Unlike traditional IDS/IPS tools, Zeek focuses on **visibility** and **analysis**, offering 50+ logs for forensics, protocol analysis, and threat hunting.

### 1.1 What Zeek Does
- **Monitors Traffic**: Captures and logs network activity passively, covering protocols like HTTP, DNS, FTP, and more.
- **Generates Logs**: Produces 54 logs in 7 categories (e.g., `conn.log`, `dns.log`, `http.log`) for detailed analysis.
- **Supports NSM**: Detects anomalies, rogue hosts, and malicious patterns, aiding incident response and threat hunting.
- **Enables Scripting**: Uses a custom event-driven language to correlate events and automate analysis.
- **Analyzes Performance**: Measures network performance and troubleshoots connectivity issues.
- **Extends Functionality**: Offers frameworks (e.g., File Analysis, Intelligence) for specialized tasks.

### 1.2 Why Zeek Matters
- **In-Depth Visibility**: Logs everything from connection details to file transfers, perfect for investigations.
- **Threat Hunting**: Excels at detecting complex threats through event correlation and scripting.
- **Open-Source**: Free with community support, plus a commercial Corelight version for enterprise needs.
- **Flexible**: Supports both security (IDS, anomaly detection) and non-security tasks (performance monitoring).

For beginners, Zeek’s scripting and log analysis can be challenging, but this guide breaks it down with examples to help you master it.

## 2. Network Monitoring vs. Network Security Monitoring (NSM)

### 2.1 Network Monitoring
- **Purpose**: Focuses on IT asset health, ensuring uptime, performance, and configuration.
- **Actions**:
  - Monitor device availability (e.g., routers, servers).
  - Analyze connection quality (e.g., latency, packet loss).
  - Balance network traffic and troubleshoot issues.
- **Scope**: Network admins, not typically SOC-focused; doesn’t prioritize deep security threats like zero-days or internal vulnerabilities.
- **Output**: Visualizations, performance metrics, and root cause analysis for IT operations.

### 2.2 Network Security Monitoring (NSM)
- **Purpose**: Detects and responds to security anomalies (e.g., rogue hosts, malicious traffic).
- **Actions**:
  - Monitor for suspicious patterns, encrypted traffic, or unusual port usage.
  - Investigate events using logs and correlate with threat intelligence.
  - Support intrusion detection and response.
- **Scope**: SOC analysts (tiers 1–3), threat hunters, and security engineers; focuses on threats, vulnerabilities using rules and signatures.
- **Output**: Alerts, logs for incident response, forensics, and threat hunting.

### 3. Zeek vs. Snort
Zeek and Snort are both NIDS tools, but they serve different purposes. Here’s a detailed comparison:

| **Feature/Feature** | **Zeek** | **Snort** |
|---------------------|----------|-----------|
| **Type**            | NSM/NIDS framework, passive logging | IDS/IPS, active detection/prevention |
| **Focus**           | Network analysis, event-based detection, threat hunting | Signature-based detection, packet inspection |
| **Detection**       | Logs events (e.g., connections, protocols); uses scripts for correlation | Matches packets to signatures; alerts on known threats |
| **Pros**            | - Deep traffic visibility<br>- Rich logs (50+)<br>- Scripting for complex threats<br>- Ideal for forensics | - Easy rule creation<br>- Cisco Talos support<br>- Community rules<br>- Fast for known attacks |
| **Cons**            | - Steep learning curve<br>- Analysis is manual or scripted | - Limited to detect complex threats<br>- Less visibility |
| **Use Cases**       | - In-depth investigations<br>- Threat hunting<br>- Chained event analysis | - Intrusion detection/prevention<br>- Stopping known threats |
| **Output**          | Tab-separated logs (e.g., `conn.log`) | Alerts in `/var/log/snort/alert` |

**Key Difference**: Zeek logs everything for analysis, ideal for understanding “what happened,” while Snort focuses on detecting and stopping known threats with signatures.

## 4. Installing Zeek

### 4.1 Prerequisites
- **OS**: Linux (preferred, e.g., Ubuntu, CentOS), Windows (less common).
- **Dependencies**: `libpcap`, `OpenSSL`, `zlib`, `pcre`, and build tools (`cmake`, `gcc`).
- **Permissions**: Root access (`sudo`) for installation and service management.
- **Network**: Interface for live traffic or PCAPs for offline analysis.

### 4.2 Installation on Linux
1. **Install Dependencies** (Ubuntu/Debian):
   ```bash
   sudo apt-get update
   sudo apt-get install -y cmake make gcc g++ flex bison libpcap-dev libssl-dev python3 python3-dev swig zlib1g-dev
   ```
2. **Download Zeek**:
   - From source: [Zeek Downloads](https://zeek.org/download.html)
   - ```bash
     wget https://download.zeek.org/zeek-6.2.0.tar.gz
     tar -xzvf zeek-6.2.0.tar.gz
     cd zeek-6.2.0
     ```
3. **Build and Install**:
   ```bash
   ./configure
   make
   sudo make install
   ```
   - Default path: `/opt/zeek`.
4. **Verify Installation**:
   ```bash
   zeek -v
   ```
   - Output: `zeek version 2.6.0` (or similar, confirms installation).

### 4.3 Post-Installation
- **Default Config**: `/opt/zeek/etc/node.cfg` (network interface) and `/opt/zeek/etc/zeekctl.cfg` (ZeekControl settings).
- **Log Path**: `/opt/zeek/logs/`.
- **Scripts**: `/opt/zeek/share/zeek/base` (base scripts), `/opt/zeek/share/zeek/site` (user scripts).

## 5. Zeek Architecture

Zeek operates in two layers:
- **Event Engine**: Processes packets, extracting metadata (e.g., source/destination IPs, protocols, sessions) without deep content analysis.
- **Policy Script Interpreter**: Performs semantic analysis using Zeek scripts to correlate events and generate logs.

This separation allows Zeek to efficiently handle high traffic volumes while enabling flexible analysis via scripting.

## 6. Zeek Frameworks

Zeek’s frameworks extend functionality via scripts. Common frameworks include:
- **Logging**: Generates structured logs (e.g., `conn.log`, `http.log`).
- **Notice**: Creates alerts for anomalies (`notice.log`).
- **File Analysis**: Extracts and analyzes files (`files.log`).
- **Intelligence**: Matches traffic to threat feeds (`intel.log`).
- **GeoLocation**: Adds location data to logs (requires packages).

Example: Loading a framework:
```plaintext
@load base/frameworks/logging
```

## 7. Zeek Logs

Zeek generates 50+ logs in 7 categories, stored as tab-separated ASCII files in `/opt/zeek/logs/` (service mode) or the working directory (PCAP mode).

### 7.1 Log Categories
| **Category** | **Description** | **Examples** |
|--------------|------------------|--------------|
| **Network** | Protocol logs | `conn.log`, `http.log`, `dns.log`, `ftp.log`, `ssl.log` |
| **Files** | File analysis | `files.log`, `x509.log`, `pe.log` |
| **NetControl** | Flow control | `netcontrol.log`, `netcontrol_drop.log` |
| **Detection** | Indicators/anomalies | `intel.log`, `notice.log`, `signatures.log` |
| **Network Observations** | Host/service summaries | `known_hosts.log`, `known_services.log` |
| **Miscellaneous** | External alerts, failures | `weird.log`, `dpd.log` |
| **Zeek Diagnostic** | System stats | `stats.log`, `reporter.log` |

### 7.2 Key Logs
| **Log** | **Description** | **Update Frequency** |
|---------|------------------|----------------------|
| `conn.log` | All connections | Per session |
| `http.log` | HTTP traffic | Per session |
| `notice.log` | Anomalies | Per session |
| `intel.log` | Threat matches | Per session |
| `known_hosts.log` | TCP handshake hosts | Daily |
| `known_services.log` | Services used | Daily |

### 7.3 Log Analysis
- **Structure**: Tab-separated, with fields like `ts` (timestamp), `uid` (session ID), `id.orig_h` (source IP).
- **Tools**: Use `cat`, `grep`, `cut`, `sort`, `uniq`, and `zeek-cut` for analysis.
- **Example** (extracting connection details):
  ```bash
  cat conn.log | zeek-cut uid proto id.orig_h id.orig_p id.resp_h id.resp_p
  ```
  - Output: `C123abc... udp 192.168.1.2 51153 192.168.1.1 53`

## 8. Working with Zeek

Zeek operates in two modes: **service mode** (live traffic) and **PCAP mode** (offline analysis).

### 8.1 Service Mode (Live Traffic)
- **Tool**: ZeekControl (`zeekctl`) manages the service.
- **Commands**:
  ```bash
  sudo zeekctl status  # Check service status
  sudo zeekctl start   # Start Zeek
  sudo zeekctl stop    # Stop Zeek
  ```
- **Logs**: Saved to `/opt/zeek/logs/`.
- **Example**:
  ```bash
  sudo su
  zeekctl start
  zeekctl status
  ```
  - Output: `Name Type Host Status Pid Started zeek standalone localhost running 2541 27 May 12:14:25`

### 8.2 PCAP Mode (Offline Analysis)
- **Command**:
  ```bash
  zeek -C -r sample.pcap
  ```
  - **Line-by-Line**:
    - `-C`: Ignores checksum errors.
    - `-r sample.pcap`: Processes the PCAP file.
  - **Output**: Logs like `conn.log`, `dns.log` in the working directory.
- **List Logs**:
  ```bash
  ls -l
  ```
  - Output: `conn.log dhcp.log dns.log packet_filter.log`

### 8.3 Key Parameters
| **Parameter** | **Description** |
|---------------|-----------------|
| `-r <file>`   | Read PCAP file |
| `-C`          | Ignore checksum errors |
| `-v`          | Show Zeek version |
| `zeekctl`     | Manage Zeek service |

## 9. Zeek Signatures

Zeek supports signatures for pattern matching, similar to Snort rules, but they’re secondary to scripting.

### 9.1 Signature Structure
- **Signature ID**: Unique name (e.g., `http-password`).
- **Conditions**:
  - **Header**: Source/destination IPs, ports, protocols.
  - **Content**: Payload patterns (e.g., `password`).
- **Action**: Log to `signatures.log`, `notice.log`, or trigger scripts.

### 9.2 Example Signature (HTTP Password)
```plaintext
signature http-password {
  ip-proto == tcp
  dst-port == 80
  payload /.*password.*/
  event "Cleartext Password Found!"
}
```
- **Run**:
  ```bash
  zeek -C -r http.pcap -s http-password.sig
  ```
- **Output** (in `notice.log`):
  ```bash
  cat notice.log | zeek-cut id.orig_h id.resp_h msg
  ```
  - `10.10.57.178 44.228.249.3 10.10.57.178: Cleartext Password Found!`

### 9.3 FTP Brute-Force Signature
```plaintext
signature ftp-admin {
  ip-proto == tcp
  dst-port == 21
  ftp /.*(admin|administrator).*/
  event "FTP Admin Login Attempt!"
}
```
- **Run**:
  ```bash
  zeek -C -r ftp.pcap -s ftp-admin.sig
  ```
- **Output** (in `signatures.log`):
  ```bash
  cat signatures.log | zeek-cut src_addr dst_addr event_msg
  ```
  - `10.234.125.254 10.121.70.151 10.234.125.254: FTP Admin Login Attempt!`

## 10. Zeek Scripts

Zeek’s scripting language enables event-driven analysis, automating tasks like data extraction and correlation.

### 10.1 Script Locations
- **Base Scripts**: `/opt/zeek/share/zeek/base` (don’t modify).
- **User Scripts**: `/opt/zeek/share/zeek/site`.
- **Policy Scripts**: `/opt/zeek/share/zeek/policy`.
- **Config File**: `/opt/zeek/share/zeek/site/local.zeek` (load scripts here).

### 10.2 Basic Script Example
```plaintext
event zeek_init() {
  print "Started Zeek!";
}
event zeek_done() {
  print "Stopped Zeek!";
}
```
- **Run**:
  ```bash
  zeek -C -r sample.pcap 101.zeek
  ```
- **Output**: `Started Zeek! ... Stopped Zeek!`

### 10.3 Extracting Connections
```plaintext
event new_connection(c: connection) {
  print fmt("New Connection Found! Source: %s:%d -> Destination: %s:%d",
            c$id$orig_h, c$id$orig_p, c$id$resp_h, c$id$resp_p);
}
```
- **Run**:
  ```bash
  zeek -C -r sample.pcap 103.zeek
  ```
- **Output**: `New Connection Found! Source: 192.168.121.2:58304 -> Destination: 192.168.120.22:53`

### 10.4 Combining Signatures and Scripts
```plaintext
event signature_match(state: signature_state, msg: string, data: string) {
  if (state$sig_id == "ftp-admin")
    print fmt("Signature hit! --> #%s", msg);
}
```
- **Run**:
  ```bash
  zeek -C -r ftp.pcap -s ftp-admin.sig 201.zeek
  ```
- **Output**: `Signature hit! --> #FTP-Admin`

## 11. Zeek Frameworks in Action

### 11.1 File Framework (Hashing)
- **Script**:
  ```plaintext
  @load base/frameworks/files/hash-all-files
  ```
- **Run**:
  ```bash
  zeek -C -r case1.pcap hash-demo.zeek
  ```
- **Output** (in `files.log`):
  ```bash
  cat files.log | zeek-cut md5 sha1
  ```
  - `cd5a4d3... 33bf88d...`

### 11.2 File Extraction
- **Script**:
  ```plaintext
  @load base/frameworks/files/extract-all-files
  ```
- **Run**:
  ```bash
  zeek -C -r case1.pcap /opt/zeek/share/zeek/policy/frameworks/files/extract-all-files.zeek
  ```
- **Output**: Files in `extract_files/` directory (e.g., `.exe`, `.docx`).

## 12. Zeek Package Manager

Zeek’s package manager (`zkg`) extends functionality with third-party scripts.

### 12.1 Commands
```bash
zkg install zeek/cybera/zeek-sniffpass  # Install package
zkg list                                 # List installed
zkg remove zeek/cybera/zeek-sniffpass    # Remove package
```

### 12.2 Example: Sniffpass (Cleartext Passwords)
- **Install**:
  ```bash
  zkg install zeek/cybera/zeek-sniffpass
  ```
- **Run**:
  ```bash
  zeek -C -r http.pcap zeek-sniffpass
  ```
- **Output** (in `notice.log`):
  ```bash
  cat notice.log | zeek-cut id.orig_h note msg
  ```
  - `10.10.57.178 SNIFFPASS::HTTP_POST_Password_Seen Password found for user ZeekBro`

## 13. Best Practices
- **Learn Logs**: Study key logs (`conn.log`, `http.log`, `notice.log`) to understand traffic.
- **Use zeek-cut**: Simplify log analysis (e.g., `cat conn.log | zeek-cut uid id.orig_h`).
- **Script Smartly**: Start with simple scripts, then explore frameworks and packages.
- **Correlate Events**: Use `uid` to link logs (e.g., `conn.log` to `http.log`).
- **Secure Logs**: Restrict access to `/opt/zeek/logs`:
  ```bash
  sudo chown root:root /opt/zeek/logs/*
  ```
- **Update Regularly**: Check for new packages and scripts via `zkg refresh`.

## 14. Real-World Scenarios

### Scenario 1: Detecting HTTP Passwords
- **Goal**: Identify cleartext password submissions.
- **Steps**:
  1. Use `zeek-sniffpass` package:
     ```bash
     zeek -C -r http.pcap zeek-sniffpass
     ```
  2. Check `notice.log`:
     ```bash
     cat notice.log | zeek-cut id.orig_h msg
     ```
     - Output: `10.10.57.178 Password found for user ZeekBro`

### Scenario 2: Investigating FTP Brute-Force
- **Goal**: Detect FTP login attempts.
- **Steps**:
  1. Use signature:
     ```plaintext
     signature ftp-admin {
       ip-proto == tcp
       dst-port == 21
       ftp /.*(admin|administrator).*/
       event "FTP Admin Login Attempt!"
     }
     ```
  2. Run:
     ```bash
     zeek -C -r ftp.pcap -s ftp-admin.sig
     ```
  3. Check `signatures.log`:
     ```bash
     cat signatures.log | zeek-cut src_addr event_msg
     ```
     - Output: `10.234.125.254 FTP Admin Login Attempt!`

### Scenario 3: Extracting Files from PCAP
- **Goal**: Analyze transferred files.
- **Steps**:
  1. Run:
     ```bash
     zeek -C -r case1.pcap /opt/zeek/share/zeek/policy/frameworks/files/extract-all-files.zeek
     ```
  2. Check `extract_files/` and `files.log`:
     ```bash
     ls extract_files
     cat files.log | zeek-cut fuid mime_type
     ```
     - Output: `.exe`, `.docx` files detected.

## 15. Conclusion
Zeek is a powerful tool for network security monitoring, offering unmatched visibility through logs and scripting. By practicing with its logs, signatures, scripts, and frameworks, you can master threat detection and analysis. Keep exploring Zeek’s documentation and packages to stay ahead in network security. Happy Zeeking!

## Key Citations
- [Zeek Official Website](https://zeek.org/)
- [Zeek Documentation](https://docs.zeek.org/)
- [Corelight Log Cheat Sheet](https://corelight.com/resources/zeek-log-cheat-sheet)
