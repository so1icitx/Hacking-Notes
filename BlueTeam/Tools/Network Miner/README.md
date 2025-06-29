# NetworkMiner for Beginners: A Comprehensive Guide to Network Forensic Analysis

This guide is your ultimate resource for mastering **NetworkMiner**, an open-source Network Forensic Analysis Tool (NFAT) developed and maintained by Netresec. I’ve created a **long, detailed, and beginner-friendly** `README.md` to help you understand NetworkMiner’s core features, installation, usage, and real-world applications, tailored to stick in your cybersecurity notebook. It’s packed with clear explanations, practical examples, and step-by-step breakdowns of its capabilities, making it perfect for learning network forensics alongside your previous queries about Snort and Zeek. This guide also compares NetworkMiner with Wireshark and highlights its strengths in forensic investigations. Let’s dive in and make NetworkMiner your go-to tool for uncovering network insights!

### Key Points
- **What NetworkMiner Does**: A passive network sniffer and PCAP parser that extracts hosts, sessions, files, credentials, and anomalies without generating network traffic.
- **Beginner-Friendly**: Easy-to-use GUI for quick overviews, ideal for Windows (also works on Linux/Mac/FreeBSD), but not suited for active sniffing or deep packet analysis.
- **Core Features**: OS fingerprinting, file extraction, credential grabbing, and keyword parsing, perfect for incident response and law enforcement.
- **Compared to Wireshark**: NetworkMiner provides a high-level overview and automated data extraction, while Wireshark excels at in-depth packet analysis.
- **Learning Curve**: Straightforward for quick insights, but limited filtering and manual analysis require pairing with tools like Wireshark for complex investigations.

## 1. Introduction to NetworkMiner

