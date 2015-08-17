EXEC            = netcalc
SYMLINK         = ipcalc
EXTRA_DIST      = configure README.md LICENSE
doc_DATA        = README.md LICENSE
dist_man1_MANS  = $(EXEC).1
bin_PROGRAMS    = $(EXEC)
netcalc_SOURCES = netcalc.c netcalc.h output.c output.h misc.c strlcpy.c strlcat.c

install-exec-hook:
	(cd $(DESTDIR)$(bindir); $(LN_S) $(EXEC) $(SYMLINK))
