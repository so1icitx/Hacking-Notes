# John the Ripper

John the Ripper (JtR) is a powerful and fast password-cracking tool designed for penetration testing and security auditing. It supports various password hash formats and provides multiple attack methods.

## Features
- Supports a wide range of hash formats (MD5, SHA-1, bcrypt, etc.).
- Uses brute-force, dictionary, and rule-based attacks.
- Can crack Linux, Windows, and archive file passwords.

## Basic Usage

### Identifying a Hash Type
```bash
john --list=formats | grep -i "md5"
```

### Cracking a Hash Using a Wordlist
```bash
john --wordlist=/usr/share/wordlists/rockyou.txt hashes.txt
```

### Viewing Cracked Passwords
```bash
john --show hashes.txt
```

---

## Cracking Linux Passwords
Linux passwords are stored in `/etc/shadow`. To crack them, extract them using the `unshadow` command:
```bash
unshadow /etc/passwd /etc/shadow > unshadowed.txt
john unshadowed.txt
```

## Cracking Windows NTLM Hashes
Extract NTLM hashes from Windows using tools like `mimikatz` and save them in a file:
```bash
john --format=nt hashes.txt
```

---

## Cracking Password-Protected ZIP and RAR Files

### Extracting Hashes from ZIP Files
Use `zip2john` to extract the hash from a password-protected ZIP file:
```bash
zip2john protected.zip > zip_hash.txt
```
Crack the hash with:
```bash
john --wordlist=/usr/share/wordlists/rockyou.txt zip_hash.txt
```

### Extracting Hashes from RAR Files
Use `rar2john` to extract the hash from a RAR archive:
```bash
rar2john protected.rar > rar_hash.txt
```
Crack the hash with:
```bash
john --wordlist=/usr/share/wordlists/rockyou.txt rar_hash.txt
```

---

## Cracking SSH Private Key Passwords

If you have an encrypted SSH private key (`id_rsa`), extract the hash using `ssh2john`:
```bash
python3 /usr/share/john/ssh2john.py id_rsa > id_rsa_hash.txt
```
Crack the hash with:
```bash
john --wordlist=/usr/share/wordlists/rockyou.txt id_rsa_hash.txt
```

---

## Using Custom Rules for Smarter Attacks

John supports advanced password mutation rules. Example: Append numbers and symbols to dictionary words.
```bash
john --wordlist=rockyou.txt --rules wordlist.txt
```

---

