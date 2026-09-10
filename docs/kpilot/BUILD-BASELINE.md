# Build Baseline

## Host

```text
OK   cmake        /opt/homebrew/bin/cmake
OK   git          /opt/homebrew/bin/git
OK   ninja        /opt/homebrew/bin/ninja
OK   qmake6       /opt/homebrew/bin/qmake6
OK   qtpaths6     /opt/homebrew/bin/qtpaths6
OK   ctest        /opt/homebrew/bin/ctest
OS   Darwin KMs-MacBook-Air.local 24.5.0 Darwin Kernel Version 24.5.0: Tue Apr 22 19:54:43 PDT 2025; root:xnu-11417.121.6~2/RELEASE_ARM64_T8132 arm64
CXX  unset
```

Tool versions recorded on 2026-09-11:

- CMake 4.4.3
- Ninja 1.13.2
- Qt 6.11.2

## Supported Build Targets

- Release target: Windows K-Pilot desktop client, optional X-Plane plugin,
  and optional MSFS 2024 SimConnect backend.
- macOS is a development host only and is not a K-Pilot release target.
- MSFS runtime validation requires a Windows machine with MSFS 2024 and is not
  available on the current development host.

## Dependency Policy

Dependencies are pinned before the first release. Build scripts may report
missing tools but must not silently replace system SDKs or download executable
installers.
