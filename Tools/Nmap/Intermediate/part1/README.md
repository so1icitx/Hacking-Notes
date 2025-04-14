
#  Nmap Service Detection, OS Detection, Traceroute, Scripting, and Output


## Why Go Beyond Port Scanning?

Finding open ports with scans like SYN (`-sS`) or TCP Connect (`-sT`) is just the start. To really understand a target, we need to:

- **Identify Services**: What’s running on port 80? Is it Apache or Nginx? What version?
- **Detect the OS**: Is it Linux, Windows, or something exotic? Knowing the system helps plan attacks.
- **Map the Network**: How many hops to the target? Any routers in the way?
- **Automate Recon**: Scripts can pull extra info—like server banners or vulnerabilities—without manual poking.
- **Save Results**: Store scan data for later analysis or sharing with a team.

These steps turn raw port data into actionable intel, helping us find weak spots (check out TryHackMe’s *Vulnerabilities 101* for more on exploiting services). Let’s break down each feature, how it works, and why it matters.

---

## Service Detection (`-sV`)

**Service Detection** probes open ports to identify the running service and its version. This is gold for pentesters because outdated versions often have known vulnerabilities.

- **How It Works**:
  - Adds `-sV` to your Nmap command.
  - Nmap completes the **TCP 3-way handshake** (SYN → SYN/ACK → ACK) to connect to the service.
  - Sends protocol-specific probes to grab the service’s **banner** (e.g., “Welcome to OpenSSH 6.7p1”).
  - Matches the response to a database of known services and versions.
- **Key Details**:
  - **Not Stealthy**: Unlike SYN scans (`-sS`), `-sV` requires a full connection, which can be logged.
  - **Intensity Control**:
    - `--version-intensity <0-9>`: 0 is lightest (quick, less accurate), 9 is deepest (slow, thorough).
    - `--version-light`: Intensity 2 (fast, decent results).
    - `--version-all`: Intensity 9 (tries everything, slow).
  - **Output**: Adds a **VERSION** column with service details (e.g., “nginx 1.6.2”).
- **Why Use It?**:
  - Pinpoints exact software versions to search for CVEs (e.g., in Exploit-DB or NIST).
  - Confirms guesses—port 22 might be SSH, but is it OpenSSH or Dropbear?
- **Downsides**:
  - Slower than basic scans due to full connections.
  - May trigger IDS if the service logs connections.
  - Requires **root privileges** for some probes.

### Example
```bash
sudo nmap -sV 10.10.161.170
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-10 05:03 BST
  Nmap scan report for 10.10.161.170
  Host is up (0.0040s latency).
  Not shown: 995 closed ports
  PORT    STATE SERVICE VERSION
  22/tcp  open  ssh     OpenSSH 6.7p1 Debian 5+deb8u8 (protocol 2.0)
  25/tcp  open  smtp    Postfix smtpd
  80/tcp  open  http    nginx 1.6.2
  110/tcp open  pop3    Dovecot pop3d
  111/tcp open  rpcbind 2-4 (RPC #100000)
  MAC Address: 02:A0:E7:B5:B6:C5 (Unknown)
  Service Info: Host: debra2.thm.local; OS: Linux; CPE: cpe:/o:linux:linux_kernel
  Nmap done: 1 IP address (1 host up) scanned in 8.40 seconds
  ```
- **Explanation**:
  - **Port 22**: Not just “ssh” but “OpenSSH 6.7p1 Debian 5+deb8u8”—specific enough to check for vulnerabilities.
  - **Port 80**: “nginx 1.6.2” tells us it’s an old web server, ripe for exploits.
  - **Service Info**: Bonus details like hostname (`debra2.thm.local`) and OS guess (`Linux`).
  - **Time**: 8.4 seconds—slower than a basic scan due to connections.

**Deep Dive**:
- Nmap guesses services based on port numbers (e.g., 22 = ssh) but uses `-sV` to *confirm* by talking to the service.
- Example: Port 80 might run a custom app, not HTTP. `-sV` catches this by analyzing the banner.
- **Intensity Matters**: Level 9 (`--version-all`) tries every probe, catching obscure services but taking ages. Level 2 (`--version-light`) is a good balance for most scans.

**Pro Tip**: Start with `--version-light` for speed, then use `--version-intensity 9` on critical ports (e.g., `-p80`) if you need more detail. Check Exploit-DB for versions like “nginx 1.6.2” to find exploits.

---

## OS Detection (`-O`)

