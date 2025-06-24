
# Investigating Malicious Activity with Sysmon Logs

Sysmon, a tool used to monitor and log events on Windows, is commonly used by enterprises as part of their monitoring and logging solutions. Part of the Windows Sysinternals package, Sysmon is similar to Windows Event Logs with further detail and granular control. From the Microsoft Docs, "System Monitor (Sysmon) is a Windows system service and device driver that, once installed on a system, remains resident across system reboots to monitor and log system activity to the Windows event log. It provides detailed information about process creations, network connections, and changes to file creation time. By collecting the events it generates using Windows Event Collection or SIEM agents and subsequently analyzing them, you can identify malicious or anomalous activity and understand how intruders and malware operate on your network."

Sysmon gathers detailed and high-quality logs as well as event tracing that assists in identifying anomalies in your environment. Sysmon is most commonly used in conjunction with security information and event management (SIEM) system or other log parsing solutions that aggregate, filter, and visualize events. When installed on an endpoint, Sysmon will start early in the Windows boot process. In an ideal scenario, the events would be forwarded to a SIEM for further analysis. However, in this room, we will focus on Sysmon itself and view the events on the endpoint itself with Windows Event Viewer.

Events within Sysmon are stored in Applications and Services Logs/Microsoft/Windows/Sysmon/Operational

## Sysmon Config Overview

Sysmon requires a config file in order to tell the binary how to analyze the events that it is receiving. You can create your own Sysmon config or you can download a config. Here is an example of a high-quality config that works well for identifying anomalies created by SwiftOnSecurity: https://github.com/SwiftOnSecurity/sysmon-config Sysmon includes 29 different types of Event IDs, all of which can be used within the config to specify how the events should be handled and analyzed. Below we will go over a few of the most important Event IDs and show examples of how they are used within config files.

When creating or modifying configuration files you will notice that a majority of rules in sysmon-config will exclude events rather than include events. This will help filter out normal activity in your environment that will in turn decrease the number of events and alerts you will have to manually audit or search through in a SIEM. On the other hand, there are rulesets like the ION-Storm sysmon-config fork that takes a more proactive approach with its ruleset by using a lot of include rules. You may have to modify configuration files to find what approach you prefer. Configuration preferences will vary depending on what SOC team so prepare to be flexible when monitoring.

**Note**: As there are so many Event IDs Sysmon analyzes, we will only be going over a few of the ones that we think are most important to understand.

### Event ID 1: Process Creation

This event will look for any processes that have been created. You can use this to look for known suspicious processes or processes with typos that would be considered an anomaly. This event will use the CommandLine and Image XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<ProcessCreate onmatch="exclude">
	 	<CommandLine condition="is">C:\Windows\system32\svchost.exe -k appmodel -p -s camsvc</CommandLine>
	</ProcessCreate>
</RuleGroup>
```

The above code snippet is specifying the Event ID to pull from as well as what condition to look for. In this case, it is excluding the svchost.exe process from the event logs.

### Event ID 3: Network Connection

The network connection event will look for events that occur remotely. This will include files and sources of suspicious binaries as well as opened ports. This event will use the Image and DestinationPort XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<NetworkConnect onmatch="include">
	 	<Image condition="image">nmap.exe</Image>
	 	<DestinationPort name="Alert,Metasploit" condition="is">4444</DestinationPort>
	</NetworkConnect>
</RuleGroup>
```

The above code snippet includes two ways to identify suspicious network connection activity. The first way will identify files transmitted over open ports. In this case, we are specifically looking for nmap.exe which will then be reflected within the event logs. The second method identifies open ports and specifically port 4444 which is commonly used with Metasploit. If the condition is met an event will be created and ideally trigger an alert for the SOC to further investigate.

### Event ID 7: Image Loaded

This event will look for DLLs loaded by processes, which is useful when hunting for DLL Injection and DLL Hijacking attacks. It is recommended to exercise caution when using this Event ID as it causes a high system load. This event will use the Image, Signed, ImageLoaded, and Signature XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<ImageLoad onmatch="include">
	 	<ImageLoaded condition="contains">\Temp\</ImageLoaded>
	</ImageLoad>
</RuleGroup>
```

The above code snippet will look for any DLLs that have been loaded within the \Temp\ directory. If a DLL is loaded within this directory it can be considered an anomaly and should be further investigated.

### Event ID 8: CreateRemoteThread

The CreateRemoteThread Event ID will monitor for processes injecting code into other processes. The CreateRemoteThread function is used for legitimate tasks and applications. However, it could be used by malware to hide malicious activity. This event will use the SourceImage, TargetImage, StartAddress, and StartFunction XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<CreateRemoteThread onmatch="include">
	 	<StartAddress name="Alert,Cobalt Strike" condition="end with">0B80</StartAddress>
	 	<SourceImage condition="contains">\</SourceImage>
	</CreateRemoteThread>
</RuleGroup>
```

