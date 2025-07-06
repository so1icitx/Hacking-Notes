# /mnt: Temporary Mount Points

The `/mnt` directory is a traditional spot for manually mounting filesystems temporarily, often used by system administrators during maintenance or troubleshooting. Think of it as a workbench where you can attach external drives or network shares for short-term tasks, distinct from the user-friendly `/media`.

## Purpose

Per the Filesystem Hierarchy Standard (FHS), `/mnt` is for **temporarily mounted filesystems**. It’s:
- Used by sysadmins for manual mounts (e.g., USBs, network shares).
- Empty by default, populated only when a filesystem is mounted.
- Not for installation programs, which should use other temporary directories.

## Contents

`/mnt` is typically empty unless a filesystem is mounted. Example subdirectories:
- **/mnt/backup**: Mounted backup drive.
- **/mnt/network**: Mounted network share.

## Usage

### Mounting a Filesystem
Mount a USB drive:
```bash
sudo mount /dev/sdb1 /mnt
```

### Checking Mounts
List mounted filesystems:
```bash
mount | grep /mnt
```

### Unmounting
Safely unmount:
```bash
sudo umount /mnt
```

### Why It’s Critical
- **Flexibility**: Allows manual mounts for troubleshooting or recovery.
- **Maintenance**: Used to access failing drives or temporary shares.
- **Separation**: Keeps temporary mounts distinct from `/media`.

## Best Practices
- **Use for Temporary Mounts**: Avoid long-term mounts; use `/media` for removable media.
- **Clean Up**: Unmount after use to avoid conflicts:
  ```bash
  sudo umount /mnt
  ```
- **Check Mount Status**: Verify mounts before operations:
  ```bash
  df -h /mnt
  ```

## Limitations
- **Manual Effort**: Requires root privileges and manual commands.
- **No Standard Structure**: Subdirectory names are site-specific.
- **Not User-Friendly**: Less automated than `/media`.

## Further Reading
- [FHS: /mnt Section](http://www.pathname.com/fhs/)
- [Arch Linux Wiki: Mount Command](https://wiki.archlinux.org/title/Mount)
- [Linux Mount HOWTO](https://tldp.org/HOWTO/Mount-HOWTO/)


