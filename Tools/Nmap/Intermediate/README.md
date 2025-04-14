
# Intermediate Nmap Port Scans

## Why Advanced Port Scans?

Port scanning is about finding which services (e.g., web servers, SSH) are running on a host by probing **TCP** or **UDP** ports. Basic scans like TCP Connect (`-sT`) or SYN (`-sS`) are great, but sometimes you need stealthier or specialized techniques to:

- **Bypass Firewalls**: Sneak past stateless firewalls that only check for SYN packets.
- **Map Firewall Rules**: Figure out what a firewall allows or blocks.
- **Stay Stealthy**: Hide your IP or make scans harder to trace.
- **Handle Tricky Networks**: Deal with systems that don’t play nice with standard scans.

Advanced scans tweak **TCP flags** (SYN, ACK, RST, FIN, PSH, URG) in unexpected ways to provoke responses that reveal port states: **open**, **closed**, **filtered**, **unfiltered**, **open|filtered**, or **closed|filtered**. Let’s explore each technique, why it works, and when to use it.

---

## Understanding TCP Flags and Port States

Before we dive in, let’s recap the **TCP flags** in the TCP header, as they’re the heart of these scans:

- **SYN**: Starts a connection (used in 3-way handshake).
- **ACK**: Acknowledges received data.
- **RST**: Resets a connection (sent by closed ports or firewalls).
- **FIN**: Signals the end of data transmission.
- **PSH**: Pushes data to the application immediately.
- **URG**: Marks data as urgent (rarely used).

**Port States** (from Nmap’s perspective):
- **Open**: A service is listening (e.g., HTTP on port 80).
- **Closed**: No service, but the port is reachable (sends RST).
- **Filtered**: Firewall or network issue blocks Nmap’s probes.
- **Unfiltered**: Reachable, but Nmap can’t confirm open or closed (common in ACK scans).
- **Open|Filtered**: Could be open or filtered (no response).
- **Closed|Filtered**: Could be closed or filtered (uncertain).

These scans rely on how systems respond to weird flag combinations, often exploiting quirks in TCP/IP implementations or firewall behavior.

---

## Null Scan (`-sN`)

The **Null Scan** is a stealthy technique that sends a TCP packet with **no flags set** (all flag bits are 0). It’s like knocking on a door with no message—open ports stay silent, but closed ports slam the door shut.

- **How It Works**:
  - Sends a TCP packet with no flags to a port.
  - **Open Port**: No response (marked `open|filtered`).
  - **Closed Port**: Sends an **RST** packet (marked `closed`).
  - **Filtered Port**: No response or blocked (also `open|filtered`).
- **Why Use It?**:
  - Bypasses **stateless firewalls** that only look for SYN packets.
  - Stealthy—no connection is attempted.
- **Downsides**:
  - Can’t distinguish between open and filtered ports.
  - Modern **stateful firewalls** block these packets.
  - Requires **root privileges** (`sudo`).
- **When to Use**: Against older systems or networks with basic firewalls.

### Example
```bash
sudo nmap -sN 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:30 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.00066s latency).
  Not shown: 994 closed ports
  PORT    STATE         SERVICE
  22/tcp  open|filtered ssh
  25/tcp  open|filtered smtp
  80/tcp  open|filtered http
  110/tcp open|filtered pop3
  111/tcp open|filtered rpcbind
  143/tcp open|filtered imap
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 96.50 seconds
  ```
- **Explanation**:
  - Six ports are marked `open|filtered` because they didn’t send an RST.
  - The other 994 ports sent RSTs, so they’re `closed`.
  - A firewall might be hiding open ports, so results aren’t definitive.

**Pro Tip**: Null scans are slow because Nmap waits for timeouts on non-responding ports. Use `-T4` for faster scans in labs, but stick to `-T1` for stealth in real engagements.

---

## FIN Scan (`-sF`)

The **FIN Scan** sends a TCP packet with only the **FIN flag** set, signaling the end of a connection that never existed. It’s another sneaky way to probe ports without starting a handshake.

- **How It Works**:
  - Sends a TCP packet with **FIN** set.
  - **Open Port**: No response (marked `open|filtered`).
  - **Closed Port**: Sends an **RST** packet (marked `closed`).
  - **Filtered Port**: No response or blocked (also `open|filtered`).
- **Why Use It?**:
  - Evades stateless firewalls expecting SYN packets.
  - Subtle—mimics a connection close, not a start.