The above code snippet shows two ways of monitoring for CreateRemoteThread. The first method will look at the memory address for a specific ending condition which could be an indicator of a Cobalt Strike beacon. The second method will look for injected processes that do not have a parent process. This should be considered an anomaly and require further investigation.

### Event ID 11: File Created

This event ID will log events when files are created or overwritten on the endpoint. This could be used to identify file names and signatures of files that are written to disk. This event uses TargetFilename XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<FileCreate onmatch="include">
	 	<TargetFilename name="Alert,Ransomware" condition="contains">HELP_TO_SAVE_FILES</TargetFilename>
	</FileCreate>
</RuleGroup>
```

The above code snippet is an example of a ransomware event monitor. This is just one example of a variety of different ways you can utilize Event ID 11.

### Event ID 12 / 13 / 14: Registry Event

This event looks for changes or modifications to the registry. Malicious activity from the registry can include persistence and credential abuse. This event uses TargetObject XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<RegistryEvent onmatch="include">
	 	<TargetObject name="T1484" condition="contains">Windows\System\Scripts</TargetObject>
	</RegistryEvent>
</RuleGroup>
```

The above code snippet will look for registry objects that are in the "Windows\System\Scripts" directory as this is a common directory for adversaries to place scripts to establish persistence.

### Event ID 15: FileCreateStreamHash

This event will look for any files created in an alternate data stream. This is a common technique used by adversaries to hide malware. This event uses TargetFilename XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<FileCreateStreamHash onmatch="include">
	 	<TargetFilename condition="end with">.hta</TargetFilename>
	</FileCreateStreamHash>
</RuleGroup>
```

The above code snippet will look for files with the .hta extension that have been placed within an alternate data stream.

### Event ID 22: DNS Event

This event will log all DNS queries and events for analysis. The most common way to deal with these events is to exclude all trusted domains that you know will be very common "noise" in your environment. Once you get rid of the noise you can then look for DNS anomalies. This event uses QueryName XML tags.

```xml
<RuleGroup name="" groupRelation="or">
	<DnsQuery onmatch="exclude">
	 	<QueryName condition="end with">.microsoft.com</QueryName>
	</DnsQuery>
</RuleGroup>
```

The above code snippet will exclude any DNS events with the .microsoft.com query. This will get rid of the noise that you see within the environment.

## Installing Sysmon

The installation for Sysmon is fairly straightforward and only requires downloading the binary from the Microsoft website. You can also download all of the Sysinternals tools with a PowerShell command if you wanted to rather than grabbing a single binary. It is also recommended to use a Sysmon config file along with Sysmon to get more detailed and high-quality event tracing. As an example config file we will be using the sysmon-config file from the SwiftOnSecurity GitHub repo.

You can find the Sysmon binary from the [Microsoft Sysinternals website](https://docs.microsoft.com/en-us/sysinternals/downloads/sysmon). You can also download the Microsoft Sysinternal Suite or use the below command to run a PowerShell module download and install all of the Sysinternals tools.

**PowerShell command**: 
```powershell
Download-SysInternalsTools C:\Sysinternals
```

To fully utilize Sysmon you will also need to download a Sysmon config or create your own config. We suggest downloading the SwiftOnSecurity sysmon-config. A Sysmon config will allow for further granular control over the logs as well as more detailed event tracing. In this room, we will be using both the SwiftOnSecurity configuration file as well as the ION-Storm config file.

## Starting Sysmon

To start Sysmon you will want to open a new PowerShell or Command Prompt as an Administrator. Then, run the below command it will execute the Sysmon binary, accept the end-user license agreement, and use SwiftOnSecurity config file.

**Command Used**: 
```powershell
Sysmon.exe -accepteula -i ..\Configurations\swift.xml
```

**Sysmon Installation**:
```
C:\Users\THM-Analyst\Desktop\Tools\Sysmon>Sysmon.exe -accepteula -i ..\Configurations\swift.xml

System Monitor v12.03 - System activity monitor
Copyright (C) 2014-2020 Mark Russinovich and Thomas Garnier
Sysinternals - www.sysinternals.com

