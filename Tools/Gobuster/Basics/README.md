
# Gobuster

Gobuster is a Golang-crafted, open-source tool designed for offensive security tasks like enumerating:  
- Web directories  
- DNS subdomains  
- Virtual hosts (vhosts)  
- Amazon S3 buckets  
- Google Cloud Storage  

It’s a staple in Kali Linux and a go-to for pentesters and bug bounty hunters. Gobuster brute-forces possibilities using wordlists and analyzes server responses to uncover hidden resources. It sits between reconnaissance and scanning in ethical hacking, focusing on finding what’s there—whether you're meant to see it or not.  

## Understanding Key Concepts  

- **Enumeration**: Listing all possible resources (e.g., directories, subdomains) regardless of access. Think of it as mapping a building’s layout—doors, windows, everything.  
- **Brute Force**: Trying every option until something clicks. Imagine testing 1,000 keys on a lock—Gobuster does this with wordlists, hammering URLs or DNS names until it finds a match.  

## Exploring Gobuster’s Help Page  

Run `gobuster --help` to see the tool’s capabilities.  

### Usage  

```bash
gobuster [command]
```

### Available Commands  

| Command | Description |
|---------|-------------|
| `dir` | Directory/file enumeration mode |
| `dns` | DNS subdomain enumeration mode |
| `fuzz` | Fuzzing mode (replaces "FUZZ" in URLs/headers/body) |
| `gcs` | Google Cloud Storage bucket enumeration |
| `s3` | Amazon S3 bucket enumeration |
| `tftp` | TFTP enumeration mode |
| `version` | Show Gobuster’s version |
| `vhost` | Virtual host enumeration mode |

## Global Flags  

| Flag | Description |
|------|-------------|
| `--debug` | Enable debug output for troubleshooting |
| `--delay duration` | Delay between requests (e.g., 500ms) |
| `-h, --help` | Show help page |
| `--no-color` | Disable colored output |
| `--no-error` | Suppress error messages |
| `-z, --no-progress` | Hide progress updates |
| `-o, --output <file>` | Save results to a file |
| `-p, --pattern <file>` | File with replacement patterns |
| `-q, --quiet` | Skip banner and noise |
| `-t, --threads <int>` | Number of concurrent threads (default: 10) |
| `-v, --verbose` | Show verbose output (e.g., errors) |
| `-w, --wordlist <file>` | Path to wordlist (use "-" for STDIN) |
| `--wordlist-offset <int>` | Resume wordlist from a specific position |

## Explanation of Key Global Flags  

### `-t, --threads <int>`  
- Controls how many concurrent threads Gobuster uses.  
- Default: 10. More threads = faster scans, but too many can slow down your system or trigger rate limits.  
- Example: `-t 64` (good for medium-sized wordlists).  

### `-w, --wordlist <file>`  
- Specifies the wordlist file.  
- Example: `-w /usr/share/wordlists/dirb/common.txt`.  

### `--delay <duration>`  
- Adds delay between requests to avoid detection.  
- Example: `--delay 1000ms` (1 request per second per thread).  

### `--debug`  
- Enables debug output, useful for troubleshooting.  
- Example: `gobuster dir -u http://example.com -w words.txt --debug`.  

### `-o, --output <file>`  
- Saves results to a file.  
- Example: `-o results.txt`.  

---

## **Mode 1: Directory and File Enumeration (`dir`)**  

### **Purpose**  
Brute-forces web directories and files to find hidden content.  

### **Basic Syntax**  
```bash
gobuster dir -u <URL> -w <wordlist> [optional flags]
```

### **Key `dir` Flags**  

| Flag | Description |
|------|-------------|
| `-u <URL>` | The target URL (e.g., `http://example.com`) |
| `-w <wordlist>` | Wordlist to use for brute-force attempts |
| `-c, --cookies <string>` | Adds cookies to requests (e.g., session tokens) |
| `-x, --extensions <list>` | Appends file extensions to scan for files |

### **Example Usage**  

```bash
gobuster dir -u "http://example.com" -w /usr/share/wordlists/dirb/common.txt -t 50 -o results.txt
```

This command:  
- Scans `http://example.com` for directories/files.  
- Uses `common.txt` as a wordlist.  
- Runs with 50 concurrent threads.  
- Saves results to `results.txt`.  

---

## **Mode 2: DNS Subdomain Enumeration (`dns`)**  

### **Purpose**  
Finds subdomains of a given domain using brute-force wordlists.  

### **Basic Syntax**  
```bash
gobuster dns -d <domain> -w <wordlist> [optional flags]
```

### **Example Usage**  

```bash
gobuster dns -d example.com -w /usr/share/wordlists/dns/subdomains-top1million-5000.txt -o subdomains.txt
```

This command:  
- Targets `example.com` to find subdomains.  
- Uses `subdomains-top1million-5000.txt`.  
- Saves results to `subdomains.txt`.  

---

## **Mode 3: Virtual Host Enumeration (`vhost`)**  

### **Purpose**  
Finds virtual hosts configured on a web server.  

### **Basic Syntax**  
```bash
gobuster vhost -u <URL> -w <wordlist> [optional flags]
```

### **Example Usage**  

```bash
gobuster vhost -u http://example.com -w vhosts-wordlist.txt -o vhosts.txt
```

This command:  
- Tests for virtual hosts on `http://example.com`.  
- Uses `vhosts-wordlist.txt`.  
- Saves results to `vhosts.txt`.  

---

## **Mode 4: Fuzzing (`fuzz`)**  

### **Purpose**  
Performs fuzzing by replacing "FUZZ" in URLs, headers, or body content.  

### **Basic Syntax**  
```bash
gobuster fuzz -u <URL> -w <wordlist> -z FUZZ
```

### **Example Usage**  

```bash
gobuster fuzz -u "http://example.com/FUZZ" -w words.txt
```

This command:  
- Replaces `FUZZ` with words from `words.txt` in the URL.  
- Helps discover hidden endpoints or parameters.  

---

