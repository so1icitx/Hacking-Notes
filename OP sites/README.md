# OP Sites

## Overview
This curated list of websites and tools supports ethical hacking, penetration testing, security research, and CTF challenges. Each resource aids specific tasks like reverse shells, privilege escalation, enumeration, or analysis. Use these responsibly on systems you have explicit permission to test.

## Categories & Resources

### Reverse Shells
Tools for generating payloads to gain remote access during pentesting.

| Site | Description | Use Case |
|------|-------------|----------|
| [RevShells](https://www.revshells.com/) | Web-based tool for generating reverse shell payloads (Bash, Python, PHP, etc.). | Craft shells for post-exploitation in CTFs or authorized tests. |
| [PayloadsAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Reverse%20Shell%20Cheatsheet.md) | Cheatsheet for reverse shell techniques across multiple languages. | Reference for quick shell creation in various environments. |

### Privilege Escalation
Resources for identifying and exploiting privilege escalation vectors.

| Site | Description | Use Case |
|------|-------------|----------|
| [GTFOBins](https://gtfobins.github.io/) | Lists Unix binaries exploitable for privilege escalation. | Find misconfigured binaries to gain higher privileges. |
| [LinPEAS](https://github.com/peass-ng/PEASS-ng/tree/master/linPEAS) | Script to enumerate Linux systems for privilege escalation vulnerabilities. | Automate checks for misconfigurations or weak permissions. |
| [LinEnum](https://github.com/rebootuser/LinEnum) | Linux enumeration script for system details and escalation paths. | Gather system info (e.g., permissions, SUID binaries). |
| [Linux Exploit Suggester](https://github.com/The-Z-Labs/linux-exploit-suggester) | Suggests exploits based on Linux system details. | Identify kernel or software vulnerabilities for escalation. |
| [Linux Smart Enumeration](https://github.com/diego-treitos/linux-smart-enumeration) | Lightweight Linux enumeration script. | Quick scans for privilege escalation opportunities. |
| [Linux Priv Checker](https://github.com/linted/linuxprivchecker) | Python script to find Linux privilege escalation vectors. | Detailed system configuration analysis. |

### Enumeration Tools
Tools for gathering information about systems or networks.

| Site | Description | Use Case |
|------|-------------|----------|
| [Enum4linux (Kali Docs)](https://www.kali.org/tools/enum4linux/) | Kali docs for `enum4linux`, enumerates Windows/Samba systems. | Extract users, shares, or groups from Windows networks. |
| [DNS Dumpster](https://dnsdumpster.com/) | Free tool for DNS reconnaissance and subdomain mapping. | Discover subdomains and network infrastructure. |
| [Shodan](https://www.shodan.io/) | Search engine for internet-connected devices. | Identify exposed services or devices for reconnaissance. |

### Kali Linux Tools
| Site | Description | Use Case |
|------|-------------|----------|
| [Kali Tools](https://www.kali.org/tools/) | Official directory of Kali Linux pentesting tools. | Explore tools like Nmap, Metasploit, or Burp Suite for various tasks. |

### Web Shells
| Site | Description | Use Case |
|------|-------------|----------|
| [R57Shell](https://www.r57shell.net/) | Resource for web shells (e.g., PHP backdoors) for compromised servers. | Deploy shells for remote control in authorized tests (use legally). |

### Security Training
| Site | Description | Use Case |
|------|-------------|----------|
| [101 Labs - CompTIA Security+](https://www.101labs.net/comptia-security/) | Labs and tutorials for CompTIA Security+ certification. | Learn foundational security concepts through hands-on exercises. |

### Exploit Databases
| Site | Description | Use Case |
|------|-------------|----------|
| [Exploit-DB](https://www.exploit-db.com/) | Archive of exploits and PoCs, maintained by Offensive Security. | Find exploits for known vulnerabilities (e.g., CVEs). |
| [CVE Details](https://www.cvedetails.com/) | Database of CVEs with severity and impact details. | Research vulnerabilities for pentesting or patching. |

### Encoding/Decoding Utilities
| Site | Description | Use Case |
|------|-------------|----------|
| [Base64 Encoder/Decoder](https://appdevtools.com/base64-encoder-decoder) | Online tool for Base64 encoding/decoding. | Decode encoded data in logs or CTF challenges. |
| [CyberChef](https://gchq.github.io/CyberChef/) | Web tool for encoding, decoding, and data analysis (Base64, hex, etc.). | Perform complex data transformations for analysis. |

### Hashing & Cracking
| Site | Description | Use Case |
|------|-------------|----------|
| [SRI Hash Generator](https://www.srihash.org/) | Generates SRI and general hashes (e.g., SHA256). | Verify file integrity or generate hashes for pentesting. |
| [CrackStation](https://crackstation.net/) | Free online hash cracker for MD5, SHA1, etc. | Crack simple hashes found in engagements. |
| [Hashes.com](https://hashes.com/en/decrypt/hash) | Hash identification and cracking service. | Identify and crack various hash types. |

### Blockchain Explorer
| Site | Description | Use Case |
|------|-------------|----------|
| [Blockchain.com Explorer](https://www.blockchain.com/explorer/) | Explorer for Bitcoin, Ethereum, and Bitcoin Cash blockchains. | Investigate cryptocurrency transactions in CTFs or forensics. |

### Malware Analysis
| Site | Description | Use Case |
|------|-------------|----------|
| [MalAPI](https://malapi.io/) | Details Windows API calls used by malware. | Analyze malware behavior during reverse engineering. |
| [VX-Underground - ATM Malware](https://vx-underground.org/Samples/ATM%20Malware) | Collection of ATM malware samples. | Study malware for educational purposes. |
| [Malware Traffic Analysis](https://www.malware-traffic-analysis.net/) | PCAPs and analysis of malware network activity. | Learn network-based malware behavior. |

### Certificate Transparency
| Site | Description | Use Case |
|------|-------------|----------|
| [CRT.sh](https://crt.sh/) | Search tool for certificate transparency logs. | Discover subdomains and certificates for a domain. |

### Additional Tools
| Site | Description | Use Case |
|------|-------------|----------|
| [MailHeader](https://mailheader.org/) | Analyzes email headers for forensic investigation. | Trace email origins in phishing or CTF scenarios. |
| [IPinfo](https://ipinfo.io/) | Provides IP address details (geolocation, ASN, etc.). | Map IPs during reconnaissance. |
| [urlscan.io](https://urlscan.io/) | Scans and analyzes URLs for malicious content. | Investigate suspicious links or websites. |
| [WannaBrowser](https://www.wannabrowser.net/) | Simulates browser behavior for web analysis. | Test web vulnerabilities or behaviors. |
| [URL2PNG](https://www.url2png.com/) | Captures website screenshots. | Document web pages for recon or reporting. |
| [ANY.RUN](https://app.any.run/) | Interactive malware analysis sandbox. | Analyze malware in a controlled environment. |
| [DMARCian SPF Survey](https://dmarcian.com/spf-survey/) | Checks SPF records for email domains. | Verify email security configurations. |
| [MXToolbox](https://mxtoolbox.com/) | Tools for DNS, email, and network diagnostics. | Troubleshoot mail or DNS issues in pentests. |

## Usage Tips
- **Bookmark Key Sites**: Save RevShells, GTFOBins, and Exploit-DB for quick access.
- **Combine Resources**: Use DNS Dumpster with Shodan for thorough recon, or pair LinPEAS with Exploit-DB for privilege escalation.
- **Practice in CTFs**: Test tools in safe environments like Hack The Box or TryHackMe.
- **Stay Ethical**: Only use on authorized systems. Unauthorized access is illegal.

## Contributions
Suggest new tools or updates by forking this repo or contacting the maintainer. This list will grow with new discoveries.


https://www.revouninstaller.com/revo-uninstaller-free-download/
https://github.com/ChrisTitusTech/winutil

## Notes
- Check sites regularly for updates, as tools evolve.
- Use VPNs or proxies for privacy during recon, if permitted.
