
#  John the Ripper

##  What is John the Ripper?
John the Ripper (JtR) is a **password-cracking** tool that supports multiple hash formats and is used for penetration testing.Uses CPU.

---

## 🔹 Installing John the Ripper
```bash
sudo apt install john
```

---

## 🔹 Basic John the Ripper Usage

### Crack a Hash
```bash
john --format=raw-md5 --wordlist=/usr/share/wordlists/rockyou.txt hashes.txt
```
**Explanation:**
- `--format=raw-md5` → Specifies the hash type (MD5).
- `--wordlist=/usr/share/wordlists/rockyou.txt` → Uses a predefined wordlist.
- `hashes.txt` → File containing the hash.

---

## 🔹 Advanced Attacks

### Cracking a ZIP File Password
```bash
zip2john secret.zip > ziphash.txt
john --wordlist=/usr/share/wordlists/rockyou.txt ziphash.txt
```
**Explanation:**
- `zip2john` → Extracts the hash from the ZIP file.
- `john` → Uses the wordlist to crack the hash.

### Cracking a RAR File Password
```bash
rar2john secret.rar > rarhash.txt
john --wordlist=/usr/share/wordlists/rockyou.txt rarhash.txt
```

### Cracking an SSH Key Password
```bash
ssh2john id_rsa > sshhash.txt
john --wordlist=/usr/share/wordlists/rockyou.txt sshhash.txt
```

---

## 🔹 Saving & Resuming Cracking Sessions
### Save Progress
```bash
john --session=mycrack --format=raw-md5 hashes.txt
```
### Resume Cracking
```bash
john --restore=mycrack
```


