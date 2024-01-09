# Skyfus app

This is a small application written in C++ to ease the gameplay of a multiples simultanous dofus account.  
Allowing a similar `Alt + Escape` / `Alt + Shift + Escape` behavior but bound to the registered dofus client.  

> This application only works for Windows. POSIX system are not supported.

The latest release (v1.0.0) has been tested and work on Dofus Retro `v1.41.9`.  
It has been tested on Windows 11 and 10. But as this application is based upon the `win32.dll` it should work on older version of Windows without any issue.

# Installation

## Build from source

```bash
git clone https://github.com/Skylli202/Skyfus-app.git
```

## Download the latest release

Download the latest release from GitHub.  
[v1.0.0](https://github.com/Skylli202/Skyfus-app/releases/tag/v1.0.0)

# Keybind

> Currently, keybind are not customizable.

| Keybind            | Action                                                                                                                                       |
| ------------------ | -------------------------------------------------------------------------------------------------------------------------------------------- |
| `F5`               | Brinf to foreground and focus the **previous** registered dofus window.                                                                      |
| `F6`               | Brinf to foreground and focus the **next** registered dofus window.                                                                          |
| `CTRL + i`         | Rescan processes to detect dofus client.                                                                                                     |
| `CTRL + P`         | Toggle console visibility.                                                                                                                   |
| `ALT + i`          | Emulate a `i` press in all registered dofus client. Usefull to open inventory tab on all the registered dofus client.                        |
| `ALT + c`          | Emulate a `c` press in all registered dofus client. Usefull to open character tab on all the registered dofus client.                        |
| `SHIFT + F1`       | Emulate a `F1` press in all registered dofus client. Usefull to be ready in pre-combat mode.                                                 |
| `F7`               | Emulate a `Escape` press in all registered dofus client. Usefull to close all the end of combat resume.                                      |
| `SHIFT + F7`       | Maximize all the registered dofus client.                                                                                                    |
| `CTRL + F1`        | [Deprecated] (Try to) Emulate a `left click` in all registered dofus client.                                                                 |
| `CTRL + u`         | [Deprecated] Shuffle the registered dofus client order to fit the `Chêne Mou` dungeon order.                                                 |
| `CTRL + SHIFT + i` | [Deprecated] Rename the registered dofus client to the name of caracter supposed to be at this order. This is hardcoded and not cutomizable. |
