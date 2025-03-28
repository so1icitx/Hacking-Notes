
# Windows CMD 


### System Reconnaissance Commands

- **`set`**  
  - **What It Does:** Dumps all environment variables, including the sacred `Path`—the list of dirs where Windows hunts for executable commands.  
  - **Long Example:**  
    ```
    C:\Users\shadow> set
    ALLUSERSPROFILE=C:\ProgramData
    APPDATA=C:\Users\shadow\AppData\Roaming
    COMPUTERNAME=SHADOWBOX
    LOGNAME=shadow
    NUMBER_OF_PROCESSORS=4
    OS=Windows_NT
    Path=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Program Files\Git\cmd;C:\Users\shadow\AppData\Local\Programs\Python\Python39\Scripts;
    USERNAME=shadow
    WINDIR=C:\Windows
    ```
    - **Scenario:** You’re dropped into a random Windows box via a reverse shell. First move? `set`. You spot `Path` includes `C:\Program Files\Git\cmd`—Git’s installed, meaning `git.exe` is callable anywhere. You also see `USERNAME=shadow` and `COMPUTERNAME=SHADOWBOX`—instant intel on who and where you are.  
  - **Explanation:** Think of `set` as your system’s DNA dump. `Path` tells you where commands like `dir` or `ping` live—if it’s missing a dir, you’ll need full paths (e.g., `C:\Windows\system32\ping.exe`). Use this to map your attack surface or troubleshoot why a tool won’t run.

- **`ver`**  
  - **What It Does:** Reveals the exact Windows version—build number included.  
  - **Long Example:**  
    ```
    C:\Users\shadow> ver
    Microsoft Windows [Version 10.0.19044.1586]
    ```
    - **Scenario:** You’re on a target machine, planning an exploit. `ver` spits out `10.0.19044.1586`—Windows 10, build 21H2. A quick Google shows this build’s unpatched for CVE-2022-12345 (fake vuln for demo). You’ve got a privilege escalation vector—time to craft that payload!  
  - **Explanation:** Version intel is gold. Older builds (e.g., XP, 7) scream “exploit me,” while newer ones hint at patch levels. Cross-reference with vuln databases for your next move.

- **`systeminfo`**  
  - **What It Does:** Unleashes a flood of system details—OS, CPU, RAM, hotfixes, network adapters, you name it.  
  - **Long Example:**  
    ```
    C:\Users\shadow> systeminfo
    Host Name:                 SHADOWBOX
    OS Name:                   Microsoft Windows 10 Pro
    OS Version:                10.0.19044 N/A Build 19044
    OS Manufacturer:           Microsoft Corporation
    OS Configuration:          Standalone Workstation
    OS Build Type:             Multiprocessor Free
    Registered Owner:          shadow
    Registered Organization:   N/A
    Product ID:                00331-10000-00001-AA123
    Original Install Date:     01/15/2023, 3:45:22 PM
    System Boot Time:          03/27/2025, 9:12:07 AM
    System Manufacturer:       Dell Inc.
    System Model:              XPS 15 9500
    System Type:               x64-based PC
    Processor(s):              1 Processor(s) Installed.
                               [01]: Intel64 Family 6 Model 158 Stepping 10 GenuineIntel ~2600 Mhz
    BIOS Version:              Dell Inc. 1.9.2, 12/10/2024
    Total Physical Memory:     16,284 MB
    Available Physical Memory: 9,872 MB
    Hotfix(s):                 12 Hotfix(s) Installed.
                               [01]: KB1234567
                               [02]: KB8901234
    Network Card(s):           2 NIC(s) Installed.
                               [01]: Intel(R) Wi-Fi 6 AX201 160MHz
                               [02]: Bluetooth Device (Personal Area Network)
    ```
    - **Scenario:** You’ve pwned a box and run `systeminfo`. It’s a Dell XPS 15, Windows 10 Pro, with 16GB RAM and hotfixes like `KB1234567`. You check the hotfix list—missing `KB9999999`, a critical patch for a known vuln. Plus, `System Boot Time` shows it rebooted yesterday—uptime’s low, maybe post-patch. You’ve got hardware, patch state, and uptime—prime exploit planning material.  
  - **Explanation:** This is your system’s full confession. Use it to spot unpatched holes, confirm hardware for driver exploits, or estimate uptime for persistence. Pipe it with `| more` if it’s too long—`systeminfo | more` lets you scroll with Spacebar.