Loading configuration file with schema version 4.10
Sysmon schema version: 4.40
Configuration file validated.
Sysmon installed.
SysmonDrv installed.
Starting SysmonDrv.
SysmonDrv started.
Starting Sysmon..
```

Now that Sysmon is started with the configuration file we want to use, we can look at the Event Viewer to monitor events. The event log is located under Applications and Services Logs/Microsoft/Windows/Sysmon/Operational

**Note**: At any time you can change the configuration file used by uninstalling or updating the current configuration and replacing it with a new configuration file. For more information look through the Sysmon help menu.

## Malicious Activity Overview

Since most of the normal activity or "noise" seen on a network is excluded or filtered out with Sysmon we're able to focus on meaningful events. This allows us to quickly identify and investigate suspicious activity. When actively monitoring a network you will want to use multiple detections and techniques simultaneously in an effort to identify threats. For this room, we will only be looking at what suspicious logs will look like with both Sysmon configs and how to optimize your hunt using only Sysmon. We will be looking at how to detect ransomware, persistence, Mimikatz, Metasploit, and Command and Control (C2) beacons. Obviously, this is only showcasing a small handful of events that could be triggered in an environment. The methodology will largely be the same for other threats. It really comes down to using an ample and efficient configuration file as it can do a lot of the heavy lifting for you.

You can either download the event logs used for this task or you can open them from the Practice directory on the provided machine.

## Sysmon "Best Practices"

Sysmon offers a fairly open and configurable platform for you to use. Generally speaking, there are a few best practices that you could implement to ensure you're operating efficiently and not missing any potential threats. A few common best practices are outlined and explained below.

- **Exclude > Include**: When creating rules for your Sysmon configuration file it is typically best to prioritize excluding events rather than including events. This prevents you from accidentally missing crucial events and only seeing the events that matter the most.
- **CLI gives you further control**: As is common with most applications the CLI gives you the most control and filtering allowing for further granular control. You can use either Get-WinEvent or wevutil.exe to access and filter logs. As you incorporate Sysmon into your SIEM or other detection solutions these tools will become less used and needed.
- **Know your environment before implementation**: Knowing your environment is important when implementing any platform or tool. You should have a firm understanding of the network or environment you are working within to fully understand what is normal and what is suspicious in order to effectively craft your rules.

## Filtering Events with Event Viewer

Event Viewer might not be the best for filtering events and out-of-the-box offers limited control over logs. The main filter you will be using with Event Viewer is by filtering the EventID and keywords. You can also choose to filter by writing XML but this is a tedious process that doesn't scale well.

## Filtering Events with PowerShell

To view and filter events with PowerShell we will be using Get-WinEvent along with XPath queries. We can use any XPath queries that can be found in the XML view of events. We will be using wevutil.exe to view events once filtered. The command line is typically used over the Event Viewer GUI as it allows for further granular control and filtering whereas the GUI does not.

- **Filter by Event ID**: `*/System/EventID=<ID>`
- **Filter by XML Attribute/Name**: `*/EventData/Data[@Name="<XML Attribute/Name>"]`
- **Filter by Event Data**: `*/EventData/Data=<Data>`

We can put these filters together with various attributes and data to get the most control out of our logs. Look below for an example of using Get-WinEvent to look for network connections coming from port 4444.

```powershell
Get-WinEvent -Path <Path to Log> -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"] and */EventData/Data=4444'
```

**Filtering Events**:
```powershell
PS C:\Users\THM-Analyst> Get-WinEvent -Path C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Hunting_Metasploit.evtx -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"] and */EventData/Data=4444'

   ProviderName: Microsoft-Windows-Sysmon

TimeCreated                     Id LevelDisplayName Message
-----------                     -- ---------------- -------
1/5/2021 2:21:32 AM              3 Information      Network connection detected:...
```

## Hunting Metasploit

Metasploit is a commonly used exploit framework for penetration testing and red team operations. Metasploit can be used to easily run exploits on a machine and connect back to a meterpreter shell. We will be hunting the meterpreter shell itself and the functionality it uses. To begin hunting we will look for network connections that originate from suspicious ports such as 4444 and 5555. By default, Metasploit uses port 4444. If there is a connection to any IP known or unknown it should be investigated. To start an investigation you can look at packet captures from the date of the log to begin looking for further information about the adversary. We can also look for suspicious processes created. This method of hunting can be applied to other various RATs and C2 beacons.

For more information about this technique and tools used check out [MITRE ATT&CK Software](https://attack.mitre.org/software/).

For more information about how malware and payloads interact with the network check out the [Malware Common Ports Spreadsheet](https://docs.google.com/spreadsheets/d/1M56u3n3v7cQ3Y2L8T9QJwe9g2-nrUwUzqY_3zU9nBPI/).

### Hunting Network Connections

We will first be looking at a modified Ion-Security configuration to detect the creation of new network connections. The code snippet below will use event ID 3 along with the destination port to identify active connections specifically connections on port 4444 and 5555.

```xml
<RuleGroup name="" groupRelation="or">
	<NetworkConnect onmatch="include">
		<DestinationPort condition="is">4444</DestinationPort>
		<DestinationPort condition="is">5555</DestinationPort>
	</NetworkConnect>