- **Downsides**:
  - Like Null, it can’t confirm open vs. filtered.
  - Some firewalls silently drop FIN packets, reducing effectiveness.
  - Requires **root privileges**.
- **When to Use**: Similar to Null scans—useful against legacy systems or misconfigured firewalls.

### Example
```bash
sudo nmap -sF 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:32 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.0018s latency).
  Not shown: 994 closed ports
  PORT    STATE         SERVICE
  22/tcp  open|filtered ssh
  25/tcp  open|filtered smtp
  80/tcp  open|filtered http
  110/tcp open|filtered pop3
  111/tcp open|filtered rpcbind
  143/tcp open|filtered imap
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 96.52 seconds
  ```
- **Explanation**:
  - Same results as the Null scan—six ports are `open|filtered`.
  - Closed ports sent RSTs, confirming they’re not listening.
  - Firewalls might be skewing results, so cross-check with other scans.

**Deep Dive**: The FIN scan relies on RFC 793, which says open ports ignore unexpected FIN packets, but closed ports reset the connection. Some modern systems (e.g., Windows) don’t follow this strictly, so FIN scans may fail there.

---

## Xmas Scan (`-sX`)

The **Xmas Scan** lights up the TCP header like a Christmas tree by setting the **FIN**, **PSH**, and **URG** flags together. It’s flashy and meant to confuse simplistic firewalls.

- **How It Works**:
  - Sends a TCP packet with **FIN**, **PSH**, and **URG** set.
  - **Open Port**: No response (marked `open|filtered`).
  - **Closed Port**: Sends an **RST** packet (marked `closed`).
  - **Filtered Port**: No response or blocked (also `open|filtered`).
- **Why Use It?**:
  - Bypasses stateless firewalls that don’t expect this odd combo.
  - Fun name, hacker cred!
- **Downsides**:
  - Same ambiguity as Null and FIN (`open|filtered` issue).
  - Stateful firewalls block it easily.
  - Requires **root privileges**.
- **When to Use**: Against older networks or for testing firewall responses.

### Example
```bash
sudo nmap -sX 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:34 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.00087s latency).
  Not shown: 994 closed ports
  PORT    STATE         SERVICE
  22/tcp  open|filtered ssh
  25/tcp  open|filtered smtp
  80/tcp  open|filtered http
  110/tcp open|filtered pop3
  111/tcp open|filtered rpcbind
  143/tcp open|filtered imap
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 84.85 seconds
  ```
- **Explanation**:
  - Identical to Null and FIN scans—six ports are `open|filtered`.
  - The Xmas scan’s funky flags didn’t change the outcome here.
  - Likely a Linux server following RFC 793 behavior.

**Real-World Note**: Xmas scans are noisy despite their stealthy intent. Modern IDS systems flag packets with multiple flags set, so use sparingly.

---

## Maimon Scan (`-sM`)

The **Maimon Scan**, named after its discoverer Uriel Maimon (1996), sets both **FIN** and **ACK** flags. It’s a niche scan that exploits quirks in some BSD-derived systems.

- **How It Works**:
  - Sends a TCP packet with **FIN** and **ACK** set.
  - **Open Port** (on BSD systems): Drops the packet (marked `open|filtered`).
  - **Closed Port**: Sends an **RST** packet (marked `closed`).
  - **Filtered Port**: No response or blocked.
- **Why Use It?**:
  - Targets rare BSD-based systems where open ports drop FIN/ACK packets.
  - Historical curiosity—shows the hacker mindset of exploiting edge cases.
- **Downsides**:
  - Useless on modern systems (Linux, Windows) where open and closed ports both send RST.
  - Requires **root privileges**.
  - Rarely effective today.
- **When to Use**: Only if you suspect a legacy BSD system (almost never).

### Example
```bash
sudo nmap -sM 10.10.252.27
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:36 BST
  Nmap scan report for ip-10-10-252-27.eu-west-1.compute.internal (10.10.252.27)
  Host is up (0.00095s latency).
  All 1000 scanned ports on ip-10-10-252-27.eu-west-1.compute.internal (10.10.252.27) are closed
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1.61 seconds
  ```
- **Explanation**:
  - No open ports detected—all sent RSTs, suggesting a non-BSD system (likely Linux).
  - The scan failed because modern TCP stacks don’t differentiate FIN/ACK behavior.

**Why It’s Obscure**: The Maimon scan hinges on a 1990s BSD bug. It’s a cool piece of hacking history but mostly a museum piece today.

---

