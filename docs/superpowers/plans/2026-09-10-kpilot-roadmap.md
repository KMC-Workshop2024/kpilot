# K-Pilot Delivery Roadmap

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Deliver a GPLv3 K-Pilot fork that preserves the xPilot interface, connects to the private OpenFSD server, retains xPilot Velocity behavior for X-Plane, and adds an MSFS 2024 backend derived from swift's proven SimConnect implementation.

**Architecture:** Keep xPilot's UI, FSD protocol, networking, and X-Plane plugin as the product baseline. Add only saved server address and port controls to the existing network settings, then introduce a simulator-backend boundary so the unchanged X-Plane path and a carefully ported swift SimConnect path share the same xPilot Velocity data. Production OpenFSD remains untouched until local, simulated-network, Windows build, and shadow-server gates all pass.

**Tech Stack:** C++17, Qt 6/QML, xPilot FSD and Velocity implementation, NNG/msgpack, Microsoft SimConnect, CMake, Qt Test, Go/OpenFSD, Docker.

---

## Non-Negotiable Safety Rules

- [ ] Never stop, replace, reconfigure, or restart the current production OpenFSD containers during Phases 0-5.
- [ ] Never reuse production container names, host ports, writable volumes, or database files for a test deployment.
- [ ] Never send production credentials to VATSIM services; private mode uses the configured OpenFSD address and private credentials only.
- [ ] Never claim MSFS 2024 runtime support from compilation alone. Label it hardware-unverified until tested by a Windows user who owns MSFS 2024.
- [ ] Preserve xPilot UI layout and behavior. The only requested settings additions are `Server Address` and `Server Port` above the existing credentials.
- [ ] Keep the original xPilot and swift copyright notices and distribute K-Pilot source under GPLv3.
- [ ] Every phase ends with a commit, test report, known-limitations entry, and an explicit go/no-go gate.

## Phase Sequence

### Phase 0: Freeze References And Establish Baselines

Deliverable: immutable upstream commit references, clean K-Pilot branch/worktree, dependency inventory, successful unmodified xPilot build where the host permits it, and captured baseline tests. No behavior changes.

Gate: proceed only when upstream xPilot and swift SHAs, licenses, build commands, and current OpenFSD production identity are recorded. Failure to build due to unavailable dependencies is documented and resolved before feature work.

Detailed plan: `2026-09-10-kpilot-phase-0-baseline.md`.

### Phase 1: Private Server Configuration Without UI Redesign

Deliverable: xPilot's existing settings screen with two additional persisted fields, strict address/port validation, and no changes to the main window or simulator behavior.

Gate: configuration unit tests pass; QML loads without warnings; existing settings remain backward compatible; screenshots match upstream except for the two requested fields.

### Phase 2: Private FSD Login And Protocol 101

Deliverable: private endpoint selection, private-password authentication compatible with OpenFSD, protocol revision 101, no VATSIM server discovery/token/AFV calls in private mode, and an in-process mock FSD integration suite.

Gate: two local K-Pilot clients authenticate to a mock/OpenFSD instance, exchange ordinary and Velocity packets, reconnect cleanly, redact passwords from logs, and leave official VATSIM mode behavior isolated.

### Phase 3: Preserve And Prove xPilot Velocity On X-Plane

Deliverable: deterministic tests for `^`, `#SL`, `#ST`, `$SF`, positional/angular extrapolation, two-second error correction, stale-vector handling, pause, stopped aircraft, and packet loss. The existing X-Plane plugin remains the rendering implementation.

Gate: local X-Plane test shows no persistent fore/aft displacement during formation scenarios; packet replay under 60-250 ms RTT, jitter, and loss remains stable; CPU and memory remain bounded during a soak test.

### Phase 4: Add MSFS 2024 Backend From swift

Deliverable: a Windows-only `Msfs2024Backend` using the same SimConnect data definitions, dynamic symbol resolution, AI lifecycle, radio/transponder handling, elevation correction, and model handling proven in swift. Network Velocity remains xPilot code.

Gate: Windows build passes with the official MSFS 2024 SDK; ABI/data-definition tests and a fake SimConnect integration harness pass; source-to-source parity notes identify every intentional deviation from swift.

### Phase 5: Package And Local Qualification

Deliverable: K-Pilot Windows installer, unchanged xPilot-style UI, optional X-Plane plugin, optional MSFS backend component, GPL notices/source archive, reproducible build instructions, and rollback-compatible configuration migration. macOS is not a release target.

Gate: fresh-install, upgrade, reconnect, malformed-packet, wrong-password, long-disconnect, and 8-hour soak suites pass. X-Plane receives live testing; MSFS remains explicitly hardware-unverified until an owner tests it.

### Phase 6: Shadow Server Only

Deliverable: a second OpenFSD container with a unique name, separate test ports, copied or synthetic users, isolated writable storage, health checks, resource limits, and logs. The production container remains running and unchanged.

Gate: K-Pilot connects to the shadow server; original Swift clients continue using production; crash/restart and load tests pass; stopping the shadow container has no effect on production.

### Phase 7: Opt-In Canary And Controlled Cutover

Deliverable: a small opt-in test group connects to the shadow server, followed by a documented cutover proposal only after acceptance. Production backup includes image digest, compose configuration, database backup, and one-command rollback.

