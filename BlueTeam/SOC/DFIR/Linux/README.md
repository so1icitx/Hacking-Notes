# Linux Forensics: System and User Activity Analysis

## Overview
Linux forensics involves analyzing system artifacts to reconstruct user activity, system configuration, and potential malicious behavior..

### OS Release Information
The `/etc/os-release` file contains details about the Linux distribution and version. Use the `cat` utility to view it.

**Command**: `cat /etc/os-release`

**Example Output**:
```
NAME="Ubuntu"
VERSION="20.04.1 LTS (Focal Fossa)"
ID=ubuntu
ID_LIKE=debian
PRETTY_NAME="Ubuntu 20.04.1 LTS"
VERSION_ID="20.04"
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
VERSION_CODENAME=focal
UBUNTU_CODENAME=focal
```

**Use Case**: Identifies the OS version (e.g., Ubuntu 20.04.1 LTS) for compatibility with forensic tools and plugins.

### User Accounts
The `/etc/passwd` file lists user account details, including username, password placeholder (`x`), UID, GID, description, home directory, and default shell. UIDs ≥ 1000 typically indicate user-created accounts.

**Command**: `cat /etc/passwd | column -t -s :`

**Example Output**:
```
root       x  0     0     root               /root                /bin/bash
daemon     x  1     1     daemon             /usr/sbin            /usr/sbin/nologin
ubuntu     x  1000  1000  Ubuntu             /home/ubuntu         /bin/bash
tryhackme  x  1001  1001  tryhackme,,,       /home/tryhackme      /bin/bash
```

**Use Case**: Identifies user accounts (e.g., `ubuntu` with UID 1000) and their home directories for further investigation.

### Group Information
The `/etc/group` file details group memberships, including group name, password placeholder (`x`), GID, and member users.

**Command**: `cat /etc/group`

**Example Output**:
```
root:x:0:
adm:x:4:syslog,ubuntu
tty:x:5:syslog
```

**Use Case**: Reveals group memberships (e.g., `ubuntu` in `adm` group) to assess privilege levels.

### Sudoers List
The `/etc/sudoers` file lists users and groups with `sudo` privileges, requiring elevated access to view.

**Command**: `sudo cat /etc/sudoers`

**Example Output**:
```
Defaults env_reset
Defaults mail_badpass
Defaults secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin"
root ALL=(ALL:ALL) ALL
%admin ALL=(ALL) ALL
%sudo ALL=(ALL:ALL) ALL
```

**Use Case**: Identifies users (e.g., `root`, `admin` group) with privilege escalation capabilities.

### Login Information
The `/var/log/wtmp` (successful logins) and `/var/log/btmp` (failed logins) files are binary logs accessed using the `last` utility.

**Command**: `sudo last -f /var/log/wtmp`

**Example Output**:
```
reboot   system boot  5.4.0-1029-aws   Tue Mar 29 17:28   still running
reboot   system boot  5.4.0-1029-aws   Tue Mar 29 04:46 - 15:52  (11:05)
wtmp begins Mon Mar 28 01:35:10 2022
```

**Use Case**: Tracks login history and system reboots for timeline reconstruction.

### Authentication Logs
The `/var/log/auth.log` file logs authentication events, including `sudo` usage and session activity.

**Command**: `cat /var/log/auth.log | tail`

**Example Output**:
```
Mar 29 17:49:52 tryhackme sudo:   ubuntu : TTY=pts/0 ; PWD=/home/ubuntu ; USER=root ; COMMAND=/usr/bin/cat /etc/sudoers
Mar 29 17:49:52 tryhackme sudo: pam_unix(sudo:session): session opened for user root by (uid=0)
Mar 29 17:49:52 tryhackme sudo: pam_unix(sudo:session): session closed for user root
```

**Use Case**: Identifies privilege escalations (e.g., `ubuntu` running `sudo cat /etc/sudoers`).

### Hostname
The `/etc/hostname` file stores the system’s hostname.

**Command**: `cat /etc/hostname`

**Example Output**:
```
tryhackme
```

**Use Case**: Confirms the system’s identity for correlation with other artifacts.

### Timezone
The `/etc/timezone` file indicates the system’s timezone.

**Command**: `cat /etc/timezone`

**Example Output**:
```
Etc/UTC
```

**Use Case**: Provides context for timestamps in logs.

### Network Configuration
The `/etc/network/interfaces` file defines network interface settings.

**Command**: `cat /etc/network/interfaces`

**Example Output**:
```
auto lo
iface lo inet loopback
auto eth0
iface eth0 inet dhcp
```

**Use Case**: Identifies network interface configurations (e.g., `eth0` using DHCP).

The `ip` utility provides live system network details, including IP and MAC addresses.

**Command**: `ip address show`

**Example Output**:
```
2: eth0: mtu 9001 qdisc mq state UP group default qlen 1000
    link/ether 02:20:61:f1:3c:e9 brd ff:ff:ff:ff:ff:ff
    inet 10.10.95.252/16 brd 10.10.255.255 scope global dynamic eth0
```

