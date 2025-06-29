# Wireshark Intermediate Guide



## Prerequisites

- Familiarity with Wireshark’s GUI, packet dissection, and basic filtering (see [Wireshark Basics Guide](#)).
- Understanding of networking protocols (TCP, UDP, ARP, DHCP, DNS, HTTP, TLS) and the OSI model.
- Knowledge of Nmap scans and common attack patterns (e.g., ARP spoofing, MITM).
- Wireshark installed (v3.1 or later for credential extraction).
- Optional: Browser configured for SSLKEYLOGFILE to decrypt HTTPS traffic.

## Wireshark’s Intermediate Capabilities

Wireshark excels in:
- **Anomaly Detection**: Identifying Nmap scans, ARP spoofing, or tunneling.
- **Host Identification**: Mapping IPs to MACs and usernames via DHCP, NetBIOS, Kerberos.
- **Protocol Analysis**: Inspecting cleartext (FTP, HTTP) and encrypted (HTTPS) traffic.
- **Security Actions**: Extracting credentials and generating firewall rules.

**Note**: Wireshark is not an IDS; it relies on analyst expertise to correlate packet-level data and detect threats.

## Setup

1. Install Wireshark from [wireshark.org](https://www.wireshark.org).
2. Ensure capture permissions (e.g., `sudo wireshark` on Linux).
3. Launch Wireshark:
   ```
   wireshark &
   ```
4. Optionally, set up an SSLKEYLOGFILE environment variable for HTTPS decryption:
   - Linux/macOS: `export SSLKEYLOGFILE=~/keylog.txt`
   - Windows: Set in System Environment Variables.
   - Use Chrome/Firefox to log TLS keys during browsing.

## Advanced Traffic Analysis Techniques

### 1. Detecting Nmap Scans

Nmap scans (TCP Connect, SYN, UDP) create distinct packet patterns. Use filters to identify them.

#### TCP Connect Scans
- **Characteristics**: Completes TCP three-way handshake; non-root users; window size > 1024 bytes; uses `nmap -sT`.
- **Pattern**:
  - Open port: SYN → SYN,ACK → ACK.
  - Closed port: SYN → RST,ACK or SYN → SYN,ACK → RST,ACK.
- **Filters**:
  - Global TCP: `tcp`
  - SYN only: `tcp.flags==2` or `tcp.flags.syn==1`
  - ACK only: `tcp.flags==16` or `tcp.flags.ack==1`
  - SYN,ACK: `tcp.flags==18` or `(tcp.flags.syn==1) and (tcp.flags.ack==1)`
  - RST: `tcp.flags==4` or `tcp.flags.reset==1`
  - RST,ACK: `tcp.flags==20` or `(tcp.flags.reset==1) and (tcp.flags.ack==1)`
  - TCP Connect Scan: `tcp.flags.syn==1 and tcp.flags.ack==0 and tcp.window_size>1024`

#### SYN Scans
- **Characteristics**: Half-open; root users; window size ≤ 1024 bytes; uses `nmap -sS`.
- **Pattern**:
  - Open port: SYN → SYN,ACK → RST.
  - Closed port: SYN → RST,ACK.
- **Filter**: `tcp.flags.syn==1 and tcp.flags.ack==0 and tcp.window_size<=1024`

#### UDP Scans
- **Characteristics**: No handshake; open ports silent; closed ports return ICMP errors; uses `nmap -sU`.
- **Pattern**:
  - Open port: UDP packet (no response).
  - Closed port: UDP → ICMP Type 3, Code 3 (port unreachable).
- **Filters**:
  - Global UDP: `udp`
  - UDP port range: `udp.dstport in {55..70}`
  - ICMP errors: `icmp.type==3 and icmp.code==3`

**Tip**: Expand ICMP packets in the Packet Details pane to view encapsulated UDP requests.

### 2. ARP Poisoning and MITM Attacks

ARP spoofing manipulates IP-to-MAC mappings to intercept traffic (MITM).

#### ARP Analysis
- **Characteristics**: Local network; no authentication; patterns include requests, responses, and gratuitous packets.
- **Anomalies**: Duplicate ARP responses for one IP; excessive ARP requests (flooding).
- **Filters**:
  - Global ARP: `arp`
  - Requests: `arp.opcode==1`
  - Responses: `arp.opcode==2`
  - ARP scanning: `arp.dst.hw_mac==00:00:00:00:00:00`
  - ARP poisoning: `arp.duplicate-address-detected` or `arp.duplicate-address-frame`
  - Specific MAC flooding: `((arp) && (arp.opcode==1)) && (arp.src.hw_mac==00:0c:29:e2:18:b4)`

#### MITM Detection
- Add MAC columns: Right-click `eth.src` or `eth.dst` > Apply as Column.
- Check HTTP traffic: `eth.dst==00:0c:29:e2:18:b4 and http`.
- Look for Expert Info warnings (Analyze > Expert Information) for duplicate IPs.

**Example**: If a MAC claims multiple IPs (e.g., gateway and host), suspect spoofing.

### 3. Identifying Hosts and Users

Map IPs, MACs, hostnames, and usernames using DHCP, NetBIOS, and Kerberos.

#### DHCP Analysis
- **Purpose**: Assigns IPs and hostnames.
- **Key Options**:
  - Option 12: Hostname.
  - Option 50: Requested IP.
  - Option 61: Client MAC.
  - Option 15: Domain name.
- **Filters**:
  - Global: `dhcp` or `bootp`
  - Request: `dhcp.option.dhcp==3`
  - ACK: `dhcp.option.dhcp==5`
  - NAK: `dhcp.option.dhcp==6`
  - Hostname: `dhcp.option.hostname contains "Galaxy"`
  - Domain: `dhcp.option.domain_name contains "example"`
  - Requested IP: `dhcp.option.requested_ip_address==172.16.13.85`

#### NetBIOS (NBNS) Analysis
- **Purpose**: Host communication; provides names and IPs.
- **Filters**:
  - Global: `nbns`
  - Name search: `nbns.name contains "LIVALJM"`
  - Registration requests: `nbns.flags in {0x2810 0x2910}`

#### Kerberos Analysis
- **Purpose**: Windows domain authentication; identifies users and hosts.
- **Filters**:
  - Global: `kerberos`
  - Username: `kerberos.CNameString contains "u5" and !(kerberos.CNameString contains "$")`
  - Hostname: `kerberos.CNameString contains "$"`
  - Protocol version: `kerberos.pvno==5`
  - Domain: `kerberos.realm contains ".org"`
  - Service: `kerberos.SNameString=="krbtgt"`

**Tip**: Use Statistics > Resolved Addresses to list IPs and hostnames.

### 4. Tunneling Traffic (ICMP and DNS)

Attackers use ICMP and DNS for data exfiltration or C2 channels.

#### ICMP Tunneling
- **Characteristics**: Large payloads (>64 bytes); encapsulates protocols (e.g., SSH, HTTP).
- **Indicators**: High ICMP volume; anomalous packet sizes.
- **Filters**:
  - Global: `icmp`
  - Large packets: `data.len>64 and icmp`
  - Example: Look for SSH in payload (manual inspection).

#### DNS Tunneling
- **Characteristics**: Long DNS queries; subdomains encode commands (e.g., `encoded-commands.maliciousdomain.com`).
- **Indicators**: High DNS volume; long query names.
- **Filters**:
  - Global: `dns`
  - Long queries: `dns.qry.name.len>15 and !mdns`
  - Known tools: `dns contains "dnscat"`
  - Suspicious domain: `dns.qry.name contains "dataexfil.com"`

**Tip**: Use Statistics > DNS to analyze query patterns.

### 5. Cleartext Protocol Analysis

Cleartext protocols (FTP, HTTP) expose sensitive data, enabling detection of attacks.

#### FTP Analysis
- **Risks**: Credential theft, MITM, data exfiltration.
- **Key Codes**:
  - x1x: Info (e.g., 211: System status).
  - x2x: Connection (e.g., 220: Service ready).
  - x3x: Authentication (e.g., 230: Login, 530: Failed).
- **Filters**:
  - Global: `ftp`
  - Status codes: `ftp.response.code==211`, `ftp.response.code==227`, `ftp.response.code==230`
  - Failed logins: `ftp.response.code in {430 530}`
  - Commands: `ftp.request.command=="USER"`, `ftp.request.command=="PASS"`
  - Password: `ftp.request.arg=="password"`
  - Brute-force: `(ftp.response.code==530) and (ftp.response.arg contains "username")`
  - Password spray: `(ftp.request.command=="PASS") and (ftp.request.arg=="password")`
  - File status: `ftp.response.code==213`
  - Command (e.g., chmod): `ftp.request.command=="SITE CHMOD"`

#### HTTP Analysis
- **Risks**: Phishing, C2, data exfiltration.
- **Filters**:
  - Global: `http`, `http2`
  - Requests: `http.request`
  - Methods: `http.request.method=="GET"`, `http.request.method=="POST"`
  - Status codes: `http.response.code==200`, `http.response.code==404`, `http.response.code==503`
  - Parameters: `http.user_agent contains "nmap"`, `http.request.uri contains "admin"`, `http.request.full_uri contains "admin"`
  - Server: `http.server contains "apache"`
  - Host: `http.host=="example.com"`, `http.host contains "keyword"`
  - Connection: `http.connection=="Keep-Alive"`
  - Form data: `urlencoded-form contains "uname"`
  - Specific URI: `http.request.full_uri=="http://testphp.vulnweb.com/userinfo.php"`

#### User Agent Analysis
- **Anomalies**: Non-standard agents, audit tools (Nmap, sqlmap), spelling errors.
- **Filters**:
  - Global: `http.user_agent`
  - Tools: `(http.user_agent contains "sqlmap") or (http.user_agent contains "Nmap") or (http.user_agent contains "Wfuzz") or (http.user_agent contains "Nikto")`
  - Custom: Sort by `http.user_agent` column to spot anomalies (e.g., “Mozlilla” vs. “Mozilla”).

#### Log4j Analysis
- **Characteristics**: Exploits via POST requests; contains `jndi:ldap` or `Exploit.class`.
- **Filters**:
  - POST requests: `http.request.method=="POST"`
  - Patterns: `(ip contains "jndi") or (ip contains "Exploit")`
  - Frame search: `(frame contains "jndi") or (frame contains "Exploit")`
  - User agent: `(http.user_agent contains "$") or (http.user_agent contains "==")`
  - Combined: `http.request.method=="POST" and ((ip contains "jndi") or (ip contains "Exploit")) and ((frame contains "jndi") or (frame contains "Exploit")) and ((http.user_agent contains "$") or (http.user_agent contains "=="))`

**Tip**: Follow TCP streams (Right-click > Follow > HTTP Stream) to view cleartext data.

### 6. Encrypted Protocol Analysis: HTTPS Decryption

HTTPS (TLS) encrypts traffic, requiring key log files for analysis.

#### HTTPS Analysis
- **Characteristics**: Encrypted; hides URLs and data without keys.
- **Filters**:
  - Global TLS: `tls`
  - HTTP requests: `http.request`
  - Client Hello: `(http.request or tls.handshake.type==1) and !(ssdp)`
  - Server Hello: `(http.request or tls.handshake.type==2) and !(ssdp)`
  - SSDP: `ssdp`
  - HTTP2: `http2`
  - Server name: `tls.handshake.extensions_server_name contains "accounts.google.com"`

#### Decryption Setup
1. Generate key log file during capture:
   - Set `SSLKEYLOGFILE` environment variable.
   - Browse with Chrome/Firefox to log keys.
2. Add key log file in Wireshark:
   - Edit > Preferences > Protocols > TLS > (Pre)-Master-Secret log filename > Select `keylog.txt`.
   - Or: Right-click packet > Protocol Preferences > TLS > Add key file.
3. View decrypted data:
   - Protocols: `Decrypted TLS`, `HTTP2`, `Decompressed Header`.
   - Export objects: File > Export Objects > HTTP.

**Example**: Decrypted HTTP2 packets reveal headers like `:authority: safebrowsing.googleapis.com`.

### 7. Hunting Cleartext Credentials

Extract credentials from cleartext protocols (FTP, HTTP, IMAP, POP, SMTP).

#### Credential Extraction
- **Method**: Tools > Credentials (Wireshark v3.1+).
- **Filters**:
  - FTP: `ftp and !ftp.request.arg and ftp.request.command` (empty password)
  - HTTP Basic Auth: `http`
  - Failed FTP logins: `ftp.response.code in {430 530}`
  - HTTP POST with credentials: `http.request.method=="POST" and urlencoded-form contains "uname"`

**Tip**: Click packet numbers in the Credentials window to inspect usernames/passwords.

### 8. Actionable Results: Firewall Rules

Generate firewall rules to block malicious traffic.

#### Firewall ACL Rules
- **Method**: Tools > Firewall ACL Rules.
- **Supported Formats**: iptables, Cisco IOS, ipfilter, ipfw, pf, Windows Firewall.
- **Examples**:
  - Deny source IP: `add deny ip from 10.121.70.151 to any in` (ipfw).
  - Allow destination MAC: `add allow MAC 00:d0:59:aa:af:80 any in` (ipfw).
- **Process**: Select a packet, generate rules, and copy to firewall config.

## Practical Tips

1. **Filter Strategically**:
   - Start broad: `tcp`, `arp`, `dns`.
   - Narrow down: `tcp.flags.syn==1`, `dns.qry.name.len>15`.
2. **Use Statistics**:
   - Protocol Hierarchy: Spot dominant protocols.
   - Conversations/Endpoints: Identify active hosts.
3. **Correlate Data**:
   - Combine ARP (`arp.duplicate-address-detected`) and HTTP (`eth.dst==<MAC>`) for MITM.
   - Check DHCP (`dhcp.option.hostname`) and Kerberos (`kerberos.CNameString`) for host/user mapping.
4. **Inspect Streams**: Follow TCP/HTTP streams for cleartext data (e.g., FTP passwords, HTTP POSTs).
5. **Save Profiles**: Edit > Configuration Profiles to store filter and column settings.
6. **Validate Anomalies**: Cross-check user agents or DNS domains via web searches.

## Conclusion

Wireshark’s intermediate features enable security analysts to detect sophisticated threats like Nmap scans, ARP poisoning, tunneling, and credential theft. By mastering filters (e.g., `tcp.flags.syn==1 and tcp.window_size>1024`, `dns contains "dnscat"`), protocol analysis, and decryption, you can uncover anomalies and take action with firewall rules. Practice with diverse traffic to refine your skills. Happy threat hunting!

## Citations

- [Wireshark pcap-filter man page](https://www.wireshark.org/docs/man-pages/pcap-filter.html)
- [Wireshark GitLab CaptureFilters](https://gitlab.com/wireshark/wireshark/-/wikis/CaptureFilters#useful-filters)
- [Wireshark Wiki CaptureFilters](https://wiki.wireshark.org/CaptureFilters)