- **`cls`**  
  - **What It Does:** Wipes the CMD screen clean.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    [tons of output fills the screen]
    C:\Users\shadow> cls
    [screen is now blank]
    C:\Users\shadow>
    ```
    - **Scenario:** You’ve just run `systeminfo`, `netstat`, and `tasklist`—the screen’s a mess. You’re about to screenshot for a report but want it clean. `cls`—bam, fresh slate. Now `ver` looks crisp for the capture.  
  - **Explanation:** Purely cosmetic, but clutch for clarity. Keeps your focus when the terminal’s drowning in output.

- **`help`**  
  - **What It Does:** Spills help for a command—syntax, options, all of it.  
  - **Long Example:**  
    ```
    C:\Users\shadow> help netstat
    Displays protocol statistics and current TCP/IP network connections.

    NETSTAT [-a] [-b] [-e] [-f] [-n] [-o] [-p proto] [-r] [-s] [-t] [-x] [-y] [interval]

      -a            Displays all connections and listening ports.
      -b            Displays the executable involved in creating each connection or listening port.
      -e            Displays Ethernet statistics.
      -n            Displays addresses and port numbers in numerical form.
      -o            Displays the owning process ID associated with each connection.
    [etc—full help page]
    ```
    - **Scenario:** You’re new to `netstat` and want all listening ports with PIDs. `help netstat` shows `-a` (all) and `-o` (PID). You craft `netstat -ao`—bam, every port and process ID. Later, you’re fuzzing `tracert`—`help tracert` reveals `-h` for max hops. You tweak it: `tracert -h 50 google.com`.  
  - **Explanation:** Your lifeline for unknown commands. Faster than Googling, and it’s right there—use it to master any tool on the fly.

### Network Siege Commands

- **`ipconfig`**  
  - **What It Does:** Dumps basic network config—IP, subnet, gateway.  
  - **Long Example:**  
    ```
    C:\Users\shadow> ipconfig
    Windows IP Configuration

    Ethernet adapter Ethernet:
       Connection-specific DNS Suffix  . : home.local
       Link-local IPv6 Address . . . . . : fe80::1d2e:4f5a:6b7c:8d9e%12
       IPv4 Address. . . . . . . . . . . : 192.168.1.150
       Subnet Mask . . . . . . . . . . . : 255.255.255.0
       Default Gateway . . . . . . . . . : 192.168.1.1

    Wireless LAN adapter Wi-Fi:
       Connection-specific DNS Suffix  . : 
       IPv4 Address. . . . . . . . . . . : 192.168.1.151
       Subnet Mask . . . . . . . . . . . : 255.255.255.0
       Default Gateway . . . . . . . . . : 192.168.1.1
    ```
    - **Scenario:** You’re on a compromised workstation. `ipconfig` reveals two adapters—Ethernet at `192.168.1.150` and Wi-Fi at `151`, both gated through `192.168.1.1`. The subnet `255.255.255.0` means a `/24` network—256 IPs to scan. You note `192.168.1.1` as the router—prime target for creds or pivoting.  
  - **Explanation:** Your network footprint starts here. IP tells you “who I am,” gateway is “where I escape,” subnet is “how big’s my playground.” First step in any net recon.

- **`ipconfig /all`**  
  - **What It Does:** Full network config—adds DNS, DHCP, MAC, lease times.  
  - **Long Example:**  
    ```
    C:\Users\shadow> ipconfig /all
    Windows IP Configuration
       Host Name . . . . . . . . . . . . : SHADOWBOX
       Primary Dns Suffix  . . . . . . . : 
       Node Type . . . . . . . . . . . . : Hybrid
       IP Routing Enabled. . . . . . . . : No
       WINS Proxy Enabled. . . . . . . . : No

    Ethernet adapter Ethernet:
       Connection-specific DNS Suffix  . : home.local
       Description . . . . . . . . . . . : Realtek PCIe GbE Family Controller
       Physical Address. . . . . . . . . : 00-14-22-01-23-45
       DHCP Enabled. . . . . . . . . . . : Yes
       Autoconfiguration Enabled . . . . : Yes
       Link-local IPv6 Address . . . . . : fe80::1d2e:4f5a:6b7c:8d9e%12(Preferred)
       IPv4 Address. . . . . . . . . . . : 192.168.1.150(Preferred)
       Subnet Mask . . . . . . . . . . . : 255.255.255.0
       Lease Obtained. . . . . . . . . . : Friday, March 27, 2025 9:15:32 AM
       Lease Expires . . . . . . . . . . : Saturday, March 28, 2025 9:15:32 AM
       Default Gateway . . . . . . . . . : 192.168.1.1
       DHCP Server . . . . . . . . . . . : 192.168.1.1
       DNS Servers . . . . . . . . . . . : 8.8.8.8
                                           8.8.4.4
       NetBIOS over Tcpip. . . . . . . . : Enabled
    ```
    - **Scenario:** Post-compromise, you run `ipconfig /all`. `MAC: 00-14-22-01-23-45`—unique hardware ID for spoofing. `DHCP Enabled: Yes` and `Lease Obtained` show it’s dynamic—IP could shift tomorrow. `DNS Servers: 8.8.8.8` (Google)—no internal DNS, so maybe no domain controller. `Host Name: SHADOWBOX`—matches `systeminfo`. You’ve got the full net picture—time to ping the gateway or spoof that MAC.  
  - **Explanation:** This is `ipconfig` on steroids. MAC for spoofing, DNS for pivoting, DHCP for persistence—every detail fuels your next hack.

- **`ping <target>`**  
  - **What It Does:** Sends ICMP packets to test if a target’s reachable.  
  - **Long Example:**  
    ```
    C:\Users\shadow> ping tryhackme.com
    Pinging tryhackme.com [94.237.54.116] with 32 bytes of data:
    Reply from 94.237.54.116: bytes=32 time=45ms TTL=54
    Reply from 94.237.54.116: bytes=32 time=46ms TTL=54
    Reply from 94.237.54.116: bytes=32 time=45ms TTL=54
    Reply from 94.237.54.116: bytes=32 time=47ms TTL=54

    Ping statistics for 94.237.54.116:
        Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
    Approximate round trip times in milli-seconds:
        Minimum = 45ms, Maximum = 47ms, Average = 45ms
    ```
    - **Scenario:** You’re testing if `tryhackme.com` is alive from your foothold. `ping` confirms it’s at `94.237.54.116`, responding in ~45ms—low latency, likely nearby. No packet loss—solid connection. TTL of 54 suggests ~10 hops (64-54). You’ve verified reachability—next, `tracert` or hit it with Burp.  
  - **Explanation:** Like sonar—ping out, listen back. Confirms target’s up and gives latency/TTL clues. Blocked? Firewall’s in play—try TCP pings elsewhere.

- **`tracert <target>`**  
  - **What It Does:** Traces the route to a target, showing each hop.  
  - **Long Example:**  
    ```
    C:\Users\shadow> tracert tryhackme.com
    Tracing route to tryhackme.com [94.237.54.116]
    over a maximum of 30 hops:

      1     1 ms     1 ms     1 ms  192.168.1.1
      2     5 ms     4 ms     5 ms  10.0.0.1
      3    10 ms     9 ms    10 ms  172.16.254.1
      4    15 ms    14 ms    15 ms  203.0.113.1
      5    20 ms    21 ms    20 ms  198.51.100.10
      6     *        *        *     Request timed out.
      7    30 ms    29 ms    30 ms  162.158.92.1
      8    40 ms    41 ms    40 ms  104.16.248.1
      9    45 ms    46 ms    45 ms  94.237.54.116

    Trace complete.
    ```
    - **Scenario:** You’re mapping the path to `tryhackme.com`. `tracert` shows your router (`192.168.1.1`), ISP hops, and a timeout at hop 6—likely a stealth router. Final hop `94.237.54.116` matches `ping`. Nine hops total, 45ms end-to-end—decent route. You spot `198.51.100.10`—a potential pivot or MiTM point if compromisable.  
  - **Explanation:** Your network GPS. Each hop’s a router; timeouts mean stealth or drops. Use it to find choke points or guess target proximity.

- **`nslookup <domain>`**  
  - **What It Does:** Resolves a domain to its IP(s) via DNS.  
  - **Long Example:**  
    ```
    C:\Users\shadow> nslookup tryhackme.com
    Server:  dns.google
    Address:  8.8.8.8

    Non-authoritative answer:
    Name:    tryhackme.com
    Addresses:  2606:4700:10::6816:f874
               2606:4700:10::6816:f974
               94.237.54.116
               104.22.73.116

    C:\Users\shadow> nslookup tryhackme.com 1.1.1.1
    Server:  one.one.one.one
    Address:  1.1.1.1

    Non-authoritative answer:
    Name:    tryhackme.com
    Addresses:  2606:4700:10::6816:f874
               2606:4700:10::6816:f974
               94.237.54.116
               104.22.73.116
    ```
    - **Scenario:** You need `tryhackme.com`’s IPs. Default `nslookup` uses `8.8.8.8`—Google DNS—and spits out IPv4 (`94.237.54.116`, `104.22.73.116`) and IPv6. You test `1.1.1.1` (Cloudflare)—same results, confirming consistency. Multiple IPs suggest load balancing—hit each with `ping` to find the active one.  
  - **Explanation:** DNS decoder—turns names into numbers. Specify a server (e.g., `1.1.1.1`) to bypass local DNS or test alt resolvers. Key for target enumeration.

- **`netstat`**  
  - **What It Does:** Lists active connections and listening ports.  
  - **Long Example:**  
    ```
    C:\Users\shadow> netstat
    Active Connections

      Proto  Local Address          Foreign Address        State
      TCP    192.168.1.150:3389    10.0.0.5:54321         ESTABLISHED
      TCP    192.168.1.150:445     192.168.1.200:49152    ESTABLISHED
      TCP    192.168.1.150:5040    142.250.190.14:443     ESTABLISHED
    ```
    - **Scenario:** You’re on a box and run `netstat`. Port `3389` (RDP) to `10.0.0.5`—someone’s remote desktopping in. `445` (SMB) to `192.168.1.200`—file sharing with another LAN host. `5040` to Google (`443`)—browser’s open. You’ve got live connections—sniff `445` or spoof `3389` next?  
  - **Explanation:** Your connection radar. Shows who’s talking to what—spot backdoors or pivot points.

- **`netstat -abon`**  
  - **What It Does:** All connections, listening ports, PIDs, and executables.  
  - **Long Example:**  
    ```
    C:\Users\shadow> netstat -abon
    Active Connections

      Proto  Local Address          Foreign Address        State           PID
      TCP    0.0.0.0:135            0.0.0.0:0              LISTENING       820
     [svchost.exe]
      TCP    0.0.0.0:445            0.0.0.0:0              LISTENING       4
     [System]
      TCP    0.0.0.0:3389           0.0.0.0:0              LISTENING       912
     [svchost.exe]
      TCP    192.168.1.150:3389     10.0.0.5:54321         ESTABLISHED     912
     [svchost.exe]
      TCP    192.168.1.150:5040     142.250.190.14:443     ESTABLISHED     4567
     [chrome.exe]
    ```
    - **Scenario:** Full `netstat -abon` scan. `135` (RPC) and `445` (SMB) are listening—standard Windows ports, ripe for exploits. `3389` (RDP) has `svchost.exe` (PID 912) with an active connection to `10.0.0.5`—remote admin? `chrome.exe` (PID 4567) on `443`—user’s browsing Google. You’ve got PIDs—`taskkill /PID 4567` could kill Chrome, or spoof `445` for SMB relay.  
  - **Explanation:** The ultimate net spy. `-a` (all), `-b` (binary), `-o` (PID), `-n` (numeric)—maps every port to its process. Critical for hunting backdoors or killing tasks.

### Directory Domination Commands

- **`cd`**  
  - **What It Does:** Shows your current directory.  
  - **Long Example:**  
    ```
    C:\Users\shadow\Documents> cd
    C:\Users\shadow\Documents
    ```
    - **Scenario:** You’re dropped into a shell—where are you? `cd` says `C:\Users\shadow\Documents`. You’re in a user’s docs dir—juicy spot for creds or loot. Next, `dir` to scope it out.  
  - **Explanation:** Your GPS in the filesystem. No args = “tell me where I am.” Keeps you oriented.

- **`cd <dir>`**  
  - **What It Does:** Changes to a specified directory.  
  - **Long Example:**  
    ```
    C:\Users\shadow> cd Documents\Projects\Secret
    C:\Users\shadow\Documents\Projects\Secret> cd
    C:\Users\shadow\Documents\Projects\Secret
    ```
    - **Scenario:** You’re in `C:\Users\shadow` and hear there’s a `Secret` folder under `Projects`. `cd Documents\Projects\Secret` drops you in. `cd` confirms it—now `dir` to raid it. Maybe `type config.txt` next for creds?  
  - **Explanation:** Your teleport command. Full path (e.g., `C:\Windows`) or relative (e.g., `Desktop`)—gets you there fast.

- **`cd ..`**  
  - **What It Does:** Moves up one directory level.  
  - **Long Example:**  
    ```
    C:\Users\shadow\Documents\Projects> cd
    C:\Users\shadow\Documents\Projects
    C:\Users\shadow\Documents\Projects> cd ..
    C:\Users\shadow\Documents> cd
    C:\Users\shadow\Documents
    C:\Users\shadow\Documents> cd ..
    C:\Users\shadow> cd
    C:\Users\shadow
    ```
    - **Scenario:** You’re deep in `Projects` and need to backtrack. `cd ..` to `Documents`, then again to `C:\Users\shadow`. You’re exploring—maybe `dir` at each level to spot loot like `passwords.txt`.  
  - **Explanation:** Your retreat button. Stack it (`cd ..\..`) to climb higher—handy for escaping deep nests.

- **`dir`**  
  - **What It Does:** Lists dir contents—files, subdirs, sizes, dates.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    Volume in drive C is OS
    Volume Serial Number is 7C8D-9E2F

    Directory of C:\Users\shadow

    03/27/2025  10:15 AM    <DIR>          .
    03/27/2025  10:15 AM    <DIR>          ..
    03/25/2025  08:30 PM    <DIR>          Desktop
    03/26/2025  09:45 AM    <DIR>          Documents
    03/20/2025  03:12 PM    <DIR>          Downloads
    01/15/2023  04:00 PM    <DIR>          Pictures
    03/27/2025  10:10 AM            1,024  notes.txt
    03/26/2025  11:20 AM           15,872  loot.zip
                   2 File(s)         16,896 bytes
                   6 Dir(s)  128,456,789,012 bytes free
    ```
    - **Scenario:** You’re in `C:\Users\shadow`. `dir` shows `Desktop`, `Documents`, and files like `notes.txt` (1KB) and `loot.zip` (15KB). `notes.txt`—small, likely text; `type notes.txt` might spill creds. `loot.zip`—archive, maybe sensitive data. Subdirs like `Documents` are next—`cd Documents` and `dir` again.  
  - **Explanation:** Your filesystem X-ray. Dates hint at activity, sizes flag big targets—first stop for looting.

