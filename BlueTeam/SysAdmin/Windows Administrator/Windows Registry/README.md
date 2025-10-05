# Windows Registry Forensics: A Deep-Dive Study Guide

## Introduction

The Windows Registry is like a digital diary for your computer, meticulously recording its configuration, user activities, and hardware interactions. For digital forensics and malware analysis, it’s a treasure trove, revealing traces of executed programs, connected devices, network activity, and user behavior. 

## What is the Windows Registry?

- **Definition**: A hierarchical database storing configuration settings for the Windows operating system, hardware, software, and user profiles.
- **Forensic Value**: Captures user actions (e.g., opened files, typed URLs), system events (e.g., USB connections, program executions), and potential malware activity (e.g., persistence mechanisms).
- **Accessing the Registry**:
  - **Live System**: Use `regedit.exe` (Windows key + R, type `regedit`) to view or edit, but avoid editing during forensics to preserve evidence.
  - **Disk Image**: Analyze registry hive files (e.g., `SYSTEM`, `NTUSER.DAT`) from a forensic copy using specialized tools.
- **Structure**:
  - **Keys**: Folders containing subkeys or values (e.g., `HKEY_LOCAL_MACHINE\Software`).
  - **Values**: Data entries, such as strings (`REG_SZ`), numbers (`REG_DWORD`), or binary data (`REG_BINARY`).
  - **Hives**: Groups of keys, subkeys, and values stored in files on disk (e.g., `C:\Windows\System32\Config\SYSTEM`).

## Registry Hives: 

Hives are the top-level structures in the registry, each stored as a file on disk. Below, I’ve detailed the primary hives, what they contain, their forensic significance, and specific examples, with a deep focus on SYSTEM, SOFTWARE, SAM, and SECURITY.

### 1. HKEY_LOCAL_MACHINE (HKLM)

