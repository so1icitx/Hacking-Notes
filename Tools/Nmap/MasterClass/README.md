
# The Ultimate Nmap guide


## 1. Target Specification Options
These options tell Nmap **what** to scan (IPs, hosts, or networks).

- **`-iL <file>` (Input from List)**  
  - **What it does**: Reads targets from a file (one IP or hostname per line).  
  - **Example**: `nmap -iL targets.txt` scans IPs listed in `targets.txt`.  
  - **Why it matters**: Great for scanning multiple targets without typing them all out.  
  - **Notebook tip**: `-iL: File input (targets.txt -> scan IPs)`.  

- **`-iR <num>` (Input Random Hosts)**  
  - **What it does**: Scans `<num>` random IPs on the internet.  
  - **Example**: `nmap -iR 10` scans 10 random hosts.  
  - **Why it matters**: Useful for research or testing, but be ethical—random scanning can look sketchy.  
  - **Notebook tip**: `-iR: Random hosts (10 -> 10 random IPs)`.  

- **`--exclude <host1[,host2]...>` (Exclude Hosts)**  
  - **What it does**: Skips specified hosts from the scan.  
  - **Example**: `nmap 192.168.1.0/24 --exclude 192.168.1.1` skips the router.  
  - **Why it matters**: Avoids scanning sensitive devices like gateways.  
  - **Notebook tip**: `--exclude: Skip hosts (no scan 192.168.1.1)`.  

- **`--excludefile <file>` (Exclude from File)**  
  - **What it does**: Skips hosts listed in a file.  
  - **Example**: `nmap 192.168.1.0/24 --excludefile skip.txt` skips IPs in `skip.txt`.  
  - **Why it matters**: Handy for excluding multiple devices without a long command.  
  - **Notebook tip**: `--excludefile: Skip file (skip.txt -> no scan)`.  

---

## 2. Host Discovery Options
These control how Nmap finds **live hosts** before scanning ports.

- **`-sn` (No Port Scan, aka Ping Scan)**  
  - **What it does**: Checks if hosts are up without scanning ports (uses ICMP, TCP, etc.).  
  - **Example**: `nmap -sn 192.168.1.0/24` lists live IPs.  
  - **Why it matters**: Quick way to map a network without touching ports.  
  - **Notebook tip**: `-sn: Ping scan (find live IPs, no ports)`.  

- **`-Pn` (No Ping)**  
  - **What it does**: Skips host discovery, assumes all targets are up.  
  - **Example**: `nmap -Pn 192.168.1.100` scans ports even if the host doesn’t reply to pings.  
  - **Why it matters**: Bypasses firewalls that block ICMP or discovery probes.  
  - **Notebook tip**: `-Pn: No ping (scan anyway)`.  

- **`-PS<portlist>` (TCP SYN Ping)**  
  - **What it does**: Sends SYN packets to specified ports to check if hosts are up.  
  - **Example**: `nmap -PS80,443 192.168.1.100` pings using ports 80 and 443.  
  - **Why it matters**: Works when ICMP is blocked, targets common ports.  
  - **Notebook tip**: `-PS: SYN ping (80,443 -> host up?)`.  

- **`-PA<portlist>` (TCP ACK Ping)**  
  - **What it does**: Sends ACK packets to check host status.  
  - **Example**: `nmap -PA80 192.168.1.100` uses ACK on port 80.  
  - **Why it matters**: Sneaky way to detect hosts behind some firewalls.  
  - **Notebook tip**: `-PA: ACK ping (80 -> host alive?)`.  

- **`-PU<portlist>` (UDP Ping)**  
  - **What it does**: Sends UDP packets to check if hosts respond.  
  - **Example**: `nmap -PU53 192.168.1.100` pings with UDP port 53 (DNS).  
  - **Why it matters**: Catches hosts running UDP services like DNS or DHCP.  
  - **Notebook tip**: `-PU: UDP ping (53 -> host up?)`.  

- **`-PY<portlist>` (SCTP INIT Ping)**  
  - **What it does**: Uses SCTP INIT packets for discovery.  
  - **Example**: `nmap -PY80 192.168.1.100` pings with SCTP on port 80.  
  - **Why it matters**: Rare, but useful for networks using SCTP (telecom).  
  - **Notebook tip**: `-PY: SCTP ping (80 -> host check)`.  

- **`-PE` (ICMP Echo Ping)**  
  - **What it does**: Sends standard ICMP echo (ping) requests.  
  - **Example**: `nmap -PE 192.168.1.100` pings like the `ping` command.  
  - **Why it matters**: Default way to check if a host is up, but often blocked.  
  - **Notebook tip**: `-PE: ICMP echo (ping -> host alive?)`.  

- **`-PP` (ICMP Timestamp Ping)**  
  - **What it does**: Sends ICMP timestamp requests.  
  - **Example**: `nmap -PP 192.168.1.100` checks via timestamp replies.  
  - **Why it matters**: Alternative to echo ping, sometimes less filtered.  
  - **Notebook tip**: `-PP: ICMP timestamp (host up?)`.  