- **`dir /a`**  
  - **What It Does:** Lists all files, including hidden/system ones.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir /a
    Directory of C:\Users\shadow

    03/27/2025  10:15 AM    <DIR>          .
    03/27/2025  10:15 AM    <DIR>          ..
    03/25/2025  08:30 PM    <DIR>          Desktop
    03/26/2025  09:45 AM    <DIR>          Documents
    03/20/2025  03:12 PM    <DIR>          Downloads
    01/15/2023  04:00 PM    <DIR>          Pictures
    03/27/2025  10:10 AM            1,024  notes.txt
    03/26/2025  11:20 AM           15,872  loot.zip
    03/25/2025  09:00 PM    <DIR>          .hidden_folder
    03/25/2025  09:05 PM               512  secret.key [HIDDEN]
                   3 File(s)         17,408 bytes
                   7 Dir(s)  128,456,789,012 bytes free
    ```
    - **Scenario:** Standard `dir` missed something. `dir /a` reveals `.hidden_folder` and `secret.key` (hidden attribute). `type secret.key`—maybe an SSH key or password. Hidden stuff’s often gold—always check with `/a`.  
  - **Explanation:** Lifts the veil—hidden files are where admins stash creds or configs. Don’t skip it.

- **`dir /s`**  
  - **What It Does:** Recursively lists all files and subdirs.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir /s
    Directory of C:\Users\shadow
    03/27/2025  10:10 AM            1,024  notes.txt
    03/26/2025  11:20 AM           15,872  loot.zip
                   2 File(s)         16,896 bytes

    Directory of C:\Users\shadow\Documents
    03/26/2025  09:45 AM    <DIR>          Projects
    03/26/2025  09:50 AM               256  todo.txt
                   1 File(s)            256 bytes

    Directory of C:\Users\shadow\Documents\Projects
    03/26/2025  09:48 AM            5,120  plan.docx
                   1 File(s)          5,120 bytes

    Total Files Listed:
                   4 File(s)         22,272 bytes
                   8 Dir(s)  128,456,789,012 bytes free
    ```
    - **Scenario:** You want everything under `C:\Users\shadow`. `dir /s` maps it—`notes.txt` at root, `todo.txt` in `Documents`, `plan.docx` in `Projects`. It’s a full sweep—`copy *.txt C:\loot` to snag all text files. Pipe it (`dir /s | more`) if it’s huge.  
  - **Explanation:** Your recursive loot scanner. Grabs every file in the tree—perfect for mass harvesting.