- **Full Name**: Handle to Key Local Machine
- **Purpose**: The central hub for system-wide configuration data, applicable to all users. It stores settings for hardware, operating system components, installed software, and security policies.
- **File Location**: `C:\Windows\System32\Config\`
  - **SYSTEM**: Mounted as `HKLM\System`. Contains hardware and system startup configurations.
  - **SOFTWARE**: Mounted as `HKLM\Software`. Stores software settings and file associations.
  - **SAM**: Mounted as `HKLM\SAM`. Holds user account and group information.
  - **SECURITY**: Mounted as `HKLM\Security`. Stores security policies and access control data.
- **Forensic Value**: Critical for identifying system configurations, installed software, connected devices, and malware persistence mechanisms (e.g., services or startup programs).
- **Detailed Contents**:
  - **SYSTEM**:
    - **What It Contains**: Hardware configurations, driver settings, system services, and control sets (e.g., `CurrentControlSet` for current boot configuration). Includes details like USB devices, network interfaces, and time zone settings.
    - **Forensic Use**: Tracks devices connected (e.g., USB drives), system boot settings, and service configurations that malware might exploit.
    - **Example**: `HKLM\SYSTEM\CurrentControlSet\Enum\USBSTOR` lists all USB storage devices with vendor IDs, serial numbers, and connection timestamps (e.g., `Ven_SanDisk&Prod_Cruzer\1234567890`).
    - **Subkeys**:
      - `ControlSet001`, `ControlSet002`: Current and last known good boot configurations.
      - `Enum`: Hardware device details (e.g., USB, network adapters).
      - `Services`: System services, including those set to start at boot.
  - **SOFTWARE**:
    - **What It Contains**: Settings for installed applications, file associations (`HKLM\Software\Classes`), and system-wide software policies. Includes details like application versions and uninstall information.
    - **Forensic Use**: Identifies installed programs, their configurations, and potential malware modifying file associations (e.g., hijacking `.exe` to run malicious code).
    - **Example**: `HKLM\Software\Microsoft\Windows\CurrentVersion\Uninstall` lists installed programs with details like `DisplayName = Notepad++`, `InstallDate = 20250701`.
    - **Subkeys**:
      - `Classes`: Defines file associations (e.g., `.pdf` opens with Adobe Acrobat).
      - `Microsoft\Windows\CurrentVersion\Run`: Programs that run at system startup, a common malware persistence point.
  - **SAM**:
    - **What It Contains**: User account details, including usernames, SIDs, password hashes (LM/NTLM), and group memberships.
    - **Forensic Use**: Reveals user accounts, their permissions, and potential unauthorized accounts created by malware. Password hashes can be extracted for cracking (with legal permission).
    - **Example**: `HKLM\SAM\Domains\Account\Users\000003E8` shows a user with SID ending in `1000` (e.g., `S-1-5-21-...-1000`), indicating a user-created account named “JohnDoe.”
    - **Subkeys**:
      - `Domains\Account\Users`: User account details, including SIDs and hashed passwords.
      - `Domains\Group`: Group memberships (e.g., Administrators, Users).
  - **SECURITY**:
    - **What It Contains**: Security policies, user rights assignments, and audit settings. Includes data like logon policies and privilege assignments.
    - **Forensic Use**: Detects changes to security settings (e.g., disabled auditing by malware) or unauthorized privilege escalations.
    - **Example**: `HKLM\SECURITY\Policy\Accounts` lists SIDs with assigned privileges, such as `SeDebugPrivilege` granted to an unexpected account.
    - **Subkeys**:
      - `Policy`: System-wide security policies (e.g., password complexity).
      - `Cache`: Cached logon credentials for domain users.
- **Learning Tip**: SAM and SECURITY require SYSTEM privileges to access in `regedit.exe`. Use forensic tools like Registry Explorer to bypass restrictions on disk images.

### 2. HKEY_USERS (HKU)

- **Full Name**: Handle to Key Users
- **Purpose**: Stores configuration data for all loaded user profiles, including the default user profile used when no user is logged in.
- **File Location**:
  - `C:\Windows\System32\Config\DEFAULT` (mounted as `HKU\.DEFAULT`).
  - `C:\Users\<username>\NTUSER.DAT` (mounted as `HKU\<SID>` for each user).
- **Forensic Value**: Captures user-specific settings across all accounts, useful for analyzing activity from multiple users or detecting rogue accounts.
- **Detailed Contents**:
  - **.DEFAULT**: Default user profile settings, applied to new accounts or when no user is logged in.
  - : User-specific settings for each loaded user profile, mirroring `HKEY_CURRENT_USER` for the active user.
- **Example**: `HKU\S-1-5-21-...-1001\Software\Microsoft\Windows` shows settings for a user with SID ending in `1001`, including recent files and application preferences.
- **Learning Tip**: SIDs ending in `1000+` (e.g., `S-1-5-21-...-1001`) indicate user-created accounts, while `S-1-5-18` (System) or `S-1-5-19` (Local Service) are built-in. Use `HKLM\SAM\Domains\Account\Users` to map SIDs to usernames.

### 3. HKEY_CURRENT_USER (HKCU)

- **Full Name**: Handle to Key Current User
- **Purpose**: Contains settings for the currently logged-in user, such as desktop preferences, application settings, and recent activity. It’s a subkey of `HKEY_USERS` mapped to the user’s SID.
- **File Location**: `C:\Users\<username>\NTUSER.DAT` (loaded into `HKCU` at login).
- **Forensic Value**: Tracks user-specific actions, like opened files, typed URLs, and application usage, ideal for reconstructing user behavior.
- **Detailed Contents**:
  - Application settings (e.g., `Software\Microsoft\Office`).
  - User preferences (e.g., desktop wallpaper, Control Panel settings).
  - Recent activity (e.g., `RecentDocs`, `TypedPaths`).
- **Example**: `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\RecentDocs\.pdf` lists the last opened PDF file, like `C:\Users\John\Documents\report.pdf`.

### 4. HKEY_CLASSES_ROOT (HKCR)

- **Full Name**: Handle to Key Classes Root
- **Purpose**: Merges `HKLM\Software\Classes` and `HKCU\Software\Classes` to define file associations and COM object settings.
- **File Location**: No separate file; derived from `HKLM\Software\Classes` (system-wide) and `HKCU\Software\Classes` (`C:\Users\<username>\AppData\Local\Microsoft\Windows\USRCLASS.DAT`).
- **Forensic Value**: Reveals file associations and potential malware tampering (e.g., changing `.exe` to launch a malicious program).
- **Detailed Contents**:
  - File extensions (e.g., `.txt`, `.pdf`) and their associated programs.
  - COM object registrations for application interactions.
- **Example**: `HKCR\.docx` shows Microsoft Word as the default program for `.docx` files.
- **Learning Tip**: Malware may modify `HKCR` to redirect file openings (e.g., `.jpg` to a malicious executable). Check `HKCU\Software\Classes` for user-specific overrides.

### 5. HKEY_CURRENT_CONFIG (HKCC)

- **Full Name**: Handle to Key Current Configuration
- **Purpose**: Stores the current hardware profile used at system startup, such as display and device settings. It’s a volatile hive linked to `HKLM\SYSTEM\CurrentControlSet\Hardware Profiles\Current`.
- **File Location**: No separate file; derived from `HKLM\SYSTEM`.
- **Forensic Value**: Verifies hardware configuration at the time of analysis, useful for detecting changes in device settings.
- **Example**: `HKCC\System\CurrentControlSet\Control\Video` shows the current display resolution.

### 6. AmCache Hive

- **Full Name**: Application Compatibility Cache Hive

**Purpose**: Tracks recently executed programs, storing execution paths, timestamps, and SHA1 hashes for compatibility purposes.

- **File Location**: `C:\Windows\AppCompat\Programs\Amcache.hve`
- **Forensic Value**: Identifies executed programs, including malware, with details like first execution, last execution, and deletion times.
- **Detailed Contents**:
  - Program execution details (e.g., path, SHA1 hash, execution time).
  - File metadata (e.g., installation time, file size).
- **Example**: `Amcache.hve\Root\File\{Volume GUID}\00000001` shows `C:\Program Files\malware.exe` executed on 2025-07-10 with SHA1 hash `a1b2c3...`.

### 7. USRCLASS.DAT

- **Full Name**: User Classes
- **Purpose**: Stores user-specific file associations and shell settings, including folder view preferences.
- **File Location**: `C:\Users\<username>\AppData\Local\Microsoft\Windows\USRCLASS.DAT`
- **Forensic Value**: Contains ShellBags, which track folder access and view settings, crucial for user activity analysis.
- **Detailed Contents**:
  - ShellBags: Folder navigation history and view preferences.
  - User-specific file associations.
- **Example**: `USRCLASS.DAT\Local Settings\Software\Microsoft\Windows\Shell\BagMRU` shows `C:\Users\John\SecretFolder` was accessed recently.

## Transaction Logs and Backups

- **Transaction Logs**:
  - **Purpose**: Act as a changelog, recording uncommitted changes to registry hives.
  - **Location**: Same directory as the hive (e.g., `C:\Windows\System32\Config\SYSTEM.LOG`, `SYSTEM.LOG1`).
  - **Forensic Value**: Captures recent modifications (e.g., new user accounts, changed startup keys) not yet in the main hive.
  - **Example**: `SAM.LOG` might show a new user added minutes before a system crash.
- **Backups**:
  - **Purpose**: Periodic snapshots of registry hives, created every 10 days.
  - **Location**: `C:\Windows\System32\Config\RegBack`
  - **Forensic Value**: Recovers deleted or modified keys, useful for detecting tampering.
  - **Example**: A deleted `Run` key in `HKLM\Software` might persist in `RegBack\SOFTWARE`.

**Learning Tip**: Always analyze transaction logs with tools like Registry Explorer to capture the latest changes. Backups in `RegBack` are a fallback for recovering older configurations.

## Forensically Important Registry Keys

Below are the most critical registry keys for malware and digital forensics, with detailed explanations, examples, and why they’re useful.

### 1. System Information

- **Purpose**: Establishes system context for accurate analysis.
- **Key Locations**:
  - **OS Version**: `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion`
    - **Details**: Contains `ProductName`, `CurrentVersion`, `BuildLab`, and `InstallDate`. Identifies the OS and patch level.
    - **Example**: `ProductName = Windows 11 Enterprise`, `CurrentVersion = 10.0.22631`, `InstallDate = 20250115`.
    - **Why**: Confirms the system’s OS version, critical for tool compatibility and timeline analysis.
  - **Computer Name**: `HKLM\SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName`
    - **Details**: Stores the machine’s hostname.
    - **Example**: `ComputerName = WORKSTATION-XYZ`.
    - **Why**: Verifies you’re analyzing the correct system.
  - **Control Sets**: `HKLM\SYSTEM\CurrentControlSet`, `ControlSet001`, `ControlSet002`
    - **Details**: `CurrentControlSet` reflects the current boot configuration; `ControlSet001` is typically the active set, `ControlSet002` is the last known good configuration. Check `HKLM\SYSTEM\Select\Current` to confirm the active set.
    - **Example**: `HKLM\SYSTEM\Select\Current = 1` indicates `ControlSet001` is active.
    - **Why**: Ensures you’re analyzing the current system state, as malware may modify older control sets.
  - **Time Zone Information**: `HKLM\SYSTEM\CurrentControlSet\Control\TimeZoneInformation`
    - **Details**: Stores time zone settings, including `TimeZoneKeyName`, `Bias`, and daylight saving details.
    - **Example**: `TimeZoneKeyName = Pacific Standard Time`, `Bias = 480` (minutes from UTC).
    - **Why**: Aligns event timestamps (some in UTC, others in local time) for accurate timeline reconstruction.

### 2. Startup Programs and Services

- **Purpose**: Identifies programs and services that run automatically, a common malware persistence mechanism.
- **Key Locations**:
  - `HKLM\Software\Microsoft\Windows\CurrentVersion\Run`
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\Run`
    - **Details**: Lists programs that run at user login or system startup.
    - **Example**: `HKLM\Software\Microsoft\Windows\CurrentVersion\Run\Malware = C:\Program Files\suspicious.exe` indicates a potential malicious program.
    - **Why**: Malware often adds itself here to persist across reboots.
  - `HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce`
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\RunOnce`
    - **Details**: Programs that run once and are then removed from the registry.
    - **Example**: `RunOnce\Install = C:\Temp\setup.exe` shows a one-time installer.
    - **Why**: Malware may use `RunOnce` for initial infection.
  - `HKLM\SYSTEM\CurrentControlSet\Services`
    - **Details**: Lists system services, with `Start` values indicating boot behavior (`0x02` = auto-start).
    - **Example**: `ServiceName\Start = 0x02`, `ImagePath = C:\Windows\malware_svc.exe` suggests a malicious service.
    - **Why**: Malware often disguises itself as a service to run continuously.

### 3. User Accounts and Logins

- **Location**: `HKLM\SAM\Domains\Account\Users`
- **Details**: Stores user account details, including usernames, SIDs, and password hashes (LM/NTLM).
- **Example**: `Users\000003E8\Name = JohnDoe`, with a hashed password in `V` value.
- **Why**: Detects unauthorized accounts or password changes by malware.
- **Learning Tip**: Non-system accounts have SIDs ending in `1000+` (e.g., `S-1-5-21-...-1001`). Use tools like `samparse` from Eric Zimmerman to extract and analyze SAM data.

### 4. USB and Removable Devices

- **Locations**:
  - `HKLM\SYSTEM\CurrentControlSet\Enum\USBSTOR`
    - **Details**: Lists USB storage devices with vendor IDs, product IDs, serial numbers, and connection timestamps.
    - **Subkeys**:
      - `Ven_Prod_Version\USBSerial#\Properties\{83da6326-97a6-4088-9453-a19231573b29}\####`
        - `0064`: First connection time
        - `0066`: Last connection time
        - `0067`: Last removal time
    - **Example**: `Ven_SanDisk&Prod_Cruzer\1234567890\Properties\...\0066` shows a USB last connected on 2025-07-10.
    - **Why**: Tracks external devices, potentially linked to data exfiltration or malware delivery.
  - `HKLM\SYSTEM\MountedDevices`
    - **Details**: Maps drive letters to volume GUIDs for all mounted devices.
    - **Example**: `\DosDevices\E:` links to a USB’s volume GUID.
    - **Why**: Correlates USB connections with drive letters.
  - `HKLM\SOFTWARE\Microsoft\Windows Portable Devices\Devices`
    - **Details**: Provides friendly names for USB devices, linked to GUIDs.
    - **Example**: `FriendlyName = SanDisk Cruzer 16GB` matches a `USBSTOR` GUID.
    - **Why**: Helps identify specific devices used on the system.

