# Subnetting with CIDR

These notes summarize a fast, repeatable way to find:

- **Network ID**
- **Broadcast address**
- **Usable host range**

…using **CIDR notation** (like `/20`, `/22`, `/29`) without converting the entire IP to binary.

---

## CIDR Refresher

CIDR (Classless Inter-Domain Routing) notation like `/20` means:

- **20 bits** are the **network portion**
- The remaining **(32 − 20) = 12 bits** are the **host portion**

In other words, CIDR tells you how many **1s** are in the subnet mask.

Example:
- `/20` → subnet mask is `255.255.240.0`

---

## The 8-bit “Value Chart” (Memorize This)

Each octet is 8 bits, with these place values:

| Bit value | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
|----------:|----:|---:|---:|---:|--:|--:|--:|--:|

This chart is your best friend when doing quick binary conversions in a single octet.

---

## Core Idea: Only Convert the “Interesting” Octet

You rarely need to convert the whole IP to binary.

1. Figure out **which octet the CIDR stops in**
2. Only work in that octet (the “focus octet”)
3. Everything before it is fixed, everything after it is host space

### Which octet is the focus?

CIDR ranges by octet:

- `/1` to `/8` → **1st octet**
- `/9` to `/16` → **2nd octet**
- `/17` to `/24` → **3rd octet**
- `/25` to `/32` → **4th octet**

So:
- `/20` and `/22` → focus is the **3rd octet**
- `/29` → focus is the **4th octet** (but the example below still works the same way)

---

## Step-by-Step Method (The Way This Video Teaches It)

### Step 1) Write the subnet bits in the focus octet

Example for `/20`:

- First 16 bits are full octets: `255.255.`
- Remaining bits in the 3rd octet: 4 bits on → `11110000`  
  That equals `240`

So `/20` is:
- `255.255.240.0`

### Step 2) Find the **Network ID** using AND logic

Conceptually:
- `IP` **AND** `Subnet Mask` = `Network ID`

You can do this quickly by:
- Converting **only** the focus octet of the IP to binary
- Lining it up with the focus octet of the mask
- Keeping only the bits that overlap with mask `1`s

### Step 3) Find the **Magic Number**
The **magic number** is the **increment size** of subnets in the focus octet.

Rule:
- Find the **lowest-value bit that is ON** in the mask’s focus octet.
- That value is the increment.

Examples:
- `240` → `11110000` → lowest ON bit is `16` → magic number = **16**
- `252` → `11111100` → lowest ON bit is `4` → magic number = **4**
- `248` → `11111000` → lowest ON bit is `8` → magic number = **8**

### Step 4) Broadcast Address
Once you have the network value in the focus octet:

- **Next Network** = `Network + MagicNumber`
- **Broadcast** = `NextNetwork − 1`

### Step 5) Usable Range
- **First usable** = `Network + 1`
- **Last usable** = `Broadcast − 1`

---

# Worked Examples

## Example 1 — `192.168.60.55/20`

### 1) Subnet mask
`/20` → `255.255.240.0`  
Focus octet = **3rd**, mask value there = **240** (`11110000`)  
Magic number = **16**

### 2) Find network in the 3rd octet
Convert `60` to binary (only this octet):

60 = 32 + 16 + 8 + 4  
Binary: `00111100`

Mask (3rd octet):  
240 = `11110000`

AND result:
`00111100`
`11110000`
= `00110000` → 32 + 16 = **48**

So the **network ID** is:
- **192.168.48.0**

### 3) Broadcast
Next network = 48 + 16 = 64  
Broadcast = 64 − 1 = **63**

So broadcast is:
- **192.168.63.255**

### 4) Usable range
- **192.168.48.1** through **192.168.63.254**

✅ **Answer**
- Network: `192.168.48.0`
- Broadcast: `192.168.63.255`
- Usable: `192.168.48.1 — 192.168.63.254`

---

## Example 2 — `172.10.85.60/22`

### 1) Subnet mask
`/22` → `255.255.252.0`  
Focus octet = **3rd**, mask value there = **252** (`11111100`)  
Magic number = **4**

### 2) Find network in the 3rd octet
Convert 85:

85 = 64 + 16 + 4 + 1  
Binary: `01010101`

Mask:
252 = `11111100`

AND:
`01010101`
`11111100`
= `01010100` → 64 + 16 + 4 = **84**

Network:
- **172.10.84.0**

### 3) Broadcast
Next network = 84 + 4 = 88  
Broadcast = 88 − 1 = **87**

Broadcast:
- **172.10.87.255**

### 4) Usable range
- **172.10.84.1** through **172.10.87.254**

✅ **Answer**
- Network: `172.10.84.0`
- Broadcast: `172.10.87.255`
- Usable: `172.10.84.1 — 172.10.87.254`

---

## Example 3 — `172.10.60.17/29`

This one lands near the end of the mask:
- `/29` means **3 host bits**
- Subnet size is `2^3 = 8` addresses per subnet
- Magic number in the last octet is **8**

### 1) Network
Last octet is 17. Subnets increment by 8:

Multiples of 8: 0, 8, 16, 24, 32, …

17 falls in the `16–23` block → network is:
- **172.10.60.16**

### 2) Broadcast
Next network = 16 + 8 = 24  
Broadcast = 24 − 1 = **23**
- **172.10.60.23**

### 3) Usable range
- **172.10.60.17** through **172.10.60.22**

✅ **Answer**
- Network: `172.10.60.16`
- Broadcast: `172.10.60.23`
- Usable: `172.10.60.17 — 172.10.60.22`

---

# Quick Checklist (Do This Every Time)

1. Identify the **focus octet** (where CIDR “cuts off”)
2. Get the mask value in that octet
3. Magic number = **lowest ON bit** in that octet
4. Network = `IP AND mask` (usually only need focus octet)
5. Broadcast = `(Network + magic) − 1`
6. Usable = `Network+1` to `Broadcast−1`

---

# Handy Notes

- The **network address** (all host bits 0) is not assignable to a host.
- The **broadcast address** (all host bits 1) is not assignable to a host.
- The easiest way to get subnetting wrong is **small arithmetic slips**—write it down.

---

