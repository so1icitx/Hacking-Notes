# Wazuh: Open-Source Endpoint Detection and Response (EDR)

## Overview
Wazuh is a free, open-source Endpoint Detection and Response (EDR) solution launched in 2015 as a fork of OSSEC. It provides robust security monitoring for endpoints across various environments, including on-premises, cloud, virtualized, and containerized systems. Designed for scalability, Wazuh is suitable for small businesses to large enterprises. It operates on a **manager-agent model**, where a central **Wazuh manager** collects, analyzes, and visualizes data from **agents** installed on monitored devices. Agents track system events (e.g., authentication, file changes) and send logs to the manager for processing, enabling threat detection and incident response.

![Wazuh Manager-Agent Architecture](./screenshots/1.png)

### Key EDR Features
Wazuh offers a comprehensive set of features to detect and respond to security threats:

- **Vulnerability Assessment**: Periodically scans endpoints for installed software and checks for known vulnerabilities using CVE databases.
- **Proactive Monitoring**: Detects suspicious activities such as unauthorized logins, brute-force attacks (e.g., SSH attempts), and privilege escalations.
- **Data Visualization**: Transforms complex event data into intuitive graphs and dashboards for quick analysis.
- **Anomaly Detection**: Establishes baseline behavior for devices to identify deviations, such as unexpected process execution or file modifications.

## Deployment
To monitor endpoints, Wazuh agents must be installed on devices to collect logs and events, which are then sent to the Wazuh manager for analysis. The deployment process is streamlined through a wizard in the Wazuh dashboard.

### Agent Deployment
1. **Access the Deployment Wizard**:
   - Navigate to `Wazuh -> Agents -> Deploy New Agent` in the Wazuh dashboard to launch an intuitive wizard.

   ![Wazuh Agent Deployment Wizard](./screenshots/2.png)

2. **Specify Prerequisites**:
   - **Operating System**: Select the target OS (e.g., Windows, Debian/Ubuntu, CentOS).
   - **Wazuh Server Address**: Provide the IP address or DNS name of the Wazuh manager (e.g., `10.10.12.255`).
   - **Agent Group**: Optionally assign the agent to a group for organizational purposes (e.g., `default`, `servers`, `workstations`).

3. **Run Installation Commands**:
   - The wizard generates a command to install and configure the agent. Copy and execute it on the target device.
   - **Windows**:
     ```powershell
     Invoke-WebRequest -Uri https://packages.wazuh.com/4.x/windows/wazuh-agent-4.x.x-x.msi -OutFile wazuh-agent.msi; msiexec.exe /i wazuh-agent.msi /q WAZUH_MANAGER='10.10.12.255' WAZUH_AGENT_GROUP='default'
     ```

     ![Installing Wazuh Agent on Windows](./screenshots/3.png)

   - **Debian/Ubuntu**:
     ```bash
     curl -sO https://packages.wazuh.com/4.x/apt/deb/wazuh-agent_4.x.x-x_amd64.deb
     sudo WAZUH_MANAGER='10.10.12.255' dpkg -i wazuh-agent_4.x.x-x_amd64.deb
     ```

     ![Installing Wazuh Agent on Debian/Ubuntu](./screenshots/4.png)

4. **Restart the Agent**:
   - Ensure the agent starts and connects to the manager:
     ```bash
     sudo systemctl restart wazuh-agent
     ```
   - Verify agent status in the Wazuh dashboard under `Agents`.

## Vulnerability Assessment
Wazuh’s **Vulnerability Detector** module scans endpoints for installed applications and their versions, comparing them against CVE databases to identify potential vulnerabilities. It performs a full scan when an agent is first installed and continues at configurable intervals (default: every 5 minutes).

