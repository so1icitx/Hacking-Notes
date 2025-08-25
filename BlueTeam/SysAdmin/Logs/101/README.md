# Logs 101: Master System Logs

## Overview
Logs are your system’s audit trail, recording every action—logins, errors, network requests—to help troubleshoot, monitor performance, and ensure security. This module teaches you the essentials of logs: what they are, their types, formats, and how to collect, manage, and analyze them. Designed for beginners and intermediate learners, it’s packed with practical examples and commands for real-world use.

## Why Logs?
Logs answer critical questions:
- **What** happened? (e.g., Failed login)
- **When**? (e.g., 2025-08-25 12:34:56)
- **Where**? (e.g., IP 10.10.133.168)
- **Who**? (e.g., User "alice")
- **Success**? (e.g., Access granted)
- **Result**? (e.g., Data breach)

**Analogy**: Logs are like a security camera, capturing every move in your system for review.

## What Are Logs?
Logs record system events with:
- **Timestamp**: When it happened (e.g., `2025-08-25T12:34:56Z`).
- **Source**: System/app (e.g., `WEBSRV-02`).
- **Event Type**: Action (e.g., login, error).
- **Details**: Context (e.g., IP, user, error code).

Logs grow fast, so efficient management is key.

## Log Types
- **System**: OS events (e.g., `/var/log/syslog`, kernel errors).
- **Application**: App-specific (e.g., crashes, warnings).
- **Security**: Logins, permissions (e.g., `/var/log/auth.log`).
- **Network**: Traffic, connections.
- **Web Server**: HTTP requests (e.g., `/var/log/apache2/access.log`).
- **Audit**: Compliance records.
- **Database**: Queries, updates.

## Log Formats
- **Structured** (easy to parse):
  - **JSON**: `{"time":"2025-08-25T12:34:56Z","user":"alice","action":"GET"}`
  - **CSV**: `"2025-08-25T12:34:56Z","alice","GET",200`
  - **W3C ELF**: `2025-08-25 12:34:56 10.10.133.168 alice GET /api 200`
- **Semi-Structured**:
  - **Syslog**: `Aug 25 12:34:56 WEBSRV-02 sshd[1234]: Accepted password for alice`
  - **Windows EVTX**: Binary, viewed via Event Viewer.
- **Unstructured**:
  - **NCSA CLF**: `10.10.133.168 - alice [25/Aug/2025:12:34:56 +0000] "GET /api" 200 4886`
  - **NCSA Combined**: Adds referrer, user-agent: `... "http://example.com" "Mozilla/5.0"`

## Log Standards
- **Syslog**: Standard for system/network logs (RFC 5424).
- **CEE**: Common Event Expression for log structure.
- **NIST 800-92**: Security log management guidelines.
- **OWASP Logging**: Security logging best practices.

## Log Collection
**Steps**:
1. **List Sources**: Servers, apps, networks (e.g., `/var/log/syslog`, Apache logs).
2. **Choose Tool**: `rsyslog`, Splunk, or Elastic Stack.
3. **Sync Time**: Use NTP (`sudo ntpdate pool.ntp.org`).
4. **Test**: Verify logs are collected.

**Example**:
```bash
sudo ntpdate pool.ntp.org
# Output: 25 Aug 2025 12:34:56 EEST
```

## Activity: Collect SSH Logs with rsyslog
Configure `rsyslog` to log SSH events to `/var/log/websrv-02/sshd.log`.

1. **Check rsyslog**:
   ```bash
   sudo systemctl status rsyslog
   ```
2. **Create Config**:
   ```bash
   sudo nano /etc/rsyslog.d/98-websrv-02-sshd.conf
   ```
   Add:
   ```
   $FileCreateMode 0644
   :programname, isequal, "sshd" /var/log/websrv-02/sshd.log
   ```
3. **Restart rsyslog**:
   ```bash
   sudo systemctl restart rsyslog
   ```
4. **Test SSH**:
   ```bash
   ssh localhost
   ```
5. **View Logs**:
   ```bash
   cat /var/log/websrv-02/sshd.log
   # Output: Aug 25 12:34:56 WEBSRV-02 sshd[1234]: Accepted password for alice
   ```

## Log Management
- **Store**: Use secure local (`/var/log`) or cloud storage.
- **Organize**: Group by source/type (e.g., `auth.log`, `access.log`).
- **Backup**: Regular backups to prevent loss.
- **Retention**:
  - **Hot**: 3-6 months, fast access.
  - **Warm**: 6 months-2 years, less frequent.
  - **Cold**: 2-5 years, archived.

