AC_INIT([netcalc], [2.1.7-rc1], [https://github.com/troglobit/netcalc/issues])
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/netcalc.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])

AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL

# Check for headers needed for portability
AC_CHECK_HEADERS([getopt.h sys/socket.h])

# Check for uint[8,16,32]_t
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Check for usually missing API's
AC_REPLACE_FUNCS([strlcat strlcpy])
AC_CONFIG_LIBOBJ_DIR([lib])

# Check configure options
AC_ARG_ENABLE(ipcalc-symlink,
        AS_HELP_STRING([--disable-ipcalc-symlink], [Disable ipcalc symlink, default enabled]))

AM_CONDITIONAL(SYMLINK, [test "x$enable_ipcalc_symlink" != "xno"])

AC_OUTPUT
