# Use v2.61 for backwards compat with Ubuntu 12.04 LTS
AC_PREREQ([2.61])
AC_INIT([netcalc],[2.0.1-pre],[https://github.com/troglobit/netcalc/issues])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([netcalc.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL

dnl - Configuration.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h stdint.h)
AC_CHECK_FUNCS(strstr strtol strtoul getopt)

dnl Check for uint[8,16,32]_t
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

AC_OUTPUT