**Factors**: Security, compliance (e.g., GDPR), storage capacity, cost.

## Log Centralization
- **Tool**: Elastic Stack, Splunk, Graylog.
- **Steps**:
  1. Connect sources (e.g., servers, apps).
  2. Set alerts for critical events (e.g., failed logins).
  3. Integrate with incident tools (e.g., ServiceNow).

## Log Analysis
**Process**:
- **Parse**: Extract fields (e.g., timestamp, IP).
- **Normalize**: Standardize formats (e.g., UTC timestamps).
- **Sort**: By time, severity.
- **Classify**: Group by type (e.g., errors).
- **Enrich**: Add context (e.g., geolocation).
- **Correlate**: Link events (e.g., login → file access).
- **Visualize**: Use charts (e.g., Kibana).
- **Report**: Summarize for stakeholders.

**Commands**:
```bash
grep "error" /var/log/syslog
# Output: Aug 25 12:34:56 WEBSRV-02 kernel: Out of memory
```
```bash
sha256sum /var/log/syslog
# Verify log integrity
```

## Key Log Examples
### Windows
- **Location**: `C:\Windows\System32\winevt\Logs`.
- **Format**: `.evtx`, exportable to `.csv`, `.xml`.
- **View**: Event Viewer, `Get-WinEvent`.
- **Logs**:
  - **Security**: Event ID 4624 (login), 1102 (log deletion).
  - **System**: OS errors.
  - **Application**: App events.
- **Example**:
  ```
  TimeCreated: 2025-08-25 12:34:56
  Id: 4624
  Message: An account was successfully logged on.
  ```

### Linux
- **Location**: `/var/log`.
- **Format**: `.log` (Syslog), binary (`journald`).
- **View**: `cat`, `grep`, `journalctl`.
- **Logs**:
  - `/var/log/syslog`: System events.
  - `/var/log/auth.log`: Logins.
  - `/var/log/kern.log`: Kernel.
- **Example**:
  ```
  Aug 25 12:34:56 WEBSRV-02 sudo[1234]: alice : command=/usr/bin/apt-get update
  ```

### Apache
- **Location**: `/var/log/apache2/{access,error}.log`.
- **Format**: Combined Log Format.
- **View**: `cat`, `lnav`.
- **Access Log**:
  ```
  192.168.1.100 - alice [25/Aug/2025:12:34:56 +0000] "POST /api" 404 1234 "http://example.com" "Mozilla/5.0"
  ```
- **Error Log**:
  ```
  [25/Aug/2025:12:34:56] [error] [pid 8777] [client 127.0.0.1] File does not exist: /htdocs/favicon.ico
  ```

## Best Practices
- **Collect Selectively**: Avoid irrelevant data.
- **Consistent Format**: Use JSON or Syslog for parsing.
- **Secure Logs**: Encrypt, restrict access.
- **Set Alerts**: Monitor critical events (e.g., `grep "failed" /var/log/auth.log`).
- **Update Regularly**: Adapt to new threats.

## Challenges
- **Volume**: Large logs need efficient storage.
- **Performance**: Logging can slow systems.
- **Diverse Formats**: Parsing varies by source.
- **Analysis**: Correlation requires skill.

## Learning Tips
- **Practice Commands**: Run `grep`, `cat` on `/var/log/syslog`.
- **Use lnav**: Interactive log viewer for beginners.
- **Cheat Sheet**: Save commands like `grep "error" /var/log/*`.
- **Simulate**: Generate test logs (e.g., `ssh localhost`).
- **Study Standards**: Check Syslog RFC 5424, NIST 800-92.

## Troubleshooting
- **No Logs**: Verify logging service (e.g., `rsyslog`).
- **Time Errors**: Sync with `ntpdate pool.ntp.org`.
- **Permission Issues**: Check with `ls -l /var/log`.
- **Large Files**: Use `tail -n 100 /var/log/syslog`.

## Resources
- [Syslog RFC 5424](https://tools.ietf.org/html/rfc5424)
- [NIST 800-92](https://csrc.nist.gov/publications/detail/sp/800-92/final)
- [OWASP Logging Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Logging_Cheat_Sheet.html)
- [rsyslog Docs](https://www.rsyslog.com/doc/)
- [lnav](https://lnav.org/)


