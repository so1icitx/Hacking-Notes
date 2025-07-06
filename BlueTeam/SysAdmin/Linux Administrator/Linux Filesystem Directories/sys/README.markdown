# /sys: Kernel and System Information Virtual Filesystem

The `/sys` directory is a dynamic interface to the Linux kernel’s hardware and driver information, part of the `sysfs` virtual filesystem. Think of it as a control panel where the kernel exposes details about CPUs, USB ports, and power states, allowing both reading and tweaking on the fly. Unlike `/proc`, which focuses on processes, `/sys` is all about hardware and kernel subsystems.

## Purpose

Per the Linux-specific FHS annex, `/sys` is the **location for kernel and system information** exposed via `sysfs`. It:
- Provides real-time data on devices, drivers, and kernel features.
- Allows writing to certain files to adjust kernel behavior.
- Supports tools like `udev` for hardware management.

## Contents

Key subdirectories and files:
- **/sys/devices/**: Hardware devices (e.g., CPUs, USBs).
- **/sys/block/**: Block devices (e.g., `/sys/block/sda`).
- **/sys/bus/**: System buses (e.g., PCI, USB).
- **/sys/power/**: Power management settings.

## Usage

### Checking Hardware
View CPU details:
```bash
cat /sys/devices/system/cpu/cpu0/topology/core_id
```

### Tweaking Settings
Adjust power state (requires root):
```bash
sudo echo 1 > /sys/power/pm_freeze
```

### Why It’s Critical
- **Hardware Interaction**: Exposes kernel-device communication.
- **Dynamic Configuration**: Allows runtime tweaks (e.g., power management).
- **Tool Support**: Used by `udev`, `systemd`, and power managers.

## Best Practices
- **Read-Only for Most**: Avoid writing unless necessary:
  ```bash
  ls /sys
  ```
![run](../screenshots/28.png)

- **Use Tools**: Leverage `sysctl` or `udevadm` for safe interaction:
  ```bash
  udevadm monitor
  ```
- **Backup Settings**: Save critical configurations before changes:
  ```bash
  sudo cp /sys/power/state /backup/power-state
  ```

## Limitations
- **Kernel-Dependent**: Structure varies by kernel version.
- **Complex**: Requires expertise to interpret or modify.
- **Root Access**: Many files require superuser privileges.

## Further Reading
- [FHS: /sys Section](http://www.pathname.com/fhs/)
- [Linux Kernel: Sysfs](https://www.kernel.org/doc/html/latest/filesystems/sysfs.html)
- [Arch Linux Wiki: Sysfs](https://wiki.archlinux.org/title/Sysfs)


