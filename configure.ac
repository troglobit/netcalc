AC_PREREQ([2.69])
AC_INIT([netcalc],[2.0-alpha1],[https://github.com/troglobit/netcalc/issues])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([sub.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_INSTALL

dnl - Configuration.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h stdint.h)

AC_CHECK_FUNCS(strstr strtol strtoul)

dnl Check if getopt_long is available.
AC_DEFUN([sii_GETOPT_LONG],
[
  AC_CHECK_HEADERS(getopt.h,
  [
    AC_DEFINE(HAVE_GETOPT_H, 1, getopt_h)
  ],
  [
    if test -f /usr/local/include/getopt.h;
    then
      echo checking for getopt.h in /usr/local/include... yes
      AC_DEFINE(HAVE_GETOPT_H, 1, getopt_h)
    fi
  ])
  AC_CHECK_FUNCS(getopt_long,,
  [
    AC_CHECK_LIB(gnugetopt, getopt_long,
    [
      AC_DEFINE(HAVE_GETOPT_LONG, 1, getopt_long)
      LIBS="$LIBS -lgnugetopt"
    ])
  ])
])


dnl Check for uint[8,16,32]_t
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

sii_GETOPT_LONG

AC_OUTPUT
