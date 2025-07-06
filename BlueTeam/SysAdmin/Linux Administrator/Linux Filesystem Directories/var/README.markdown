# /var: Variable Data

The `/var` directory stores data that changes during system operation, like logs, caches, and spool files. Think of it as the system’s journal, tracking activity and storing dynamic data that grows over time. It’s designed to handle variable-sized files, often placed on a separate partition to prevent filling the root filesystem.

## Purpose

Per the Filesystem Hierarchy Standard (FHS), `/var` contains **variable data files** for:
- Logs, caches, and spool data (e.g., mail, print queues).
- Data that persists across reboots (unlike `/tmp`).
- Shareable (e.g., `/var/mail`) and unshareable (e.g., `/var/log`) data.

## Contents

Key subdirectories:
- **/var/log**: System logs (e.g., `messages`, `lastlog`).
- **/var/cache**: Application caches (e.g., `/var/cache/man`).
- **/var/spool**: Spool data (e.g., `/var/spool/mail`).
- **/var/lib**: Application state data (e.g., `/var/lib/dpkg`).
- **/var/tmp**: Persistent temporary files.
- Optional: `/var/account`, `/var/crash`, `/var/games`.

## Usage

### Checking Logs
View system logs:
```bash
sudo less /var/log/messages
```

### Managing Cache
Clear package cache (e.g., on Debian):
```bash
sudo apt clean or sudo pacman -Sc
```
![var](/../screenshots/37.png)

### Why It’s Critical
- **System Tracking**: Logs and state data monitor system health.
- **Persistence**: Stores data that survives reboots (unlike `/tmp`).
- **Separation**: Keeps variable data away from read-only `/usr`.

## Best Practices
- **Monitor Space**: Logs and caches can grow large:
  ```bash
  du -sh /var/*
  ```
- **Rotate Logs**: Use `logrotate` to manage log size:
  ```bash
  sudo logrotate /etc/logrotate.conf
  ```
- **Backup Critical Data**: Save logs or state data:
  ```bash
  sudo tar -czf /backup/var.tar.gz /var/log
  ```

## Limitations
- **Size Management**: Requires monitoring to avoid disk exhaustion.
- **Security**: Logs may contain sensitive data; restrict access:
  ```bash
  sudo chmod -R 750 /var/log
  ```
- **Complexity**: Subdirectory structure varies by distribution.

## Further Reading
- [FHS: /var Section](http://www.pathname.com/fhs/)
- [Logrotate Documentation](https://github.com/logrotate/logrotate)
- [Arch Linux Wiki: /var](https://wiki.archlinux.org/title/Filesystem_hierarchy)


