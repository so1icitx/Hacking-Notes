
#  Hashcat 

##  What is Hashcat?
Hashcat is a **GPU-accelerated** password cracker that supports advanced attack techniques like brute force, rule-based, and hybrid attacks.

---

## 🔹 Installing Hashcat
```bash
sudo apt install hashcat
```

---

## 🔹 Basic Hashcat Usage

###  Cracking a Hash
```bash
hashcat -m 0 -a 0 -o cracked.txt hashes.txt /usr/share/wordlists/rockyou.txt
```
**Explanation:**
- `-m 0` → Specifies the hash type (MD5).
- `-a 0` → Uses a dictionary attack.
- `-o cracked.txt` → Saves cracked passwords.

---

## 🔹 Attack Modes
| Mode | Description |
|------|------------|
| `-a 0` | Dictionary attack (wordlist-based) |
| `-a 1` | Combination attack (merges two wordlists) |
| `-a 3` | Brute force attack (tries all possibilities) |
| `-a 6` | Hybrid attack (wordlist + brute force) |

###  Brute Force Example
```bash
hashcat -m 0 -a 3 hashes.txt ?l?l?l?l?l?l
```
**Explanation:**
- `?l` → Lowercase letters.
- `?u` → Uppercase letters.
- `?d` → Digits.
- `?s` → Special characters.

---

## 🔹 Optimizing Hashcat for Faster Cracking

### Increase Performance
```bash
hashcat -m 0 -a 0 --force -w 3 hashes.txt /usr/share/wordlists/rockyou.txt
```
**Explanation:**
- `--force` → Forces execution even if warnings appear.
- `-w 3` → Increases workload for faster cracking.

---