### 5. Network Configuration and History

- **Locations**:
  - `HKLM\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\Interfaces\{GUID}`
    - **Details**: Stores network adapter settings (e.g., IP address, DNS servers, DHCP settings).
    - **Example**: `IPAddress = 192.168.1.100`, `NameServer = 8.8.8.8`.
    - **Why**: Verifies network configuration and potential unauthorized changes.
  - `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkList\Signatures\Managed`
  - `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkList\Signatures\Unmanaged`
    - **Details**: Lists past network connections (e.g., Wi-Fi SSIDs) with last connection times.
    - **Example**: `ProfileName = CoffeeShopWiFi`, last write time = 2025-07-01.
    - **Why**: Tracks system movement and network activity, useful for geolocation or breach analysis.
  - `HKLM\SOFTWARE\Microsoft\WZCSVC\Parameters\Interfaces\{GUID}`
    - **Details**: Stores wireless network SSIDs and connection details in binary format.
    - **Example**: `Static#0000` contains `HomeWiFi` as the SSID.
    - **Why**: Identifies networks the system connected to, aiding in tracking physical locations.

### 6. ShellBags

- **Locations**:
  - `USRCLASS.DAT\Local Settings\Software\Microsoft\Windows\Shell\Bags`
  - `USRCLASS.DAT\Local Settings\Software\Microsoft\Windows\Shell\BagMRU`
  - `NTUSER.DAT\Software\Microsoft\Windows\Shell\BagMRU`
  - `NTUSER.DAT\Software\Microsoft\Windows\Shell\Bags`
