
# PowerShell: 

PowerShell is Microsoft’s cross-platform titan for task automation—command-line shell, scripting language, and config manager in one. 


## Start It Up

### Launching PowerShell
- **From `cmd.exe`**:
  ```
  C:\Users\captain> powershell
  Windows PowerShell
  Copyright (C) Microsoft Corporation. All rights reserved.
  PS C:\Users\captain>
  ```
- **Windows GUI**: Start → “PowerShell” | `Win + R` → `powershell`.
- **Linux/macOS**: Install PowerShell Core (`pwsh`), then `pwsh`.

## Cmdlets: Your Commands

Cmdlets are .NET-powered, Verb-Noun style:
- **Verb**: Get, Set, New, Remove.
- **Noun**: Content, Item, Process.

### Every Cmdlet with Examples
- **`Get-Command`**  
  - **What**: Lists all commands (cmdlets, aliases, etc.).
  - **Example**:
    ```
    PS> Get-Command
    CommandType     Name                           Version    Source
    -----------     ----                           -------    ------
    Alias           cd -> Set-Location
    Alias           dir -> Get-ChildItem
    Cmdlet          Get-ChildItem                  7.0.0.0    Microsoft.PowerShell.Management
    Cmdlet          Get-Process                    7.0.0.0    Microsoft.PowerShell.Management
    ```

- **`Get-Help`**  
  - **What**: Shows help for any cmdlet.
  - **Example**:
    ```
    PS> Get-Help Get-Date -examples
    NAME
        Get-Date
    EXAMPLES
        Get-Date
        Tuesday, March 30, 2025 2:45:12 PM
    ```

- **`Get-Alias`**  
  - **What**: Lists shortcuts for commands.
  - **Example**:
    ```
    PS> Get-Alias
    CommandType     Name                           Definition
    -----------     ----                           ----------
    Alias           cd                            Set-Location
    Alias           dir                           Get-ChildItem
    Alias           cat                           Get-Content
    ```

- **`Find-Module`**  
  - **What**: Searches for modules online.
  - **Example**:
    ```
    PS> Find-Module -Name "PowerShell*"
    Version    Name                   Repository    Description
    -------    ----                   ----------    -----------
    2.2.5      PowerShellGet          PSGallery     PowerShell artifact management
    ```

- **`Install-Module`**  
  - **What**: Downloads and installs a module.
  - **Example**:
    ```
    PS> Install-Module -Name PowerShellGet
    Untrusted repository prompt: [Y] Yes  [N] No
    ```

- **`Get-ChildItem`** (alias: `dir`)  
  - **What**: Lists files and folders.
  - **Example**:
    ```
    PS> dir
    Mode                LastWriteTime         Length Name
    ----                -------------         ------ ----
    d-----        5/8/2021   9:15 AM                Desktop
    -a----        9/4/2024  12:14 PM           264 captain-hat.txt
    ```

- **`Set-Location`** (alias: `cd`)  
  - **What**: Changes current directory.
  - **Example**:
    ```
    PS> cd .\Desktop
    PS C:\Users\captain\Desktop>
    ```

- **`New-Item`**  
  - **What**: Creates files or folders.
  - **Example**:
    ```
    PS> New-Item -Path ".\test.txt" -ItemType File -Value "Hello"
    Mode                LastWriteTime         Length Name
    ----                -------------         ------ ----
    -a----        3/30/2025  3:00 PM             5 test.txt
    ```

- **`Remove-Item`** (alias: `del`)  
  - **What**: Deletes files or folders.
  - **Example**:
    ```
    PS> Remove-Item -Path ".\test.txt"
    PS> dir | Where-Object Name -eq "test.txt"  # It’s gone
    ```

- **`Copy-Item`** (alias: `copy`)  
  - **What**: Copies files or folders.
  - **Example**:
    ```
    PS> Copy-Item -Path ".\captain-hat.txt" -Destination ".\hat-copy.txt"
    PS> dir
    Mode                LastWriteTime         Length Name
    ----                -------------         ------ ----
    -a----        9/4/2024  12:14 PM           264 captain-hat.txt
    -a----        3/30/2025  3:05 PM           264 hat-copy.txt
    ```

- **`Move-Item`**  
  - **What**: Moves files or folders.
  - **Example**:
    ```
    PS> Move-Item -Path ".\hat-copy.txt" -Destination ".\Desktop"
    PS> dir .\Desktop
    Mode                LastWriteTime         Length Name
    ----                -------------         ------ ----
    -a----        3/30/2025  3:05 PM           264 hat-copy.txt
    ```

- **`Get-Content`** (alias: `cat`)  
  - **What**: Reads file contents.
  - **Example**:
    ```
    PS> cat .\captain-hat.txt
    Don't touch my hat!
    ```

- **`Sort-Object`**  
  - **What**: Sorts objects by a property.
  - **Example**:
    ```
    PS> dir | Sort-Object Length
    Mode                LastWriteTime         Length Name
    ----                -------------         ------ ----
    d-----        5/8/2021   9:15 AM                Desktop
    -a----        9/4/2024  12:14 PM           264 captain-hat.txt
    ```

- **`Where-Object`**  
  - **What**: Filters objects based on a condition.
  - **Example**:
    ```
    PS> dir | Where-Object Length -gt 200
    Mode                LastWriteTime         Length Name
    ----                -------------         ------ ----
    -a----        9/4/2024  12:14 PM           264 captain-hat.txt
    ```