- **`tree`**  
  - **What It Does:** Draws a visual dir tree.  
  - **Long Example:**  
    ```
    C:\Users\shadow> tree
    Folder PATH listing
    Volume serial number is 7C8D-9E2F
    C:.
    ├───Desktop
    ├───Documents
    │   └───Projects
    ├───Downloads
    └───Pictures
    ```
    - **Scenario:** You’re lost in `C:\Users\shadow`. `tree` sketches the layout—`Documents` has `Projects`, no files listed, just structure. You `cd Documents\Projects`—maybe `dir` there finds `plan.docx`. Clean visual to plan your raid.  
  - **Explanation:** Your dir blueprint. No file details, just the skeleton—quick way to see the hierarchy.

- **`mkdir <name>`**  
  - **What It Does:** Creates a new directory.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    [empty dir]
    C:\Users\shadow> mkdir stash
    C:\Users\shadow> dir
    03/27/2025  10:20 AM    <DIR>          stash
                   0 File(s)              0 bytes
    C:\Users\shadow> cd stash
    C:\Users\shadow\stash> mkdir logs
    C:\Users\shadow\stash> dir
    03/27/2025  10:21 AM    <DIR>          logs
                   0 File(s)              0 bytes
    ```
    - **Scenario:** You’re staging an exfil. `mkdir stash` makes a hideout, then `mkdir logs` inside it for organization. `copy *.txt C:\Users\shadow\stash\logs`—now your loot’s tucked away.  
  - **Explanation:** Your dir forge. Build nests for loot or scripts—stack it (e.g., `mkdir loot\backups`) for instant subdirs.

- **`rmdir <name>`**  
  - **What It Does:** Deletes an empty directory.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    03/27/2025  10:20 AM    <DIR>          stash
    C:\Users\shadow> cd stash
    C:\Users\shadow\stash> dir
    03/27/2025  10:21 AM    <DIR>          logs
    C:\Users\shadow\stash> rmdir logs
    C:\Users\shadow\stash> dir
    [empty]
    C:\Users\shadow\stash> cd ..
    C:\Users\shadow> rmdir stash
    C:\Users\shadow> dir
    [empty again]
    ```
    - **Scenario:** You’ve exfiltrated from `stash\logs`. `rmdir logs` wipes the inner dir (empty), then `rmdir stash` erases the root (also empty). Tracks gone—clean exit. Fails if files remain—use `del` first.  
  - **Explanation:** Your dir eraser. Only works on empty dirs—plan your cleanup.