- **`-PM` (ICMP Netmask Ping)**  
  - **What it does**: Sends ICMP address mask requests.  
  - **Example**: `nmap -PM 192.168.1.100` uses netmask queries.  
  - **Why it matters**: Rare, but can reveal hosts on old systems.  
  - **Notebook tip**: `-PM: ICMP netmask (host check)`.  

- **`-PO<protocollist>` (IP Protocol Ping)**  
  - **What it does**: Pings using specific IP protocols (e.g., ICMP, IGMP).  
  - **Example**: `nmap -PO1,2 192.168.1.100` tries protocols 1 (ICMP) and 2 (IGMP).  
  - **Why it matters**: Bypasses filters by using unusual protocols.  
  - **Notebook tip**: `-PO: Protocol ping (1,2 -> host?)`.  

- **`-PR` (ARP Ping)**  
  - **What it does**: Uses ARP requests for local network hosts.  
  - **Example**: `nmap -PR 192.168.1.100` pings via ARP.  
  - **Why it matters**: Super reliable on LANs, ignores firewalls.  
  - **Notebook tip**: `-PR: ARP ping (LAN host check)`.  

- **`--disable-arp-ping` (Disable ARP Ping)**  
  - **What it does**: Stops Nmap from using ARP for local hosts.  
  - **Example**: `nmap --disable-arp-ping 192.168.1.100` uses TCP instead.  
  - **Why it matters**: Useful if ARP is blocked or causes issues.  
  - **Notebook tip**: `--disable-arp-ping: No ARP (TCP instead)`.  

- **`--traceroute` (Traceroute)**  
  - **What it does**: Maps the network path to each host.  
  - **Example**: `nmap --traceroute 192.168.1.100` shows routers to target.  
  - **Why it matters**: Reveals network topology for planning attacks.  
  - **Notebook tip**: `--traceroute: Path to host (routers)`.  

- **`-n` (No DNS Resolution)**  
  - **What it does**: Skips DNS lookups, uses IPs only.  
  - **Example**: `nmap -n 192.168.1.100` avoids resolving to a hostname.  
  - **Why it matters**: Faster scans, avoids DNS logs.  
  - **Notebook tip**: `-n: No DNS (IPs only)`.  

- **`-R` (Always DNS Resolution)**  
  - **What it does**: Forces DNS resolution for all IPs.  
  - **Example**: `nmap -R 192.168.1.100` looks up hostnames.  
  - **Why it matters**: Helps identify servers by name, but slower.  
  - **Notebook tip**: `-R: Always DNS (get hostnames)`.  

- **`--resolve-all` (Resolve All IPs)**  
  - **What it does**: Resolves all IPs for hosts with multiple addresses.  
  - **Example**: `nmap --resolve-all example.com` checks all IPs for `example.com`.  
  - **Why it matters**: Ensures you scan every IP a hostname might have.  
  - **Notebook tip**: `--resolve-all: All IPs (full DNS)`.  

- **`--system-dns` (Use System DNS)**  
  - **What it does**: Uses the OS’s DNS resolver, not Nmap’s.  
  - **Example**: `nmap --system-dns 192.168.1.100` relies on system DNS.  
  - **Why it matters**: Bypasses Nmap’s DNS for custom setups.  
  - **Notebook tip**: `--system-dns: OS DNS (not Nmap’s)`.  

- **`--dns-servers <server1[,server2]...>` (Custom DNS Servers)**  
  - **What it does**: Specifies DNS servers for resolution.  
  - **Example**: `nmap --dns-servers 8.8.8.8,1.1.1.1 192.168.1.100`.  
  - **Why it matters**: Uses trusted or internal DNS to avoid tampering.  
  - **Notebook tip**: `--dns-servers: Custom DNS (8.8.8.8)`.  

---

## 3. Scan Techniques
These define **how** Nmap scans ports (e.g., stealthy or aggressive).

- **`-sS` (TCP SYN Scan)**  
  - **What it does**: Sends SYN packets, doesn’t complete TCP handshake (stealthy).  
  - **Example**: `sudo nmap -sS 192.168.1.100` scans quietly.  
  - **Why it matters**: Fast, less likely to be logged, needs root.  
  - **Notebook tip**: `-sS: SYN scan (stealth, sudo)`.  

- **`-sT` (TCP Connect Scan)**  
  - **What it does**: Completes TCP handshakes (like a browser).  
  - **Example**: `nmap -sT 192.168.1.100` uses full connections.  
  - **Why it matters**: No root needed, but noisier, more detectable.  
  - **Notebook tip**: `-sT: TCP connect (no sudo, loud)`.  

- **`-sU` (UDP Scan)**  
  - **What it does**: Scans UDP ports.  
  - **Example**: `sudo nmap -sU 192.168.1.100` checks UDP 53, 123, etc.  
  - **Why it matters**: Finds services like DNS or NTP, slower due to UDP.  
  - **Notebook tip**: `-sU: UDP scan (slow, sudo)`.  

- **`-sY` (SCTP INIT Scan)**  
  - **What it does**: Scans SCTP ports using INIT packets.  
  - **Example**: `sudo nmap -sY 192.168.1.100` targets SCTP services.  
  - **Why it matters**: Rare, for telecom networks using SCTP.  
  - **Notebook tip**: `-sY: SCTP scan (INIT, niche)`.  

