# Netstat on Windows: Monitoring Network Connections

Netstat on Windows is a versatile command-line tool for inspecting network connections, listening ports, and routing tables.

## Why Use Netstat on Windows?

Windows systems rely on netstat to:
- **Monitor Connections**: View active TCP/UDP connections to web servers, printers, or suspicious IPs.
- **Identify Processes**: Link connections to executables or Process IDs (PIDs) for troubleshooting or security.
- **Check Routing**: Inspect routing tables to diagnose network path issues.
- **Secure Systems**: Detect unauthorized connections or open ports that could be exploited.

Run netstat in Command Prompt or PowerShell, ideally as Administrator, to access all features.

## Key Commands and Options

Below are the most useful netstat commands for Windows, with example outputs and explanations. Commands often require Administrator privileges (`Run as Administrator`).

### Basic Netstat
Display active connections:
```powershell
netstat
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State
TCP    192.168.1.100:51234    example.com:443       ESTABLISHED
TCP    192.168.1.100:137      0.0.0.0:0             LISTENING
```

**Step-by-Step Explanation**:
1. **Proto**: Shows TCP (reliable, connection-oriented) or UDP (connectionless).
2. **Local Address**: Your PC’s IP (e.g., `192.168.1.100`) and port (e.g., `51234` for a browser, `137` for NetBIOS).
3. **Foreign Address**: Remote IP or hostname (e.g., `example.com:443` for HTTPS) or `0.0.0.0:0` for listening ports.
4. **State**: `ESTABLISHED` for active connections (e.g., browser to web server); `LISTENING` for open ports waiting for connections.
5. **Why It’s Slow**: Netstat resolves IPs to hostnames (e.g., `93.184.216.34` to `example.com`), causing delays.

**Use Case**: Check if your browser is connected to expected websites or detect unexpected connections.

### Numeric Output (-n)
Skip DNS lookups for faster output:
```powershell
netstat -n
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State
TCP    192.168.1.100:51234    93.184.216.34:443    ESTABLISHED
TCP    192.168.1.100:137      0.0.0.0:0            LISTENING
```

**Step-by-Step Explanation**:
1. **No Hostnames**: Shows raw IPs (e.g., `93.184.216.34`) instead of resolving to `example.com`, speeding up output.
2. **Same Structure**: Columns remain the same, but numeric IPs make it easier to analyze or script.
3. **Use Case**: Ideal for quick checks or when DNS resolution is unnecessary or slow.

### All Connections and Listening Ports (-a)
Show all connections, including listening ports:
```powershell
netstat -a
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State
TCP    0.0.0.0:80             0.0.0.0:0             LISTENING
TCP    192.168.1.100:51234    93.184.216.34:443    ESTABLISHED
UDP    192.168.1.100:68       0.0.0.0:0             *
```

**Step-by-Step Explanation**:
1. **Includes Listening**: Shows ports in `LISTENING` state (e.g., `0.0.0.0:80` for a web server).
2. **UDP Entries**: UDP connections appear with `*` in the State column (connectionless protocol).
3. **Use Case**: Identify all open ports, useful for security audits to find exposed services.

### Filter by Protocol (-p)
Show connections for a specific protocol (e.g., TCP, UDP):
```powershell
netstat -p TCP
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State
TCP    192.168.1.100:51234    93.184.216.34:443    ESTABLISHED
TCP    0.0.0.0:445            0.0.0.0:0             LISTENING
```

**Step-by-Step Explanation**:
1. **Protocol Filter**: Only TCP connections are shown (use `UDP`, `TCPv6`, or `UDPv6` for others).
2. **Use Case**: Focus on specific protocols (e.g., TCP for web traffic, UDP for DNS).

### Show Executable (-b)
Display the executable responsible for each connection:
```powershell
netstat -b
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State
TCP    192.168.1.100:51234    93.184.216.34:443    ESTABLISHED
 [chrome.exe]
TCP    0.0.0.0:445            0.0.0.0:0             LISTENING
 [svchost.exe]
```

**Step-by-Step Explanation**:
1. **Executable Name**: Shows the program (e.g., `chrome.exe` for a browser connection).
2. **Requires Admin**: Must run as Administrator to see all processes.
3. **Use Case**: Identify which programs are making connections (e.g., is `chrome.exe` connecting to an unusual IP?).

### Show Process ID (-o)
Display the Process ID (PID) for each connection:
```powershell
netstat -o
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State           PID
TCP    192.168.1.100:51234    93.184.216.34:443    ESTABLISHED     10904
TCP    0.0.0.0:445            0.0.0.0:0             LISTENING       260
```

**Step-by-Step Explanation**:
1. **PID Column**: Adds the unique PID (e.g., `10904` for Chrome).
2. **Match with Task Manager**: Open Task Manager, go to Details tab, sort by PID to find the process (e.g., `10904` is `chrome.exe`).
3. **Use Case**: Pinpoint specific processes for troubleshooting (e.g., multiple `chrome.exe` instances).

### Combined Options (-a -n -b -o)
Combine for detailed, fast output:
```powershell
netstat -a -n -b -o
```

**Example Output**:
```
Proto  Local Address          Foreign Address        State           PID
TCP    192.168.1.100:51234    93.184.216.34:443    ESTABLISHED     10904
 [chrome.exe]
TCP    0.0.0.0:80             0.0.0.0:0             LISTENING       1234
 [httpd.exe]
UDP    192.168.1.100:68       0.0.0.0:0             *               5678
 [dhclient.exe]
```

**Step-by-Step Explanation**:
1. **All Connections**: `-a` includes listening ports and all protocols.
2. **Numeric IPs**: `-n` skips DNS lookups for speed.
3. **Executable and PID**: `-b` and `-o` show the program and PID.
4. **Use Case**: Comprehensive view for security audits or troubleshooting

### Routing Table (-r)
Display the routing table:
```powershell
netstat -r
```

**Example Output**:
```
IPv4 Route Table
Destination     Gateway         Genmask         Interface
0.0.0.0         192.168.1.254   0.0.0.0         192.168.1.100
192.168.1.0     0.0.0.0         255.255.255.0   192.168.1.100
```

**Step-by-Step Explanation**:
1. **Destination**: Target network (e.g., `0.0.0.0` for default route).
2. **Gateway**: Where packets are sent (e.g., `192.168.1.254` for router).
3. **Genmask**: Subnet mask defining the network range.
4. **Interface**: Local interface used (e.g., `192.168.1.100`).
5. **Use Case**: Troubleshoot routing issues (e.g., packets not reaching a destination).

## Further Reading
- [Microsoft: Netstat Command](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/netstat)
- [VirusTotal](https://www.virustotal.com/)
- [PowerShell: Get-NetTCPConnection](https://learn.microsoft.com/en-us/powershell/module/nettcpip/get-nettcpconnection)