### File Manipulation Commands

- **`type <file>`**  
  - **What It Does:** Dumps a text file’s contents to the screen.  
  - **Long Example:**  
    ```
    C:\Users\shadow> type notes.txt
    Meeting at 3 PM
    Password: Shadow123!
    Server IP: 192.168.1.200
    Don’t forget to lock the door
    ```
    - **Scenario:** You spot `notes.txt` in `C:\Users\shadow`. `type notes.txt` spills it—creds (`Shadow123!`), an IP (`192.168.1.200`), and a memo. You `ping 192.168.1.200`—it’s alive. Jackpot—next, `netstat` to see if it’s connected.  
  - **Explanation:** Your text file reader. Small files only—big ones flood the screen; use `more` instead.

- **`more <file>`**  
  - **What It Does:** Pages through a file—Spacebar for next page, Enter for next line.  
  - **Long Example:**  
    ```
    C:\Users\shadow> more server.log
    03/27/2025 09:00:01 - System boot
    03/27/2025 09:01:12 - User login: shadow
    03/27/2025 09:05:45 - Connection from 10.0.0.5
    [Spacebar]
    03/27/2025 09:10:22 - File access: C:\secrets\data.txt
    03/27/2025 09:15:00 - Error: Access denied
    [Ctrl+C to exit]
    ```
    - **Scenario:** `server.log` is huge. `more server.log` shows one page—boot and login times. Spacebar flips—file access and errors. You spot `C:\secrets\data.txt`—`cd \secrets` and `type data.txt` next. `Ctrl+C` bails you out.  
  - **Explanation:** Your big-file tamer. Prevents scroll chaos—perfect for logs or configs.

