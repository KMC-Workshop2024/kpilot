# K-Pilot Phase 0 Baseline Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Create a reproducible, non-destructive K-Pilot development baseline before changing xPilot behavior.

**Architecture:** Record exact upstream sources and licenses, create an isolated K-Pilot branch/worktree, inventory build dependencies, and compile/test the unchanged application. Treat the running NAS server as read-only inventory; no container lifecycle or configuration commands are permitted.

**Tech Stack:** Git, CMake, Qt 6, C++17, xPilot, swift reference source, shell scripts, Markdown.

---

### Task 1: Record Upstream Provenance And Licensing

**Files:**
- Create: `docs/kpilot/UPSTREAM.md`
- Create: `docs/kpilot/LICENSE-COMPLIANCE.md`

- [ ] **Step 1: Capture immutable revisions**

Run:

```bash
git rev-parse HEAD
git remote get-url origin
git -C ../swift-pilotclient-reference rev-parse HEAD
git -C ../swift-pilotclient-reference remote get-url origin
```

Expected: four non-empty lines containing two full commit SHAs and the official xPilot/swift GitHub URLs.

- [ ] **Step 2: Write the provenance record**

Create `docs/kpilot/UPSTREAM.md` with the verified revisions:

```markdown
# Upstream Sources

## xPilot
- Repository: https://github.com/xpilot-project/xpilot
- Commit: `4bd5b8079e43d130c87ed5df8e32e86822e06a27`
- Role: K-Pilot UI, FSD protocol, Velocity networking, X-Plane bridge and rendering

## swift
- Repository: https://github.com/swift-project/pilotclient
- Commit: `29ecd719b7c4d0e6db30518685d70ef70a09d841`
- Role: reference implementation for MSFS 2024 SimConnect only

K-Pilot changes must identify whether code is original, adapted from xPilot, or adapted from swift.
```

- [ ] **Step 3: Write the license obligations**

Create `docs/kpilot/LICENSE-COMPLIANCE.md` containing:

```markdown
# License Compliance

K-Pilot is a GPL-3.0 derivative of xPilot and incorporates GPL-3.0-compatible code adapted from swift.

- Preserve existing file-level copyright and SPDX notices.
- Mark materially adapted files in commit history and documentation.
- Ship the complete corresponding K-Pilot source with distributed binaries.
- Ship xPilot, swift, and third-party license notices.
- Do not use xPilot or swift trademarks, names, icons, signing identities, VATSIM client IDs, or private keys as K-Pilot identity.
- Do not enable connection to VATSIM without authorization and a valid client identity.
```

- [ ] **Step 4: Verify the documents contain no unresolved placeholders**

Run:

```bash
rg -n 'TBD|TODO|PLACEHOLDER' docs/kpilot
```

Expected: no output.

- [ ] **Step 5: Commit the provenance documents**

```bash
git add docs/kpilot/UPSTREAM.md docs/kpilot/LICENSE-COMPLIANCE.md
git commit -m "docs: record K-Pilot upstream provenance"
```

### Task 2: Establish A Reproducible Dependency Inventory

**Files:**
- Create: `scripts/kpilot/check-build-environment.sh`
- Create: `docs/kpilot/BUILD-BASELINE.md`

- [ ] **Step 1: Add a read-only environment checker**

Create `scripts/kpilot/check-build-environment.sh`:

```bash
#!/usr/bin/env bash
set -eu

check() {
    if command -v "$1" >/dev/null 2>&1; then
        printf 'OK   %-12s %s\n' "$1" "$(command -v "$1")"
    else
        printf 'MISS %-12s\n' "$1"
    fi
}

check cmake
check git
check ninja
check qmake6
check qtpaths6
check ctest

printf 'OS   %s\n' "$(uname -a)"
printf 'CXX  %s\n' "${CXX:-unset}"
```

- [ ] **Step 2: Make the checker executable and run it**

Run:

```bash
chmod +x scripts/kpilot/check-build-environment.sh
scripts/kpilot/check-build-environment.sh | tee /tmp/kpilot-build-environment.txt
```

Expected: every required tool is reported as `OK`, or each missing dependency is explicitly listed as `MISS`.

- [ ] **Step 3: Document the baseline without installing anything implicitly**

Create `docs/kpilot/BUILD-BASELINE.md` with:

```markdown
# Build Baseline

## Host

Paste the output of `scripts/kpilot/check-build-environment.sh` here.

## Supported Build Targets

- macOS: K-Pilot desktop client and X-Plane plugin.
- Windows: K-Pilot desktop client, X-Plane plugin, and MSFS 2024 SimConnect backend.
- MSFS runtime validation requires a Windows machine with MSFS 2024 and is not available on the current Mac.

## Dependency Policy

Dependencies are pinned before the first release. Build scripts may report missing tools but must not silently replace system SDKs or download executable installers.
```

- [ ] **Step 4: Commit the environment inventory**

```bash
git add scripts/kpilot/check-build-environment.sh docs/kpilot/BUILD-BASELINE.md
git commit -m "build: add K-Pilot environment inventory"
```

### Task 3: Compile The Unmodified xPilot Baseline

**Files:**
- Create: `docs/kpilot/TEST-BASELINE.md`
- Modify only if required for local path configuration: `CMakeUserPresets.json`

- [ ] **Step 1: Initialize all upstream submodules**

Run:

```bash
git submodule update --init --recursive
git submodule status --recursive
```