**OS Detection** guesses the target’s operating system and version based on TCP/IP stack behavior, like how it responds to probes or sets packet fields.

- **How It Works**:
  - Adds `-O` (uppercase “O” for OS).
  - Sends a mix of TCP and UDP probes to analyze responses.
  - Compares the “fingerprint” to Nmap’s database of known OSes.
  - Looks at quirks like TTL, window size, or TCP options.
- **Key Details**:
  - Needs at least **one open and one closed port** for reliable guesses.
  - Outputs:
    - **Device Type**: E.g., “general purpose.”
    - **Running**: OS and version range (e.g., “Linux 3.X”).
    - **OS CPE**: Standard ID for the OS.
    - **OS Details**: Specific guess (e.g., “Linux 3.13”).
- **Why Use It?**:
  - Helps tailor attacks (e.g., Linux exploits vs. Windows).
  - Guides tool choices (e.g., Metasploit modules).
- **Downsides**:
  - **Accuracy Varies**:
    - Virtualization or custom stacks can confuse Nmap.
    - Example: A Fedora 5.13 kernel might be misread as Linux 2.6.X.
  - Requires **root privileges**.
  - Adds scan time.
- **When to Use**: Early in recon to understand the target’s ecosystem.

### Example
```bash
sudo nmap -sS -O 10.10.161.170
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-10 05:04 BST
  Nmap scan report for 10.10.161.170
  Host is up (0.00099s latency).
  Not shown: 994 closed ports
  PORT    STATE SERVICE
  22/tcp  open  ssh
  25/tcp  open  smtp
  80/tcp  open  http
  110/tcp open  pop3
  111/tcp open  rpcbind
  143/tcp open  imap
  MAC Address: 02:A0:E7:B5:B6:C5 (Unknown)
  Device type: general purpose
  Running: Linux 3.X
  OS CPE: cpe:/o:linux:linux_kernel:3.13
  OS details: Linux 3.13
  Network Distance: 1 hop
  Nmap done: 1 IP address (1 host up) scanned in 3.91 seconds
  ```
- **Explanation**:
  - **OS Guess**: “Linux 3.X,” specifically “Linux 3.13.”
  - **Reality**: Target runs kernel 3.16—close, but not exact.
  - **Other Info**: “General purpose” device, one hop away (same subnet).
  - **Why Close?**: Nmap matched TCP stack traits but missed the exact kernel due to similar behavior in 3.X series.

**Deep Dive**:
- Nmap’s OS detection relies on subtle differences, like how Linux sets TCP timestamps vs. Windows. But:
  - **Virtualization**: VMs can mask the real OS.
  - **Firewalls**: Packet filtering distorts fingerprints.
  - **Custom Stacks**: Admins tweaking TCP settings throw Nmap off.
- Accuracy improves with more ports scanned, so combine `-O` with `-p-` (all ports) if you’re desperate.

**Pro Tip**: Treat OS guesses as hints, not gospel. Cross-check with service versions (e.g., “Debian” in SSH banner) or manual recon (e.g., HTTP headers). Report wrong guesses to `https://nmap.org/submit/`.

---

## Traceroute (`--traceroute`)

**Traceroute** maps the network path to the target, showing routers (hops) between you and the system.

- **How It Works**:
  - Adds `--traceroute` to your scan.
  - Sends packets with decreasing **TTL** (Time to Live) values.
  - Each router decrements TTL and responds with an **ICMP Time Exceeded** message if TTL hits 0, revealing its IP.
  - Continues until the target is reached.
- **Key Details**:
  - Differs from Linux’s `traceroute` (low TTL, increasing) or Windows’s `tracert`.
  - Shows **HOP**, **RTT** (round-trip time), and **ADDRESS**.
- **Why Use It?**:
  - Understands network topology—critical for planning attacks or spotting firewalls.
  - Identifies latency or blocking routers.
- **Downsides**:
  - Many routers block ICMP, hiding themselves.
  - Adds slight scan time.
  - Requires **root privileges**.
- **When to Use**: To map the network before deep scanning or when troubleshooting connectivity.

### Example
```bash
sudo nmap -sS --traceroute 10.10.161.170
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-10 05:05 BST
  Nmap scan report for 10.10.161.170
  Host is up (0.0015s latency).
  Not shown: 994 closed ports
  PORT    STATE SERVICE
  22/tcp  open  ssh
  25/tcp  open  smtp
  80/tcp  open  http
  110/tcp open  pop3
  111/tcp open  rpcbind
  143/tcp open  imap
  MAC Address: 02:A0:E7:B5:B6:C5 (Unknown)
  TRACEROUTE
  HOP RTT     ADDRESS
  1   1.48 ms 10.10.161.170
  Nmap done: 1 IP address (1 host up) scanned in 1.59 seconds
  ```
