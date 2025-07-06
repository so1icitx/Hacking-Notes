# /tmp: Temporary Files

The `/tmp` directory is a shared space for temporary files created by applications or users, designed to be short-lived and cleared at reboot. Think of it as a whiteboard where programs jot down notes—like autosaves or caches—that don’t need to stick around. It’s writable by all users but protected by a sticky bit to prevent unauthorized deletions.

## Purpose

Per the Filesystem Hierarchy Standard (FHS), `/tmp` is for **temporary files** that:
- Are not preserved between program invocations.
- Are typically deleted on system reboot.
- Support applications like browsers, editors, or installers.

## Contents

Examples of files:
- **Browser caches**: Temporary web data.
- **Autosaves**: Editor backups (e.g., `vim` swap files).
- **Installer files**: Temporary files during software installation.

## Usage

### Creating Temporary Files
Write a temporary file:
```bash
echo "temp data" > /tmp/testfile
```
![run](../screenshots/29.png)
![run](../screenshots/30.png)

### Checking Contents
List files:
```bash
ls /tmp
```
![run](../screenshots/31.png)

### Why It’s Critical
- **Shared Space**: Allows apps to store transient data.
- **Security**: Sticky bit ensures users can only delete their own files:
  ```bash
  ls -ld /tmp
  ```
![run](../screenshots/32.png)

- **Performance**: Often uses `tmpfs` (RAM-based) for speed.

## Best Practices
- **Clean Regularly**: Remove old files to free space:
  ```bash
  rm -rf /tmp/*
  ```
- **Use `tmpfs`**: Mount `/tmp` in RAM for performance (check `/etc/fstab`).
- **Avoid Persistent Data**: Use `/var/tmp` for files needing to survive reboots.

## Limitations
- **Ephemeral**: Files are deleted at reboot or periodically.
- **RAM Usage**: If on `tmpfs`, large files consume memory:
  ```bash
  df -h /tmp
  ```
![run](../screenshots/33.png)

- **Security**: Shared access requires careful permission management.

## Further Reading
- [FHS: /tmp Section](http://www.pathname.com/fhs/)
- [Arch Linux Wiki: tmpfs](https://wiki.archlinux.org/title/Tmpfs)
- [POSIX.1-2008 Standard](https://pubs.opengroup.org/onlinepubs/9699919799/)

