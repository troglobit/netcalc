AUTOMAKE_OPTIONS= subdir-objects
bin_PROGRAMS    = netcalc
netcalc_CFLAGS  = -W -Wall -Wextra
netcalc_SOURCES = netcalc.c netcalc.h output.c output.h misc.c cprintf.c
netcalc_LDADD   = $(LIBS) $(LIBOBJS)

if SYMLINK
install-exec-hook:
	(cd $(DESTDIR)$(bindir); $(LN_S) netcalc ipcalc)
endif
