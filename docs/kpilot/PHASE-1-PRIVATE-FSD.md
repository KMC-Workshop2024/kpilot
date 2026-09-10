# Phase 1: Private FSD Connection

## Implemented

- Added persistent server address and port settings above the existing credentials.
- Validated IPv4, IPv6, DNS names, and TCP port range before connecting.
- Preserved CID values as strings, including leading zeroes such as `0921`.
- Used FSD protocol revision 101 for normal and fast position updates.
- Disabled the VATSIM server challenge and JWT request for configured private servers.
- Disabled official AFV startup for private-server sessions.
- Kept the original VATSIM connection path when no private address is configured.

## Local Verification

- `server_endpoint` Qt test: passed.
- QML parser: no syntax errors; only pre-existing import/context warnings.
- All changed C++ translation units and QML resources compiled successfully.
- The macOS-only development link still stops at the documented Homebrew Qt
  `FolderListModel` bundle incompatibility. Windows remains the release target.

## Production Status

No NAS files, containers, ports, or running services were changed.