</RuleGroup>
```

Open `C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Hunting_Metasploit.evtx` in Event Viewer to view a basic Metasploit payload being dropped onto the machine.

Once we identify the event it can give us some important information we can use for further investigation like the ProcessID and Image.

### Hunting for Open Ports with PowerShell

To hunt for open ports with PowerShell we will be using the PowerShell module Get-WinEvent along with XPath queries. We can use the same XPath queries that we used in the rule to filter out events from NetworkConnect with DestinationPort. The command line is typically used over the Event Viewer GUI because it can allow for further granular control and filtering that the GUI does not offer. For more information about using XPath and the command line for event viewing, check out the Windows Event Log room by Heavenraiza.

```powershell
Get-WinEvent -Path <Path to Log> -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"] and */EventData/Data=4444'
```

**Hunting Metasploit**:
```powershell
PS C:\Users\THM-Analyst> Get-WinEvent -Path C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Hunting_Metasploit.evtx -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"] and */EventData/Data=4444'

   ProviderName: Microsoft-Windows-Sysmon

TimeCreated                     Id LevelDisplayName Message
-----------                     -- ---------------- -------
1/5/2021 2:21:32 AM              3 Information      Network connection detected:...
```

We can break this command down by its filters to see exactly what it is doing. It is first filtering by Event ID 3 which is the network connection ID. It is then filtering by the data name in this case DestinationPort as well as the specific port that we want to filter. We can adjust this syntax along with our events to get exactly what data we want in return.

## Hunting Malware Overview

Malware has many forms and variations with different end goals. The two types of malware that we will be focusing on are RATs and backdoors. RATs or Remote Access Trojans are used similar to any other payload to gain remote access to a machine. RATs typically come with other Anti-Virus and detection evasion techniques that make them different than other payloads like MSFVenom. A RAT typically also uses a Client-Server model and comes with an interface for easy user administration. Examples of RATs are Xeexe and Quasar. To help detect and hunt malware we will need to first identify the malware that we want to hunt or detect and identify ways that we can modify configuration files, this is known as hypothesis-based hunting. There are of course a plethora of other ways to detect and log malware however we will only be covering the basic way of detecting open back connect ports.

For more information about this technique and examples of malware check out [MITRE ATT&CK Software](https://attack.mitre.org/software/).

You can download the event logs used in this room from this task or you can open them in the Practice folder on the provided machine.

### Hunting Rats and C2 Servers

The first technique we will use to hunt for malware is a similar process to hunting Metasploit. We can look through and create a configuration file to hunt and detect suspicious ports open on the endpoint. By using known suspicious ports to include in our logs we can add to our hunting methodology in which we can use logs to identify adversaries on our network then use packet captures or other detection strategies to continue the investigation. The code snippet below is from the Ion-Storm configuration file which will alert when specific ports like 1034 and 1604 as well as exclude common network connections like OneDrive, by excluding events we still see everything that we want without missing anything and cutting down on noise.

When using configuration files in a production environment you must be careful and understand exactly what is happening within the configuration file an example of this is the Ion-Storm configuration file excludes port 53 as an event. Attackers and adversaries have begun to use port 53 as part of their malware/payloads which would go undetected if you blindly used this configuration file as-is.

For more information about the ports that this configuration file alerts on check out this [spreadsheet](https://docs.google.com/spreadsheets/d/1M56u3n3v7cQ3Y2L8T9QJwe9g2-nrUwUzqY_3zU9nBPI/).

```xml
<RuleGroup name="" groupRelation="or">
	<NetworkConnect onmatch="include">
		<DestinationPort condition="is">1034</DestinationPort>
		<DestinationPort condition="is">1604</DestinationPort>
	</NetworkConnect>
	<NetworkConnect onmatch="exclude">
		<Image condition="image">OneDrive.exe</Image>
	</NetworkConnect>
</RuleGroup>
```

Open `C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Hunting_Rats.evtx` in Event Viewer to view a live rat being dropped onto the server.

In the above example, we are detecting a custom RAT that operates on port 8080. This is a perfect example of why you want to be careful when excluding events in order to not miss potential malicious activity.

### Hunting for Common Back Connect Ports with PowerShell

Just like previous sections when using PowerShell we will again be using the PowerShell module Get-WinEvent along with XPath queries to filter our events and gain granular control over our logs. We will need to filter on the NetworkConnect event ID and the DestinationPort data attribute. If you're using a good configuration file with a reliable set of rules it will do a majority of the heavy lifting and filtering to what you want should be easy.

```powershell
Get-WinEvent -Path <Path to Log> -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"] and */EventData/Data=<Port>'
```

**Hunting Connections**:
```powershell
PS C:\Users\THM-Analyst> Get-WinEvent -Path C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Hunting_Rats.evtx -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"] and */EventData/Data=8080'

   ProviderName: Microsoft-Windows-Sysmon

