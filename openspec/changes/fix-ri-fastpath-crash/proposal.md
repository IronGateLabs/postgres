# Fix RI fast-path crash in PG 19devel

## Problem

PostgreSQL 19devel introduced a fast-path optimization for referential integrity
(FK) trigger processing that eliminates SPI overhead. This optimization has a bug
in index reference count management that causes a server crash (`SIGABRT`) when
multiple immediate FK constraints fire during SPI-based inserts.

## Discovery

Found via PostGIS topology CI. The `toTopoGeom()` function uses SPI to insert
into the `edge_data` table which has 6 FK constraints referencing `node` and
`face` tables. PG 18 with the same GEOS version passes; PG 19devel crashes.

## Crash Details

```
SIGABRT in ri_FastPathTeardown
  → index_close
  → RelationClose
  → RelationDecrementReferenceCount
  → Assert failure: rel->rd_refcnt > 0
```

The `ri_FastPathEndBatch` / `ri_FastPathTeardown` code closes an FK index whose
reference count is already zero, likely due to double-close or missing increment
when batching multiple FK checks on the same relation.

## Scope

1. **Validate** — build PG 19devel, confirm crash with minimal reproduction
2. **Locate** — find the exact bug in `src/backend/utils/adt/ri_triggers.c`
3. **Fix** — correct the reference count management
4. **Test** — verify PostGIS topology test passes, add PG regression test
5. **Submit** — patch to pgsql-hackers mailing list

## Minimal Reproduction

```sql
CREATE TABLE parent1 (id serial PRIMARY KEY);
CREATE TABLE parent2 (id serial PRIMARY KEY);
CREATE TABLE parent3 (id serial PRIMARY KEY);
INSERT INTO parent1 VALUES (1);
INSERT INTO parent2 VALUES (1);
INSERT INTO parent3 VALUES (1);

CREATE TABLE child (
    id serial PRIMARY KEY,
    p1_id int REFERENCES parent1(id),
    p2_id int REFERENCES parent2(id),
    p3_id int REFERENCES parent3(id),
    p4_id int REFERENCES parent1(id),
    p5_id int REFERENCES parent2(id),
    p6_id int REFERENCES parent3(id)
);

CREATE FUNCTION insert_via_spi() RETURNS void AS $$
BEGIN
    INSERT INTO child (p1_id, p2_id, p3_id, p4_id, p5_id, p6_id)
    VALUES (1, 1, 1, 1, 1, 1);
END;
$$ LANGUAGE plpgsql;

-- Expected: succeeds. Actual on PG 19devel: SIGABRT
SELECT insert_via_spi();
```

## Related

- IronGateLabs/postgres#1
- IronGateLabs/postgis#12
- IronGateLabs/postgis#11 (PostGIS workaround: deferred FK constraints)