- **`copy <src> <dest>`**  
  - **What It Does:** Copies files from one spot to another.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    C:\Users\shadow> mkdir backup
    C:\Users\shadow> copy notes.txt backup\notes_backup.txt
            1 file(s) copied.
    C:\Users\shadow> cd backup
    C:\Users\shadow\backup> dir
    03/27/2025  10:10 AM            1,024  notes_backup.txt
    C:\Users\shadow\backup> type notes_backup.txt
    Meeting at 3 PM
    Password: Shadow123!
    ```
    - **Scenario:** You’ve got `notes.txt` with creds. `mkdir backup` sets a stash, `copy notes.txt backup\notes_backup.txt` duplicates it. `type notes_backup.txt` confirms—creds intact. Now exfil `backup`—you’ve got a safe copy.  
  - **Explanation:** Your file cloner. Duplicate loot or stage it—full paths or relative work fine.

- **`move <src> <dest>`**  
  - **What It Does:** Relocates files—cuts, doesn’t copy.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    03/27/2025  11:20 AM           15,872  loot.zip
    C:\Users\shadow> mkdir stash
    C:\Users\shadow> move loot.zip stash\loot.zip
            1 file(s) moved.
    C:\Users\shadow> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    C:\Users\shadow> cd stash
    C:\Users\shadow\stash> dir
    03/27/2025  11:20 AM           15,872  loot.zip
    ```
    - **Scenario:** `loot.zip` is hot—move it to `stash`. `move loot.zip stash\loot.zip` shifts it—no trace left in root. `dir` confirms—clean move. Now zip it up or exfil from `stash`.  
  - **Explanation:** Your file teleporter. Moves, not copies—great for cleanup or stealth staging.

