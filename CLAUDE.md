# CLAUDE.md — PostgreSQL

PostgreSQL is an advanced object-relational database management system (version 19devel).

## Build

PostgreSQL supports two build systems: **Meson** (preferred) and **Autoconf/Make**.

### Meson (recommended)

```bash
meson setup build
meson compile -C build
meson install -C build
```

### Autoconf/Make

```bash
./configure
make
make install
```

## Tests

### Meson

```bash
meson test -C build                    # Core regression tests
meson test -C build --suite regress    # Regression suite only
```

### Make

```bash
make check           # Core regression tests (starts a temp server)
make check-world     # All tests (regression, isolation, TAP, contrib)
```

### Key test directories

- `src/test/regress/` — Main SQL regression tests (pg_regress)
- `src/test/isolation/` — Concurrency/isolation tests
- `src/test/recovery/` — Recovery and replication tests
- `src/test/perl/` — Perl TAP test infrastructure
- `src/test/authentication/`, `src/test/ssl/`, `src/test/ldap/`, `src/test/kerberos/` — Feature-specific tests
- `contrib/*/` — Each contrib module has its own tests

## Code Style

- **C code:** Tabs, width 4. Run `src/tools/pgindent/pgindent` to format.
- **Python:** Spaces, width 4.
- **Perl:** Tabs, width 4.
- See `.editorconfig` for full details.

Use `pgindent --check <file>` to verify formatting (exits 2 if changes needed).

## Repository Layout

```
src/backend/       Core database engine (parser, optimizer, executor, storage, catalog, replication, etc.)
src/bin/           CLI tools (psql, pg_dump, initdb, pg_basebackup, etc.)
src/include/       Header files
src/interfaces/    Client libraries (libpq, ecpg)
src/test/          Test suites
src/tools/         Developer tools (pgindent, find_typedef, etc.)
src/pl/            Procedural languages (PL/pgSQL, PL/Perl, PL/Python, PL/Tcl)
contrib/           Optional extension modules (62+)
doc/               Documentation (SGML)
```

## Key Conventions

- C11 standard
- BSD-style license (see `COPYRIGHT`)
- Commit messages reference mailing list discussions
- Patches go through the pgsql-hackers mailing list
- Run `pgindent` before submitting patches