- **Explanation**:
  - **One Hop**: The target is on the same subnet (no routers).
  - **RTT**: 1.48 ms—fast, local connection.
  - **Why No Routers?**: Direct link, typical in lab setups like TryHackMe.

**Deep Dive**:
- Nmap’s traceroute starts with a high TTL (e.g., 30) and decreases, unlike standard traceroute’s low-to-high approach. This can bypass some filters but fails if routers block ICMP.
- **Use Case**: If you see unexpected hops or high latency, a firewall or NAT might be in play—time to adjust your scan strategy.

**Pro Tip**: Combine `--traceroute` with `-Pn` (no ping) if the target blocks ICMP pings but allows other traffic. It’s a quick way to confirm network paths.

---

## Nmap Scripting Engine (NSE) (`-sC`, `--script`)

The **Nmap Scripting Engine (NSE)** supercharges Nmap with **Lua scripts** that automate tasks like grabbing banners, checking vulnerabilities, or even brute-forcing logins. Think of it as Nmap’s plugin system—no coding required!

- **How It Works**:
  - NSE runs scripts written in Lua to interact with services.
  - Scripts are stored in `/usr/share/nmap/scripts/` (on Linux, like TryHackMe’s AttackBox).
  - Trigger with:
    - `-sC`: Runs **default** category scripts.
    - `--script=<name>`: Runs specific scripts (e.g., `http-date`).
    - `--script=<pattern>`: Runs matching scripts (e.g., `http*`).
- **Key Details**:
  - **Script Categories**:
    | **Category** | **Description** |
    |--------------|-----------------|
    | `auth`       | Checks authentication issues |
    | `broadcast`  | Discovers hosts via broadcasts |
    | `brute`      | Brute-forces logins |
    | `default`    | Safe, general-purpose scripts (`-sC`) |
    | `discovery`  | Grabs info (e.g., DNS names, DB tables) |
    | `dos`        | Tests for DoS vulnerabilities |
    | `exploit`    | Exploits services (use cautiously) |
    | `external`   | Uses third-party services (e.g., VirusTotal) |
    | `fuzzer`     | Fuzzes inputs for bugs |
    | `intrusive`  | Aggressive scripts (may crash targets) |
    | `malware`    | Detects backdoors |
    | `safe`       | Won’t harm targets |
    | `version`    | Enhances version detection |
    | `vuln`       | Scans for vulnerabilities |
  - **Script Count**: ~600 in a default Nmap install (e.g., 130+ `http*` scripts).
  - **Custom Scripts**: Write your own or download from trusted sources (beware malware!).
- **Why Use It?**:
  - Automates recon: Pulls SSH keys, HTTP titles, or vuln info in one go.
  - Saves time vs. manual checks (e.g., `nc` to grab banners).
  - Finds low-hanging fruit (e.g., default credentials).
- **Downsides**:
  - **Risky Scripts**: `brute`, `dos`, or `exploit` can crash or exploit targets—use only with permission.
  - **Time**: Scripts slow scans, especially `intrusive` or `vuln`.
  - **False Positives**: Some scripts misfire on unusual setups.
  - Requires **root privileges** for many scripts.
- **When to Use**: After finding open ports to gather detailed intel or test for specific issues.

