# VATSIM Velocity Validation

## Verdict

K-Pilot and the modified OpenFSD server implement the same public Velocity
mechanism used by xPilot on VATSIM:

1. A protocol revision 101 pilot logs in.
2. The server sends `$SF...:1` when another revision 101 pilot is within 5 NM.
3. The client sends `^` position and velocity packets at 5 Hz.
4. The server forwards those packets only to nearby revision 101 clients.
5. The receiving client extrapolates position every render frame and applies
   position error correction over two seconds.

Legacy clients remain supported through ordinary `@` packets, but they do not
receive the 5 Hz Velocity path and therefore cannot achieve the same formation
accuracy.

## Quantitative Bound

At 250 knots and 250 ms round-trip latency, a conservative one-way-delay model
gives these maximum packet-age distances before client-side extrapolation:

| Path | Update interval | Conservative packet age | Distance at 250 kt |
|---|---:|---:|---:|
| Legacy FSD | 5.0 s | 5.25 s | 675 m |
| Velocity | 0.2 s | 0.45 s | 58 m |

This is about a 91% reduction in the raw trailing-distance bound. It does not
constitute a guarantee of 90% absolute positional accuracy in every scenario.
Packet loss, jitter, simulator model origins, terrain clamping, and incorrect
source velocity can still add error.

## Source Trace

- `NetworkManager` sends protocol revision 101 and 5 Hz position/velocity data.
- `openfsd` enables fast packets inside 5 NM and forwards them between revision
  101 clients.
- `NetworkAircraft` extrapolates the received velocity each frame, clears stale
  rotational velocity after 500 ms, and corrects accumulated position error
  over two seconds.

## Release Gate

Protocol equivalence is confirmed. Production deployment still requires packet
replay at 60-250 ms RTT with jitter/loss, an X-Plane formation test, and a
shadow-server test. No production service may be changed before those gates.
