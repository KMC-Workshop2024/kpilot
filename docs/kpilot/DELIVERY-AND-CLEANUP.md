# K-Pilot Delivery and Cleanup

## Delivery gate

The final Windows installer is copied to the user's Desktop only after the
Windows CI pipeline has completed these checks:

1. Protocol, private-FSD, velocity, and MSFS ABI tests pass.
2. The X-Plane plugin and desktop client compile on Windows 2022.
3. Qt runtime and QML plugins are deployed.
4. The installer completes a silent install.
5. The installed executable runs its `--version` path successfully.
6. The uninstaller completes and removes the executable.

MSFS 2024 simulator behavior remains hardware-unverified until it is tested on
a Windows machine with MSFS 2024 and SimConnect available. CI must use fakes for
the no-simulator and failure paths and must never describe those tests as an
in-simulator acceptance test.

## Acceptance gate

Build intermediates are retained while the user evaluates the delivered
installer. Cleanup starts only after the user explicitly confirms that the
delivery is acceptable and has no remaining issue.

## Remove after explicit acceptance

- Local `build/` directories and generated object files.
- Downloaded CI artifact archives after the final installer is retained.
- Disposable dependency clones and package caches created only for compilation.
- Temporary installer smoke-test directories.

## Always retain

- K-Pilot and OpenFSD source trees.
- Git repositories and commit history.
- Architecture, protocol, validation, deployment, and rollback documentation.
- Tests, CI workflow, packaging scripts, and installer source.
- The final tested `K-Pilot-Setup.exe`.
- The production server backup and rollback procedure until a later explicit
  request authorizes their removal.