- **`-sN` (TCP Null Scan)**  
  - **What it does**: Sends TCP packets with no flags set.  
  - **Example**: `sudo nmap -sN 192.168.1.100` tests port responses.  
  - **Why it matters**: Stealthy, exploits RFC 793 quirks, needs root.  
  - **Notebook tip**: `-sN: Null scan (no flags, stealth)`.  

- **`-sF` (TCP FIN Scan)**  
  - **What it does**: Sends TCP FIN packets.  
  - **Example**: `sudo nmap -sF 192.168.1.100` uses FIN to probe.  
  - **Why it matters**: Sneaky, bypasses some firewalls, needs root.  
  - **Notebook tip**: `-sF: FIN scan (stealth, sudo)`.  

- **`-sX` (TCP Xmas Scan)**  
  - **What it does**: Sends TCP packets with FIN, PSH, URG flags (like a Christmas tree).  
  - **Example**: `sudo nmap -sX 192.168.1.100` lights up ports.  
  - **Why it matters**: Stealthy, confuses some systems, needs root.  
  - **Notebook tip**: `-sX: Xmas scan (FIN+PSH+URG, sneaky)`.  

- **`-sA` (TCP ACK Scan)**  
  - **What it does**: Sends ACK packets to map firewall rules.  
  - **Example**: `sudo nmap -sA 192.168.1.100` checks filtering.  
  - **Why it matters**: Shows which ports are filtered, not open/closed.  
  - **Notebook tip**: `-sA: ACK scan (firewall map)`.  

- **`-sW` (TCP Window Scan)**  
  - **What it does**: Like ACK scan, but checks TCP window sizes.  
  - **Example**: `sudo nmap -sW 192.168.1.100` analyzes responses.  
  - **Why it matters**: Detects open ports on quirky systems.  
  - **Notebook tip**: `-sW: Window scan (TCP sizes)`.  

- **`-sM` (TCP Maimon Scan)**  
  - **What it does**: Sends FIN+ACK packets (named after Uriel Maimon).  
  - **Example**: `sudo nmap -sM 192.168.1.100` probes with FIN+ACK.  
  - **Why it matters**: Rare, exploits odd TCP stack behaviors.  
  - **Notebook tip**: `-sM: Maimon scan (FIN+ACK)`.  

- **`--scanflags <flags>` (Custom TCP Flags)**  
  - **What it does**: Sets custom TCP flags (e.g., URG, PSH).  
  - **Example**: `sudo nmap --scanflags URGPSH 192.168.1.100`.  
  - **Why it matters**: Tests specific firewall or system responses.  
  - **Notebook tip**: `--scanflags: Custom flags (URG+PSH)`.  

- **`-sI <zombiehost[:probeport]>` (Idle Scan)**  
  - **What it does**: Uses a “zombie” host to scan stealthily.  
  - **Example**: `sudo nmap -sI zombie.com 192.168.1.100`.  
  - **Why it matters**: Hides your IP, but complex and rare.  
  - **Notebook tip**: `-sI: Idle scan (zombie hides you)`.  

- **`-sO` (IP Protocol Scan)**  
  - **What it does**: Scans for supported IP protocols (e.g., TCP, UDP, ICMP).  
  - **Example**: `sudo nmap -sO 192.168.1.100`.  
  - **Why it matters**: Finds non-TCP/UDP services like GRE.  
  - **Notebook tip**: `-sO: Protocol scan (TCP, UDP, etc.)`.  

- **`-b <FTP relay host>` (FTP Bounce Scan)**  
  - **What it does**: Uses an FTP server to bounce scans.  
  - **Example**: `sudo nmap -b ftp.example.com 192.168.1.100`.  
  - **Why it matters**: Ancient, mostly obsolete, but historically sneaky.  
  - **Notebook tip**: `-b: FTP bounce (old-school)`.  

---

## 4. Port Specification
These control **which ports** Nmap scans.

- **`-p <port ranges>` (Port Range)**  
  - **What it does**: Specifies ports to scan (single, range, or list).  
  - **Example**: `nmap -p 80,443,22 192.168.1.100` or `nmap -p 1-100`.  
  - **Why it matters**: Focuses scans to save time or hit specific services.  
  - **Notebook tip**: `-p: Ports (80,443 or 1-100)`.  

- **`-p-` (All Ports)**  
  - **What it does**: Scans all 65,535 TCP or UDP ports.  
  - **Example**: `nmap -p- 192.168.1.100`.  
  - **Why it matters**: Thorough, finds hidden services, but slow.  
  - **Notebook tip**: `-p-: All ports (65k, slow)`.  

- **`-F` (Fast Scan)**  
  - **What it does**: Scans only the 100 most common ports.  
  - **Example**: `nmap -F 192.168.1.100`.  
  - **Why it matters**: Quick for initial recon.  
  - **Notebook tip**: `-F: Fast scan (top 100 ports)`.  

- **`--top-ports <n>` (Top Ports)**  
  - **What it does**: Scans the top `<n>` most common ports.  
  - **Example**: `nmap --top-ports 50 192.168.1.100`.  
  - **Why it matters**: Balances speed and coverage.  
  - **Notebook tip**: `--top-ports: Top n ports (50)`.  