- **Purpose**: Stores folder view preferences (e.g., icon size, column widths) and access history for folders opened in Windows Explorer.
- **Detailed Contents**:
  - **BagMRU**: Tracks the hierarchy of accessed folders, with each subkey representing a folder path.
  - **Bags**: Stores view settings for each folder, including timestamps and folder types (e.g., Documents, Pictures).
  - **Data Format**: Binary data with folder names, GUIDs, and timestamps (last write time indicates access).
- **Forensic Value**: Reveals folders a user accessed, even if deleted, helping reconstruct activity (e.g., accessing a hidden folder with stolen data).
- **Example**: `BagMRU\0\1` shows `C:\Users\John\SecretFolder` was opened on 2025-07-10, with view settings in `Bags\123`.
- **Why**: ShellBags persist even after folder deletion, making them critical for recovering evidence of file access. Malware may access specific folders (e.g., for data theft), leaving traces here.
- **Learning Tip**: Use ShellBags Explorer to parse these keys, as binary data is hard to interpret manually. Timestamps in `BagMRU` show when folders were last accessed.

### 7. Recent Documents and Files

- **Locations**:
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\RecentDocs`
    - **Details**: Lists recently opened files, organized by extension (e.g., `.pdf`, `.docx`), with full paths and MRU (Most Recently Used) order.
    - **Example**: `RecentDocs\.pdf\0 = C:\Users\John\report.pdf` indicates the last opened PDF.
    - **Why**: Shows user file access, potentially revealing evidence or malicious files.
  - `HKCU\Software\Microsoft\Office\VERSION\UserMRU\LiveID_####\FileMRU`
    - **Details**: Tracks recent files opened in Microsoft Office (e.g., Word, Excel). Version numbers vary (e.g., `16.0` for Office 2016, `15.0` for 2013).
    - **Example**: `NTUSER.DAT\Software\Microsoft\Office\16.0\Word\UserMRU\LiveID_1234\FileMRU\Item 1 = C:\Users\John\doc.docx`.
    - **Why**: Identifies Office document usage, useful for tracking sensitive file access.
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\ComDlg32\OpenSavePidlMRU`
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\ComDlg32\LastVisitedPidlMRU`
    - **Details**: Records file paths from Open/Save dialog boxes, showing recent file interactions.
    - **Example**: `OpenSavePidlMRU\.txt\0 = C:\Users\John\notes.txt`.
    - **Why**: Reveals files opened or saved, even if not in `RecentDocs`.