### Configuration
Edit the Wazuh manager’s configuration file at `/var/ossec/etc/ossec.conf`:
```xml
<vulnerability-detector>
  <enabled>yes</enabled>
  <interval>5m</interval>
  <run_on_start>yes</run_on_start>
</vulnerability-detector>
```
- `<enabled>yes</enabled>`: Activates the vulnerability scanner.
- `<interval>5m</interval>`: Sets scans to run every 5 minutes.
- `<run_on_start>yes</run_on_start>`: Triggers a scan upon agent startup.

Restart the Wazuh manager:
```bash
sudo systemctl restart wazuh-manager
```
Vulnerability alerts appear in the Wazuh dashboard under `Security Events` or `Vulnerability Detection`, showing details like CVE ID, severity, and affected software.

## Security Event Monitoring
Wazuh monitors security events, such as authentication attempts, and generates alerts based on predefined rules. Alerts are stored in `/var/ossec/logs/alerts/alerts.log` on the Wazuh manager and can be viewed using commands like `grep` or `less`.

### Example: SSH Brute-Force Alert
An alert for an unsuccessful SSH login attempt (rule ID 5710) might look like:
```
Field: Value
agent.ip: 10.10.73.118
agent.name: ip-10-10-73-118
rule.description: sshd: Attempt to login using a non-existent user
rule.mitre.technique: Brute-Force
rule.mitre.id: T1110
rule.id: 5710
location: /var/log/auth.log
```
To view alerts:
```bash
sudo less /var/ossec/logs/alerts/alerts.log
```

### Example: Successful Login Alert
A login session opened by the `ubuntu` user for the `root` account:
```
** Alert 1634284538.566764: - pam,syslog,authentication_success,pci_dss_10.2.5,gpg13_7.8,gpg13_7.9,gdpr_IV_32.2,hipaa_164.312.b
2021 Oct 15 07:55:38 ip-10-10-218-190->/var/log/auth.log
Rule: 5501 (level 3) -> 'PAM: Login session opened.'
User: root
Oct 15 07:55:37 ip-10-10-218-190 sudo: pam_unix(sudo:session): session opened for user root by ubuntu(uid=0)
uid: 0
```

## Sysmon Integration (Windows)
Wazuh integrates with **Sysmon**, a Windows tool that logs detailed system events (e.g., process creation, network connections) to the Windows Event Log. These events are collected by the Wazuh agent and sent to the manager for analysis.

### Sysmon Configuration
Create a Sysmon configuration file (`detect_powershell.xml`) to monitor `powershell.exe` execution:
```xml
<Sysmon schemaversion="3.30">
  <HashAlgorithms>md5</HashAlgorithms>
  <EventFiltering>
    <!-- SYSMON EVENT ID 1: PROCESS CREATION -->
    <ProcessCreate onmatch="include">
      <Image condition="contains">powershell.exe</Image>
    </ProcessCreate>
    <!-- SYSMON EVENT ID 2: FILE CREATION TIME CHANGED -->
    <FileCreateTime onmatch="include"/>
    <!-- SYSMON EVENT ID 3: NETWORK CONNECTION INITIATED -->
    <NetworkConnect onmatch="include"/>
    <!-- SYSMON EVENT ID 5: PROCESS ENDED -->
    <ProcessTerminate onmatch="include"/>
    <!-- SYSMON EVENT ID 6: DRIVER LOADED -->
    <DriverLoad onmatch="include"/>
    <!-- SYSMON EVENT ID 7: DLL LOADED -->
    <ImageLoad onmatch="include"/>
    <!-- SYSMON EVENT ID 8: REMOTE THREAD CREATED -->
    <CreateRemoteThread onmatch="include"/>
    <!-- SYSMON EVENT ID 9: RAW DISK ACCESS -->
    <RawAccessRead onmatch="include"/>
    <!-- SYSMON EVENT ID 10: INTER-PROCESS ACCESS -->
    <ProcessAccess onmatch="include"/>
    <!-- SYSMON EVENT ID 11: FILE CREATED -->
    <FileCreate onmatch="include"/>
    <!-- SYSMON EVENT ID 12, 13, 14: REGISTRY MODIFICATION -->
    <RegistryEvent onmatch="include"/>
    <!-- SYSMON EVENT ID 15: ALTERNATE DATA STREAM CREATED -->
    <FileCreateStreamHash onmatch="include"/>
    <PipeEvent onmatch="include"/>
  </EventFiltering>
</Sysmon>
```
Install Sysmon with the configuration:
```bash
Sysmon64.exe -accepteula -i detect_powershell.xml
```

