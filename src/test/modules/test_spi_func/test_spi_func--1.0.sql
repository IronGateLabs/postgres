/* src/test/modules/test_spi_func/test_spi_func--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION test_spi_func" to load this file. \quit

CREATE FUNCTION spi_exec(query text)
RETURNS void
AS 'MODULE_PATHNAME', 'spi_exec'
LANGUAGE C STRICT;
