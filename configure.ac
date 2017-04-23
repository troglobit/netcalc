AC_INIT([netcalc],[2.1.2],[https://github.com/troglobit/netcalc/issues])
AM_INIT_AUTOMAKE([1.11 foreign])

AC_CONFIG_SRCDIR([netcalc.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL

dnl - Configuration.
AC_HEADER_STDC

dnl Check for uint[8,16,32]_t
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

AC_ARG_ENABLE(ipcalc-symlink,
        AS_HELP_STRING([--disable-ipcalc-symlink], [Disable ipcalc symlink, default enabled]))

AM_CONDITIONAL(SYMLINK, [test "x$enable_ipcalc_symlink" != "xno"])

AC_OUTPUT