![Sysmon Configuration Installation](./screenshots/5.png)
![Sysmon Configuration Applied](./screenshots/6.png)

Verify Sysmon events in Event Viewer under `Microsoft-Windows-Sysmon/Operational`:

![Sysmon Event Viewer Search](./screenshots/7.png)
![Sysmon Event Log](./screenshots/8.png)

Launch a PowerShell prompt and check Event Viewer for the event:

![PowerShell Event in Event Viewer](./screenshots/9.png)

### Wazuh Agent Configuration
Edit `C:\Program Files (x86)\ossec-agent\ossec.conf` on the Windows agent:
```xml
<localfile>
  <location>Microsoft-Windows-Sysmon/Operational</location>
  <log_format>eventchannel</log_format>
</localfile>
```
Restart the agent:
```bash
net stop wazuh
net start wazuh
```

### Wazuh Server Configuration
Add a Sysmon rule to `/var/ossec/etc/rules/local_rules.xml`:
```xml
<group name="sysmon,">
  <rule id="255000" level="12">
    <if_group>sysmon_event1</if_group>
    <field name="sysmon.image">\\powershell.exe||\\.ps1||\\.ps2</field>
    <description>Sysmon - Event 1: Suspicious executable: $(sysmon.image)</description>
    <group>sysmon_event1,powershell_execution,</group>
    <mitre>
      <id>T1059</id>
    </mitre>
  </rule>
</group>
```
Restart the Wazuh manager:
```bash
sudo systemctl restart wazuh-manager
```

## Apache2 Log Monitoring (Linux)
Wazuh uses the `0250-apache_rules.xml` ruleset to analyze Apache2 logs for warnings, errors, or suspicious activities (e.g., SQL injection attempts).

### Configuration
Edit `/var/ossec/etc/ossec.conf` on the Linux agent running Apache2:
```xml
<localfile>
  <location>/var/log/example.log</location>
  <log_format>syslog</log_format>
</localfile>
```
Restart the agent:
```bash
sudo systemctl restart wazuh-agent
```
Alerts appear in the Wazuh dashboard under `Security Events`, with details like HTTP status codes or malicious request patterns.

## Auditd Integration (Linux)
Wazuh integrates with **auditd** on Linux (Debian/Ubuntu, CentOS) to monitor system events, such as commands executed as root, which could indicate a breach.

### Install Auditd
```bash
sudo apt-get install auditd audispd-plugins
sudo systemctl enable auditd.service
sudo systemctl start auditd.service
```

### Configure Auditd
Edit `/etc/audit/rules.d/audit.rules` to monitor root commands:
```bash
## Delete existing rules
-D

## Increase buffers for busy systems
-b 8192

## Set failure mode to syslog
-f 1

## Monitor root command execution
-a exit,always -F arch=b64 -F euid=0 -S execve -k audit-wazuh-c
```
Apply the rule:
```bash
sudo auditctl -R /etc/audit/rules.d/audit.rules
```

### Wazuh Agent Configuration
Edit `/var/ossec/etc/ossec.conf`:
```xml
<localfile>
  <location>/var/log/audit/audit.log</location>
  <log_format>audit</log_format>
</localfile>
```
Restart the agent:
```bash
sudo systemctl restart wazuh-agent
```

![Auditd Log Configuration](./screenshots/10.png)

## Wazuh API
The Wazuh API (port 55000/TCP, TLS-encrypted) enables command-line interaction with the manager for tasks like checking server status or managing agents.

