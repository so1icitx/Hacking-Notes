# Wireshark Basics Guide

## Introduction

**Wireshark** is an open-source, cross-platform network packet analyzer that captures and inspects live network traffic or analyzes packet capture (PCAP) files. It’s a leading tool for network troubleshooting, security analysis, and protocol study, used by network administrators, security analysts, and students. 
## Prerequisites

- Basic networking knowledge (e.g., IP addresses, ports, TCP, UDP, HTTP).
- Familiarity with the OSI model.
- Wireshark installed on your system (Windows, macOS, Linux).
- Optional: Review Wireshark’s documentation for filter syntax.

## What Wireshark Does

Wireshark captures and analyzes network packets to:
- **Troubleshoot Networks**: Identify congestion, latency, or connection failures.
- **Detect Security Issues**: Spot rogue hosts, suspicious traffic, or abnormal port usage.
- **Study Protocols**: Examine response codes, payloads, and protocol behavior.

**Note**: Wireshark is not an Intrusion Detection System (IDS). It reads packets without modifying them, relying on the analyst’s expertise to interpret data.

## Installing Wireshark

1. Download from [wireshark.org](https://www.wireshark.org).
2. Install on your OS:
   - **Windows/macOS**: Run the installer.
   - **Linux**: Use package manager, e.g., `sudo apt install wireshark` (Ubuntu).
3. Ensure permissions for packet capture (e.g., add user to `wireshark` group on Linux).
4. Launch Wireshark:
   ```
   wireshark &
   ```

## Wireshark GUI Overview

Wireshark’s GUI is a single window with five key sections:

1. **Toolbar**:
   - Menus: File, Edit, View, Capture, Statistics, Analyze.
   - Shortcuts for capturing, filtering, saving, and exporting.

2. **Display Filter Bar**:
   - Enter queries to filter visible packets.
   - Color-coded: Green (valid), Red (invalid), Yellow (warning).

3. **Recent Files**:
   - Lists recently opened PCAPs for quick access.

4. **Capture Filter and Interfaces**:
   - Lists network interfaces (e.g., `eth0`, `lo`) for live capture.
   - Apply capture filters to limit sniffed traffic.

5. **Status Bar**:
   - Shows capture status, interface, packet counts, and profile.

When a PCAP is loaded or traffic is captured, three panes display:
- **Packet List Pane**: Summarizes packets (source, destination, protocol, info).
- **Packet Details Pane**: Breaks down selected packet’s protocols (OSI layers).
- **Packet Bytes Pane**: Shows hex and ASCII representations, highlighting selected fields.

## Core Features

### Loading PCAP Files

To analyze a PCAP:
1. File > Open > Select a `.pcap` or `.pcapng` file.
2. Drag and drop the file or double-click in Recent Files.
3. View packets in the Packet List, Details, and Bytes panes.

### Packet Coloring

Wireshark colors packets to identify protocols or anomalies:
- **Default Rules**: TCP (green), HTTP (light green), errors (red).
- **Customize**:
  - Permanent: View > Coloring Rules.
  - Temporary: Right-click packet > Conversation Filter.
- **Toggle**: View > Colorize Packet List.

### Traffic Sniffing

Capture live traffic:
1. Select an interface (e.g., `eth0`) in the Capture section.
2. Click the blue shark button to start.
3. Stop with the red square or restart with the green arrow.
4. Status Bar shows captured packet counts.

**Note**: Requires sufficient permissions (e.g., root on Linux).

### Merging PCAP Files

Combine PCAPs:
1. File > Merge > Select a second PCAP.
2. Save the merged file: File > Save As.

### Viewing File Details

Inspect PCAP metadata:
- Statistics > Capture File Properties.
- Shows file hash, capture time, interfaces, and comments.

### Packet Dissection

Wireshark decodes packets into OSI layers:
- **Layer 1 (Frame)**: Physical layer (e.g., timestamp, frame size).
- **Layer 2 (Ethernet)**: MAC addresses.
- **Layer 3 (IP)**: Source/destination IPs.
- **Layer 4 (TCP/UDP)**: Ports, sequence numbers.
- **Layer 5 (Application)**: Protocols like HTTP, DNS, with payloads.
- **Errors**: Flags issues like TCP retransmissions.

Click a packet in the Packet List to view details; selected fields highlight in the Bytes pane.

### Packet Numbers

Each packet is numbered for navigation:
- Status Bar shows total packets.
- Go > Go to Packet > Enter a number to jump.

### Finding Packets

Search packet contents:
1. Edit > Find Packet.
2. Input types: Display Filter, Hex, String, Regex.
3. Search in: Packet List, Details, or Bytes.
4. Example: Search for “login” (String, case-insensitive).

### Marking Packets

Highlight packets:
- Right-click > Mark/Unmark Packet.
- Marked packets appear black (resets on file close).

### Packet Comments

Add notes:
- Right-click > Packet Comment.
- Comments persist in the PCAP until removed.

### Exporting Packets

Save specific packets:
- File > Export Specified Packets > Select range or marked packets.

### Exporting Objects

Extract files (e.g., images, HTML):
- File > Export Objects > HTTP (supports DICOM, IMF, SMB, TFTP).

### Time Display Format

Adjust timestamps:
- View > Time Display Format > UTC Date and Time (recommended).
- Default: Seconds Since Capture Start.

### Expert Info

Detect anomalies:
- Analyze > Expert Information.
- Severities:
  - Chat (Blue): Normal activity.
  - Note (Cyan): Notable events.
  - Warn (Yellow): Warnings (e.g., retransmissions).
  - Error (Red): Malformed packets.
- Groups: Checksum, Malformed, etc.

## Packet Filtering

Wireshark’s filtering reduces noise to focus on relevant traffic.

### Capture Filters

- Applied before capturing to limit traffic.
- Use Berkeley Packet Filter (BPF) syntax ([pcap-filter](https://www.wireshark.org/docs/man-pages/pcap-filter.html)).
- Syntax:
  - **Scope**: `host`, `net`, `port`, `portrange`.
  - **Direction**: `src`, `dst`.
  - **Protocol**: `ether`, `ip`, `tcp`, `udp`.
- Examples:
  - `tcp port 80`: Capture HTTP traffic.
  - `host 192.168.1.100`: Traffic to/from an IP.
  - `net 10.0.0.0/8`: Subnet traffic ([GitLab CaptureFilters](https://gitlab.com/wireshark/wireshark/-/wikis/CaptureFilters#useful-filters)).
  - `not arp`: Exclude ARP traffic ([Wireshark Wiki](https://wiki.wireshark.org/CaptureFilters)).
- Set via Capture > Capture Filters.

**Note**: Incorrect capture filters may miss critical traffic.

### Display Filters

- Applied post-capture to filter visible packets.
- Protocol-based syntax (3000+ protocols supported).
- Examples:
  - `ip.addr==192.168.1.100`: Filter IP address.
  - `tcp.port==443`: Filter HTTPS traffic.
  - `http.request.method=='GET'`: Filter GET requests.
  - `dns.qry.type==1`: Filter DNS A records.
- Enter in the Display Filter Bar.

**Comparison Operators**:
- `==` (equal), `!=` (not equal), `>` (greater), `<` (less), `>=`, `<=`.

**Logical Operators**:
- `&&` (and), `||` (or), `!` (not).
- Example: `ip.src==192.168.1.100 && tcp.port==80`.

**Advanced Operators**:
- `contains`: `http.server contains 'Apache'`.
- `matches`: `http.host matches "\.(php|html)"`.
- `in`: `tcp.port in {80 443 8080}`.
- `upper`: `upper(http.server) contains 'APACHE'`.
- `lower`: `lower(http.server) contains 'apache'`.
- `string`: `string(frame.number) matches "[13579]$"`.

**Display Filter Expressions**:
- Analyze > Display Filter Expression.
- GUI tool to build filters by selecting protocols and fields.

### Filtering Techniques

1. **Apply as Filter**:
   - Right-click field (e.g., `ip.src`) > Apply as Filter > Selected.
   - Auto-generates filter (e.g., `ip.src==192.168.1.100`).

2. **Conversation Filter**:
   - Right-click packet > Conversation Filter > TCP.
   - Shows related packets by IP/port.

3. **Colorize Conversation**:
   - Right-click packet > Colorize Conversation.
   - Highlights related packets.

4. **Prepare as Filter**:
   - Right-click field > Prepare a Filter > Selected.
   - Adds query to filter bar for editing.

5. **Apply as Column**:
   - Right-click field > Apply as Column.
   - Adds field to Packet List (e.g., `http.request.method`).

6. **Follow Stream**:
   - Right-click packet > Follow > TCP/UDP/HTTP Stream.
   - Reconstructs application data (e.g., HTTP requests, usernames).

## Statistics

Summarize traffic via the Statistics menu:
- **Resolved Addresses**: Lists IPs and DNS hostnames.
- **Protocol Hierarchy**: Shows protocol distribution (e.g., TCP vs. HTTP).
- **Conversations**: Lists endpoint pairs (IP, TCP, UDP).
- **Endpoints**: Lists unique endpoints with MAC resolution.
- **IPv4/IPv6 Statistics**: Filters by IP version.
- **DNS**: Breaks down DNS queries and responses.
- **HTTP**: Analyzes requests and response codes.

## Profiles

Save custom settings:
- Edit > Configuration Profiles.
- Create profiles for different tasks (e.g., security vs. troubleshooting).
- Switch via Status Bar > Profile.

## Name Resolution and GeoIP

- **Enable Resolution**:
  - Edit > Preferences > Name Resolution.
  - Resolve MAC addresses, IPs, and ports.
- **GeoIP**:
  - Add MaxMind DB: Edit > Preferences > Name Resolution > MaxMind DB Path.
  - Shows geolocation for IPs in Packet Details.

## Practical Tips

1. **Start Simple**: Filter common protocols:
   ```
   http
   dns
   tcp.port==80
   ```
2. **Follow Streams**: Reconstruct HTTP or TCP conversations.
3. **Use Statistics**: Check Protocol Hierarchy for traffic overview.
4. **Bookmark Filters**: Save frequent filters (e.g., `http.request`) via Display Filter Bar.
5. **Practice Filters**:
   - Capture: `tcp port 80 and host 192.168.1.100`.
   - Display: `ip.addr==192.168.1.100 && http.response.code==404`.

## Conclusion

Wireshark is a powerful tool for network analysis, offering deep insights into traffic through packet sniffing, dissection, and filtering. This guide covered its GUI, core features, and filtering techniques (capture and display), enabling you to troubleshoot networks, detect anomalies, and study protocols. Experiment with filters and statistics to build your skills. Happy analyzing!

## Citations

- [Wireshark pcap-filter man page](https://www.wireshark.org/docs/man-pages/pcap-filter.html)
- [Wireshark GitLab CaptureFilters](https://gitlab.com/wireshark/wireshark/-/wikis/CaptureFilters#useful-filters)
- [Wireshark Wiki CaptureFilters](https://wiki.wireshark.org/CaptureFilters)
- Wireshark Official Documentation
