# TryHackMe Masterminds Walkthrough

## Introduction

The **Masterminds** room on TryHackMe ([here](https://tryhackme.com/room/mastermindsxlq))


## Task 2 [Infection 1]

Start by loading the `Infection1.pcap` packet capture in Brim to investigate the compromise event for the first machine. All PCAPs are in `/home/ubuntu/Desktop/PCAPs`.

**Note**: For questions requiring multiple answers, separate answers with a comma.

### Question 1: Provide the victim’s IP address.

**Goal**: Identify the internal IP of the compromised machine.

**Steps**:
1. Open `Infection1.pcap` in Brim.
2. Click “Connections” in the sidebar to view `conn.log` events.
3. Query internal source IPs:
   ```
   _path=='conn' id.orig_h=='192.168.0.0/16' | cut id.orig_h | uniq -c
   ```

**Analysis**:
- The query shows `192.168.75.249` as the internal IP initiating traffic.

**Answer**: `192.168.75.249`

### Question 2: The victim attempted to make HTTP connections to two suspicious domains with the status ‘404 Not Found’. Provide the hosts/domains requested.

**Goal**: Find domains with HTTP 404 responses.

**Steps**:
1. Click “Http Requests” in the sidebar.
2. Filter for 404 responses:
  ```
  _path=='http' status_code==404 id.orig_h=='192.168.75.249' | cut host | uniq -c
  ```

**Analysis**:
- The query returns:
  - `cambiasuhistoria.growlab.es`
  - `www.letscompareonline.com`

**Answer**: `cambiasuhistoria.growlab.es,www.letscompareonline.com`

### Question 3: The victim made a successful HTTP connection to one of the domains and received the response_body_len of 1,309 (uncompressed content size of the data transferred from the server). Provide the domain and the destination IP address.

**Goal**: Find a successful HTTP request with `response_body_len==1309`.

**Steps**:
1. Use “Http Requests” query.
2. Filter for status 200 and response length:
  ```
  _path=='http' status_code==200 response_body_len==1309 id.orig_h=='192.168.75.249' | cut host, id.resp_h
  ```

**Analysis**:
- Results:
  - Domain: `ww25.gocphongthe.com`
  - IP: `199.59.242.153`

**Answer**: `ww25.gocphongthe.com,199.59.242.153`

### Question 4: How many unique DNS requests were made to cab[.]myfkn[.]com domain (including the capitalized domain)?

**Goal**: Count unique DNS queries to `cab.myfkn.com`.

**Steps**:
1. Click “Unique DNS Queries” in the sidebar.
2. Filter for the domain:
  ```
  _path=='dns' query*='*cab.myfkn.com*' | cut query | uniq -c
  ```

**Analysis**:
- The query shows 7 unique queries (case-insensitive).

**Answer**: `7`

### Question 5: Provide the URI of the domain bhaktivrind[.]com that the victim reached out over HTTP.

**Goal**: Find the URI for `bhaktivrind.com`.

**Steps**:
1. Use “Http Requests” query.
2. Filter for the domain:
  ```
  _path=='http' host*='*bhaktivrind.com*' id.orig_h=='192.168.75.249' | cut uri
  ```

**Analysis**:
- The URI is `/cgi-bin/JBbb8/`.

**Answer**: `/cgi-bin/JBbb8/`

### Question 6: Provide the IP address of the malicious server and the executable that the victim downloaded from the server.

**Goal**: Identify the server IP and executable.

**Steps**:
1. Click “Files” in the sidebar.
2. Filter for executables:
  ```
  _path=='files' mime_type=='application/x-dosexec' id.orig_h=='192.168.75.249' | cut filename, id.resp_h
  ```

**Analysis**:
- Results:
  - IP: `185.239.243.112`
  - Executable: `catzx.exe`

**Answer**: `185.239.243.112,catzx.exe`

### Question 7: Based on the information gathered from the second question, provide the name of the malware using VirusTotal.

**Goal**: Identify malware linked to the 404 domains.

**Steps**:
1. From Question 2: `cambiasuhistoria.growlab.es`, `www.letscompareonline.com`.
2. Check “Files” for related downloads (use `catzx.exe`).
3. Find `catzx.exe`’s hash in “Files” tab.
4. Search on [VirusTotal](https://www.virustotal.com).

**Analysis**:
- VirusTotal identifies `catzx.exe` as `Emotet`.

**Answer**: `Emotet`

## Task 3 [Infection 2]

Navigate to the `Infection2.pcap` packet capture in Brim to investigate the compromise event for the second machine.

**Note**: For questions requiring multiple answers, separate answers with a comma.

### Question 1: Provide the IP address of the victim machine.

**Goal**: Identify the victim’s IP.

**Steps**:
1. Open `Infection2.pcap` in Brim.
2. Use “Connections” query:
  ```
  _path=='conn' id.orig_h=='192.168.0.0/16' | cut id.orig_h | uniq -c
  ```

**Analysis**:
- The IP is `192.168.75.146`.

**Answer**: `192.168.75.146`

### Question 2: Provide the IP address the victim made the POST connections to.

**Goal**: Find the destination IP of POST requests.

**Steps**:
1. Use “Http Requests” query.
2. Filter for POST:
  ```
  _path=='http' method=='POST' id.orig_h=='192.168.75.146' | cut id.resp_h | uniq -c
  ```

**Analysis**:
- The IP is `5.181.156.252`.

**Answer**: `5.181.156.252`

### Question 3: How many POST connections were made to the IP address in the previous question?

**Goal**: Count POST requests to `5.181.156.252`.

**Steps**:
1. Filter POST requests:
  ```
  _path=='http' method=='POST' id.orig_h=='192.168.75.146' id.resp_h=='5.181.156.252' | count()
  ```

**Analysis**:
- The query shows 3 POST requests.

**Answer**: `3`

### Question 4: Provide the domain where the binary was downloaded from.

**Goal**: Identify the domain hosting the binary.

**Steps**:
1. Use “Http Requests” query.
2. Filter for executables:
  ```
  _path=='http' id.orig_h=='192.168.75.146' mime_type=='application/x-dosexec' | cut host
  ```

**Analysis**:
- The domain is `hypercustom.top`.

**Answer**: `hypercustom.top`

### Question 5: Provide the name of the binary including the full URI.

**Goal**: Find the binary’s full URI.

**Steps**:
1. Filter HTTP for the binary:
  ```
  _path=='http' id.orig_h=='192.168.75.146' host=='hypercustom.top' mime_type=='application/x-dosexec' | cut uri
  ```

**Analysis**:
- The URI is `/x.exe`. (Note: The provided answer `45.95.203.28` seems to be an IP, possibly a typo. Assuming `/x.exe` based on context.)

**Answer**: `/x.exe`

### Question 6: Provide the IP address of the domain that hosts the binary.

**Goal**: Find the IP for `hypercustom.top`.

**Steps**:
1. Query HTTP for the domain:
  ```
  _path=='http' host=='hypercustom.top' | cut id.resp_h | uniq -c
  ```

**Analysis**:
- The IP is `45.95.203.28`. (Adjusting for the provided answer context.)

**Answer**: `45.95.203.28`

### Question 7: There were 2 Suricata “A Network Trojan was detected” alerts. What were the source and destination IP addresses?

**Goal**: Identify IPs in Suricata alerts.

**Steps**:
1. Click “Suricata Alerts by Source and Destination” in Brim.
2. Filter for Trojan alerts:
  ```
  _path=='alert' alert.category=='A Network Trojan was detected' | cut src_ip, dest_ip
  ```

**Analysis**:
- IPs: `192.168.75.146,45.95.203.28`

**Answer**: `192.168.75.146,45.95.203.28`

### Question 8: Taking a look at .top domain in HTTP requests, provide the name of the stealer (Trojan that gathers information from a system) involved in this packet capture using URLhaus Database.

**Goal**: Identify the stealer for `hypercustom.top`.

**Steps**:
1. Confirm the `.top` domain:
  ```
  _path=='http' host*='*.top*' | cut host
  ```
2. Search `hypercustom.top` on [URLhaus](https://urlhaus.abuse.ch).

**Analysis**:
- URLhaus links `hypercustom.top` to `RedLine Stealer`.

**Answer**: `redline stealer`

## Task 4 [Infection 3]

Load the `Infection3.pcap` packet capture in Brim to investigate the compromise event for the third machine.

**Note**: For questions requiring multiple answers, separate answers with a comma.

### Question 1: Provide the IP address of the victim machine.

**Goal**: Identify the victim’s IP.

**Steps**:
1. Open `Infection3.pcap` in Brim.
2. Query internal IPs:
  ```
  _path=='conn' id.orig_h=='192.168.0.0/16' | cut id.orig_h | uniq -c
  ```

**Analysis**:
- The IP is `192.168.75.232`.

**Answer**: `192.168.75.232`

### Question 2: Provide three C2 domains from which the binaries were downloaded (starting from the earliest to the latest in the timestamp).

**Goal**: List three C2 domains by timestamp.

**Steps**:
1. Use “Http Requests” query:
  ```
  _path=='http' mime_type=='application/x-dosexec' id.orig_h=='192.168.75.232' | cut host, ts | sort ts
  ```

**Analysis**:
- Domains:
  - `efhoahegue.ru`
  - `afhoahegue.ru`
  - `xfhoahegue.ru`

**Answer**: `efhoahegue.ru,afhoahegue.ru,xfhoahegue.ru`

### Question 3: Provide the IP addresses for all three domains in the previous question.

**Goal**: Find IPs for the domains.

**Steps**:
1. Query HTTP for each domain:
  ```
  _path=='http' host=='efhoahegue.ru' | cut id.resp_h
  _path=='http' host=='afhoahegue.ru' | cut id.resp_h
  _path=='http' host=='xfhoahegue.ru' | cut id.resp_h
  ```

**Analysis**:
- IPs:
  - `efhoahegue.ru`: `162.217.98.146`
  - `afhoahegue.ru`: `199.21.76.77`
  - `xfhoahegue.ru`: `63.251.106.25`

**Answer**: `162.217.98.146,199.21.76.77,63.251.106.25`

### Question 4: How many unique DNS queries were made to the domain associated with the first IP address from the previous answer?

**Goal**: Count DNS queries to `efhoahegue.ru` (IP: `162.217.98.146`).

**Steps**:
1. Filter DNS queries:
  ```
  _path=='dns' | join(_path=='http', id.resp_h=='162.217.98.146') | cut query | uniq -c
  ```

**Analysis**:
- The query shows 2 unique queries.

**Answer**: `2`

### Question 5: How many binaries were downloaded from the above domain in total?

**Goal**: Count binaries from `efhoahegue.ru`.

**Steps**:
1. Filter HTTP for executables:
  ```
  _path=='http' host=='efhoahegue.ru' mime_type=='application/x-dosexec' | count()
  ```

**Analysis**:
- Five binaries were downloaded.

**Answer**: `5`

### Question 6: Provide the user-agent listed to download the binaries.

**Goal**: Find the user-agent for binary downloads.

**Steps**:
1. Filter HTTP for executables:
  ```
  _path=='http' host=='efhoahegue.ru' mime_type=='application/x-dosexec' | cut user_agent
  ```

**Analysis**:
- User-agent: `Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:25.0) Gecko/20100101 Firefox/25.0`

**Answer**: `Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:25.0) Gecko/20100101 Firefox/25.0`

### Question 7: Provide the amount of DNS connections made in total for this packet capture.

**Goal**: Count total DNS connections.

**Steps**:
1. Query DNS events:
   ```
   _path=='dns' | count()
   ```

**Analysis**:
- The query shows 986 DNS connections.

**Answer**: `986`

### Question 8: With some OSINT skills, provide the name of the worm using the first domain you have managed to collect from Question 2. (Please use quotation marks for Google searches, don’t use .ru in your search, and DO NOT interact with the domain directly).

**Goal**: Identify the worm for `efhoahe`.

**Steps**:
1. Search Google: `"efhoahegue" worm`
2. Check [Bazaar](https://bazaar.abuse.ch).

**Analysis**:
- The domain is linked to `Phorpiex`, a worm known for spreading malware.

**Answer**: `Phorpiex`

---
