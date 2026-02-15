

# `ps` Command 

This is a practical, admin-focused reference for the Linux `ps` command, something you can return to quickly to refresh both **usage** and the **logic behind it**.

`ps` reads process information from Linux’s virtual filesystem **`/proc`** and formats it into a snapshot. It does **not** stream updates like `top`. Its “snapshot” nature makes it excellent for scripts, logging, and debugging.

---

## 1. How `ps` Thinks: Selection vs Formatting

Almost everything confusing about `ps` becomes clear if you split options into two categories:

### A) Selection options (WHAT processes are included)

These decide which PIDs get into the output.
Examples:

* `-e` / `-A` → all processes
* `-u user` → processes owned by user
* `-p PID` → specific PID(s)
* `-C name` → processes by command name
* `-t tty` → by terminal

### B) Formatting options (HOW they are displayed)

These decide which columns show and in what format.
Examples:

* `-f` → “full” format (predefined set of columns)
* `-o ...` → custom output columns
* `u` (BSD) → user-oriented format
* `l` (BSD) → long format
* `--forest` → hierarchy tree formatting

**Mental model:**
`ps` does:

1. Build a list of processes (selection)
2. Print them (formatting)

---

## 2. The Three Syntax Styles (And Why Mixing Can Break)

`ps` supports three styles due to Unix history:

1. **UNIX/System V style** (single dash):

   ```bash
   ps -ef
   ```

2. **BSD style** (no dash):

   ```bash
   ps aux
   ```

3. **GNU long options** (double dash):

   ```bash
   ps --pid 1234
   ```

### Why mixing is dangerous

Options with the same letter can mean different things in different styles.
Example:

* `ps aux` (BSD) → `a u x` are **format/selection** flags
* `ps -aux` (POSIX-ish interpretation) → `-a` + `-u x` (user named `x`) + ???
  This is why `ps -aux` is a classic footgun.

**Rule:** try to stick to one style per command.

---

## 3. The “Big Two” Commands

### A) BSD standard: `ps aux`

```bash
ps aux
```

* `a` → all users’ processes
* `u` → user-oriented format (%CPU, %MEM, etc.)
* `x` → include processes without a controlling terminal (daemons)

**Key columns:**

* **%CPU** – CPU usage averaged since start
* **%MEM** – percentage of RAM
* **VSZ** – virtual memory (can be huge and misleading)
* **RSS** – resident memory (actual RAM)
* **STAT** – state codes (see Section 6)
* **COMMAND** – full command line (may be truncated depending on settings)

---

### B) UNIX standard: `ps -ef`

```bash
ps -ef
```

* `-e` → every process
* `-f` → “full” format (predefined columns)

**Why admins like `-ef`:**

* It includes **PPID**, which is essential for tracking process ancestry.
* It includes **TIME** (CPU time), which is crucial for diagnosing CPU burners.

---

## 3B. The `TIME` Column (CPU time, not wall time)

In `ps -ef`, `TIME` is:

* **Total CPU time consumed** by the process since it started
* Not “how long it has existed”

That means:

* Process up for 3 days but idle → low `TIME`
* Process running hard → `TIME` climbs quickly

This is why `TIME` is often more reliable than `%CPU` in one-off snapshots.

---

## 4. Why Some Options “Don’t Combine” (And What to Do Instead)

This is the part you asked for: real explanations.

### 4A. `-f` vs `-o`: Why they often conflict

* `-f` means: “use ps’s predefined full-format columns”
* `-o` means: “use exactly the columns I specify”

If you do:

```bash
ps -ef -o pid,user,cmd
```

You’re effectively saying:

* “Use full format…”
* “…but also override output format with my custom format.”

Different `ps` implementations handle this differently:

* Some treat `-o` as overriding `-f`
* Some append columns
* Some produce confusing output

✅ Best practice:

* If you want custom columns, **don’t use `-f`**.
  Use:

```bash
ps -e -o pid,ppid,user,tty,stat,etime,time,cmd
```

---

### 4B. `-e` with filters like `-C`, `-p`, `-u`: What’s actually happening?

You *can* sometimes combine these, but the **meaning depends on implementation** and it can become logically redundant or confusing.

#### Example: `-e` with `-C`

```bash
ps -e -C nginx
```

Conceptually:

* `-e` selects “everything”
* `-C nginx` selects “only nginx”

So the result should just be nginx anyway. In many systems this works and simply narrows it down. But it’s redundant and can behave differently across variants.

✅ Best practice:

* If filtering by name, skip `-e`:

```bash
ps -C nginx -o pid,ppid,user,stat,cmd
```

#### Example: `-e` with `-p`

```bash
ps -e -p 1234
```

Also redundant: `-p` already pins to PID 1234.

✅ Best practice:

```bash
ps -p 1234 -o pid,ppid,user,etime,time,cmd
```

#### Why this matters

In scripting, redundancy increases the chance of:

* portability problems
* unexpected output across distros
* confusion when someone else reads your commands

---

### 4C. “Selection can be OR or AND depending on flags”

Some selectors combine:

* as **intersection** (“AND”)
* or as **union** (“OR”)

This varies by implementation and option family.

✅ Best practice for scripts:

* use **one primary selector** (`-p` or `-C` or `-u`)
* then customize output with `-o`

---

## 5. Kernel Threads: What They Are and Hiding Them

Kernel threads look like:

```
[kworker/0:1]
[kthreadd]
```

They are:

* created by kernel code
* not backed by an on-disk executable
* shown in brackets
* usually no TTY

### Hide kernel threads (optional readability)

```bash
export LIBPROC_HIDE_KERNEL=1
```

Show them again:

```bash
export LIBPROC_HIDE_KERNEL=0
```

What it does:

* affects programs using **libproc** (including `ps`)
* only filters display
* doesn’t change the kernel

⚠️ For security checks, don’t hide them by default.

Manual filter:

```bash
ps aux | grep -v '^\s*\['
```

---

## 6. Process States (STAT)

| Code | Meaning               | Notes                          |
| ---- | --------------------- | ------------------------------ |
| R    | Running               | on CPU or runnable             |
| S    | Sleeping              | normal waiting                 |
| D    | Uninterruptible sleep | usually I/O wait; hard to kill |
| Z    | Zombie                | dead; parent hasn’t reaped     |
| T    | Stopped               | Ctrl+Z or debugger             |

Modifiers:

* `<` high priority
* `N` nice/low priority
* `s` session leader
* `l` multi-threaded
* `+` foreground

---

## 7. Related Commands 

### 7A. `pgrep` / `pkill` ,  Find (and optionally signal) processes safely

`pgrep` searches the process table and returns matching **PIDs** (or command lines), without needing `ps | grep` tricks.

Why it’s better than `ps aux | grep name`:

* No false positives (no “grep name” process showing up)
* Script-friendly (returns exit codes)
* Can match by **user**, **regex**, **parent PID**, and more
* Easy to feed into other commands: `kill $(pgrep ...)`

#### Common usage

List PIDs by name:

```bash
pgrep nginx
```

List PID + full command line (great for verification):

```bash
pgrep -a nginx
```

Filter by user (only nginx processes owned by `www-data`):

```bash
pgrep -u www-data nginx
pgrep -u www-data -a nginx
```

Match using a full command line pattern:

```bash
pgrep -f "python.*manage.py"
```

#### Important flags

* `-a` : show PID + command line
* `-u user` : match only processes owned by user
* `-f` : match against the full command line (not just process name)
* `-n` : newest match only
* `-o` : oldest match only
* `-l` : show PID + short name (less detail than `-a`)

#### Safety notes (especially for servers)

* **Prefer `pgrep -a` before killing anything**, so you can see what you’re about to target.
* Be careful with `-f` because it can match more than you intend.

Example safe workflow:

```bash
pgrep -a -u www-data nginx
kill -TERM $(pgrep -u www-data nginx)
```

> Related: `pkill` uses the same matching rules as `pgrep`, but sends a signal instead of printing PIDs.
> Example: `pkill -HUP nginx` (reload-style signal pattern for some daemons)

---

### 7B. `pstree` ,  Visualize process hierarchy (parent → children)

While `ps --forest` is great, `pstree` often gives a clearer “at-a-glance” hierarchy view. This is especially useful for spotting:

* unexpected child processes (e.g., a web server spawning a shell)
* orphaned worker processes
* supervisor patterns (systemd → service → workers)
* malware that hides by blending into a tree

Basic tree:

```bash
pstree
```

Include PIDs:

```bash
pstree -p
```

Show a specific user’s processes:

```bash
pstree -u username
pstree -pu username
```

Show tree for a specific PID (very useful for investigation):

```bash
pstree -p <PID>
```

#### Practical examples

Find what spawned a suspicious PID:

1. Use `ps` to get PPID:

```bash
ps -p <PID> -o pid,ppid,cmd
```

2. Visualize in context:

```bash
pstree -p <PPID>
```

If you see a chain like:

* `nginx` → `sh` → `curl` → `bash`
  that’s usually a **big red flag**.

---

### 7C. `pwdx` ,  Process working directory (where it’s “standing”)

`pwdx` prints the **current working directory** of a running process.

Command:

```bash
pwdx <PID>
```

Example:

```bash
pwdx 1234
# 1234: /var/www/app
```

#### What it really does

`pwdx` reads the symbolic link:

```bash
/proc/<PID>/cwd
```

So these two are equivalent:

```bash
pwdx <PID>
readlink /proc/<PID>/cwd
```

#### Why CWD matters

A process’s working directory can reveal:

* the directory a service launched from
* whether something is running out of suspicious places:

  * `/tmp`
  * `/var/tmp`
  * `/dev/shm`
  * hidden directories in `$HOME`
* why relative paths behave strangely (e.g., log files appearing “in the wrong place”)

#### Caveats

* If the directory was deleted, you may see something like `(deleted)` behavior when inspecting `/proc/<PID>/cwd`.
* Access can be restricted by permissions (you may need root for other users’ processes).

---



