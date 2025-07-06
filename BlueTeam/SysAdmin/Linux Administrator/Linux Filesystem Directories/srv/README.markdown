# /srv: Data for System Services

The `/srv` directory is where Linux stores data served by the system, such as web or FTP server content. Think of it as a public library where services like Apache or vsftpd share their resources with users or remote clients. It’s particularly useful in server environments but may be empty on desktop systems.

## Purpose

Per the Filesystem Hierarchy Standard (FHS), `/srv` contains **site-specific data** for services provided by the system. It:
- Hosts data for protocols like HTTP, FTP, or CVS.
- Is site-specific, with no mandated subdirectory structure.
- Keeps service data separate from configurations (`/etc`) or variable data (`/var`).

## Contents

Subdirectories are site-specific, e.g.:
- **/srv/www**: Web server files (e.g., HTML, PHP).
- **/srv/ftp**: FTP server root.
- **/srv/cvs**: Version control repositories.

Example:
```bash
/srv/www/html/index.html
```

## Usage

### Setting Up a Web Server
Place web content:
```bash
sudo mkdir -p /srv/www/html
sudo cp index.html /srv/www/html
```

### Checking Service Data
List contents:
```bash
ls /srv
```
![run](../screenshots/27.png)

### Why It’s Critical
- **Service Organization**: Centralizes data for hosted services.
- **Server Environments**: Standard location for web, FTP, or other server data.
- **Flexibility**: No fixed structure, allowing custom setups.

## Best Practices
- **Use for Services**: Place service data here, not user data (use `/home`).
- **Protect Permissions**: Restrict access to prevent unauthorized changes:
  ```bash
  sudo chmod -R 755 /srv/www
  ```
- **Backup Regularly**: Service data may be critical:
  ```bash
  sudo tar -czf /backup/srv.tar.gz /srv
  ```

## Limitations
- **Non-Standard Use**: Not all systems use `/srv`; some place data in `/var`.
- **Empty by Default**: Only populated if services are configured.
- **No Structure Mandate**: Requires careful organization to avoid chaos.

## Further Reading
- [FHS: /srv Section](http://www.pathname.com/fhs/)
- [Apache Documentation](https://httpd.apache.org/docs/)
- [Arch Linux Wiki: Web Server](https://wiki.archlinux.org/title/Web_server)