### 8. Typed Paths and Search Queries

- **Locations**:
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\TypedPaths`
    - **Details**: Records paths manually typed into the Windows Explorer address bar.
    - **Example**: `url1 = C:\Users\John\Downloads\Secret`.
    - **Why**: Shows user navigation intent, potentially to sensitive or malicious locations.
  - `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\WordWheelQuery`
    - **Details**: Stores search terms entered in Windows Explorer.
    - **Example**: `MRUListEx\0 = confidential` indicates a search for “confidential.”
    - **Why**: Reveals what users were looking for, possibly linked to data theft.

### 9. UserAssist

- **Location**: `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\UserAssist\{GUID}\Count`
- **Details**: Tracks applications launched via Windows Explorer, with execution counts and timestamps. Entries are encoded in ROT-13 for obfuscation.
- **Example**: `{CEBFF5CD-ACE2-4F4F-9178-9926F41749EA}\Count\notepad.exe` shows Notepad launched 15 times, last on 2025-07-01.
- **Why**: Identifies frequently used programs, excluding command-line executions, useful for detecting unauthorized software.

### 10. ShimCache (AppCompatCache)

- **Location**: `HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\AppCompatCache`
- **Details**: Tracks all executed applications for compatibility, storing file names, sizes, and last modified times.
- **Example**: Lists `C:\Program Files\suspicious.exe` with a last modified time of 2025-07-10.
- **Why**: Detects executed programs, even if deleted, critical for malware investigations.

### 11. Background Activity Monitor (BAM) and Desktop Activity Moderator (DAM)

- **Locations**:
  - `HKLM\SYSTEM\CurrentControlSet\Services\bam\UserSettings\{SID}`
  - `HKLM\SYSTEM\CurrentControlSet\Services\dam\UserSettings\{SID}`
- **Details**: Tracks background application activity, including full paths and last execution times, for power optimization (Modern Standby).
- **Example**: `bam\UserSettings\S-1-5-21-...-1001\firefox.exe` shows Firefox ran on 2025-07-10.
- **Why**: Provides additional evidence of program execution, complementing AmCache and ShimCache.

## Tools for Registry Forensics

Eric Zimmerman’s tools are highly recommended for their ability to parse complex registry data. Below are key tools with their purposes and examples.

### 1. Registry Explorer

- **Purpose**: A user-friendly interface for viewing and analyzing registry hives, supporting transaction logs and multiple hives.
- **Features**:
  - Merges transaction logs for the latest data.
  - Bookmarks for quick access to forensic keys (e.g., `USBSTOR`, `Run`).
  - Timeline view for key modifications.
- **Example**: Load `SYSTEM` and `SAM`, navigate to `HKLM\SAM\Domains\Account\Users` to list user accounts.
- **Why**: Simplifies navigation and highlights critical artifacts.

### 2. ShellBags Explorer

- **Purpose**: Parses ShellBags data from `NTUSER.DAT` and `USRCLASS.DAT`, showing folder access history.
- **Example**: Displays `C:\Users\John\SecretFolder` accessed on 2025-07-10, with view settings (e.g., list view).
- **Why**: Critical for reconstructing folder navigation, especially for deleted folders.

### 3. AppCompatCacheParser

- **Purpose**: Extracts ShimCache data, outputting execution details in CSV format.
- **Command**: `AppCompatCacheParser.exe --csv C:\Output -f C:\Windows\System32\Config\SYSTEM -c 1`
- **Example**: Lists `malware.exe` with execution time and path.
- **Why**: Identifies executed programs, even if removed from the system.

### 4. AmcacheParser

- **Purpose**: Parses `Amcache.hve`, extracting program execution details (e.g., paths, SHA1 hashes).
- **Example**: Outputs `C:\Program Files\app.exe` with first execution on 2025-07-01.
- **Why**: Complements ShimCache for comprehensive execution history.

### 5. RegRipper

- **Purpose**: Generates text reports for specific registry keys (e.g., `Run`, `UserAssist`).
- **Example**: `rip.exe -r NTUSER.DAT -p userassist` lists executed programs with timestamps.
- **Why**: Automates artifact extraction, but lacks transaction log support.

### 6. Other Tools

- **FTK Imager**: Acquires registry hives from disk images.
- **Cellebrite BlackLight**: Parses registry artifacts automatically.
- **RECmd**: Eric Zimmerman’s command-line tool for batch parsing registry data.

**Learning Tip**: Start with Registry Explorer for its versatility, then use specialized tools like ShellBags Explorer for specific artifacts. Download from https://github.com/EricZimmerman.


## Resources

- **Eric Zimmerman’s Tools**: https://github.com/EricZimmerman
- **Microsoft Registry Documentation**: https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry
- **RegRipper**: https://github.com/keydet89/RegRipper3.0
- **SANS Windows Registry Forensics Cheat Sheet**: https://www.sans.org/posters/windows-forensic-analysis/