TimeCreated                     Id LevelDisplayName Message
-----------                     -- ---------------- -------
1/5/2021 4:44:35 AM              3 Information      Network connection detected:...
1/5/2021 4:44:31 AM              3 Information      Network connection detected:...
1/5/2021 4:44:27 AM              3 Information      Network connection detected:...
1/5/2021 4:44:24 AM              3 Information      Network connection detected:...
1/5/2021 4:44:20 AM              3 Information      Network connection detected:...
```

## Persistence Overview

Persistence is used by attackers to maintain access to a machine once it is compromised. There is a multitude of ways for an attacker to gain persistence on a machine. We will be focusing on registry modification as well as startup scripts. We can hunt persistence with Sysmon by looking for File Creation events as well as Registry Modification events. The SwiftOnSecurity configuration file does a good job of specifically targeting persistence and techniques used. You can also filter by the Rule Names in order to get past the network noise and focus on anomalies within the event logs.

You can download the event logs used in this room from this task or you can open them in the Practice folder on the provided machine.

### Hunting Startup Persistence

We will first be looking at the SwiftOnSecurity detections for a file being placed in the \Startup\ or \Start Menu directories. Below is a snippet of the config that will aid in event tracing for this technique. For more information about this technique check out [MITRE ATT&CK T1547](https://attack.mitre.org/techniques/T1547/).

```xml
<RuleGroup name="" groupRelation="or">
	<FileCreate onmatch="include">
		<TargetFilename name="T1023" condition="contains">\Start Menu</TargetFilename>
		<TargetFilename name="T1165" condition="contains">\Startup\</TargetFilename>
	</FileCreate>
</RuleGroup>
```

Open `C:\Users\THM-Analyst\Desktop\Scenarios\Practice\T1023.evtx` in Event Viewer to view a live attack on the machine that involves persistence by adding a malicious EXE into the Startup folder.

When looking at the Event Viewer we see that persist.exe was placed in the Startup folder. Threat Actors will almost never make it this obvious but any changes to the Start Menu should be investigated. You can adjust the configuration file to be more granular and create alerts past just the File Created tag. We can also filter by the Rule Name T1023.

Once you have identified that a suspicious binary or application has been placed in a startup location you can begin an investigation on the directory.

### Hunting Registry Key Persistence

We will again be looking at another SwiftOnSecurity detection this time for a registry modification that adjusts that places a script inside CurrentVersion\Windows\Run and other registry locations. For more information about this technique check out [MITRE ATT&CK T1112](https://attack.mitre.org/techniques/T1112/).

```xml
<RuleGroup name="" groupRelation="or">
	<RegistryEvent onmatch="include">
		<TargetObject name="T1060,RunKey" condition="contains">CurrentVersion\Run</TargetObject>
		<TargetObject name="T1484" condition="contains">Group Policy\Scripts</TargetObject>
		<TargetObject name="T1060" condition="contains">CurrentVersion\Windows\Run</TargetObject>
	</RegistryEvent>
</RuleGroup>
```

Open `C:\Users\THM-Analyst\Desktop\Scenarios\Practice\T1060.evtx` in Event Viewer to view an attack where the registry was modified to gain persistence.

When looking at the event logs we see that the registry was modified and malicious.exe was added to `HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\Persistence`. We also see that the exe can be found at `%windir%\System32\malicious.exe`.

Just like the startup technique, we can filter by the RuleName T1060 to make finding the anomaly easier.

## Evasion Techniques Overview

There are a number of evasion techniques used by malware authors to evade both anti-virus and detections. Some examples of evasion techniques are Alternate Data Streams, Injections, Masquerading, Packing/Compression, Recompiling, Obfuscation, Anti-Reversing Techniques. In this task, we will be focusing on Alternate Data Streams and Injections. Alternate Data Streams are used by malware to hide its files from normal inspection by saving the file in a different stream apart from $DATA. Sysmon comes with an event ID to detect newly created and accessed streams allowing us to quickly detect and hunt malware that uses ADS. Injection techniques come in many different types: Thread Hijacking, PE Injection, DLL Injection, and more. In this room, we will be focusing on DLL Injection and backdooring DLLs. This is done by taking an already used DLL that is used by an application and overwriting or including your malicious code within the DLL.

For more information about this technique check out [MITRE ATT&CK T1564](https://attack.mitre.org/techniques/T1564/) and [T1055](https://attack.mitre.org/techniques/T1055/).

You can download the event logs used in this room from this task or you can open them in the Practice folder on the provided machine.

### Hunting Alternate Data Streams

The first technique we will be looking at is hiding files using alternate data streams using Event ID 15. Event ID 15 will hash and log any NTFS Streams that are included within the Sysmon configuration file. This will allow us to hunt for malware that evades detections using ADS. To aid in hunting ADS we will be using the SwiftOnSecurity Sysmon configuration file. The code snippet below will hunt for files in the Temp and Startup folder as well as .hta and .bat extension.

```xml
<RuleGroup name="" groupRelation="or">
	<FileCreateStreamHash onmatch="include">
		<TargetFilename condition="contains">Downloads</TargetFilename>
		<TargetFilename condition="contains">Temp\7z</TargetFilename>
		<TargetFilename condition="ends with">.hta</TargetFilename>
		<TargetFilename condition="ends with">.bat</TargetFilename>
	</FileCreateStreamHash>