## TCP ACK Scan (`-sA`)

The **TCP ACK Scan** sends a TCP packet with only the **ACK** flag set, mimicking a response to a non-existent connection. It’s not about finding open ports—it’s about mapping firewalls.

- **How It Works**:
  - Sends a TCP packet with **ACK** set.
  - **Open or Closed Port**: Sends an **RST** packet (marked `unfiltered`).
  - **Filtered Port**: No response or blocked (marked `filtered`).
- **Why Use It?**:
  - Reveals which ports a firewall allows through (`unfiltered`).
  - Doesn’t care about port state—focuses on firewall rules.
- **Downsides**:
  - Can’t distinguish open vs. closed ports (all `unfiltered` ports send RST).
  - Requires **root privileges**.
  - Limited to firewall mapping, not service discovery.
- **When to Use**: To understand a firewall’s configuration before launching other scans.

### Example (No Firewall)
```bash
sudo nmap -sA 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:37 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.0013s latency).
  All 1000 scanned ports on 10.10.9.65 are unfiltered
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1.68 seconds
  ```
- **Explanation**:
  - All ports are `unfiltered`—no firewall is blocking ACK packets.
  - Useless here since we can’t learn about services.

### Example (With Firewall)
```bash
sudo nmap -sA 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-07 11:34 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.00046s latency).
  Not shown: 997 filtered ports
  PORT    STATE      SERVICE
  22/tcp  unfiltered ssh
  25/tcp  unfiltered smtp
  80/tcp  unfiltered http
  MAC Address: 02:78:C0:D0:4E:E9 (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 15.45 seconds
  ```
- **Explanation**:
  - Only ports 22, 25, and 80 are `unfiltered`—the firewall allows ACK packets through these.
  - The other 997 ports are `filtered`, meaning the firewall blocks them.
  - **Key Insight**: These ports *might* have services, but the ACK scan only shows firewall rules, not port states.

**Pro Tip**: Use ACK scans early to map firewall rules, then follow with SYN (`-sS`) or Connect (`-sT`) scans on `unfiltered` ports to find actual services.

---

## TCP Window Scan (`-sW`)

The **TCP Window Scan** is a cousin of the ACK scan, sending **ACK** packets but inspecting the **TCP Window field** in the RST replies to guess port states.

- **How It Works**:
  - Sends a TCP packet with **ACK** set.
  - **Open Port** (on some systems): RST with a positive window size.
  - **Closed Port**: RST with a zero window size.
  - **Filtered Port**: No response or blocked.
- **Why Use It?**:
  - Exploits quirks in some TCP stacks (e.g., older systems) where open ports return different window sizes.
  - Helps map firewalls like the ACK scan.
- **Downsides**:
  - Rarely works—most modern systems use zero window sizes for all RSTs.
  - Requires **root privileges**.
  - Confusing results if the target doesn’t cooperate.
- **When to Use**: When you suspect an old system or need to double-check firewall rules.

### Example (No Firewall)
```bash
sudo nmap -sW 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:38 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.0011s latency).
  All 1000 scanned ports on ip-10-10-252-27.eu-west-1.compute.internal (10.10.252.27) are closed
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1.60 seconds
  ```
- **Explanation**:
  - All ports marked `closed`—the target’s TCP stack doesn’t differentiate window sizes.
  - No useful info without a firewall.

### Example (With Firewall)
```bash
sudo nmap -sW 10.10.9.65
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-09-07 11:39 BST
  Nmap scan report for 10.10.9.65
  Host is up (0.00040s latency).
  Not shown: 997 filtered ports
  PORT    STATE  SERVICE
  22/tcp  closed ssh
  25/tcp  closed smtp
  80/tcp  closed http
  MAC Address: 02:78:C0:D0:4E:E9 (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 14.84 seconds
  ```
- **Explanation**:
  - Ports 22, 25, and 80 are marked `closed`, but this likely means they’re `unfiltered` and responding differently.
  - The firewall blocks the other 997 ports (`filtered`).
  - **Caution**: “Closed” here doesn’t mean no service—it just means RSTs were received. Cross-check with a SYN scan.

**Deep Dive**: Window scans rely on outdated TCP implementations where open ports leaked window size info. Most systems now standardize RST behavior, making this scan less reliable.

---

## Custom Scan (`--scanflags`)

Want to roll your own scan? The **Custom Scan** lets you set any combination of TCP flags using `--scanflags`. It’s like cooking your own hacking recipe!

