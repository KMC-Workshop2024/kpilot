# Production Safety

## Allowed Before Phase 6

- `docker ps --no-trunc`
- `docker inspect` for container IDs returned by `docker ps --no-trunc`
- `docker logs` for container IDs returned by `docker ps --no-trunc`
- Read-only health/API requests
- Image digest and compose-file inspection

## Forbidden Before Phase 6

- `docker stop`, `restart`, `rm`, `kill`, `update`, `rename`, or
  `compose up/down`
- Writes to production volumes or database files
- Host-port reuse
- Production configuration edits

## Shadow Deployment Rules

The shadow server must use unique container names, different host ports,
isolated writable storage, health checks, resource limits, and synthetic or
copied test users. A shadow failure must have no effect on production.

## Current Production Identity

On 2026-09-11, TCP connection to the NAS SSH endpoint timed out without a
response. Production container identity was therefore not collected, and no
login, Docker command, upload, or production change was attempted.
