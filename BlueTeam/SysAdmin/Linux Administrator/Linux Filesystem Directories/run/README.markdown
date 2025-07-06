# /run: Runtime Variable Data

The `/run` directory is a temporary, in-memory filesystem that stores runtime data for processes and the system since boot. Think of it as a bulletin board where running programs post short-lived notes like process IDs (PIDs) or sockets. It’s cleared at each boot, ensuring a fresh start.

## Purpose

Per the Filesystem Hierarchy Standard (FHS), `/run` contains **system information data** describing the system since boot. It:
- Stores transient files like PID files (e.g., `/run/crond.pid`) and sockets.
- Replaces older `/var/run` for consistency and early boot access.
- Supports subdirectories for programs or users (with restricted access).

## Contents

Key entries include:
- **/run/<program>.pid**: PID files (e.g., `/run/crond.pid` with ASCII PID and newline).
- **/run/user/<uid>**: User-specific runtime data (e.g., `/run/user/1000`).
- **/run/utmp**: Tracks logged-in users (used by `who`).
- Sockets for inter-process communication.

## Usage

### Checking Runtime Data
View logged-in users:
```bash
who
cat /run/utmp
```
![run](../screenshots/22.png)
![run](../screenshots/25.png)

### Inspecting PIDs
Check a service’s PID:
```bash
cat /run/sshd.pid
```
![run](../screenshots/23.png)

### Why It’s Critical
- **Early Boot Access**: Available before `/var` is mounted.
- **Process Communication**: Enables services to track and communicate.
- **Compatibility**: Replaces `/var/run` with a unified location.

## Best Practices
- **Don’t Modify**: Programs manage `/run`; manual changes can disrupt services.
- **Restrict Access**: `/run` is not writable by unprivileged users:
  ```bash
  ls -ld /run
  ```
![run](../screenshots/24.png)

- **Use Tools**: Use `systemctl` or `pidof` to interact with PIDs:
  ```bash
  pidof sshd
  ```

## Limitations
- **Ephemeral**: Cleared at boot, so not for persistent data.
- **Security**: User subdirectories need careful permission management.
- **Backward Compatibility**: Some programs still use `/var/run` (often a symlink to `/run`).

## Further Reading
- [FHS: /run Section](http://www.pathname.com/fhs/)
- [Systemd: Run Directory](https://www.freedesktop.org/wiki/Software/systemd/)
- [Arch Linux Wiki: Run](https://wiki.archlinux.org/title/Run)

`/run` is your system’s live status board—observe, don’t scribble! 📬