- **`del <file>`** or **`erase <file>`**  
  - **What It Does:** Deletes a file—gone forever (unless recovered).  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    03/27/2025  11:20 AM           15,872  loot.zip
    C:\Users\shadow> del loot.zip
    C:\Users\shadow> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    C:\Users\shadow> erase notes.txt
    C:\Users\shadow> dir
    [empty]
    ```
    - **Scenario:** You’ve exfiltrated `loot.zip`—`del loot.zip` wipes it. `notes.txt`’s exposed—`erase notes.txt` nukes it. `dir` shows nothing—clean slate, no evidence.  
  - **Explanation:** Your file shredder. `del` or `erase`—same deal, pick your vibe. No recycle bin here—permanent unless forensics digs it up.

- **`copy *.ext <dest>`**  
  - **What It Does:** Copies all files with a given extension.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    03/27/2025  10:12 AM              512  log.txt
    03/27/2025  11:20 AM           15,872  loot.zip
    C:\Users\shadow> mkdir texts
    C:\Users\shadow> copy *.txt texts
    notes.txt
    log.txt
            2 file(s) copied.
    C:\Users\shadow> cd texts
    C:\Users\shadow\texts> dir
    03/27/2025  10:10 AM            1,024  notes.txt
    03/27/2025  10:12 AM              512  log.txt
    ```
    - **Scenario:** You want all text files. `mkdir texts` preps a dir, `copy *.txt texts` grabs `notes.txt` and `log.txt`. `dir` in `texts` confirms—batch loot secured. Next, `type *.txt` (with `more`) to read them.  
  - **Explanation:** Your wildcard harvester. `*` grabs all matches—`.txt`, `.log`, whatever—streamlines mass copying.

### Process Execution Commands

- **`tasklist`**  
  - **What It Does:** Lists all running processes—name, PID, memory use.  
  - **Long Example:**  
    ```
    C:\Users\shadow> tasklist
    Image Name                     PID Session Name        Session#    Mem Usage
    ========================= ======== ================ =========== ============
    System Idle Process              0 Services                   0         8 K
    System                           4 Services                   0       120 K
    svchost.exe                    820 Services                   0    12,344 K
    chrome.exe                    4567 Console                    1   145,672 K
    notepad.exe                   7890 Console                    1     5,128 K
    cmd.exe                       2345 Console                    1     3,456 K
    sshd.exe                      9123 Services                   0     7,890 K
    ```
    - **Scenario:** You’re on a box—`tasklist` shows `chrome.exe` (PID 4567, 145MB)—user’s browsing. `sshd.exe` (PID 9123)—SSH server’s running. `notepad.exe` (PID 7890)—open doc? You note PIDs—`taskkill /PID 4567` could kill Chrome, or sniff `sshd` traffic.  
  - **Explanation:** Your process radar. Maps what’s running—spot suspicious apps or kill targets.

- **`tasklist /FI "imagename eq <name>"`**  
  - **What It Does:** Filters processes by name.  
  - **Long Example:**  
    ```
    C:\Users\shadow> tasklist /FI "imagename eq chrome.exe"
    Image Name                     PID Session Name        Session#    Mem Usage
    ========================= ======== ================ =========== ============
    chrome.exe                    4567 Console                    1   145,672 K
    chrome.exe                    4589 Console                    1    89,344 K
    chrome.exe                    4601 Console                    1    67,890 K
    ```
    - **Scenario:** `tasklist` is cluttered—focus on Chrome. `tasklist /FI "imagename eq chrome.exe"` lists three instances—PIDs 4567, 4589, 4601. High mem (145MB) on 4567—main window? `taskkill /PID 4567` to disrupt the user, then check `netstat` for browser connections.  
  - **Explanation:** Your process sniper. `/FI` filters—`"imagename eq"` locks onto exact names. Case-insensitive, precise.

- **`taskkill /PID <pid>`**  
  - **What It Does:** Terminates a process by PID.  
  - **Long Example:**  
    ```
    C:\Users\shadow> tasklist
    Image Name                     PID Session Name        Session#    Mem Usage
    chrome.exe                    4567 Console                    1   145,672 K
    notepad.exe                   7890 Console                    1     5,128 K
    C:\Users\shadow> taskkill /PID 4567
    SUCCESS: The process with PID 4567 has been terminated.
    C:\Users\shadow> tasklist
    Image Name                     PID Session Name        Session#    Mem Usage
    notepad.exe                   7890 Console                    1     5,128 K
    ```
    - **Scenario:** Chrome’s (PID 4567) hogging resources or phoning home. `taskkill /PID 4567`—gone. `tasklist` confirms—only `notepad.exe` left. You’ve cleared a path—maybe kill `7890` next if it’s logging your moves.  
  - **Explanation:** Your process assassin. PID precision—kills exactly what you target. No GUI needed.