- **`-g <port>` / `--source-port <port>` (Source Port)**  
  - **What it does**: Uses a specific source port for scans.  
  - **Example**: `sudo nmap -g 53 192.168.1.100`.  
  - **Why it matters**: Tricks firewalls expecting trusted ports (e.g., 53 for DNS).  
  - **Notebook tip**: `-g: Source port (53 -> firewall bypass)`.  

- **`-r` (Sequential Port Scan)**  
  - **What it does**: Scans ports in order, not randomized.  
  - **Example**: `nmap -r 192.168.1.100`.  
  - **Why it matters**: Predictable for debugging, but slower.  
  - **Notebook tip**: `-r: Sequential ports (1,2,3…)`.  

---

## 5. Service/Version Detection
These identify **services** and their versions on open ports.

- **`-sV` (Service Version Detection)**  
  - **What it does**: Probes open ports to identify services and versions.  
  - **Example**: `nmap -sV 192.168.1.100` shows `80/tcp open http Apache/2.4.7`.  
  - **Why it matters**: Version info reveals exploitable software.  
  - **Notebook tip**: `-sV: Service versions (Apache/2.4.7)`.  

- **`--version-intensity <0-9>` (Version Intensity)**  
  - **What it does**: Sets probe depth (0=light, 9=try all).  
  - **Example**: `nmap -sV --version-intensity 9 192.168.1.100`.  
  - **Why it matters**: Higher intensity gets more details, but slower.  
  - **Notebook tip**: `--version-intensity: Probe depth (9=max)`.  

- **`--version-light` (Light Version Scan)**  
  - **What it does**: Uses intensity 2 for quick scans.  
  - **Example**: `nmap -sV --version-light 192.168.1.100`.  
  - **Why it matters**: Fast but less thorough.  
  - **Notebook tip**: `--version-light: Quick versions (intensity 2)`.  

- **`--version-all` (Full Version Scan)**  
  - **What it does**: Uses intensity 9 for max probes.  
  - **Example**: `nmap -sV --version-all 192.168.1.100`.  
  - **Why it matters**: Gets every detail, but slow.  
  - **Notebook tip**: `--version-all: Max versions (intensity 9)`.  

- **`--version-trace` (Version Trace)**  
  - **What it does**: Shows detailed probe/response data.  
  - **Example**: `nmap -sV --version-trace 192.168.1.100`.  
  - **Why it matters**: Debugs why version detection fails.  
  - **Notebook tip**: `--version-trace: Debug versions (probe log)`.  

---

## 6. Script Scanning (Nmap Scripting Engine - NSE)
These run **scripts** to enhance scans.

- **`-sC` (Default Scripts)**  
  - **What it does**: Runs safe, default NSE scripts.  
  - **Example**: `nmap -sC 192.168.1.100` checks banners, titles, etc.  
  - **Why it matters**: Automates common recon tasks.  
  - **Notebook tip**: `-sC: Default scripts (safe NSE)`.  

- **`--script <script|category|dir>` (Run Scripts)**  
  - **What it does**: Runs specific scripts or categories.  
  - **Example**: `nmap --script http-vuln* 192.168.1.100` checks HTTP vulnerabilities.  
  - **Why it matters**: Targets specific checks (e.g., vuln, auth).  
  - **Notebook tip**: `--script: Run NSE (http-vuln*, vuln cat)`.  

- **`--script-args <name=value,...>` (Script Arguments)**  
  - **What it does**: Passes arguments to scripts.  
  - **Example**: `nmap --script http-put --script-args file=test.txt 192.168.1.100`.  
  - **Why it matters**: Customizes script behavior.  
  - **Notebook tip**: `--script-args: Script params (file=test.txt)`.  

- **`--script-args-file <file>` (Script Args File)**  
  - **What it does**: Loads script args from a file.  
  - **Example**: `nmap --script http-put --script-args-file args.txt 192.168.1.100`.  
  - **Why it matters**: Simplifies complex script configs.  
  - **Notebook tip**: `--script-args-file: Args file (args.txt)`.  

- **`--script-trace` (Script Trace)**  
  - **What it does**: Shows all script I/O.  
  - **Example**: `nmap --script http-title --script-trace 192.168.1.100`.  
  - **Why it matters**: Debugs script failures.  
  - **Notebook tip**: `--script-trace: Debug scripts (I/O log)`.  

- **`--script-updatedb` (Update Script DB)**  
  - **What it does**: Updates the NSE script database.  
  - **Example**: `nmap --script-updatedb`.  
  - **Why it matters**: Ensures scripts are current.  
  - **Notebook tip**: `--script-updatedb: Update NSE DB`.  

---

## 7. OS Detection
These guess the **operating system** and its details.

- **`-O` (OS Detection)**  
  - **What it does**: Identifies OS and version via TCP/IP stack.  
  - **Example**: `sudo nmap -O 192.168.1.100` shows `Linux 4.X`.  
  - **Why it matters**: OS info guides exploit selection.  
  - **Notebook tip**: `-O: OS detect (Linux, Windows)`.  

- **`--osscan-limit` (Limit OS Scan)**  
  - **What it does**: Skips OS detection on hosts with no open ports.  
  - **Example**: `sudo nmap -O --osscan-limit 192.168.1.100`.  
  - **Why it matters**: Speeds up scans on unresponsive hosts.  
  - **Notebook tip**: `--osscan-limit: Skip OS (no ports)`.  