Expected: every required submodule has a commit SHA and none begins with `-`.

- [ ] **Step 2: Configure outside the source tree**

Run the platform-appropriate documented xPilot configuration command, using `build/kpilot-baseline` as the build directory. On macOS with Qt discoverable through `Qt6_HOME`, use:

```bash
cmake -S client -B build/kpilot-baseline/client -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake -S plugin -B build/kpilot-baseline/plugin -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

Expected: both commands finish with `Configuring done` and `Generating done`. If a dependency is missing, stop and record the exact error; do not modify product code to bypass it.

- [ ] **Step 3: Build the unchanged client and plugin**

Run:

```bash
cmake --build build/kpilot-baseline/client --parallel
cmake --build build/kpilot-baseline/plugin --parallel
```

Expected: exit code 0 and unmodified xPilot client/plugin artifacts.

- [ ] **Step 4: Run all available tests and a QML startup smoke test**

Run:

```bash
ctest --test-dir build/kpilot-baseline/client --output-on-failure
ctest --test-dir build/kpilot-baseline/plugin --output-on-failure
```

Expected: all discovered tests pass. If the upstream project reports `No tests were found`, record that as a baseline gap and require Phase 1 to introduce Qt Test coverage before behavior changes.

- [ ] **Step 5: Record exact evidence**

Create `docs/kpilot/TEST-BASELINE.md` with the following headings. Under each heading, paste only the exact commands already executed and their observed output, exit status, or artifact path; do not write estimates or future work:

```markdown
# Test Baseline

## Revisions
- xPilot: `4bd5b8079e43d130c87ed5df8e32e86822e06a27`
- swift reference: `29ecd719b7c4d0e6db30518685d70ef70a09d841`

## Configure

## Build

## Tests

## Known Baseline Gaps
```

- [ ] **Step 6: Verify no product source changed during baseline work**

Run:

```bash
git status --short
git diff -- client plugin
```

Expected: no modifications under `client/` or `plugin/`; only baseline documentation, scripts, and optional ignored build output exist.

- [ ] **Step 7: Commit the baseline evidence**

```bash
git add docs/kpilot/TEST-BASELINE.md
git commit -m "test: record unmodified xPilot baseline"
```

### Task 4: Capture Production Identity Without Changing It

**Files:**
- Create: `docs/kpilot/PRODUCTION-SAFETY.md`

- [ ] **Step 1: Write the allowed read-only NAS command set**

Create `docs/kpilot/PRODUCTION-SAFETY.md`:

```markdown
# Production Safety

## Allowed Before Phase 6

- `docker ps --no-trunc`
- `docker inspect` for container IDs returned by `docker ps --no-trunc`
- `docker logs` for container IDs returned by `docker ps --no-trunc`
- read-only health/API requests
- image digest and compose-file inspection

## Forbidden Before Phase 6

- `docker stop`, `restart`, `rm`, `kill`, `update`, `rename`, or `compose up/down`
- writes to production volumes or database files
- host-port reuse
- production configuration edits

## Shadow Deployment Rules

The shadow server must use unique container names, different host ports, isolated writable storage, health checks, resource limits, and synthetic or copied test users. A shadow failure must have no effect on production.
```

- [ ] **Step 2: If the NAS is reachable, collect only read-only identity**

Run `docker ps --no-trunc`, copy the exact FSD and web container IDs from that output, and pass those IDs unchanged to two read-only `docker inspect` invocations. Do not use name guesses or lifecycle commands.

```bash
docker ps --no-trunc
```

Expected: current container names, image IDs, digests, ports, mounts, restart policies, and health status are captured. If unreachable, record `NAS unreachable` and leave production untouched.

- [ ] **Step 3: Append the observed production identity**

Append factual results under `## Current Production Identity`. Do not include passwords, tokens, private keys, or database contents.

- [ ] **Step 4: Commit the safety contract**

```bash
git add docs/kpilot/PRODUCTION-SAFETY.md
git commit -m "docs: define K-Pilot production safety gates"
```

### Task 5: Close Phase 0

**Files:**
- Modify: `docs/superpowers/plans/2026-09-10-kpilot-roadmap.md`
- Create: `docs/kpilot/PHASE-0-RESULT.md`

- [ ] **Step 1: Run the phase verification**

```bash
git status --short
git log --oneline -5
rg -n 'TBD|TODO|PLACEHOLDER' docs/kpilot
```

Expected: clean tracked worktree, expected Phase 0 commits, and no unresolved placeholders in completed evidence files.

- [ ] **Step 2: Write the phase result**

Create `docs/kpilot/PHASE-0-RESULT.md` with the following fields, populated only from evidence produced in Tasks 1-4:

```text
Phase: 0 - Baseline
Commit:
Artifacts: upstream, license, build, test, and production-safety records
Commands executed:
Test results:
Known limitations:
Go/no-go decision: GO only if the unchanged baseline builds; otherwise NO-GO with the exact dependency blocker
Next plan: Phase 1 private server configuration
```

- [ ] **Step 3: Mark completed roadmap safety and Phase 0 items**

Change only checkboxes supported by evidence from `- [ ]` to `- [x]`.

- [ ] **Step 4: Commit Phase 0 closure**

```bash
git add docs/superpowers/plans/2026-09-10-kpilot-roadmap.md docs/kpilot/PHASE-0-RESULT.md
git commit -m "docs: close K-Pilot phase 0"
```
