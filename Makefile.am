EXEC            = netcalc
SYMLINK         = ipcalc
EXTRA_DIST      = configure
doc_DATA	= README.md LICENSE
bin_PROGRAMS    = $(EXEC)
netcalc_SOURCES = netcalc.c output.c misc.c strlcpy.c strlcat.c

install-exec-hook:
	(cd $(DESTDIR)$(bindir); $(LN_S) $(EXEC) $(SYMLINK))