### Example (Default Scripts)
```bash
sudo nmap -sS -sC 10.10.161.170
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-10 05:08 BST
  Nmap scan report for ip-10-10-161-170.eu-west-1.compute.internal (10.10.161.170)
  Host is up (0.0011s latency).
  Not shown: 994 closed ports
  PORT    STATE SERVICE
  22/tcp  open  ssh
  | ssh-hostkey: 
  |   1024 d5:80:97:a3:a8:3b:57:78:2f:0a:78:ae:ad:34:24:f4 (DSA)
  |   2048 aa:66:7a:45:eb:d1:8c:00:e3:12:31:d8:76:8e:ed:3a (RSA)
  |   256 3d:82:72:a3:07:49:2e:cb:d9:87:db:08:c6:90:56:65 (ECDSA)
  |_  256 dc:f0:0c:89:70:87:65:ba:52:b1:e9:59:f7:5d:d2:6a (EdDSA)
  25/tcp  open  smtp
  |_smtp-commands: debra2.thm.local, PIPELINING, SIZE 10240000, VRFY, ETRN, STARTTLS, ENHANCEDSTATUSCODES, 8BITMIME, DSN
  | ssl-cert: Subject: commonName=debra2.thm.local
  | Not valid before: 2021-08-10T12:10:58
  |_Not valid after:  2031-08-08T12:10:58
  |_ssl-date: TLS randomness does not represent time
  80/tcp  open  http
  |_http-title: Welcome to nginx on Debian!
  110/tcp open  pop3
  |_pop3-capabilities: RESP-CODES CAPA TOP SASL UIDL PIPELINING AUTH-RESP-CODE
  111/tcp open  rpcbind
  | rpcinfo: 
  |   program version   port/proto  service
  |   100000  2,3,4        111/tcp  rpcbind
  |   100000  2,3,4        111/udp  rpcbind
  |   100024  1          38099/tcp  status
  |_  100024  1          54067/udp  status
  143/tcp open  imap
  |_imap-capabilities: LITERAL+ capabilities IMAP4rev1 OK Pre-login ENABLE have LOGINDISABLEDA0001 listed SASL-IR ID more post-login LOGIN-REFERRALS IDLE
  MAC Address: 02:A0:E7:B5:B6:C5 (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 2.21 seconds
  ```
- **Explanation**:
  - **Port 22 (ssh)**: Lists all SSH host keys (DSA, RSA, ECDSA, EdDSA)—useful for verifying server identity or spotting weak keys.
  - **Port 25 (smtp)**: Shows Postfix commands and an SSL cert for `debra2.thm.local`, hinting at misconfigs or trust issues.
  - **Port 80 (http)**: Grabs the page title (“Welcome to nginx on Debian!”)—a default page, screaming “check me for misconfigs!”
  - **Port 111 (rpcbind)**: Details RPC services, which might lead to exploits like NFS shares.
  - **Takeaway**: Default scripts add rich context, turning ports into stories.

### Example (Specific Script)
```bash
sudo nmap -sS -n --script "http-date" 10.10.161.170
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-10 08:04 BST
  Nmap scan report for 10.10.161.170
  Host is up (0.0011s latency).
  Not shown: 994 closed ports
  PORT    STATE SERVICE
  22/tcp  open  ssh
  25/tcp  open  smtp
  80/tcp  open  http
  |_http-date: Fri, 10 Sep 2021 07:04:26 GMT; 0s from local time.
  110/tcp open  pop3
  111/tcp open  rpcbind
  143/tcp open  imap
  MAC Address: 02:44:87:82:AC:83 (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1.78 seconds
  ```
- **Explanation**:
  - **http-date**: Pulls the server’s time from port 80’s HTTP headers.
  - **Result**: “Fri, 10 Sep 2021 07:04:26 GMT”—matches local time, no skew.
  - **Use Case**: Time mismatches could hint at misconfigured servers or spoofed responses.

**Deep Dive**:
- **Script Safety**:
  - **Safe Scripts** (e.g., `http-date`): Low risk, just query the service.
  - **Intrusive Scripts** (e.g., `http-brute`): Might lock accounts or crash weak servers.
  - **Exploit Scripts** (e.g., `http-vuln-cve2014-3704`): Can pwn systems—only run with explicit permission.
- **Finding Scripts**: Check `/usr/share/nmap/scripts/` or Nmap’s online docs. Example: `less /usr/share/nmap/scripts/ftp-brute.nse` shows it’s a brute-forcer—avoid unless authorized.
- **Custom Scripts**:
  - Write Lua scripts for niche tasks (e.g., custom banner grabs).
  - Download from trusted sources only—unverified scripts could be malicious.

**Pro Tip**:
- Start with `-sC` for safe defaults.
- Use `--script "http*"` for web-focused recon, but check each script’s description first (e.g., `cat /usr/share/nmap/scripts/http-brute.nse`).
- Combine with `-p<port>` to target specific services (e.g., `-p80 --script "http-title"`).

---

## Saving Nmap Output

Nmap lets you save scan results in various formats to analyze later, share with a team, or parse with tools like `grep`. Here are the main options:

- **Normal Output (`-oN <file>`)**:
  - Human-readable text, same as terminal output.
  - Great for quick reference or manual review.
- **XML Output (`-oX <file>`)**:
  - Structured XML format.
  - Ideal for parsing with scripts or tools like Metasploit, Nessus, or custom Python code.
