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

Deliverable: K-Pilot macOS app, Windows installer, unchanged xPilot-style UI, X-Plane plugin, MSFS backend, GPL notices/source archive, reproducible build instructions, and rollback-compatible configuration migration.

Gate: fresh-install, upgrade, reconnect, malformed-packet, wrong-password, long-disconnect, and 8-hour soak suites pass. macOS/X-Plane receives live testing; MSFS remains explicitly hardware-unverified until an owner tests it.

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
| macOS packaging | Required | Required | Optional | N/A |
| Windows packaging | Required in Windows CI/VM | Required | Required | N/A |
| MSFS SimConnect ABI | Required | Hardware unavailable | Required by external tester | Deferred |
| Server stability/rollback | Required | N/A | Required | Explicit approval only |

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