- **`--osscan-guess` (Aggressive OS Guess)**  
  - **What it does**: Makes bolder OS guesses if unsure.  
  - **Example**: `sudo nmap -O --osscan-guess 192.168.1.100`.  
  - **Why it matters**: Improves detection on weird systems.  
  - **Notebook tip**: `--osscan-guess: Bold OS guess`.  

---

## 8. Timing and Performance
These control **speed** and **stealth**.

- **`-T<0-5>` (Timing Template)**  
  - **What it does**: Sets scan speed (0=paranoid, 5=insane).  
  - **Example**: `nmap -T3 192.168.1.100` (normal speed).  
  - **Why it matters**: Balances speed vs. detection risk.  
  - **Notebook tip**: `-T: Speed (0=slow, 5=fast)`.  

- **`--min-hostgroup <n>` (Min Host Group)**  
  - **What it does**: Sets min number of hosts scanned in parallel.  
  - **Example**: `nmap --min-hostgroup 50 192.168.1.0/24`.  
  - **Why it matters**: Speeds up large scans.  
  - **Notebook tip**: `--min-hostgroup: Min parallel (50 hosts)`.  

- **`--max-hostgroup <n>` (Max Host Group)**  
  - **What it does**: Sets max parallel hosts.  
  - **Example**: `nmap --max-hostgroup 10 192.168.1.0/24`.  
  - **Why it matters**: Limits resource use on your machine.  
  - **Notebook tip**: `--max-hostgroup: Max parallel (10 hosts)`.  

- **`--min-parallelism <n>` (Min Parallel Probes)**  
  - **What it does**: Sets min simultaneous probes.  
  - **Example**: `nmap --min-parallelism 100 192.168.1.100`.  
  - **Why it matters**: Forces faster scans, risks overwhelming target.  
  - **Notebook tip**: `--min-parallelism: Min probes (100)`.  

- **`--max-parallelism <n>` (Max Parallel Probes)**  
  - **What it does**: Limits simultaneous probes.  
  - **Example**: `nmap --max-parallelism 10 192.168.1.100`.  
  - **Why it matters**: Reduces noise, gentler on networks.  
  - **Notebook tip**: `--max-parallelism: Max probes (10)`.  

- **`--min-rtt-timeout <time>` (Min RTT Timeout)**  
  - **What it does**: Sets min time to wait for replies.  
  - **Example**: `nmap --min-rtt-timeout 100ms 192.168.1.100`.  
  - **Why it matters**: Speeds up on fast networks.  
  - **Notebook tip**: `--min-rtt-timeout: Min wait (100ms)`.  

- **`--max-rtt-timeout <time>` (Max RTT Timeout)**  
  - **What it does**: Sets max reply wait time.  
  - **Example**: `nmap --max-rtt-timeout 500ms 192.168.1.100`.  
  - **Why it matters**: Prevents hanging on slow networks.  
  - **Notebook tip**: `--max-rtt-timeout: Max wait (500ms)`.  

- **`--initial-rtt-timeout <time>` (Initial RTT Timeout)**  
  - **What it does**: Sets starting reply wait time.  
  - **Example**: `nmap --initial-rtt-timeout 300ms 192.168.1.100`.  
  - **Why it matters**: Fine-tunes for network conditions.  
  - **Notebook tip**: `--initial-rtt-timeout: Start wait (300ms)`.  

- **`--max-retries <n>` (Max Retries)**  
  - **What it does**: Sets max probe retries.  
  - **Example**: `nmap --max-retries 2 192.168.1.100`.  
  - **Why it matters**: Balances accuracy vs. speed.  
  - **Notebook tip**: `--max-retries: Max tries (2)`.  

- **`--host-timeout <time>` (Host Timeout)**  
  - **What it does**: Skips hosts taking too long.  
  - **Example**: `nmap --host-timeout 30m 192.168.1.100`.  
  - **Why it matters**: Avoids scans stalling on one target.  
  - **Notebook tip**: `--host-timeout: Skip slow (30m)`.  

- **`--scan-delay <time>` (Scan Delay)**  
  - **What it does**: Waits between probes.  
  - **Example**: `nmap --scan-delay 1s 192.168.1.100`.  
  - **Why it matters**: Evades rate-limiting firewalls.  
  - **Notebook tip**: `--scan-delay: Probe pause (1s)`.  

- **`--max-scan-delay <time>` (Max Scan Delay)**  
  - **What it does**: Caps adaptive delays.  
  - **Example**: `nmap --max-scan-delay 10s 192.168.1.100`.  
  - **Why it matters**: Keeps scans moving despite congestion.  
  - **Notebook tip**: `--max-scan-delay: Max pause (10s)`.  

- **`--min-rate <n>` (Min Packet Rate)**  
  - **What it does**: Sets min packets per second.  
  - **Example**: `nmap --min-rate 100 192.168.1.100`.  
  - **Why it matters**: Forces fast scans, risks detection.  
  - **Notebook tip**: `--min-rate: Min packets (100/s)`.  