- **Grepable Output (`-oG <file>`)**:
  - One-line-per-host format, easy to `grep`.
  - Perfect for quick searches or piping to other tools.
- **All Formats (`-oA <basename>`)**:
  - Saves in **all three** formats (`<basename>.nmap`, `<basename>.xml`, `<basename>.gnmap`).
  - Best for flexibility—cover all bases.
- **Script Kiddie Output (`-oS <file>`)**):
  - A jokey, 1337-h4x0r style text format (rarely used).
  - Fun for laughs, not practical.

### Example
```bash
sudo nmap -sS -sV -oA scan_results 10.10.161.170
```
- **What it does**:
  - Runs a SYN scan with service detection.
  - Saves three files:
    - `scan_results.nmap`: Normal text.
    - `scan_results.xml`: XML for tools.
    - `scan_results.gnmap`: Grepable format.
- **Normal Output (`scan_results.nmap`)**:
  ```
  # Nmap 7.60 scan initiated 2021-09-10 05:03 BST as: nmap -sS -sV -oA scan_results 10.10.161.170
  Nmap scan report for 10.10.161.170
  Host is up (0.0040s latency).
  Not shown: 995 closed ports
  PORT    STATE SERVICE VERSION
  22/tcp  open  ssh     OpenSSH 6.7p1 Debian 5+deb8u8 (protocol 2.0)
  ...
  Nmap done: 1 IP address (1 host up) scanned in 8.40 seconds
  ```
- **XML Snippet (`scan_results.xml`)**:
  ```xml
  <nmaprun scanner="nmap" args="nmap -sS -sV -oA scan_results 10.10.161.170" start="1631246639">
    <host>
      <address addr="10.10.161.170" addrtype="ipv4"/>
      <ports>
        <port protocol="tcp" portid="22">
          <state state="open" reason="syn-ack" reason_ttl="64"/>
          <service name="ssh" product="OpenSSH" version="6.7p1 Debian 5+deb8u8" extrainfo="protocol 2.0"/>
        </port>
        ...
      </ports>
    </host>
  </nmaprun>
  ```
- **Grepable Output (`scan_results.gnmap`)**:
  ```
  Host: 10.10.161.170 () Status: Up
  Host: 10.10.161.170 () Ports: 22/open/tcp//ssh//OpenSSH 6.7p1 Debian 5+deb8u8 (protocol 2.0)/, 25/open/tcp//smtp//Postfix smtpd/, ...
  ```

### Example (Grep for Open Ports)
```bash
grep "open" scan_results.gnmap
```
- **Output**:
  ```
  Host: 10.10.161.170 () Ports: 22/open/tcp//ssh//OpenSSH 6.7p1 Debian 5+deb8u8 (protocol 2.0)/, 25/open/tcp//smtp//Postfix smtpd/, ...
  ```
- **Use Case**: Quickly find all open ports for further analysis.

**Deep Dive**:
- **Normal**: Best for humans, but hard to parse programmatically.
- **XML**: The go-to for automation—tools like `xml2json` or Python’s `xml.etree` can process it.
- **Grepable**: Simplifies scripting (e.g., `awk` to extract IPs with port 80 open).
- **All (`-oA`)**: Default choice for serious scans—gives you options without re-running.
- **Storage Tip**: Save to a dated folder (e.g., `scans/2025-04-14/`) to stay organized.

**Pro Tip**:
- Use `-oA` for every scan to future-proof your work.
- Parse `.gnmap` with `grep "80/open"` to focus on web servers, or import `.xml` into Metasploit for exploit matching.
- Add `-v` or `--reason` to outputs for more context (e.g., why a port is “open”).

---

## Practical Workflow Example

Here’s a real-world scan combining everything:

```bash
sudo nmap -sS -sV --version-intensity 5 -O --traceroute -sC -oA full_scan_2021-09-10 10.10.161.170
```
- **Breakdown**:
  - **`-sS`**: Stealth SYN scan for speed.
  - **`-sV --version-intensity 5`**: Balanced service detection (not too slow, not too shallow).
  - **`-O`**: Guesses the OS.
  - ** `--traceroute`**: Maps the network path.
  - **`-sC`**: Runs default scripts for extra recon.
  - **`-oA full_scan_2021-09-10`**: Saves normal, XML, and grepable outputs.
