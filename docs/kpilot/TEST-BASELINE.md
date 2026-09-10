# Test Baseline

## Revisions

- xPilot: `4bd5b8079e43d130c87ed5df8e32e86822e06a27`
- swift reference: `29ecd719b7c4d0e6db30518685d70ef70a09d841`
- K-Pilot planning baseline: `78c88530159a2064b64741838873223a28dd8a0f`

## Configure

Both commands exited with status 0 and reported `Configuring done` and
`Generating done`:

```bash
Qt6_HOME=/opt/homebrew/opt/qt cmake -S client -B build/kpilot-baseline/client -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
Qt6_HOME=/opt/homebrew/opt/qt cmake -S plugin -B build/kpilot-baseline/plugin -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

## Build

```bash
cmake --build build/kpilot-baseline/plugin --parallel 8
```

Exit status 0. Artifact:
`build/kpilot-baseline/plugin/mac_x64/xPilot.xpl`, a universal x86_64/arm64
Mach-O bundle.

```bash
cmake --build build/kpilot-baseline/client --parallel 8
```

Exit status 1. Qt 6.11.2 rejects
`client/src/audio/notification_sound_engine.h` because it defines methods using
`QVariant` and `QVariantMap` without including their complete declarations.

## Tests

```bash
ctest --test-dir build/kpilot-baseline/client --output-on-failure
ctest --test-dir build/kpilot-baseline/plugin --output-on-failure
```

Both commands exited with status 0 and reported `No tests were found`.

## Known Baseline Gaps

- Upstream provides no CTest tests for either target at this revision.
- The desktop client requires an explicit Qt 6.11 include compatibility fix
  before it can complete a build.
- `afv-native` pins `xpilot-project/speexdsp` commit
  `de18e74b5e2668531f60dfdd9e9af4a922a8efa4`, but that repository is no longer
  publicly available. The development baseline temporarily uses the official
  Xiph SpeexDSP mirror at `7a158783df74efe7c2d1c6ee8363c1e695c71226` for
  headers while retaining xPilot's pinned prebuilt library. This substitution
  is not approved for a release build.
