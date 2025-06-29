
#  Hashing - Beginner Guide

##  What is Hashing?
Hashing is a one-way cryptographic function that transforms input data into a fixed-length hash. It is **irreversible** and is used for password storage, integrity verification, and digital signatures.

### 🔹 Key Properties of Hashing
-  The same input always produces the same output.
- A small input change results in a drastically different hash.
- Hashes should be quick to generate.
- Two different inputs should not produce the same hash.

---

## 🔹 Common Hashing Algorithms

| Algorithm | Length | Example Hash |
|-----------|--------|--------------|
| **MD5** | 32 hex | `5d41402abc4b2a76b9719d911017c592` |
| **SHA-1** | 40 hex | `2fd4e1c67a2d28fced849ee1bb76e7391b93eb12` |
| **SHA-256** | 64 hex | `e632b7095b0bf32c260fa4c539e9fd7b852d0de454e9be26f24d0d6f91d069d3` |

---

## 🔹 Hashing Commands & Examples

###  Hash a String
```bash
echo -n "password" | md5sum
```
**Explanation:**
- `echo -n "password"` → Outputs "password" (without a newline).
- `|` → Pipes the output to the next command.
- `md5sum` → Generates the MD5 hash.

**Example Output:**
```
5f4dcc3b5aa765d61d8327deb882cf99  -
```

###  Hash a File
```bash
sha256sum myfile.txt
```
**Explanation:**
- `sha256sum` → Computes the SHA-256 hash.
- `myfile.txt` → The target file.

---

## 🔹 Hashing Security & Attacks

###  Hash Collisions
A **collision** occurs when two different inputs produce the same hash. Weak hash functions like **MD5** and **SHA-1** are vulnerable to collisions.

###  Rainbow Table Attacks
A **rainbow table** is a precomputed list of hash-to-password mappings. **Salting** prevents this attack by adding random data before hashing.

###  Salting Example
```bash
echo -n "password123SALT" | sha256sum
```
Salting ensures that even if two users have the same password, their hashes will be different.

---