- **`--max-rate <n>` (Max Packet Rate)**  
  - **What it does**: Caps packets per second.  
  - **Example**: `nmap --max-rate 50 192.168.1.100`.  
  - **Why it matters**: Stays stealthy, avoids flooding.  
  - **Notebook tip**: `--max-rate: Max packets (50/s)`.  

---

## 9. Firewall/IDS Evasion
These help Nmap **bypass** firewalls and intrusion detection systems.

- **`-f` (Fragment Packets)**  
  - **What it does**: Splits packets into fragments.  
  - **Example**: `sudo nmap -f 192.168.1.100`.  
  - **Why it matters**: Confuses old firewalls, harder to detect.  
  - **Notebook tip**: `-f: Fragment packets (firewall dodge)`.  

- **`--mtu <n>` (MTU Size)**  
  - **What it does**: Sets max transmission unit for fragmentation.  
  - **Example**: `sudo nmap --mtu 16 192.168.1.100`.  
  - **Why it matters**: Smaller fragments sneak past some IDS.  
  - **Notebook tip**: `--mtu: Fragment size (16 bytes)`.  

- **`-D <decoy1[,decoy2]...>` (Decoy Scan)**  
  - **What it does**: Spoofs scans from fake IPs.  
  - **Example**: `sudo nmap -D 1.2.3.4,5.6.7.8 192.168.1.100`.  
  - **Why it matters**: Hides your IP among decoys.  
  - **Notebook tip**: `-D: Decoys (fake IPs)`.  

- **`-S <IP>` (Source IP Spoof)**  
  - **What it does**: Fakes your source IP.  
  - **Example**: `sudo nmap -S 1.2.3.4 192.168.1.100`.  
  - **Why it matters**: Tricks logs, but needs network setup.  
  - **Notebook tip**: `-S: Spoof IP (1.2.3.4)`.  

- **`-e <interface>` (Network Interface)**  
  - **What it does**: Specifies network interface.  
  - **Example**: `sudo nmap -e eth0 192.168.1.100`.  
  - **Why it matters**: Uses specific NIC for spoofing or routing.  
  - **Notebook tip**: `-e: Interface (eth0)`.  

- **`--source-mac <MAC>` (Source MAC Spoof)**  
  - **What it does**: Fakes your MAC address.  
  - **Example**: `sudo nmap --source-mac 00:11:22:33:44:55 192.168.1.100`.  
  - **Why it matters**: Bypasses MAC-based filters.  
  - **Notebook tip**: `--source-mac: Spoof MAC (00:11:…)`.  

- **`--proxies <proxy1[,proxy2]...>` (Proxy Scan)**  
  - **What it does**: Routes scans through HTTP/SOCKS4 proxies.  
  - **Example**: `nmap --proxies http://proxy.com:8080 192.168.1.100`.  
  - **Why it matters**: Hides your IP via proxies.  
  - **Notebook tip**: `--proxies: Scan via proxy`.  

- **`--data <hex string>` (Custom Payload)**  
  - **What it does**: Adds custom data to packets.  
  - **Example**: `sudo nmap --data 0xDEADBEEF 192.168.1.100`.  
  - **Why it matters**: Tests specific firewall rules.  
  - **Notebook tip**: `--data: Custom packet data (DEADBEEF)`.  

- **`--data-string <string>` (Custom String Payload)**  
  - **What it does**: Adds a string to packets.  
  - **Example**: `sudo nmap --data-string "TEST" 192.168.1.100`.  
  - **Why it matters**: Easier than hex for custom tests.  
  - **Notebook tip**: `--data-string: Packet string (TEST)`.  

- **`--data-length <n>` (Append Random Data)**  
  - **What it does**: Adds `<n>` bytes of random data.  
  - **Example**: `sudo nmap --data-length 50 192.168.1.100`.  
  - **Why it matters**: Changes packet size to dodge IDS.  
  - **Notebook tip**: `--data-length: Random data (50 bytes)`.  

- **`--ip-options <options>` (IP Options)**  
  - **What it does**: Sets IP options (e.g., record route).  
  - **Example**: `sudo nmap --ip-options R 192.168.1.100`.  
  - **Why it matters**: Tests firewall handling of IP headers.  
  - **Notebook tip**: `--ip-options: IP opts (R=route)`.  

- **`--ttl <n>` (Time to Live)**  
  - **What it does**: Sets packet TTL.  
  - **Example**: `sudo nmap --ttl 10 192.168.1.100`.  
  - **Why it matters**: Spoofs distance or bypasses filters.  
  - **Notebook tip**: `--ttl: Packet TTL (10 hops)`.  

- **`--randomize-hosts` (Randomize Host Order)**  
  - **What it does**: Scans hosts in random order.  
  - **Example**: `nmap --randomize-hosts 192.168.1.0/24`.  
  - **Why it matters**: Makes scans less predictable.  
  - **Notebook tip**: `--randomize-hosts: Shuffle hosts`.  

- **`--spoof-mac <MAC|vendor|0>` (MAC Spoofing)**  
  - **What it does**: Spoofs MAC as specific address, vendor, or random.  
  - **Example**: `sudo nmap --spoof-mac Apple 192.168.1.100`.  
  - **Why it matters**: Bypasses MAC filters or hides identity.  
  - **Notebook tip**: `--spoof-mac: Fake MAC (Apple, 0=rand)`.  

