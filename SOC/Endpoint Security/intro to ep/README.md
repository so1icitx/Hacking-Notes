# Intro to Endpoint Security: Fundamentals and Logging


## Endpoint Security Fundamentals

### Core Windows Processes

Understanding core Windows processes is essential for distinguishing normal behavior from potential threats. The **Task Manager**, a built-in Windows GUI tool, displays running processes, resource usage (CPU, memory), and allows termination of unresponsive programs. Below is a list of core Windows processes and their typical parent-child relationships, considered normal behavior:

- **System**: Parent to System Idle Process (PID 0).
- **System > smss.exe**: Session Manager Subsystem, initializes user sessions.
- **csrss.exe**: Client/Server Runtime Subsystem, handles GUI and console windows.
- **wininit.exe**: Initializes system components.
- **wininit.exe > services.exe**: Manages Windows services.
- **wininit.exe > services.exe > svchost.exe**: Hosts multiple services.
- **lsass.exe**: Local Security Authentication Server, handles authentication.
- **winlogon.exe**: Manages user logins/logouts.
- **explorer.exe**: Windows Explorer, provides the desktop and file explorer.

**Note**: Processes like `csrss.exe`, `lsass.exe`, `winlogon.exe`, and `explorer.exe` should not have a parent process under normal circumstances, except `System`, which has `System Idle Process` as its parent.

### Sysinternals Tools

**Sysinternals** is a suite of over 70 Windows tools for system analysis, categorized into File and Disk Utilities, Networking Utilities, Process Utilities, Security Utilities, System Information, and Miscellaneous. Two key tools for endpoint investigation are:

- **TCPView**: A networking utility displaying TCP and UDP endpoints, including local/remote addresses, connection states, and owning processes. It’s an enhanced alternative to `netstat`.
  - **Use Case**: Correlate network connections with processes to identify suspicious activity.


- **Process Explorer**: A process utility showing active processes, their owning accounts, and details like handles (files/directories opened) or DLLs/memory-mapped files loaded.
  - **Use Case**: Inspect process details, such as services or network activity, to detect anomalies.


## Endpoint Logging and Monitoring

Endpoint logging enables auditing, aggregating, and analyzing events across systems to detect anomalies. Below are key tools and concepts for endpoint monitoring.

### Windows Event Logs

Windows Event Logs store system, security, and application events in a proprietary binary format (`.evt` or `.evtx`), located at `C:\Windows\System32\winevt\Logs`. They can be accessed via:

- **Event Viewer**: GUI-based tool for viewing logs.

- **Wevtutil.exe**: Command-line tool for log management.
- **Get-WinEvent**: PowerShell cmdlet for querying logs.

**Use Case**: Review security logs for unauthorized logins or system changes.

### Sysmon

**Sysmon**, part of Sysinternals, provides detailed event logging and tracing for Windows systems. It generates high-quality logs with 27 Event IDs, configurable via XML files (e.g., SwiftOnSecurity’s configuration). Sysmon integrates with SIEMs for aggregation and visualization.

- **Example**: Event ID 3 (Network Connection) logs process-initiated network activity.



### OSQuery

**OSQuery**, an open-source tool by Facebook, allows SQL-based queries on endpoints (Windows, Linux, macOS, FreeBSD). Use `osqueryi` for interactive queries or **Kolide Fleet** for multi-endpoint management.

- **Example Query**:
  ```sql
  SELECT pid, name, path FROM processes WHERE name='lsass.exe';
  ```
  **Output**:
  ```
  +-----+-----------+-------------------------------+
  | pid | name      | path                          |
  +-----+-----------+-------------------------------+
  | 748 | lsass.exe | C:\Windows\System32\lsass.exe |
  +-----+-----------+-------------------------------+
  ```

### Wazuh

**Wazuh** is an open-source Endpoint Detection and Response (EDR) solution using a manager-agent model. It monitors devices for vulnerabilities, suspicious activities (e.g., unauthorized logins), and visualizes data via graphs.

- **Features**:
  - Vulnerability auditing.
  - Proactive monitoring for threats.
  - Baseline behavior recording for anomaly detection.


### Event Correlation

Event correlation connects related artifacts across log sources (e.g., Sysmon, firewall logs) to reconstruct incidents. Example:

- **Firewall Log**: Source/destination IP, ports, protocol, action.
- **Sysmon Log (Event ID 3)**: Process name, user, machine name.
- **Correlated Artifacts**:
  - Source/Destination IP and Ports
  - Protocol
  - Action Taken
  - Process Name
  - User Account
  - Machine Name

**Use Case**: Trace a malicious process initiating a network connection.



### Baselining

Baselining establishes normal behavior for users, network traffic, and processes to identify outliers. Examples:

| **Baseline** | **Unusual Activity** |
|--------------|----------------------|
| Employees in London work 9 AM–6 PM. | VPN login from Singapore at 3 AM. |
| One workstation per employee. | User authenticates to multiple workstations. |
| Access limited to O365 apps. | 3GB upload to Google Drive. |
| Only Microsoft apps installed. | `firefox.exe` running on workstations. |

**Use Case**: Detect unauthorized processes or network activity.


## Practical Tips

- **Task Manager**: Use to quickly identify rogue processes deviating from core Windows processes.
- **Sysinternals**: Run **TCPView** and **Process Explorer** in a sandbox to analyze live systems safely.
- **Event Logs**: Use **Event Viewer** for initial review, then **Get-WinEvent** for scripted analysis.
- **Sysmon**: Deploy with a custom configuration (e.g., SwiftOnSecurity’s) for granular logging.
- **OSQuery**: Combine with **Kolide Fleet** for scalable endpoint querying.
- **Wazuh**: Configure agents to monitor critical systems and integrate with SIEMs.
- **Correlation**: Cross-reference Sysmon and firewall logs to build incident timelines.
- **Baselining**: Regularly update baselines to reflect new applications or user behaviors.


# Notes
- Use SwiftOnSecurity’s config: https://github.com/SwiftOnSecurity/sysmon-config
- Integrate with SIEM for aggregation and alerting.