### Authentication
Authenticate and store the token:
```bash
TOKEN=$(curl -u <username>:<password> -k -X GET "https://10.10.12.255:55000/security/user/authenticate?raw=true")
```
Verify authentication:
```bash
curl -k -X GET "https://10.10.12.255:55000/" -H "Authorization: Bearer $TOKEN"
```
Output:
```json
{
  "data": {
    "title": "Wazuh API",
    "api_version": "4.0.0",
    "revision": 4000,
    "license_name": "GPL 2.0",
    "license_url": "https://github.com/wazuh/wazuh/blob/master/LICENSE",
    "hostname": "wazuh-master",
    "timestamp": "2021-10-25T07:05:00+0000"
  },
  "error": 0
}
```

### Query Server Status
Check running services:
```bash
curl -k -X GET "https://10.10.12.255:55000/manager/status?pretty=true" -H "Authorization: Bearer $TOKEN"
```
Output:
```json
{
  "data": {
    "affected_items": [
      {
        "wazuh-agentlessd": "running",
        "wazuh-analysisd": "running",
        "wazuh-authd": "running",
        "wazuh-csyslogd": "running",
        "wazuh-dbd": "stopped",
        "wazuh-monitord": "running",
        "wazuh-execd": "running",
        "wazuh-integratord": "running",
        "wazuh-logcollector": "running",
        "wazuh-maild": "running",
        "wazuh-remoted": "running",
        "wazuh-reportd": "stopped",
        "wazuh-syscheckd": "running",
        "wazuh-clusterd": "running",
        "wazuh-modulesd": "running",
        "wazuh-db": "running",
        "wazuh-apid": "stopped"
      }
    ],
    "total_affected_items": 1,
    "total_failed_items": 0,
    "failed_items": []
  },
  "message": "Processes status were successfully read in specified node",
  "error": 0
}
```

### Query Agents
List active agents:
```bash
curl -k -X GET "https://10.10.12.255:55000/agents?pretty=true&status=active" -H "Authorization: Bearer $TOKEN"
```
Output:
```json
{
  "data": {
    "affected_items": [
      {
        "node_name": "worker2",
        "status": "active",
        "manager": "wazuh-worker2",
        "version": "Wazuh v3.13.1",
        "id": "001",
        "name": "wazuh-agent1"
      }
    ],
    "total_affected_items": 9,
    "total_failed_items": 0,
    "failed_items": []
  },
  "message": "All selected agents information was returned",
  "error": 0
}
```

### API Console
Access the Wazuh API console via the dashboard under `Tools` to run queries (e.g., `/manager/info`) using GET/POST/PUT/DELETE methods.

![Wazuh API Console](./screenshots/11.png)

## Additional Tips for MSSP Analysts
- **Compliance**: Wazuh supports PCI DSS, HIPAA, GDPR, and NIST with built-in rulesets and compliance reports.
- **File Integrity Monitoring (FIM)**: Monitor critical files (e.g., `/etc/passwd`) for unauthorized changes:
  ```xml
  <syscheck>
    <directories check_all="yes">/etc</directories>
  </syscheck>
  ```
- **Active Response**: Automate responses like blocking IPs:
  ```xml
  <active-response>
    <command>firewall-drop</command>
    <location>local</location>
    <rules_id>5710</rules_id>
  </active-response>
  ```
- **Dashboard**: Use the Wazuh dashboard for real-time alerts, filtering by rule ID (e.g., 5710 for SSH brute-force) or MITRE ATT&CK ID (e.g., T1110).
- **Custom Rules**: Add rules to `/var/ossec/etc/rules/local_rules.xml` for specific threats, ensuring they persist across updates.

## Documentation
- Wazuh Documentation: [https://documentation.wazuh.com](https://documentation.wazuh.com)
- Wazuh API Reference: [https://documentation.wazuh.com/current/user-manual/api/](https://documentation.wazuh.com/current/user-manual/api/)
