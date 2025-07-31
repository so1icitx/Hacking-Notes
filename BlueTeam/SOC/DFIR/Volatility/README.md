# Volatility: Memory Forensics Framework

## Overview
**Volatility** is a free, open-source memory forensics tool developed and maintained by the Volatility Foundation. Written in Python, it is widely used by malware analysts, SOC teams, and blue team professionals for extracting digital artifacts from volatile memory (RAM) dumps. It provides deep visibility into the runtime state of a system, independent of the operating system, making it ideal for investigating malware, system compromises, and user activity. Volatility supports Windows, Linux, and macOS, with plugins designed for plug-and-play analysis of memory dumps.

### Key Features
- **Cross-Platform Support**: Analyzes raw dumps (`.raw`), crash dumps, VMware (`.vmem`), Hyper-V (`.bin`), Parallels (`.mem`), and VirtualBox (`.sav`) files.
- **Plugin Architecture**: Modular plugins (e.g., `windows.pslist`, `windows.netstat`, `windows.malfind`) extract processes, network connections, registry data, and more.
- **Volatility 3 Advancements**: Built on Python 3, eliminates manual profile selection (used in Volatility 2), automatically identifying OS and build versions.
- **Repository**: [Volatility 3 GitHub](https://github.com/volatilityfoundation/volatility3).

### Memory Extraction Methods
Memory dumps can be extracted from bare-metal or virtual machines using tools like:
- **FTK Imager**: Captures raw memory dumps.
- **Redline**: Uses its own agent and session structure.
- **DumpIt.exe**: Lightweight memory dumper for Windows.
- **win32dd.exe / win64dd.exe**: Command-line memory acquisition.
- **Memoryze**: Enterprise-grade memory capture.
- **FastDump**: Rapid memory extraction.
- **Virtual Machines**: Collect virtual memory files (e.g., `.vmem` for VMware, `.bin` for Hyper-V).

**Note**: Extraction from bare-metal hosts can be time-consuming. Virtual machine memory files vary by hypervisor, and caution is needed to avoid data corruption during extraction.

### Key Plugins
Volatility 3 uses an OS-specific naming convention (e.g., `windows.<plugin>`, `linux.<plugin>`, `mac.<plugin>`) due to differing memory structures across operating systems. Below are key plugins for Windows analysis, with syntax examples:

#### System Information
- **windows.info**: Extracts host OS and build details from the memory dump.
  - **Syntax**: `python3 vol.py -f <file> windows.info`
  - **Use Case**: Identifies OS version when context is unknown.

#### Process Analysis
- **windows.pslist**: Lists active and terminated processes from the doubly-linked process list, similar to Task Manager.
  - **Syntax**: `python3 vol.py -f <file> windows.pslist`
  - **Pros**: Simple, mirrors active processes.
  - **Cons**: Misses processes unlinked by rootkits.
- **windows.psscan**: Scans for `_EPROCESS` structures to detect hidden processes (e.g., rootkit evasion).
  - **Syntax**: `python3 vol.py -f <file> windows.psscan`
  - **Pros**: Counters unlinking evasion.
  - **Cons**: May produce false positives.
- **windows.pstree**: Displays processes hierarchically by parent process ID (PPID).
  - **Syntax**: `python3 vol.py -f <file> windows.pstree`
  - **Use Case**: Visualizes process relationships.

#### Network Analysis
- **windows.netstat**: Identifies network connections and associated memory structures.
  - **Syntax**: `python3 vol.py -f <file> windows.netstat`
  - **Note**: Can be unstable on older Windows builds; consider `bulk_extractor` for PCAP extraction.

#### DLL and Code Analysis
- **windows.dlllist**: Lists DLLs associated with processes, useful for identifying malicious libraries.
  - **Syntax**: `python3 vol.py -f <file> windows.dlllist`
- **windows.malfind**: Detects injected code by scanning for executable memory regions (RWE/RX) or file-less malware.
  - **Syntax**: `python3 vol.py -f <file> windows.malfind`
  - **Output**: Includes PID, offset, and Hex/ASCII/Disassembly views of injected areas.
- **windows.yarascan**: Searches memory for patterns or strings using YARA rules.
  - **Syntax**: `python3 vol.py -f <file> windows.yarascan`

#### Driver and Hooking Analysis
- **windows.ssdt**: Detects System Service Descriptor Table (SSDT) hooks, a common malware evasion technique.
  - **Syntax**: `python3 vol.py -f <file> windows.ssdt`
  - **Use Case**: Identifies kernel-level hooking; requires baseline comparison to avoid false positives.
- **windows.modules**: Lists loaded kernel modules.
  - **Syntax**: `python3 vol.py -f <file> windows.modules`
- **windows.driverscan**: Scans for drivers, including hidden ones missed by `modules`.
  - **Syntax**: `python3 vol.py -f <file> windows.driverscan`
  - **Use Case**: Detects malicious drivers after initial investigation.

#### Additional Plugins
- `modscan`: Scans for kernel modules.
- `driverirp`: Analyzes driver IRP hooks.
- `callbacks`: Detects callback manipulations.
- `idt`: Inspects Interrupt Descriptor Table hooks.
- `apihooks`: Identifies API hooking.
- `moddump`: Dumps kernel modules.
- `handles`: Lists process handles.
- **Note**: Some plugins are Volatility 2-specific or third-party.

### Usage Tips
- **Help Menu**: Run `python3 vol.py --help` to list available plugins.
- **Third-Party Plugins**: Extend functionality for advanced analysis.
- **Evasion Awareness**: Understand malware techniques (e.g., hooking, unlinking) to interpret plugin outputs effectively.
- **Stability**: Some plugins (e.g., `netstat`) may be unstable on older Windows builds; cross-reference with tools like `bulk_extractor`.