- **How It Works**:
  - Specify flags like `SYN`, `RST`, `FIN`, `ACK`, `PSH`, `URG` (e.g., `--scanflags RSTSYNFIN`).
  - You must predict how the target will respond to interpret results.
  - Responses depend on the target’s TCP stack and firewall.
- **Why Use It?**:
  - Test experimental flag combos to bypass unique firewalls.
  - Explore edge cases or research TCP behavior.
- **Downsides**:
  - Requires deep knowledge to interpret results.
  - High risk of inconsistent or misleading output.
  - Needs **root privileges**.
- **When to Use**: For advanced pentesting or when standard scans fail.

### Example
```bash
sudo nmap --scanflags URGACKPSHRSTSYNFIN 10.10.9.65
```
- **What it does**:
  - Sends a packet with all flags set (a “kitchen sink” scan).
  - Open ports might ignore it; closed ports might send RST.
  - Results vary wildly—expect `open|filtered` or `closed`.

**Warning**: Custom scans are a last resort. Without understanding the target’s behavior, you’re shooting in the dark. Always test in a lab first!

---

## Spoofing Scans (`-S`, `--spoof-mac`)

**Spoofing Scans** let you fake your source **IP** or **MAC** address to hide your identity or trick the target.

- **How It Works**:
  - **IP Spoofing (`-S`)**: Sets a fake source IP (`-S SPOOFED_IP`).
  - **MAC Spoofing (`--spoof-mac`)**: Sets a fake MAC address (`--spoof-mac SPOOFED_MAC`).
  - Requires:
    - `-e <interface>` to specify the network interface.
    - `-Pn` to disable ping checks.
    - Ability to sniff replies (e.g., on the same subnet or with network access).
  - **Steps**:
    1. Send spoofed packets to the target.
    2. Target replies to the spoofed IP/MAC.
    3. Sniff the network to catch responses.
- **Why Use It?**:
  - Hides your real IP from logs (if you can capture replies).
  - Tests firewall rules by mimicking other IPs.
- **Downsides**:
  - Useless without network sniffing (replies go to the spoofed IP).
  - MAC spoofing only works on the same subnet (Ethernet/Wi-Fi).
  - Requires **root privileges**.
- **When to Use**: In controlled environments where you can monitor traffic (e.g., a lab or pentest with permission).

### Example
```bash
sudo nmap -e eth0 -Pn -S 10.10.10.99 10.10.9.65
```
- **What it does**:
  - Spoofs the source IP as `10.10.10.99`.
  - Sends packets via `eth0` without pinging.
  - You’d need to run `tcpdump` or Wireshark to see replies.

```bash
sudo nmap --spoof-mac 00:11:22:33:44:55 10.10.9.65
```
- **What it does**:
  - Spoofs the MAC address.
  - Only works if you’re on the same subnet as `10.10.9.65`.

**Reality Check**: Spoofing is tough in real-world scenarios. You rarely control the network enough to see replies, making this more theoretical than practical.

---

## Decoy Scan (`-D`)

**Decoy Scans** flood the target with packets from multiple fake IPs, mixing your real IP among them to confuse logs.

- **How It Works**:
  - Use `-D` to specify decoy IPs (e.g., `-D 10.10.0.1,10.10.0.2,ME`).
  - `ME` includes your real IP; `RND` generates random IPs.
  - Target sees packets from all IPs, diluting your footprint.
- **Why Use It?**:
  - Makes it harder for defenders to pinpoint your IP.
  - Overwhelms logs with fake sources.
- **Downsides**:
  - Doesn’t hide your IP completely—it’s still there.
  - Decoy IPs must be online, or the target might filter them.
  - Requires **root privileges**.
- **When to Use**: During pentests to obscure your activity (with permission).

### Example
```bash
nmap -D 10.10.0.1,10.10.0.2,ME 10.10.9.65
```
- **What it does**:
  - Scans `10.10.9.65` with packets from `10.10.0.1`, `10.10.0.2`, and your IP.
  - Logs show three sources, muddying the waters.

```bash
nmap -D 10.10.0.1,RND,RND,ME 10.10.9.65
```
- **What it does**:
  - Uses one real IP, two random IPs, and your IP.
  - Random IPs change each run, adding chaos.

**Pro Tip**: Use decoys sparingly—too many fake IPs can trigger IDS alerts or slow your scan.

---

## Idle (Zombie) Scan (`-sI`)

The **Idle Scan** (aka Zombie Scan) is the ultimate stealth technique, using an **idle host** (zombie) to bounce packets off, making it look like the zombie is scanning the target.

