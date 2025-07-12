# Netstat on Linux: Mastering Network Connections

Netstat on Linux is a powerful command-line tool for monitoring network connections, listening ports, and routing tables. 



## Why Use Netstat on Linux?

Linux systems use netstat to:
- **Track Connections**: Monitor TCP/UDP connections to servers, devices, or potential threats.
- **Identify Services**: Check which ports are listening and which processes are responsible.
- **Analyze Routing**: Inspect routing tables for network troubleshooting.
- **Enhance Security**: Detect suspicious connections or open ports that could be exploited.

Run netstat in a terminal, often with `sudo` for full visibility, as some options require root privileges.

## Key Commands and Options

Below are essential netstat commands for Linux, with example outputs and explanations. Install `net-tools` if netstat is missing (`sudo apt install net-tools` on Debian/Ubuntu).

### Basic Netstat
Display active connections:
```bash
netstat
```

**Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 192.168.1.100:45876     93.184.216.34:443      ESTABLISHED
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN
```

**Step-by-Step Explanation**:
1. **Proto**: TCP or UDP protocol.
2. **Recv-Q/Send-Q**: Queued data for receiving/sending (usually 0 for active connections).
3. **Local Address**: Your system’s IP and port (e.g., `192.168.1.100:45876` for a browser, `0.0.0.0:22` for SSH).
4. **Foreign Address**: Remote IP and port (e.g., `93.184.216.34:443` for `example.com`, `0.0.0.0:*` for listening).
5. **State**: TCP states like `ESTABLISHED` (active) or `LISTEN` (waiting for connections).
6. **Why It’s Slow**: Resolves IPs to hostnames, causing delays.

**Use Case**: Quick check for active connections, but slow due to DNS lookups.

### Numeric Output (-n)
Use numeric IPs to avoid DNS delays:
```bash
netstat -n
```

**Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 192.168.1.100:45876     93.184.216.34:443      ESTABLISHED
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN
udp        0      0 192.168.1.100:68        0.0.0.0:*               -
```

**Step-by-Step Explanation**:
1. **Numeric IPs**: Shows `93.184.216.34` instead of `example.com`, speeding up output.
2. **UDP Entries**: No state for UDP (connectionless).
3. **Use Case**: Faster for scripting or when hostnames aren’t needed.

### All Connections and Listening Ports (-a)
Show all connections, including listening ports:
```bash
netstat -a
```

**Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN
tcp        0      0 192.168.1.100:45876     example.com:443        ESTABLISHED
udp        0      0 192.168.1.100:68        0.0.0.0:*               -
```

**Step-by-Step Explanation**:
1. **Listening Ports**: Shows ports like `0.0.0.0:80` (web server) in `LISTEN` state.
2. **All Protocols**: Includes TCP, UDP, and their IPv6 variants.
3. **Use Case**: Audit open ports for security or verify service availability.

### Filter by Protocol (-t, -u)
Show TCP or UDP connections:
```bash
netstat -t  # TCP only
netstat -u  # UDP only
```

**TCP Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 192.168.1.100:45876     93.184.216.34:443      ESTABLISHED
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN
```

**UDP Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State
udp        0      0 192.168.1.100:68        0.0.0.0:*               -
```

**Step-by-Step Explanation**:
1. **-t**: Filters for TCP connections, showing states like `ESTABLISHED` or `LISTEN`.
2. **-u**: Filters for UDP, typically showing no state (connectionless).
3. **Use Case**: Isolate specific protocols (e.g., TCP for SSH, UDP for DNS).

### Show Listening Ports with Processes (-tlp, -ulp)
List listening TCP/UDP ports with process details:
```bash
sudo netstat -tlp  # TCP listening ports
sudo netstat -ulp  # UDP listening ports
```

**TCP Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1234/sshd
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      5678/apache2
```

**Step-by-Step Explanation**:
1. **-t/-u**: Filters TCP or UDP.
2. **-l**: Shows only listening ports.
3. **-p**: Adds PID and program name (e.g., `1234/sshd` for SSH).
4. **Requires sudo**: Root privileges needed to view process details.
5. **Use Case**: Identify services (e.g., SSH on port 22, Apache on port 80) and their processes.

### Combined Options (-anp)
Combine for detailed output:
```bash
sudo netstat -anp
```

**Example Output**:
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 192.168.1.100:45876     93.184.216.34:443      ESTABLISHED 9876/firefox
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1234/sshd
udp        0      0 192.168.1.100:68        0.0.0.0:*               -           4567/dhclient
```

**Step-by-Step Explanation**:
1. **-a**: Includes all connections and listening ports.
2. **-n**: Uses numeric IPs for speed.
3. **-p**: Shows PID and program name (requires sudo).
4. **Use Case**: Comprehensive view for security audits or troubleshooting.

### Routing Table (-r)
Display the routing table:
```bash
netstat -r
```

**Example Output**:
```
Kernel IP routing table
Destination     Gateway         Genmask         Flags   Iface
0.0.0.0         192.168.1.254   0.0.0.0         UG      enp0s3
192.168.1.0     0.0.0.0         255.255.255.0   U       enp0s3
```

**Step-by-Step Explanation**:
1. **Destination**: Target network (e.g., `0.0.0.0` for default route).
2. **Gateway**: Next hop (e.g., `192.168.1.254` for router).
3. **Genmask**: Subnet mask.
4. **Iface**: Interface (e.g., `enp0s3` for Ethernet).
5. **Use Case**: Diagnose routing issues (e.g., incorrect gateway).

### Filter by Port (with grep)
Search for connections on a specific port (e.g., SSH on 22):
```bash
sudo netstat -anp | grep :22
```

**Example Output**:
```
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1234/sshd
```

**Step-by-Step Explanation**:
1. **-anp**: Shows all connections, numeric IPs, and processes.
2. **grep :22**: Filters for port 22 (SSH).
3. **Use Case**: Confirm if a service (e.g., SSH) is running or check for unauthorized use.


## Further Reading
- [Linux Man Page: netstat](https://man7.org/linux/man-pages/man8/netstat.8.html)
- [Arch Linux Wiki: ss](https://wiki.archlinux.org/title/Ss)
- [VirusTotal](https://www.virustotal.com/)
- [Ubuntu: Net-Tools](https://packages.ubuntu.com/net-tools)