</RuleGroup>
```

Open `C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Hunting_ADS.evtx` in Event Viewer to view hidden files using an alternate data stream.

**Listing Data Streams**:
```
C:\Users\THM-Threat>dir /r
 Volume in drive C has no label.
 Volume Serial Number is C0C4-7EC1

 Directory of C:\Users\THM-Threat

10/23/2022  02:56 AM    <DIR>          .
10/23/2022  02:56 AM    <DIR>          ..
01/02/2021  12:43 AM    <DIR>          3D Objects
01/02/2021  12:43 AM    <DIR>          Contacts
01/05/2021  11:53 PM    <DIR>          Desktop
01/02/2021  12:43 AM    <DIR>          Documents
01/10/2021  12:11 AM    <DIR>          Downloads
01/02/2021  12:43 AM    <DIR>          Favorites
01/02/2021  12:43 AM    <DIR>          Links
01/02/2021  12:43 AM    <DIR>          Music
10/23/2022  02:56 AM                 0 not_malicious.txt
                                    13 not_malicious.txt:malicious.txt:$DATA
```

As you can see the event will show us the location of the file name as well as the contents of the file this will be useful if an investigation is necessary.

### Detecting Remote Threads

Adversaries also commonly use remote threads to evade detections in combination with other techniques. Remote threads are created using the Windows API CreateRemoteThread and can be accessed using OpenThread and ResumeThread. This is used in multiple evasion techniques including DLL Injection, Thread Hijacking, and Process Hollowing. We will be using the Sysmon event ID 8 from the SwiftOnSecurity configuration file. The code snippet below from the rule will exclude common remote threads without including any specific attributes this allows for a more open and precise event rule.

```xml
<RuleGroup name="" groupRelation="or">
	<CreateRemoteThread onmatch="exclude">
		<SourceImage condition="is">C:\Windows\system32\svchost.exe</SourceImage>
		<TargetImage condition="is">C:\Program Files (x86)\Google\Chrome\Application\chrome.exe</TargetImage>
	</CreateRemoteThread>
</RuleGroup>
```

Open `C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Detecting_RemoteThreads.evtx` in Event Viewer to observe a Process Hollowing attack that abuses the notepad.exe process.

As you can see in the above image powershell.exe is creating a remote thread and accessing notepad.exe. This is obviously a PoC and could in theory execute any other kind of executable or DLL. The specific technique used in this example is called Reflective PE Injection.

### Detecting Evasion Techniques with PowerShell

We have already gone through a majority of the syntax required to use PowerShell with events. Like previous tasks, we will be using Get-WinEvent along with the XPath to filter and search for files that use an alternate data stream or create a remote thread. In both of the events, we will only need to filter by the EventID because the rule used within the configuration file is already doing a majority of the heavy lifting.

**Detecting Remote Thread Creation**:
```powershell
Get-WinEvent -Path <Path to Log> -FilterXPath '*/System/EventID=8'
```

**Detecting Remote Threads**:
```powershell
PS C:\Users\THM-Analyst> Get-WinEvent -Path C:\Users\THM-Analyst\Desktop\Scenarios\Practice\Detecting_RemoteThreads.evtx -FilterXPath '*/System/EventID=8'

   ProviderName: Microsoft-Windows-Sysmon

TimeCreated                     Id LevelDisplayName Message
-----------                     -- ---------------- -------
7/3/2019 8:39:30 PM              8 Information      CreateRemoteThread detected:...
7/3/2019 8:39:30 PM              8 Information      CreateRemoteThread detected:...
7/3/2019 8:39:30 PM              8 Information      CreateRemoteThread detected:...
7/3/2019 8:39:30 PM              8 Information      CreateRemoteThread detected:...
7/3/2019 8:39:30 PM              8 Information      CreateRemoteThread detected:...
```

## Investigation Methodology

This section analyzes Sysmon logs (`Investigation-1.evtx` to `Investigation-4.evtx`) to answer specific questions about malicious activity, including USB-based attacks, masquerading payloads, persistence, and C2 communications. The provided PowerShell commands and Event Viewer outputs are used to derive answers, with screenshots (0.png to 11.png) placed exactly where specified in the user’s input (`(! <number>.png !)`) as `screenshots/<number>.png`. No additional images are included, and no artifacts (e.g., command outputs as images) are added, per the user’s instructions.

### Investigation 1: Ugh, BILL THAT'S THE WRONG USB!

**Log**: `C:\Users\THM-Analyst\Desktop\Scenarios\Investigations\Investigation-1.evtx`

**Questions**:
1. **Full registry key of the USB device calling svchost.exe?**
2. **Device name when being called by RawAccessRead?**
3. **First exe the process executes?**

**Analysis**:
- **Registry Key (Event ID 13)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-1.evtx -FilterXPath '*/System/EventID=13' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 3/6/2018 6:57:51 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 13
  Message      : Registry value set:
               TargetObject: HKLM\SOFTWARE\Microsoft\Windows Portable Devices\Devices\WPDBUSENUMROOT#UMB#2&37C186B&0&STORAGE#VOLUME#_??_USBSTOR#DISK&VEN_SANDISK&PROD_U3_CRUZER_MICRO&REV_8.01#4054910EF19005B3&0#\FriendlyName
  ```
  **Answer**: `HKLM\SOFTWARE\Microsoft\Windows Portable Devices\Devices\WPDBUSENUMROOT#UMB#2&37C186B&0&STORAGE#VOLUME#_??_USBSTOR#DISK&VEN_SANDISK&PROD_U3_CRUZER_MICRO&REV_8.01#4054910EF19005B3&0#\FriendlyName`
  ![Registry Key Output](./screenshots/0.png)