- **How It Works**:
  - Requires an idle host with predictable **IP ID** increments (in the IP header).
  - **Steps**:
    1. Probe the zombie to get its current IP ID.
    2. Spoof a SYN packet from the zombie’s IP to the target’s port.
    3. Check the zombie’s new IP ID:
       - **Closed/Filtered Port**: No response to zombie, IP ID increments by 1.
       - **Open Port**: Target sends SYN/ACK to zombie, zombie sends RST, IP ID increments by 2.
  - Uses `-sI ZOMBIE_IP` to specify the zombie.
- **Why Use It?**:
  - Your IP never appears in the target’s logs—only the zombie’s.
  - Perfect for covert scanning.
- **Downsides**:
  - Hard to find a truly idle zombie with predictable IP IDs (modern systems randomize them).
  - Slow and complex.
  - Requires **root privileges**.
- **When to Use**: Rare—only in pentests where stealth is critical and you’ve found a suitable zombie.

### Example
```bash
sudo nmap -sI 10.10.10.99 10.10.9.65
```
- **What it does**:
  - Uses `10.10.10.99` as the zombie to scan `10.10.9.65`.
  - Checks IP IDs to infer open ports.

**Deep Dive**:
- **Scenario 1 (Closed Port)**: Target sends RST to zombie; zombie’s IP ID stays put.
- **Scenario 2 (Open Port)**: Target sends SYN/ACK; zombie sends RST, bumping IP ID.
- **Scenario 3 (Filtered)**: No response, same as closed.
- Finding a zombie is the bottleneck—printers or old servers were ideal in the 90s, but randomization kills this today.

**Warning**: Idle scans are a hassle and often fail due to zombie issues. Use only if you’re confident in your setup.

---

## Fragmented Packets (`-f`, `-ff`, `--mtu`)

**Packet Fragmentation** breaks IP packets into smaller chunks to sneak past firewalls or IDS systems that don’t reassemble fragments properly.

- **How It Works**:
  - Splits the TCP/UDP payload into fragments:
    - `-f`: 8-byte fragments (or less).
    - `-ff`: 16-byte fragments.
    - `--mtu <num>`: Custom size (multiple of 8).
  - Uses **IP header** fields like **Identification** and **Fragment Offset** to track pieces.
  - Target reassembles fragments to process the packet.
- **Why Use It?**:
  - Bypasses firewalls/IDS that don’t handle fragments well.
  - Makes packets look less suspicious to basic filters.
- **Downsides**:
  - Modern firewalls reassemble fragments, negating the benefit.
  - Increases scan time due to multiple packets.
  - Requires **root privileges**.
- **When to Use**: Against older or poorly configured firewalls.

### Example
```bash
sudo nmap -sS -p80 -f 10.20.30.144
```
- **What it does**:
  - Runs a SYN scan on port 80.
  - Splits the 24-byte TCP header into three 8-byte fragments (each with a 20-byte IP header).
- **Comparison**:
  - Without `-f`: One packet (20-byte IP header + 24-byte TCP header).
  - With `-f`: Three packets, each carrying 8 bytes of TCP data.

```bash
sudo nmap -sS -p80 -ff 10.20.30.144
```
- **What it does**:
  - Splits into two fragments: 16 bytes and 8 bytes.

**Technical Note**: Fragmentation targets the **IP data** (TCP/UDP payload), not the IP header. The **Fragment Offset** tells the target how to reassemble, but misconfigured systems might drop or misread fragments.

---

## Firewall and IDS Evasion

**Firewalls** and **Intrusion Detection Systems (IDS)** are your enemies when scanning. Here’s how they work and how to dodge them:

- **Firewall**:
  - Blocks or allows packets based on rules (e.g., IP, port, protocol).
  - **Stateless**: Checks individual packets (Null, FIN, Xmas can bypass).
  - **Stateful**: Tracks connections (harder to fool).
- **IDS**:
  - Looks for patterns (e.g., too many SYN packets).
  - Inspects packet contents for malicious signatures.
- **Evasion Techniques**:
  - **Fragmentation (`-f`)**: Breaks packets to confuse filters.
  - **Decoys (`-D`)**: Floods logs with fake IPs.
  - **Spoofing (`-S`)**: Hides your IP (if you can sniff).
  - **Idle Scan (`-sI`)**: Uses a zombie to stay invisible.
  - **Timing (`-T0`, `-T1`)**: Slows scans to avoid detection.
  - **Data Length (`--data-length`)**: Adds random data to look normal.

