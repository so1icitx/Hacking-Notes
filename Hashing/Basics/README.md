# Hashing

## What is a Hash Function?
A **hash function** is a one-way cryptographic function that takes input data of any size and produces a fixed-size output called a hash. Unlike encryption, hashing cannot be reversed. It ensures data integrity, secures passwords, and is widely used in cybersecurity.

### Characteristics of a Secure Hash Function:
- The same input always produces the same output.
-  A tiny change in input drastically changes the hash.
- Impossible to derive the input from the hash. (nearly)
- No two inputs should produce the same hash. (should)
- Efficient to compute but difficult to reverse.

### Hashing Example:
```bash
$ echo -n "password123" | md5sum
482c811da5d5b4bc6d497ffa98491e38
```

## Why Hashing is Important
1. **Password Storage**: Instead of storing plaintext passwords, systems store hashed versions.
2. **Data Integrity**: Ensures files have not been altered (checksum verification).
3. **Cryptographic Signatures**: Used in digital certificates to verify authenticity.
4. **File Deduplication**: Detects duplicate files based on hash comparison.
5. **Authentication**: Hashes are used in password authentication without revealing the original password.

## Hash Collisions
A **collision** happens when two different inputs produce the same hash. This is a major weakness in cryptographic security.

### Example:
Older algorithms like **MD5** and **SHA-1** are vulnerable to collisions, making them insecure for cryptographic use.

## Common Hash Algorithms
| Algorithm | Hash Length | Security Status |
|-----------|------------|-----------------|
| MD5       | 128-bit    | Broken (collisions found) |
| SHA-1     | 160-bit    | Weak (collisions found) |
| SHA-256   | 256-bit    | Secure |
| SHA-512   | 512-bit    | Secure |
| bcrypt    | Variable   | Secure (Password Hashing) |
| Argon2    | Variable   | Highly Secure (Recommended for passwords) |

## Hashing in Cybersecurity
### 1. **Password Hashing and Salting**
When passwords are stored, they should be hashed with a **salt** (a unique, random value added to prevent attacks like rainbow tables).

**Example of Salting a Hash:**
```bash
$ echo -n "password123mysalt" | sha256sum
1a2b3c4d5e6f...
```

### 2. **Hashing for Integrity Checking**
When downloading software, hashes are provided to verify integrity.
```bash
$ sha256sum downloaded_file.iso
```
Compare the output with the hash provided by the developer.

---