- **Device Name (Event ID 9)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-1.evtx -FilterXPath '*/System/EventID=9' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 3/6/2018 6:57:51 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 9
  Message      : RawAccessRead detected:
               Device: \Device\HarddiskVolume3
  ```
  **Context**: Event ID 9 detects raw disk reads (e.g., `\\.\` notation), often used for data exfiltration or accessing locked files (e.g., `ntds.dit`). The user referenced a Google search and Sysmon documentation confirming this behavior.
  **Answer**: `\Device\HarddiskVolume3`
  ![RawAccessRead Output](./screenshots/2.png)

- **First Executable (Event ID 1)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-1.evtx -FilterXPath '*/System/EventID=1' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 3/6/2018 6:57:51 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 1
  Message      : Process Create:
               Image: C:\Windows\System32\rundll32.exe
  ```
  **Answer**: `rundll32.exe`
  [Process Creation Output](./screenshots/1.png)

### Investigation 2: This Isn't an HTML File?

**Log**: `C:\Users\THM-Analyst\Desktop\Scenarios\Investigations\Investigation-2.evtx`

**Questions**:
1. **Full path of the payload?**
2. **Full path of the file the payload masked itself as?**
3. **Signed binary that executed the payload?**
4. **IP of the adversary?**
5. **Back connect port?**

**Analysis**:
- **Payload, Masked File, Signed Binary (Event ID 1)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-2.evtx -FilterXPath '*/System/EventID=1' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 6/15/2019 7:13:42 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 1
  Message      : Process Create:
               Image: C:\Windows\System32\mshta.exe
               OriginalFileName: "C:\Windows\System32\mshta.exe" "C:\Users\IEUser\AppData\Local\Microsoft\Windows\Temporary Internet Files\Content.IE5\S97WTYG7\update.hta"
               ParentImage: "C:\Program Files\Internet Explorer\iexplore.exe" C:\Users\IEUser\Downloads\update.html
  ```
  **Answers**:
  - Payload: `C:\Users\IEUser\AppData\Local\Microsoft\Windows\Temporary Internet Files\Content.IE5\S97WTYG7\update.hta`
  - Masked File: `C:\Users\IEUser\Downloads\update.html`
  - Signed Binary: `C:\Windows\System32\mshta.exe` (Microsoft-signed, as per `Company: Microsoft Corporation`)
  ![Payload and Binary Output](./screenshots/4.png)

- **Adversary IP and Port (Event ID 3)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-2.evtx -FilterXPath '*/System/EventID=3' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 6/15/2019 7:13:44 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 3
  Message      : Network connection detected:
               DestinationIp: 10.0.2.18
               DestinationPort: 4443
  ```
  **Answers**:
  - IP: `10.0.2.18`
  - Port: `4443`
  ![Network Connection Output](./screenshots/5.png)

### Investigation 3.1: Where's the Bouncer When You Need Him

**Log**: `C:\Users\THM-Analyst\Desktop\Scenarios\Investigations\Investigation-3.1.evtx`

**Questions**:
1. **IP of the suspected adversary?**
2. **Hostname of the affected endpoint?**
3. **Hostname of the C2 server?**
4. **Registry key where the payload was stored?**
5. **PowerShell launch code for the payload?**

**Analysis**:
- **IP, Hostnames (Event ID 3)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-3.1.evtx -FilterXPath '*/System/EventID=3' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 2/12/2018 9:15:59 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 3
  Message      : Network connection detected:
               SourceIp: 172.16.199.179
               SourceHostname: DESKTOP-O153T4R.localdomain
               DestinationIp: 172.30.1.253
               DestinationHostname: empirec2
  ```
  **Answers**:
  - Adversary IP: `172.30.1.253`
  - Endpoint Hostname: `DESKTOP-O153T4R`
  - C2 Hostname: `empirec2`
  ![Network Connection Output](./screenshots/6.png)

- **Registry Key, PowerShell Code (Event ID 13)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-3.1.evtx -FilterXPath '*/System/EventID=13' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 2/12/2018 9:15:57 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 13
  Message      : Registry value set:
               TargetObject: HKLM\SOFTWARE\Microsoft\Network\debug
               Details: <Base64-encoded payload>
  TimeCreated  : 2/12/2018 9:15:57 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 13
  Message      : Registry value set:
               TargetObject: HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe\Debugger
               Details: "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -c "$x=$((gp HKLM:Software\Microsoft\Network debug).debug);start -Win Hidden -A \"-enc $x\" powershell"
  ```
  **Answers**:
  - Registry Key: `HKLM\SOFTWARE\Microsoft\Network\debug`
  - PowerShell Code: `"C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -c "$x=$((gp HKLM:Software\Microsoft\Network debug).debug);start -Win Hidden -A \"-enc $x\" powershell"`
  ![Registry Event Output](./screenshots/7.png)