### Bonus Utility Commands

- **`chkdsk`**  
  - **What It Does:** Scans disk for errors, bad sectors.  
  - **Long Example:**  
    ```
    C:\Users\shadow> chkdsk C:
    The type of the file system is NTFS.
    Volume label is OS.

    Stage 1: Examining basic file system structure ...
      51200 file records processed.
    Stage 2: Examining file name linkage ...
      62418 index entries processed.
    Stage 3: Examining security descriptors ...
      Security descriptor verification completed.
    CHKDSK is verifying free space ...
      128456789012 bytes available on disk.
    ```
    - **Scenario:** Box is laggy—`chkdsk C:` checks the C: drive. No errors—disk’s clean. If it found bad sectors, you’d know the hardware’s failing—time to exfil fast before it dies.  
  - **Explanation:** Your disk doctor. `/f` fixes errors—use it if recon shows corruption.

- **`driverquery`**  
  - **What It Does:** Lists all installed drivers.  
  - **Long Example:**  
    ```
    C:\Users\shadow> driverquery | more
    Module Name  Display Name           Driver Type   Link Date
    ===========  =====================  ============  =========
    ACPI         ACPI Driver            Kernel        11/20/2024
    atapi        IDE Controller         Kernel        06/15/2023
    disk         Disk Driver            Kernel        03/10/2025
    [Spacebar for next page]
    ```
    - **Scenario:** Hunting for driver exploits—`driverquery | more` lists `ACPI`, `atapi`, `disk`. Old `Link Date` (e.g., 2023) on `atapi`—Google it, might be vuln to CVE-2023-XXXX. Paged output keeps it readable.  
  - **Explanation:** Your driver spy. Old drivers = vuln goldmines—pipe `| more` for sanity.

- **`sfc /scannow`**  
  - **What It Does:** Scans and repairs system files.  
  - **Long Example:**  
    ```
    C:\Users\shadow> sfc /scannow
    Beginning system scan.  This process will take some time.
    Beginning verification phase of system scan.
    Verification 100% complete.
    Windows Resource Protection found corrupt files and successfully repaired them.
    Details are included in the CBS.Log file at C:\Windows\Logs\CBS\CBS.log
    ```
    - **Scenario:** Exploit crashed—system files might be toast. `sfc /scannow` finds and fixes corruption. Check `C:\Windows\Logs\CBS\CBS.log` with `more`—details on what broke. Keeps your foothold stable.  
  - **Explanation:** Your system healer. Restores integrity—run it if things go sideways.

### Output Mastery Commands

- **`command | more`**  
  - **What It Does:** Pipes output to a pager—Spacebar flips pages, `Ctrl+C` exits.  
  - **Long Example:**  
    ```
    C:\Users\shadow> dir /s | more
    Directory of C:\Users\shadow
    03/27/2025  10:10 AM            1,024  notes.txt
    03/26/2025  11:20 AM           15,872  loot.zip
    [Spacebar]
    Directory of C:\Users\shadow\Documents
    03/26/2025  09:50 AM               256  todo.txt
    [Ctrl+C]
    C:\Users\shadow>
    ```
    - **Scenario:** `dir /s` floods the screen—`dir /s | more` tames it. First page shows root files, Spacebar flips to `Documents`. Done? `Ctrl+C`—back to prompt. Keeps recon manageable.  
  - **Explanation:** Your scroll savior. Any long output—`systeminfo`, `netstat`—gets paged. Essential for big data.

- **`command /?`**  
  - **What It Does:** Dumps a command’s help page—syntax, options, examples.  
  - **Long Example:**  
    ```
    C:\Users\shadow> tasklist /?
    TASKLIST [/S system [/U username [/P [password]]]]
             [/M [module] | /V | /FI filter] [/FO format] [/NH]

    Description:
        Displays a list of currently running tasks on the local or remote machine.

    Parameter List:
        /S     system           Specifies the remote system to connect to.
        /U     [domain\]user    Specifies the user context under which the command should execute.
        /P     [password]       Specifies the password for the given user context.
        /FI    filter           Applies a filter to select a set of tasks.
        /V                      Verbose output.
    Examples:
        TASKLIST
        TASKLIST /FI "IMAGENAME eq notepad.exe"
        TASKLIST /V /FO CSV > tasks.csv
    ```
    - **Scenario:** `tasklist`—how do you filter? `tasklist /?`—bam, `/FI "IMAGENAME eq notepad.exe"`. You run it, then see `/V` for verbose—`tasklist /V /FI "IMAGENAME eq chrome.exe"`. Every option’s there—your command tutor.  
  - **Explanation:** Most commands support `/?`—instant mastery, no browser needed.