- **`--badsum` (Bad Checksum)**  
  - **What it does**: Sends packets with invalid checksums.  
  - **Example**: `sudo nmap --badsum 192.168.1.100`.  
  - **Why it matters**: Tests firewall checksum handling.  
  - **Notebook tip**: `--badsum: Invalid checksum`.  

---

## 10. Output Options
These control **how** Nmap saves or displays results.

- **`-oN <file>` (Normal Output)**  
  - **What it does**: Saves scan in human-readable format.  
  - **Example**: `nmap -oN scan.txt 192.168.1.100`.  
  - **Why it matters**: Easy to read later or share.  
  - **Notebook tip**: `-oN: Normal save (scan.txt)`.  

- **`-oX <file>` (XML Output)**  
  - **What it does**: Saves in XML for tools like Metasploit.  
  - **Example**: `nmap -oX scan.xml 192.168.1.100`.  
  - **Why it matters**: Machine-readable for automation.  
  - **Notebook tip**: `-oX: XML save (scan.xml)`.  

- **`-oG <file>` (Grepable Output)**  
  - **What it does**: Saves in a format easy to grep.  
  - **Example**: `nmap -oG scan.grep 192.168.1.100`.  
  - **Why it matters**: Quick parsing with Unix tools.  
  - **Notebook tip**: `-oG: Grep save (scan.grep)`.  

- **`-oA <basename>` (All Outputs)**  
  - **What it does**: Saves in normal, XML, and grepable formats.  
  - **Example**: `nmap -oA scan 192.168.1.100` creates `scan.nmap`, `.xml`, `.grep`.  
  - **Why it matters**: Covers all bases for reporting.  
  - **Notebook tip**: `-oA: All saves (scan.*)`.  

- **`-oS <file>` (Script Kiddie Output)**  
  - **What it does**: Saves in a “1337 h4x0r” style (joke format).  
  - **Example**: `nmap -oS scan.txt 192.168.1.100`.  
  - **Why it matters**: Fun, but rarely used seriously.  
  - **Notebook tip**: `-oS: 1337 save (joke)`.  

- **`-v` (Increase Verbosity)**  
  - **What it does**: Shows more scan details (stack with `-vv`, `-vvv`).  
  - **Example**: `nmap -v 192.168.1.100`.  
  - **Why it matters**: Helps track progress in real-time.  
  - **Notebook tip**: `-v: More info (vv, vvv)`.  

- **`-d` (Debugging)**  
  - **What it does**: Shows detailed debug info (stack with `-dd`, `-ddd`).  
  - **Example**: `nmap -d 192.168.1.100`.  
  - **Why it matters**: Troubleshoots why scans fail.  
  - **Notebook tip**: `-d: Debug (dd, ddd)`.  

- **`--reason` (Show Reasons)**  
  - **What it does**: Explains why ports are open/closed/filtered.  
  - **Example**: `nmap --reason 192.168.1.100`.  
  - **Why it matters**: Clarifies scan results for analysis.  
  - **Notebook tip**: `--reason: Why port state`.  

- **`--stats-every <time>` (Periodic Stats)**  
  - **What it does**: Shows scan progress every `<time>`.  
  - **Example**: `nmap --stats-every 10s 192.168.1.100`.  
  - **Why it matters**: Monitors long scans.  
  - **Notebook tip**: `--stats-every: Progress (10s)`.  

- **`--packet-trace` (Packet Trace)**  
  - **What it does**: Logs all sent/received packets.  
  - **Example**: `nmap --packet-trace 192.168.1.100`.  
  - **Why it matters**: Deep debugging of network issues.  
  - **Notebook tip**: `--packet-trace: Log packets`.  

- **`--open` (Show Open Ports Only)**  
  - **What it does**: Only displays open or open|filtered ports.  
  - **Example**: `nmap --open 192.168.1.100`.  
  - **Why it matters**: Cuts clutter, focuses on actionable ports.  
  - **Notebook tip**: `--open: Open ports only`.  

- **`--iflist` (List Interfaces)**  
  - **What it does**: Shows network interfaces and routes.  
  - **Example**: `nmap --iflist`.  
  - **Why it matters**: Helps pick the right interface for scans.  
  - **Notebook tip**: `--iflist: Show interfaces`.  

- **`--append-output` (Append Output)**  
  - **What it does**: Appends to output files instead of overwriting.  
  - **Example**: `nmap --append-output -oN scan.txt 192.168.1.100`.  
  - **Why it matters**: Preserves previous scan data.  
  - **Notebook tip**: `--append-output: Add to file`.  

- **`--resume <file>` (Resume Scan)**  
  - **What it does**: Resumes an aborted scan from a file.  
  - **Example**: `nmap --resume scan.xml`.  
  - **Why it matters**: Saves time after interruptions.  
  - **Notebook tip**: `--resume: Continue scan (scan.xml)`.  

- **`--stylesheet <path|URL>` (XML Stylesheet)**  
  - **What it does**: Applies a stylesheet to XML output.  
  - **Example**: `nmap --stylesheet style.xsl -oX scan.xml 192.168.1.100`.  
  - **Why it matters**: Makes XML prettier for humans.  
  - **Notebook tip**: `--stylesheet: XML style (style.xsl)`.  