**Use Case**: Reveals active network interfaces and IPs.

### Active Network Connections
The `netstat` utility lists active network connections on a live system.

**Command**: `netstat -natp`

**Example Output**:
```
Proto Recv-Q Send-Q Local Address     Foreign Address     State       PID/Program name
tcp   0      0      0.0.0.0:80        0.0.0.0:*           LISTEN      -
tcp   0      0      10.10.95.252:80   10.100.1.33:51934   ESTABLISHED -
```

**Use Case**: Identifies active connections (e.g., web server on port 80) for potential malicious activity.

### Running Processes
The `ps` utility lists running processes on a live system.

**Command**: `ps aux`

**Example Output**:
```
USER   PID  %CPU %MEM  VSZ    RSS TTY  STAT START TIME COMMAND
ubuntu 1672 0.0  0.1   5264  4588 pts/0 Ss  17:29 0:00 bash
ubuntu 1985 0.0  0.0   5892  2872 pts/0 R+  17:40 0:00 ps au
```

**Use Case**: Detects running processes for signs of malicious activity.

### DNS Information
The `/etc/hosts` file maps hostnames to IP addresses.

**Command**: `cat /etc/hosts`

**Example Output**:
```
127.0.0.1 localhost
::1 ip6-localhost ip6-loopback
```

**Use Case**: Identifies local DNS mappings.

The `/etc/resolv.conf` file lists DNS servers for resolution.

**Command**: `cat /etc/resolv.conf`

**Example Output**:
```
nameserver 127.0.0.53
options edns0 trust-ad
search eu-west-1.compute.internal
```

**Use Case**: Reveals DNS servers used by the system.

### Persistence Mechanisms
Persistence mechanisms allow programs (e.g., malware) to survive reboots.

#### Cron Jobs
The `/etc/crontab` file schedules periodic tasks.

**Command**: `cat /etc/crontab`

**Example Output**:
```
17 * * * * root cd / && run-parts --report /etc/cron.hourly
25 6 * * * root test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.daily )
```

**Use Case**: Identifies scheduled tasks that may indicate persistence.

#### Service Startup
The `/etc/init.d` directory lists services that start on boot.

**Command**: `ls /etc/init.d/`

**Example Output**:
```
acpid  cron  networking  ssh  ufw
```

**Use Case**: Detects services that may be maliciously configured.

#### .bashrc
The `~/.bashrc` file runs commands when a bash shell starts.

**Command**: `cat ~/.bashrc`

**Example Output**:
```
if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi
```

**Use Case**: Checks for malicious commands executed at shell startup. System-wide settings are in `/etc/bash.bashrc` and `/etc/profile`.

### Evidence of Execution
#### Sudo Execution History
Sudo commands are logged in `/var/log/auth.log`.

**Command**: `cat /var/log/auth.log* | grep -i COMMAND | tail`

**Example Output**:
```
Mar 29 17:49:52 tryhackme sudo: ubuntu : TTY=pts/0 ; PWD=/home/ubuntu ; USER=root ; COMMAND=/usr/bin/cat /etc/sudoers
```

**Use Case**: Tracks privileged command execution.

#### Bash History
The `~/.bash_history` file stores user command history.

**Command**: `cat ~/.bash_history`

**Example Output**:
```
cd Downloads/
unzip PracticalMalwareAnalysis-Labs-master.zip
mkdir wannacry
mv Ransomware.WannaCry.zip wannacry/
```

**Use Case**: Reveals commands executed by users (e.g., malware analysis activities).

#### Vim Accessed Files
The `~/.viminfo` file logs files opened in Vim.

**Command**: `cat ~/.viminfo`

**Example Output**:
```
> ~/Downloads/str
	*	1636562410	0
	"	1139	0
```

**Use Case**: Tracks files edited or viewed in Vim.

### Log Files
Log files in `/var/log` record system and application activity.

#### Syslog
The `/var/log/syslog` file logs system-wide events.

**Command**: `cat /var/log/syslog* | head`

**Example Output**:
```
Mar 29 00:00:37 tryhackme systemd[1]: Finished Daily man-db regeneration.
Mar 29 00:17:01 tryhackme CRON[7726]: (root) CMD ( cd / && run-parts --report /etc/cron.hourly )
```

**Use Case**: Provides a timeline of system activity, including cron jobs. Use `*` to include rotated logs (`syslog.1`, `syslog.2`).

#### Auth Logs
Covered above, logs authentication events.

#### Third-Party Logs
The `/var/log` directory contains logs for applications like Apache (`/var/log/apache2`) and Samba (`/var/log/samba`).

**Command**: `ls /var/log`

**Example Output**:
```
apache2  auth.log  syslog  samba
```

**Use Case**: Investigates web server or file-sharing activity.

**Apache Logs**:
```
access.log  error.log  other_vhosts_access.log
```