### Example
```bash
sudo nmap -sS -T1 -f --data-length 100 10.10.9.65
```
- **What it does**:
  - Slow SYN scan (`-T1`) with 8-byte fragments (`-f`).
  - Adds 100 bytes of random data to obscure the packet.

**Reality Check**: Evasion is tough against modern systems. Stateful firewalls and smart IDS catch most tricks, so combine techniques and test carefully.

---

## Debugging and Verbosity

Nmap can explain itself better with these options:

- **Reason (`--reason`)**: Shows why Nmap called a port open, closed, etc.
- **Verbose (`-v`, `-vv`)**: More details on scan progress.
- **Debug (`-d`, `-dd`)**: Deep technical output for troubleshooting.

### Example
```bash
sudo nmap -sS --reason 10.10.252.27
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:40 BST
  Nmap scan report for ip-10-10-252-27.eu-west-1.compute.internal (10.10.252.27)
  Host is up, received arp-response (0.0020s latency).
  Not shown: 994 closed ports
  Reason: 994 resets
  PORT    STATE SERVICE REASON
  22/tcp  open  ssh     syn-ack ttl 64
  25/tcp  open  smtp    syn-ack ttl 64
  80/tcp  open  http    syn-ack ttl 64
  110/tcp open  pop3    syn-ack ttl 64
  111/tcp open  rpcbind syn-ack ttl 64
  143/tcp open  imap    syn-ack ttl 64
  MAC Address: 02:45:BF:8A:2D:6B (Unknown)
  Nmap done: 1 IP address (1 host up) scanned in 1.59 seconds
  ```
- **Explanation**:
  - Host is up due to an “arp-response.”
  - Ports are open because of “syn-ack” replies.
  - Closed ports sent “resets.”

```bash
sudo nmap -sS -vv 10.10.252.27
```
- **Output**:
  ```
  Starting Nmap 7.60 ( https://nmap.org ) at 2021-08-30 10:41 BST
  Initiating ARP Ping Scan at 10:41
  Scanning 10.10.252.27 [1 port]
  Completed ARP Ping Scan at 10:41, 0.22s elapsed (1 total hosts)
  ...
  Discovered open port 22/tcp on 10.10.252.27
  ...
  Nmap done: 1 IP address (1 host up) scanned in 1.59 seconds
  ```
- **Explanation**:
  - Shows real-time steps (ARP ping, port discoveries).
  - Perfect for understanding Nmap’s flow.

**Pro Tip**: Use `-v` for most scans, `--reason` when results seem off, and `-d` only when you’re desperate—it’s a firehose of data!

---

## Summary of Advanced Scan Options

| **Scan Type**         | **Option**              | **Flags Set**         | **Purpose**                              | **Privileges** | **Best For**                       |
|-----------------------|-------------------------|-----------------------|------------------------------------------|----------------|------------------------------------|
| Null Scan             | `-sN`                   | None                  | Detects open/filtered ports              | Root           | Stateless firewall bypass          |
| FIN Scan              | `-sF`                   | FIN                   | Detects open/filtered ports              | Root           | Legacy systems                    |
| Xmas Scan             | `-sX`                   | FIN, PSH, URG         | Detects open/filtered ports              | Root           | Older networks                    |
| Maimon Scan           | `-sM`                   | FIN, ACK              | Targets BSD systems (rare)               | Root           | Historical curiosity              |
| ACK Scan              | `-sA`                   | ACK                   | Maps firewall rules (unfiltered ports)   | Root           | Firewall analysis                 |
| Window Scan           | `-sW`                   | ACK                   | Checks window size for port state        | Root           | Old systems, firewall mapping     |
| Custom Scan           | `--scanflags <flags>`   | User-defined          | Experiments with flag combos             | Root           | Advanced testing                  |
| Spoofed Scan          | `-S`, `--spoof-mac`     | N/A                   | Hides source IP/MAC                     | Root           | Controlled environments            |
| Decoy Scan            | `-D`                    | N/A                   | Obscures real IP with fakes              | Root           | Log confusion                     |
| Idle (Zombie) Scan    | `-sI`                   | N/A                   | Scans via idle host                     | Root           | Ultimate stealth (if zombie found) |
| Fragmentation         | `-f`, `-ff`, `--mtu`    | N/A                   | Breaks packets to evade firewalls        | Root           | Old firewalls/IDS                 |

---

