/*-------------------------------------------------------------------------
 *
 * test_spi_func.c
 *		SQL-callable C function that uses SPI to execute a query.
 *
 *		Useful for testing code paths that only trigger under C-level
 *		SPI (not PL/pgSQL), such as resource owner interactions with
 *		RI fast-path FK checks.
 *
 * Copyright (c) 2026, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *		src/test/modules/test_spi_func/test_spi_func.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/spi.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(spi_exec);

/*
 * spi_exec(query text) - execute a SQL query via SPI.
 *
 * Opens a fresh SPI connection, executes the query, and closes the
 * connection.  This mimics the SPI usage pattern of C-language
 * extensions (e.g., PostGIS topology functions) where each call
 * to SPI_connect / SPI_execute / SPI_finish creates and destroys
 * a short-lived SPI context.
 */
Datum
spi_exec(PG_FUNCTION_ARGS)
{
	const char *query = text_to_cstring(PG_GETARG_TEXT_PP(0));
	int			ret;

	SPI_connect();

	ret = SPI_execute(query, false, 0);

	if (ret < 0)
		elog(ERROR, "SPI_execute failed: error code %d", ret);

	SPI_finish();

	PG_RETURN_VOID();
}
