### Osquery for Incident Response and Threat Hunting

This guide provides a concise overview of using **osquery**, an open-source agent developed by Facebook in 2014, for security analysts, incident responders, and threat hunters. Osquery treats the operating system as a relational database, enabling SQL-like queries to extract system data (e.g., running processes, user accounts, network connections). It supports Windows, Linux, macOS, and FreeBSD, and is particularly useful for identifying suspicious activities. 

---

## Osquery Basics

**Osquery** allows querying system data using SQL, with tables representing system components (e.g., `users`, `processes`, `programs`). The interactive mode (`osqueryi`) is a primary interface for running queries.

- **Start osqueryi**:
  ```bash
  osqueryi
  ```
- **Help Command**:
  ```sql
  .help
  ```
  Lists meta-commands (e.g., `.tables`, `.schema`) for exploring tables and settings.

- **List Tables**:
  ```sql
  .tables
  ```
  Example (partial output):
  ```
  => appcompat_shims
  => arp_cache
  => atom_packages
  => authenticode
  => autoexec
  ...
  ```
  Filter tables (e.g., containing "user"):
  ```sql
  .tables user
  ```
  Output:
  ```
  => user_groups
  => user_ssh_keys
  => userassist
  => users
  ```

- **Table Schema**:
  View column details:
  ```sql
  .schema users
  ```
  Output:
  ```
  CREATE TABLE users(`uid` BIGINT, `gid` BIGINT, `uid_signed` BIGINT, `gid_signed` BIGINT, `username` TEXT, `description` TEXT, `directory` TEXT, `shell` TEXT, `uuid` TEXT, `type` TEXT, `is_hidden` INTEGER HIDDEN, `pid_with_namespace` INTEGER HIDDEN, PRIMARY KEY (`uid`, `username`, `uuid`, `pid_with_namespace`)) WITHOUT ROWID;
  ```

- **Query Syntax**:
  ```sql
  SELECT column1, column2 FROM table WHERE condition;
  ```
  Example:
  ```sql
  SELECT gid, uid, description, username, directory FROM users;
  ```
  Output (partial):
  ```
  | gid | uid  | description                                                | username           | directory                                   |
  |-----|------|------------------------------------------------------------|--------------------|---------------------------------------------|
  | 544 | 500  | Built-in account for administering the computer/domain     | Administrator      |                                             |
  | 544 | 1002 |                                                            | James              | C:\Users\James                              |
  ```

- **Display Modes**:
  Set output format (e.g., `csv`, `line`, `pretty`):
  ```sql
  .mode line
  ```