- **What You Get**:
  - Open ports with versions (e.g., “nginx 1.6.2”).
  - OS guess (e.g., “Linux 3.X”).
  - Network hops (e.g., one hop to target).
  - Script goodies (e.g., HTTP titles, SSH keys).
  - Three output files for analysis.

**Follow-Up**:
- Check `full_scan_2021-09-10.nmap` for a quick read.
- Grep `full_scan_2021-09-10.gnmap` for specific ports (`grep "80/open"`).
- Import `full_scan_2021-09-10.xml` into Metasploit or a custom script.
- Search Exploit-DB for “nginx 1.6.2” or “OpenSSH 6.7p1” exploits.

---

## Summary of Nmap Features

| **Feature**          | **Option**               | **Purpose**                              | **Privileges** | **Best For**                       |
|----------------------|--------------------------|------------------------------------------|----------------|------------------------------------|
| Service Detection    | `-sV`                    | Identifies service and version           | Root (some)    | Finding vulnerable software        |
| OS Detection         | `-O`                     | Guesses OS and version                   | Root           | Tailoring attacks                 |
| Traceroute           | `--traceroute`           | Maps network path to target              | Root           | Network topology recon            |
| Default Scripts      | `-sC`                    | Runs safe, general-purpose scripts       | Root (some)    | Quick automated recon             |
| Specific Scripts     | `--script=<name>`        | Runs targeted scripts                    | Root (some)    | Deep service analysis             |
| Normal Output        | `-oN <file>`             | Saves human-readable text                | None           | Manual review                     |
| XML Output           | `-oX <file>`             | Saves structured XML                     | None           | Automation, tool integration      |
| Grepable Output      | `-oG <file>`             | Saves one-line-per-host text             | None           | Scripting, quick searches         |
| All Outputs          | `-oA <basename>`         | Saves normal, XML, grepable              | None           | Comprehensive archiving           |

---

## Practical Tips and Caveats

- **Start Light**: Use `-sS -sC` for quick recon, then add `-sV` or specific scripts (`--script "http*"`) for juicy ports.
- **Balance Intensity**: `--version-intensity 5` is a sweet spot for service detection—fast enough for labs, thorough enough for results.
- **Script Safety**: Avoid `brute`, `dos`, or `exploit` scripts unless authorized—they can break things or get you in trouble.
- **OS Skepticism**: Nmap’s OS guesses are educated but not perfect. Verify with service banners or manual checks.
- **Traceroute Limits**: Firewalls blocking ICMP can hide routers—try TCP-based scans if needed.
- **Output Organization**: Name files with dates and targets (e.g., `scan_MACHINE_IP_2025-04-14.nmap`) to avoid chaos.
- **Stealth vs. Depth**: `-sV` and scripts are noisy (full connections), so use `-T1` for stealth or save them for trusted networks.
- **Ethics First**: Only scan systems you’re allowed to (like TryHackMe VMs). Unauthorized scanning is a crime.

---

## Bonus: Analyzing Results

Once you’ve got your scan data, here’s how to use it:

- **Service Versions**:
  - Search Exploit-DB or NIST NVD for CVEs (e.g., “nginx 1.6.2 exploit”).
  - Example: Nginx 1.6.2 has known DoS vulnerabilities—check CVE-2014-0133.
- **OS Info**:
  - Linux 3.X suggests an older distro (e.g., Debian 8). Look for kernel exploits or misconfigs.
  - Cross-check with banners (e.g., “Debian” in SSH output).
- **Script Output**:
  - HTTP title “Welcome to nginx on Debian!” screams default config—check for exposed files or weak settings.
  - SSH keys might reveal outdated algorithms (e.g., DSA is deprecated).
- **Output Parsing**:
  - Use `grep` on `.gnmap`: `grep "22/open" scans/*.gnmap` to find all SSH servers.
  - Convert `.xml` to JSON with Python for custom reports.
  - Feed `.xml` to Metasploit’s `db_import` for exploit matching.

**Example Script** (Python to parse XML):
```python
import xml.etree.ElementTree as ET
tree = ET.parse('full_scan_2021-09-10.xml')
for port in tree.findall('.//port'):
    state = port.find('state').attrib['state']
    service = port.find('service').attrib.get('name', 'unknown')
    version = port.find('service').attrib.get('product', '') + ' ' + port.find('service').attrib.get('version', '')
    if state == 'open':
        print(f"Port {port.attrib['portid']}: {service} ({version.strip()})")
```
- **Output**: Lists open ports with versions—easy to feed into vuln scanners.

---

---

