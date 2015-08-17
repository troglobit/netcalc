EXTRA_DIST      = configure
doc_DATA	= README.md LICENSE
bin_PROGRAMS    = netcalc
netcalc_SOURCES = sub-func.c sub-output.c sub.c strlcpy.c strlcat.c

sub-func.o: sub-func.c sub.h sub-o.h
sub-output.o: sub-output.c sub.h
sub.o: sub.c sub.h sub-o.h