### Investigation 3.2: Where's the Bouncer When You Need Him

**Log**: `C:\Users\THM-Analyst\Desktop\Scenarios\Investigations\Investigation-3.2.evtx`

**Questions**:
1. **IP of the adversary?**
2. **Full path of the payload location?**
3. **Full command used to create the scheduled task?**
4. **Suspicious process accessed by schtasks.exe?**

**Analysis**:
- **Adversary IP (Event ID 3)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-3.2.evtx -FilterXPath '*/System/EventID=3' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 2/5/2018 7:08:55 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 3
  Message      : Network connection detected:
               DestinationIp: 172.168.103.188
  ```
  **Answer**: `172.168.103.188`
  ![Network Connection Output](./screenshots/8.png)

- **Payload Path, Scheduled Task Command**:
  **Answers** (from provided text):
  - Payload Path: `c:\users\q\AppData:blah.txt`
  - Command: `"C:\WINDOWS\system32\schtasks.exe" /Create /F /SC DAILY /ST 09:00 /TN Updater /TR "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -NonI -W hidden -c \"IEX ([Text.Encoding]::UNICODE.GetString([Convert]::FromBase64String($(cmd /c ''more < c:\users\q\AppData:blah.txt'''))))\""`
  ![Scheduled Task Output](./screenshots/9.png)

- **Suspicious Process**:
  **Answer** (from Event Viewer): `lsass.exe` (accessed by `schtasks.exe`, suspicious as it may indicate credential access attempts).
  ![Suspicious Process Output](./screenshots/10.png)

### Investigation 4: Mom Look! I Built a Botnet!

**Log**: `C:\Users\THM-Analyst\Desktop\Scenarios\Investigations\Investigation-4.evtx`

**Questions**:
1. **IP of the adversary?**
2. **Port used by the adversary?**
3. **C2 framework utilized?**

**Analysis**:
- **IP, Port, C2 Framework (Event ID 3)**:
  ```powershell
  Get-WinEvent -Path .\Investigation-4.evtx -Oldest -FilterXPath '*/System/EventID=3' | fl
  ```
  **Output** (relevant):
  ```
  TimeCreated  : 2/14/2018 9:51:21 AM
  ProviderName : Microsoft-Windows-Sysmon
  Id           : 3
  Message      : Network connection detected:
               DestinationIp: 172.30.1.253
               DestinationHostname: empirec2
               DestinationPort: 80
  ```
  **Answers**:
  - IP: `172.30.1.253`
  - Port: `80`
  - C2 Framework: `Empire`
  ![C2 Connection Output](./screenshots/11.png)

## Practical Tips for Analysts

1. **Event Viewer**:
   - Filter by Event ID (e.g., 3 for network, 13 for registry) or RuleName (e.g., T1060).
   - Use XML view to build XPath queries for precise filtering.

2. **PowerShell (`Get-WinEvent`)**:
   - Combine filters for granularity:
     ```powershell
     Get-WinEvent -Path <Log> -FilterXPath '*/System/EventID=3 and */EventData/Data[@Name="DestinationPort"]=4444'
     ```
   - Export results: `| Export-Csv output.csv`.

3. **Sysmon Config Tuning**:
   - Use SwiftOnSecurity for balanced exclusion rules.
   - Test ION-Storm for proactive inclusion rules, but validate exclusions (e.g., port 53).
   - Baseline environment to reduce false positives.

4. **Threat Hunting**:
   - **Metasploit**: Look for ports 4444/5555 (Event ID 3).
   - **RATs**: Monitor suspicious ports (e.g., 1034, 1604, 8080) and exclude known apps (e.g., OneDrive).
   - **Persistence**: Check startup folders (Event ID 11, RuleName T1165) and registry keys (Event ID 13, RuleName T1060).
   - **Evasion**: Detect ADS (Event ID 15) and remote threads (Event ID 8).

5. **SIEM Integration**:
   - Forward Sysmon logs to Splunk/Elastic for correlation with Windows Event Logs or network traffic.
   - Example: Correlate Event ID 3 with firewall logs for C2 detection.

6. **MITRE ATT&CK Mapping**:
   - Persistence: T1547 (Startup), T1112 (Registry).
   - Evasion: T1564 (ADS), T1055 (Injection).
   - C2: T1071 (Standard Ports).

