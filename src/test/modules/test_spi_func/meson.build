test_spi_func_sources = files(
  'test_spi_func.c',
)

if host_system == 'windows'
  test_spi_func_sources += rc_lib_gen.process(win32ver_rc, extra_args: [
    '--NAME', 'test_spi_func',
    '--FILEDESC', 'test_spi_func - SQL-callable C SPI function',])
endif

test_spi_func = shared_module('test_spi_func',
  test_spi_func_sources,
  kwargs: pg_test_mod_args,
)
test_install_libs += test_spi_func

test_install_data += files(
  'test_spi_func.control',
  'test_spi_func--1.0.sql',
)

tests += {
  'name': 'test_spi_func',
  'sd': meson.current_source_dir(),
  'bd': meson.current_build_dir(),
  'regress': {
    'sql': [
      'ri_fastpath',
    ],
  },
}