Gate: no production switch without explicit user approval after reviewing test evidence. The old container is retained offline or on its original configuration until K-Pilot has completed real mixed X-Plane/MSFS sessions.

## Verification Matrix

| Area | Local automated | Local real app | Shadow server | Production |
|---|---:|---:|---:|---:|
| Settings persistence/validation | Required | Required | N/A | N/A |
| Private FSD auth/protocol 101 | Required | Required | Required | Deferred |
| Velocity parsing/prediction | Required | X-Plane required | Required | Deferred |
| Windows packaging | Required in Windows CI/VM | Required | Required | N/A |
| MSFS SimConnect ABI | Required | Hardware unavailable | Required by external tester | Deferred |
| Server stability/rollback | Required | N/A | Required | Explicit approval only |

## Installation Contract

### Windows Installer

- [ ] Deliver one signed or clearly identified unsigned `K-Pilot-Setup.exe`; do not require users to install a compiler, Qt, the MSFS SDK, or copy DLLs manually.
- [ ] Install the K-Pilot desktop executable and its runtime dependencies under the user-selected application directory, with per-user installation as the default.
- [ ] Preserve xPilot's installer flow for detecting X-Plane installations and selecting one or more X-Plane roots, and add a clearly visible `Skip X-Plane` action. Copy the renamed K-Pilot `.xpl` and its resources only into each selected `X-Plane/Resources/plugins/K-Pilot` directory.
- [ ] After the X-Plane page, show an `Install MSFS 2024 support` checkbox. When selected, install the SimConnect backend component and its K-Pilot-owned runtime files. When cleared, do not install, load, or probe the MSFS backend; its runtime cost must be zero.
- [ ] Treat MSFS 2024 as an out-of-process SimConnect target. Do not install a package into the MSFS Community directory and do not alter `UserCfg.opt`, `SimConnect.xml`, or simulator packages for a normal same-PC installation.
- [ ] At install time, report whether MSFS 2024 appears to be installed, but allow its optional component to be selected when the simulator is absent. When installed, identify MSFS 2024 at runtime from the `SIMCONNECT_RECV_OPEN` application/version response rather than trusting an installation path.
- [ ] Connect locally with `SimConnect_Open` and `SIMCONNECT_OPEN_CONFIGINDEX_LOCAL`, allowing MSFS 2024 to select its default local Named Pipe. A custom `SimConnect.cfg` is supported only as an advanced, explicit remote-simulator option.
- [ ] Display simulator state as `Not installed`, `Installed / not running`, `Connecting`, `Connected`, or `Version mismatch`; never claim MSFS support merely because a folder was detected.
- [ ] Installing the X-Plane plugin must not disable an installed MSFS component. Probe only installed backends, connect only to the simulator that is running, and never inject the same network aircraft into both simulators simultaneously.
- [ ] If both X-Plane and MSFS 2024 are running, show a one-time simulator-choice dialog, use exactly one active backend, and remember the choice. If only one simulator is running, select it automatically without prompting.
- [ ] Register a normal Windows uninstaller that removes K-Pilot-owned files and shortcuts only. It must not remove aircraft models, simulator packages, xPilot, swift, or user configuration unless the user explicitly selects K-Pilot settings removal.

### Upgrade And Coexistence

- [ ] K-Pilot and xPilot must be installable side by side, use different process names, application identifiers, configuration roots, plugin folders, IPC endpoints, shortcuts, and uninstall records.
- [ ] A K-Pilot upgrade preserves server address, port, credentials, audio settings, model mappings, and the installed simulator-component selections unless the user changes them in the installer.
- [ ] Installer rollback retains the previous K-Pilot package until the new desktop client and selected simulator bridge pass a startup self-test.
- [ ] Packaging must not include VATSIM client IDs, private keys, official signing identities, or production NAS credentials.

### Installation Acceptance Tests

- [ ] Test fresh install, upgrade, repair, uninstall, cancellation, non-ASCII Windows username, application path containing spaces, absent simulator, X-Plane-only, MSFS-only, and both simulators installed.
- [ ] Verify both X-Plane installer branches: selecting a valid directory installs the K-Pilot plugin; choosing `Skip X-Plane` installs no X-Plane files.
- [ ] Verify both MSFS installer branches: selecting the checkbox installs and loads the backend on demand; clearing it installs no backend files and performs no SimConnect probing.
- [ ] Verify both-sim behavior: idle probing remains bounded, only one backend owns aircraft injection, the conflict prompt appears once, and the remembered choice can be changed later.
- [ ] Verify that launching K-Pilot before MSFS shows `Installed / not running`, launching MSFS later changes to `Connected`, and closing/restarting MSFS reconnects without restarting K-Pilot.
- [ ] Verify that deleting or crashing K-Pilot cannot crash MSFS 2024; this follows the Microsoft-recommended out-of-process architecture and must be confirmed by the fake SimConnect harness before hardware validation.
- [ ] Verify that no normal MSFS installation step writes to the Community directory or edits simulator configuration.

## Checkpoint Record

After each phase, append:

```text
Phase:
Commit:
Artifacts:
Commands executed:
Test results:
Known limitations:
Go/no-go decision:
Next plan:
```