NetworkMiner is an open-source **Network Forensic Analysis Tool (NFAT)** developed by Netresec since 2007 ([Netresec Official Site](https://www.netresec.com/?page=NetworkMiner)). It’s a passive tool designed to analyze network traffic, either live or from PCAP files, to detect operating systems, sessions, hostnames, open ports, and more without sending packets. Widely used by incident response teams, law enforcement, and security analysts, NetworkMiner excels at providing a quick overview of network activity for forensic investigations.

### 1.1 What NetworkMiner Does
- **Sniffs Traffic**: Captures live packets passively (Windows-only sniffing).
- **Parses PCAPs**: Analyzes captured traffic for detailed insights.
- **Extracts Data**: Reassembles files, images, emails, and certificates from traffic.
- **Identifies Hosts**: Maps IPs, MACs, hostnames, and OSes using fingerprinting.
- **Grabs Credentials**: Extracts passwords, hashes, and cookies from protocols like HTTP, FTP, and SMTP.
- **Detects Anomalies**: Flags potential issues like port scans or EternalBlue exploits.
- **Parses Keywords**: Identifies cleartext strings for quick evidence discovery.

### 1.2 Why NetworkMiner Matters
- **Forensic Focus**: Provides context for investigations (e.g., host details, attack indicators).
- **User-Friendly**: GUI simplifies analysis, ideal for beginners and quick overviews.
- **Open-Source**: Free, with a professional version for advanced features (e.g., OSINT lookups).
- **Cross-Platform**: Runs on Windows, Linux, Mac OS X, and FreeBSD, though sniffing is Windows-only.

For beginners, NetworkMiner’s intuitive interface makes it easy to start, but its limitations in active sniffing and deep analysis mean it’s best used alongside tools like Wireshark or Zeek.

## 2. Network Forensics Overview

Network forensics aims to detect malicious activities, security breaches, and anomalies by analyzing network traffic. NetworkMiner supports this by providing:
- **Host Context**: IPs, MACs, hostnames, and OS information.
- **Attack Indicators**: Traffic spikes, port scans, or tool signatures (e.g., Nmap).
- **Data Extraction**: Files, credentials, and keywords for evidence.

### 2.1 Supported Data Types
- **Live Traffic**: Captures real-time packets (Windows-only, less reliable).
- **Traffic Captures**: Analyzes PCAP files for offline forensics.
- **Log Files**: Not directly supported, but extracted data (e.g., credentials) can be correlated with logs.

## 3. NetworkMiner vs. Wireshark

NetworkMiner and Wireshark both analyze network traffic, but their purposes differ significantly:

| **Feature**                | **NetworkMiner**                              | **Wireshark**                                 |
|----------------------------|-----------------------------------------------|----------------------------------------------|
| **Purpose**                | Quick overview, data extraction, forensics   | In-depth packet analysis                    |
| **GUI**                    | ✅ Easy-to-use, tabbed interface               | ✅ Detailed, customizable                   |
| **Sniffing**               | ✅ Windows-only, not primary function         | ✅ Robust, cross-platform                   |
| **PCAP Handling**          | ✅ Parses for quick insights                   | ✅ Detailed packet decoding                  |
| **OS Fingerprinting**      | ✅ Uses Satori, p0f                           | ❌ Manual or via plugins                     |
| **Credential Discovery**   | ✅ Automated extraction                       | ✅ Manual or scripted                        |
| **File Extraction**        | ✅ Automated, user-friendly                    | ✅ Manual reassembly                         |
| **Filtering Options**      | Limited                                      | ✅ Extensive BPF and display filters         |
| **Protocol Analysis**      | ❌ Basic                                      | ✅ Comprehensive                             |
| **Payload Analysis**       | ❌ Limited                                    | ✅ Detailed                                  |
| **Statistical Analysis**   | ❌ None                                       | ✅ Built-in graphs, stats                    |
| **Best Use Case**         | Initial PCAP overview, low-hanging fruit      | Deep packet inspection, protocol debugging  |

**Best Practice**: Use NetworkMiner for a quick PCAP overview, then switch to Wireshark for detailed analysis.

## 4. Installing NetworkMiner

### 4.1 Prerequisites
- **OS**: Windows (preferred), Linux/Mac OS X/FreeBSD (less common).
- **Dependencies**: .NET Framework (Windows), Mono (Linux/Mac for non-sniffing features).
- **Permissions**: Admin/root for sniffing; standard user for PCAP parsing.
- **PCAP Files**: Optional for offline analysis.

### 4.2 Installation on Windows
1. **Download**: Get the free version from [Netresec Downloads](https://www.netresec.com/?page=NetworkMiner).
   - Example: `NetworkMiner_2-8-1.zip`.
2. **Extract**: Unzip to a directory (e.g., `C:\NetworkMiner`).
3. **Run**:
   ```bash
   NetworkMiner.exe
   ```
4. **Verify**: Launch the GUI; no installation required (portable).

### 4.3 Installation on Linux
1. **Install Mono**:
   ```bash
   sudo apt update
   sudo apt install -y mono-complete
   ```
2. **Download and Extract**: Same as Windows.
3. **Run**:
   ```bash
   mono NetworkMiner.exe
   ```
   - Note: Sniffing not supported; use for PCAP parsing only.

### 4.4 Post-Installation
- **Professional Version**: Optional paid version for OSINT lookups, advanced features.
- **Default Path**: Logs and extracted files saved in the working directory.
- **Updates**: Check via Help > Check for Updates.

## 5. NetworkMiner Operating Modes

NetworkMiner operates in two modes:

### 5.1 Sniffer Mode
- **Description**: Captures live traffic passively (Windows-only).
- **Limitations**: Less reliable than Wireshark or tcpdump; not recommended as a primary sniffer.
- **Use Case**: Quick live analysis when other sniffers are unavailable.
- **Command** (GUI-based):
  - Select a network interface in the GUI and click “Start.”

### 5.2 Packet Parsing/Processing Mode
- **Description**: Analyzes PCAP files for forensic insights.
- **Strengths**: Extracts hosts, sessions, files, credentials, and keywords automatically.
- **Use Case**: Initial investigation to identify “low-hanging fruit” before deep analysis.
- **Command** (GUI-based):
  - File > Open > Select PCAP file.

## 6. NetworkMiner Capabilities

### 6.1 Traffic Sniffing
- Captures packets passively on Windows.
- Logs data like IPs, ports, and protocols.
- Example: Select interface in GUI, view hosts in “Hosts” tab.

### 6.2 PCAP Parsing
- Processes PCAP files to extract structured data.
- Example: Load `capture.pcap`, view sessions in “Sessions” tab.

### 6.3 Protocol Analysis
- Identifies protocols (e.g., HTTP, FTP, DNS) but lacks deep decoding.
- Example: “Sessions” tab shows protocol per session.

### 6.4 OS Fingerprinting
- Uses Satori and p0f to identify OSes based on packet characteristics.
- Example: “Hosts” tab shows OS (e.g., “Windows 10”, “Ubuntu”).

### 6.5 File Extraction
- Reassembles files (e.g., images, emails, HTML) from traffic.
- Example: “Files” tab lists extracted files with metadata.

### 6.6 Credential Grabbing
- Extracts credentials (e.g., FTP, HTTP, NTLM hashes).
- Example: “Credentials” tab shows usernames/passwords.

### 6.7 Cleartext Keyword Parsing
- Identifies strings in cleartext traffic.
- Example: “Keywords” tab lists terms like “password”.

## 7. NetworkMiner GUI Overview

NetworkMiner’s GUI is its primary interface, with tabs for different data types.

### 7.1 File Menu
- **Actions**:
  - Open PCAP file.
  - Receive PCAP over IP (advanced, not covered).
  - Drag-and-drop PCAP support.
- **Example**:
  - File > Open > `capture.pcap`.

### 7.2 Tools Menu
- **Actions**:
  - Clear dashboard to reset analysis.
  - Remove captured data.
- **Example**:
  - Tools > Clear All.

### 7.3 Help Menu
- **Actions**:
  - Check for updates.
  - View version info.
- **Example**:
  - Help > Check for Updates.

### 7.4 Case Panel
- Lists loaded PCAPs with metadata (e.g., file size, date).
- **Actions**: Reload, remove, view details.
- **Example**: Right-click `capture.pcap` > Show Metadata.

### 7.5 Hosts Tab
- **Data**:
  - IP, MAC, hostname, OS, open ports, packets sent/received.
- **Features**:
  - Sort by column (e.g., IP).
  - Color-code hosts.
  - Right-click to copy values.
- **Example**:
  - Shows `192.168.1.100`, OS: “Windows 10”, Ports: “80, 443”.

### 7.6 Sessions Tab
- **Data**:
  - Frame number, client/server IPs, ports, protocol, start time.
- **Features**:
  - Filter by keyword (e.g., “HTTP”).
  - Search types: ExactPhrase, AllWords, AnyWord, RegExe.
- **Example**:
  - Shows session: `192.168.1.100:49152 -> 8.8.8.8:53 (DNS)`.

### 7.7 DNS Tab
- **Data**:
  - Frame number, client/server, ports, DNS query/answer, TTL.
- **Features**:
  - Filter queries (e.g., “google.com”).
- **Example**:
  - Query: `example.com`, Answer: `93.184.216.34`.

### 7.8 Credentials Tab
- **Data**:
  - Protocol, username, password, hashes (e.g., NTLM, Kerberos).
- **Features**:
  - Copy credentials for cracking with Hashcat/John the Ripper.
- **Example**:
  - FTP: `user:admin pass:password123`.

### 7.9 Files Tab
- **Data**:
  - Filename, extension, size, IPs, ports, protocol, timestamp.
- **Features**:
  - Open files, view reconstructed paths.
- **Example**:
  - File: `image.jpg`, Size: 50KB, Protocol: HTTP.

### 7.10 Images Tab
- **Data**: Extracted images with source/destination details.
- **Features**: Zoom, open, view metadata on hover.
- **Example**: Displays `logo.png` from HTTP traffic.

### 7.11 Parameters Tab
- **Data**: Parameter names/values, frame number, IPs, ports.
- **Features**: Copy parameters for analysis.
- **Example**: Parameter: `search=login`, Value: `admin`.

### 7.12 Keywords Tab
- **Data**: Cleartext keywords, context, frame number, IPs.
- **Features**:
  - Add keywords, reload PCAP to filter.
  - Example: Add “password”, reload to find matches.
- **Example**: Keyword: `secret`, Context: `password=secret`.

### 7.13 Messages Tab
- **Data**: Emails, chats, sender/receiver, protocol, timestamp.
- **Features**: View attachments, use built-in viewer.
- **Example**: Email from `user@example.com` via SMTP.

### 7.14 Anomalies Tab
- **Data**: Detected issues (e.g., EternalBlue, spoofing).
- **Note**: Limited IDS capabilities; not a primary detection tool.
- **Example**: Flags `EternalBlue exploit attempt`.

## 8. Version Differences

NetworkMiner has evolved, with key differences between versions 1.6 and 2.7:

| **Feature**                | **Version 1.6**                              | **Version 2.7**                              |
|----------------------------|---------------------------------------------|---------------------------------------------|
| **MAC Address Processing** | ❌ No conflict detection                    | ✅ Detects MAC conflicts                     |
| **Frame Processing**       | ✅ Detailed frame info                      | ❌ Removed                                  |
| **Parameter Processing**   | Limited                                    | ✅ Extensive parameter extraction            |
| **Cleartext Processing**   | ✅ Dedicated cleartext tab                  | ❌ Integrated into Keywords tab              |
| **Stability/Features**     | Basic                                      | Improved, with new forensic tools           |

**Recommendation**: Use 2.7 for modern features, 1.6 for legacy frame/cleartext analysis.

## 9. Using NetworkMiner

### 9.1 Loading a PCAP
- **Steps**:
  1. Open NetworkMiner.
  2. File > Open > Select `capture.pcap`.
  3. Wait for parsing to complete.
- **Output**: Populates tabs (Hosts, Sessions, etc.).
- **Example**:
  - Load `http_traffic.pcap`, view hosts in “Hosts” tab.

### 9.2 Sniffing Live Traffic (Windows)
- **Steps**:
  1. Open NetworkMiner.
  2. Select network interface (e.g., Ethernet).
  3. Click “Start.”
- **Output**: Real-time data in tabs.
- **Note**: Use Wireshark for reliable sniffing.

### 9.3 Extracting Files
- **Steps**:
  1. Load PCAP.
  2. Go to “Files” or “Images” tab.
  3. Right-click file > Open or Save.
- **Example**: Extract `document.pdf` from HTTP traffic.

### 9.4 Grabbing Credentials
- **Steps**:
  1. Load PCAP.
  2. Go to “Credentials” tab.
  3. Copy username/password or hashes.
- **Example**: Find FTP credentials: `user:guest pass:123456`.

### 9.5 Filtering Keywords
- **Steps**:
  1. Go to “Keywords” tab.
  2. Add keyword (e.g., “login”).
  3. Reload PCAP.
- **Output**: Matches like `login=admin` in context.

## 10. Best Practices
- **Use for Overview**: Start with NetworkMiner for quick insights, then use Wireshark for deep analysis.
- **Combine Tools**: Pair with Zeek for logs or Snort for IDS alerts.
- **Filter Sparingly**: Limited filtering means manual searches in large PCAPs are inefficient.
- **Secure Outputs**: Protect extracted files/credentials:
  ```bash
  chmod 600 extracted_files/*
  ```
- **Update Regularly**: Check for new versions via Help > Check for Updates.
- **Correlate Data**: Use “Hosts” and “Sessions” tabs to map activity before diving into specifics.


## 12. Conclusion
NetworkMiner is an excellent tool for network forensics, offering a user-friendly way to extract critical data from PCAPs and live traffic. While limited in active sniffing and deep analysis, its strengths in OS fingerprinting, file extraction, and credential grabbing make it ideal for initial investigations. Practice with its GUI and combine it with Wireshark or Zeek for comprehensive analysis. Happy forensic hunting!

## Key Citations
- [NetworkMiner Official Page](https://www.netresec.com/?page=NetworkMiner)
- [Netresec Blog](https://www.netresec.com/?page=Blog)
- [Satori OS Fingerprinting](https://github.com/satori-network/satori)
