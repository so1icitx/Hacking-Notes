# /usr: Secondary Hierarchy for User-Space Resources

The `/usr` directory, short for **Unix System Resources**, is a major hub for user-space applications, libraries, and documentation. It’s like a library where most of the system’s non-critical software lives, complementing the essential tools in `/bin` and `/lib`. Designed to be shareable and read-only, `/usr` keeps Linux organized and efficient.

## Purpose

Per the Filesystem Hierarchy Standard (FHS), `/usr` is a **secondary hierarchy** for:
- Shareable, read-only data (e.g., binaries, libraries, manuals).
- Non-essential software that complements `/bin` and `/lib`.
- Architecture-independent and dependent resources.

## Contents

Key subdirectories:
- **/usr/bin**: Most user commands (e.g., `perl`, `python`).
- **/usr/lib**: Libraries for `/usr/bin` and `/usr/sbin`.
- **/usr/share**: Architecture-independent data (e.g., manuals, locale data).
- **/usr/local**: Locally installed software.
- **/usr/include**: C header files.
- Optional: `/usr/games`, `/usr/libexec`, `/usr/src`.

## Usage

### Running Commands
Execute a program:
```bash
/usr/bin/python3 --version
```
![run](../screenshots/34.png)

### Checking Manuals
Read a man page:
```bash
man -M /usr/share/man ls
```
![run](../screenshots/35.png)

### Why It’s Critical
- **Software Hub**: Hosts most user-space applications.
- **Shareable**: Can be mounted read-only across systems.
- **Organization**: Mirrors root-level directories for non-essential tools.

## Best Practices
- **Don’t Modify Directly**: Managed by the package manager:
  ```bash
  sudo apt install <package> or sudo pacman -S <package>
  ```
- **Check Space**: Monitor `/usr` size:
  ```bash
  sudo du -sh /usr
  ```
![run](../screenshots/36.png)

- **Use `/usr/local`**: Install local software here, not in `/usr`.

## Limitations
- **Read-Only**: Often mounted read-only, requiring `/var` for variable data.
- **Size**: Can grow large with many installed packages.
- **Symlinks**: Legacy symlinks (e.g., `/usr/spool` to `/var/spool`) may confuse users.

## Further Reading
- [FHS: /usr Section](http://www.pathname.com/fhs/)
- [Arch Linux Wiki: /usr](https://wiki.archlinux.org/title/Filesystem_hierarchy)
- [GNU Coding Standards](https://www.gnu.org/prep/standards/)