- **Online Schema**:
  Explore tables at [osquery.io/schema/5.17.0](https://osquery.io/schema/5.17.0).

---

## Investigation Questions and Answers

Using the provided osquery outputs, the following answers address the questions about system activities on a Windows host. All answers are derived from the user’s commands and outputs.

### 1. **Which table stores the evidence of process execution in Windows OS?**
- **Answer**: `userassist`
- **Explanation**: The `userassist` table logs programs executed by users, capturing paths and execution details. From the schema:
  ```sql
  .schema userassist
  ```
  Output:
  ```
  CREATE TABLE userassist(`path` TEXT, `last_execution_time` BIGINT, `count` INTEGER, `sid` TEXT);
  ```
  This table tracks user-initiated program executions, as seen in the query:
  ```sql
  SELECT path FROM userassist;
  ```

### 2. **One of the users seems to have executed a program to remove traces from the disk; what is the name of that program?**
- **Answer**: `DiskWipe.exe`
- **Explanation**: Querying the `userassist` table:
  ```sql
  SELECT path FROM userassist;
  ```
  Output includes:
  ```
  C:\Users\James\Documents\DiskWipe.exe
  ```
  `DiskWipe.exe` is a program associated with disk wiping, indicating an attempt to remove data traces.

### 3. **Create a search query to identify the VPN installed on this host. What is the name of the software?**
- **Answer**: `ProtonVPN`
- **Query**:
  ```sql
  SELECT name FROM programs WHERE name LIKE '%VPN%';
  ```
- **Output**:
  ```
  | name         |
  |--------------|
  | ProtonVPN    |
  | ProtonVPNTap |
  | ProtonVPNTun |
  | ProtonVPN    |
  ```
- **Explanation**: The query searches the `programs` table for entries containing "VPN" using the `LIKE` operator with wildcards (`%`). `ProtonVPN` is identified as the primary VPN software.

### 4. **How many services are running on this host?**
- **Answer**: `214`
- **Query**:
  ```sql
  SELECT count(*) FROM services;
  ```
- **Output**:
  ```
  | count(*) |
  |----------|
  | 214      |
  ```
- **Explanation**: The `services` table lists all services, and `count(*)` returns the total number of services running on the host.

### 5. **A table `autoexec` contains the list of executables that are automatically executed on the target machine. There seems to be a batch file that runs automatically. What is the name of that batch file (with the extension `.bat`)?**
- **Answer**: `batstartup.bat`
- **Query**:
  ```sql
  SELECT name FROM autoexec WHERE name LIKE '%.bat%';
  ```
- **Output**:
  ```
  | name           |
  |----------------|
  | batstartup.bat |
  | batstartup.bat |
  ```
- **Explanation**: The `autoexec` table lists auto-executing programs. The query filters for `.bat` files, identifying `batstartup.bat`.

### 6. **What is the full path of the batch file found in the above question? (Last in the List)**
- **Answer**: `C:\Users\James\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\batstartup.bat`
- **Query**:
  ```sql
  SELECT path FROM autoexec WHERE name LIKE '%.bat%';
  ```
- **Output**:
  ```
  | path                                                                                        |
  |---------------------------------------------------------------------------------------------|
  | C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup\batstartup.bat                 |
  | C:\Users\James\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\batstartup.bat |
  ```
- **Explanation**: The query lists paths for `.bat` files in `autoexec`. The last entry is the user-specific startup folder path for `batstartup.bat`.

---

## Practical Tips for MSSP Analysts

1. **Interactive Mode**:
   - Use `osqueryi` for ad-hoc queries.
   - Run `.tables` to list queryable tables and `.schema <table>` to understand columns.

2. **Query Building**:
   - Start with `SELECT` and `FROM`, end with `;`.
   - Use `WHERE` with operators (`=`, `LIKE`, `%`) for filtering:
     ```sql
     SELECT * FROM users WHERE username = 'James';
     ```
   - Limit results with `LIMIT`:
     ```sql
     SELECT name, version FROM programs LIMIT 5;
     ```

3. **Joining Tables**:
   - Combine tables (e.g., `processes` and `users`) using `JOIN`:
     ```sql
     SELECT p.pid, p.name, u.username FROM processes p JOIN users u ON u.uid = p.uid LIMIT 10;
     ```

4. **Threat Hunting**:
   - **Process Execution**: Query `userassist` for executed programs:
     ```sql
     SELECT path, last_execution_time FROM userassist WHERE path LIKE '%.exe';
     ```
   - **Network Activity**: Check `socket_events` or `processes` for suspicious connections:
     ```sql
     SELECT pid, name, remote_address FROM socket_events WHERE remote_port = 4444;
     ```
   - **Persistence**: Monitor `autoexec` for startup items:
     ```sql
     SELECT path, name FROM autoexec WHERE path LIKE '%Startup%';
     ```

5. **Performance**:
   - Avoid querying tables like `file` without a `WHERE` clause:
     ```sql
     SELECT path FROM file WHERE path LIKE 'C:\Users\%\AppData\%%';
     ```
   - Use `count(*)` to assess table size before querying.

6. **SIEM Integration**:
   - Forward osquery logs to Splunk/Elastic for correlation with Sysmon or network logs.
   - Example: Correlate `processes` with Sysmon Event ID 1 for process creation.

7. **MITRE ATT&CK Mapping**:
   - **Persistence**: T1547 (Startup Items, `autoexec` table).
   - **Execution**: T1059 (Command Execution, `userassist` table).
   - **Defense Evasion**: T1070 (Disk Wiping, `DiskWipe.exe` in `userassist`).

---

## Conclusion

Osquery is a powerful tool for querying system data as a relational database, enabling MSSP analysts to detect suspicious activities like unauthorized program execution (`DiskWipe.exe`), persistence (`batstartup.bat`), and network connections (VPNs like `ProtonVPN`). Using the provided outputs, this guide answers all questions accurately, leveraging `userassist`, `programs`, `services`, and `autoexec` tables. Analysts can extend these queries for threat hunting by combining tables, filtering with `WHERE`, and integrating with SIEM solutions.

---