- **`--webxml` (Web XML Output)**  
  - **What it does**: Uses Nmap’s online stylesheet for XML.  
  - **Example**: `nmap --webxml -oX scan.xml 192.168.1.100`.  
  - **Why it matters**: Easy XML formatting without local files.  
  - **Notebook tip**: `--webxml: Online XML style`.  

- **`--no-stylesheet` (No XML Stylesheet)**  
  - **What it does**: Skips stylesheet in XML output.  
  - **Example**: `nmap --no-stylesheet -oX scan.xml 192.168.1.100`.  
  - **Why it matters**: Keeps XML raw for parsing.  
  - **Notebook tip**: `--no-stylesheet: Raw XML`.  

---

## 11. Miscellaneous Options
These handle **general behavior** or special cases.

- **`-6` (IPv6 Scanning)**  
  - **What it does**: Enables IPv6 scanning.  
  - **Example**: `nmap -6 2001:db8::1`.  
  - **Why it matters**: Scans modern IPv6 networks.  
  - **Notebook tip**: `-6: IPv6 scan`.  

- **`-A` (Aggressive Scan)**  
  - **What it does**: Combines `-sC`, `-sV`, `-O`, `--traceroute`.  
  - **Example**: `sudo nmap -A 192.168.1.100`.  
  - **Why it matters**: All-in-one deep scan, but noisy.  
  - **Notebook tip**: `-A: All-in (scripts, versions, OS)`.  

- **`--datadir <dir>` (Custom Data Directory)**  
  - **What it does**: Sets directory for Nmap data files (e.g., scripts).  
  - **Example**: `nmap --datadir /custom/nmap 192.168.1.100`.  
  - **Why it matters**: Uses custom or updated data.  
  - **Notebook tip**: `--datadir: Custom data (/custom/nmap)`.  

- **`--servicedb <file>` (Service DB)**  
  - **What it does**: Uses a custom service database.  
  - **Example**: `nmap --servicedb custom-services 192.168.1.100`.  
  - **Why it matters**: Overrides default port-service mappings.  
  - **Notebook tip**: `--servicedb: Custom services`.  

- **`--versiondb <file>` (Version DB)**  
  - **What it does**: Uses a custom version database.  
  - **Example**: `nmap --versiondb custom-versions 192.168.1.100`.  
  - **Why it matters**: Improves version detection with new data.  
  - **Notebook tip**: `--versiondb: Custom versions`.  

- **`--send-eth` (Raw Ethernet)**  
  - **What it does**: Sends packets at Ethernet layer.  
  - **Example**: `sudo nmap --send-eth 192.168.1.100`.  
  - **Why it matters**: Bypasses OS stack for precision.  
  - **Notebook tip**: `--send-eth: Raw Ethernet`.  

- **`--send-ip` (Raw IP)**  
  - **What it does**: Sends packets at IP layer.  
  - **Example**: `sudo nmap --send-ip 192.168.1.100`.  
  - **Why it matters**: Uses OS stack, less control but simpler.  
  - **Notebook tip**: `--send-ip: Raw IP`.  

- **`--privileged` (Assume Privileges)**  
  - **What it does**: Assumes user has root privileges.  
  - **Example**: `nmap --privileged 192.168.1.100`.  
  - **Why it matters**: Bypasses permission checks (risky).  
  - **Notebook tip**: `--privileged: Assume root`.  

- **`--unprivileged` (No Privileges)**  
  - **What it does**: Runs as non-root, limits features.  
  - **Example**: `nmap --unprivileged 192.168.1.100`.  
  - **Why it matters**: Safe for restricted environments.  
  - **Notebook tip**: `--unprivileged: No root`.  

- **`--release-memory` (Free Memory)**  
  - **What it does**: Frees memory after scan.  
  - **Example**: `nmap --release-memory 192.168.1.100`.  
  - **Why it matters**: Reduces memory leaks on long scans.  
  - **Notebook tip**: `--release-memory: Free RAM`.  

- **`-h` (Help)**  
  - **What it does**: Shows Nmap’s help screen.  
  - **Example**: `nmap -h`.  
  - **Why it matters**: Quick reference for options.  
  - **Notebook tip**: `-h: Help screen`.  

- **`-V` / `--version` (Version)**  
  - **What it does**: Displays Nmap’s version.  
  - **Example**: `nmap -V`.  
  - **Why it matters**: Checks if you’re up-to-date.  
  - **Notebook tip**: `-V: Nmap version`.  

---

## 12. Runtime Interaction
These aren’t flags but **keys** you press during a scan (included for completeness).

- **`<Enter>`**: Shows scan status.  
- **Notebook tip**: `Enter: Scan status`.  

- **`v/V`**: Increase/decrease verbosity.  
- **Notebook tip**: `v/V: More/less info`.  

- **`d/D`**: Increase/decrease debug level.  
- **Notebook tip**: `d/D: More/less debug`.  

- **`p/P`**: Enable/disable packet tracing.  
- **Notebook tip**: `p/P: Packet trace on/off`.  

- **`?`**: Shows runtime help.  
- **Notebook tip**: `?: Runtime help`.  

---

\