- **`Select-Object`**  
  - **What**: Picks specific properties.
  - **Example**:
    ```
    PS> dir | Select-Object Name,Length
    Name              Length
    ----              ------
    Desktop              
    captain-hat.txt      264
    ```

- **`Select-String`**  
  - **What**: Searches file contents for text.
  - **Example**:
    ```
    PS> cat .\captain-hat.txt | Select-String "hat"
    Don't touch my hat!
    ```

- **`Get-ComputerInfo`**  
  - **What**: Shows system details.
  - **Example**:
    ```
    PS> Get-ComputerInfo
    WindowsProductName : Windows Server 2022 Datacenter
    OsTotalVisibleMemorySize : 8388608
    ```

- **`Get-LocalUser`**  
  - **What**: Lists local user accounts.
  - **Example**:
    ```
    PS> Get-LocalUser
    Name          Enabled Description
    ----          ------- -----------
    captain       True    The beloved captain
    Guest         False   Built-in guest account
    ```

- **`Get-NetIPConfiguration`**  
  - **What**: Shows network config.
  - **Example**:
    ```
    PS> Get-NetIPConfiguration
    InterfaceAlias : Ethernet
    IPv4Address    : 10.10.178.209
    ```

- **`Get-NetIPAddress`**  
  - **What**: Lists IP addresses.
  - **Example**:
    ```
    PS> Get-NetIPAddress
    IPAddress : 10.10.178.209
    InterfaceAlias : Ethernet
    AddressFamily : IPv4
    ```

- **`Get-Process`**  
  - **What**: Lists running processes.
  - **Example**:
    ```
    PS> Get-Process
    Handles  NPM(K)    PM(K)      WS(K) CPU(s)     Id ProcessName
    -------  ------    -----      ----- ------     -- -----------
    123      12       5432       8765  1.23      512 cmd
    ```

- **`Stop-Process`**  
  - **What**: Kills a process.
  - **Example**:
    ```
    PS> Stop-Process -Id 512
    PS> Get-Process -Id 512 -ErrorAction SilentlyContinue  # Gone
    ```

- **`Get-Service`**  
  - **What**: Lists services.
  - **Example**:
    ```
    PS> Get-Service
    Status   Name               DisplayName
    ------   ----               -----------
    Running  Dnscache           DNS Client
    ```

- **`Start-Service`**  
  - **What**: Starts a stopped service.
  - **Example**:
    ```
    PS> Start-Service -Name "AppIDSvc"
    PS> Get-Service -Name "AppIDSvc"
    Status   Name               DisplayName
    ------   ----               -----------
    Running  AppIDSvc           Application Identity
    ```

- **`Stop-Service`**  
  - **What**: Stops a running service.
  - **Example**:
    ```
    PS> Stop-Service -Name "AppIDSvc"
    PS> Get-Service -Name "AppIDSvc"
    Status   Name               DisplayName
    ------   ----               -----------
    Stopped  AppIDSvc           Application Identity
    ```

- **`Get-NetTCPConnection`**  
  - **What**: Lists TCP connections.
  - **Example**:
    ```
    PS> Get-NetTCPConnection
    LocalAddress    LocalPort RemoteAddress   RemotePort State
    ------------    --------- -------------   ---------- -----
    10.10.178.209   22        10.14.87.60     53523      Established
    ```

- **`Get-FileHash`**  
  - **What**: Computes a file’s hash.
  - **Example**:
    ```
    PS> Get-FileHash -Path ".\captain-hat.txt"
    Algorithm       Hash                            Path
    ---------       ----                            ----
    SHA256          54D2EC3C12BF3D...             C:\Users\captain\captain-hat.txt
    ```

- **`ForEach-Object`**  
  - **What**: Loops through objects, running a command on each.
  - **Example**:
    ```
    PS> dir | ForEach-Object { Write-Output "File: $($_.Name)" }
    File: Desktop
    File: captain-hat.txt
    ```
  - **Note**: `$_` is the current object (e.g., a file). `$_.Name` gets its name.

- **`Invoke-Command`**  
  - **What**: Runs commands remotely.
  - **Example**:
    ```
    PS> Invoke-Command -ComputerName Server01 -ScriptBlock { Get-Process }
    Handles  NPM(K)    PM(K)      WS(K) CPU(s)     Id ProcessName
    -------  ------    -----      ----- ------     -- -----------
    200      15       6000       9000  2.00     1000 cmd
    ```

- **`Set-ExecutionPolicy`**  
  - **What**: Changes script execution rules.
  - **Example**:
    ```
    PS> Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy RemoteSigned
    Execution Policy Change
    Are you sure? [Y] Yes
    ```

## Mixed Example Explained
Here’s that pipeline again, step-by-step:
```
PS> Get-Process | Where-Object WS -gt 10000 | Select-Object Name,Id | ForEach-Object { Get-Service | Where-Object ProcessId -eq $_.Id }
Status   Name               DisplayName
------   ----               -----------
Running  Dnscache           DNS Client
```
- **Step 1**: `Get-Process` → All processes.
- **Step 2**: `Where-Object WS -gt 10000` → Filters to processes using >10 MB memory.
- **Step 3**: `Select-Object Name,Id` → Keeps just name and ID (e.g., `powershell`, `1024`).
- **Step 4**: `ForEach-Object { ... }` → For each process, `$_` is that process. `$_.Id` is its ID (e.g., `1024`).
- **Step 5**: `Get-Service | Where-Object ProcessId -eq $_.Id` → Finds services with a matching `ProcessId`.

