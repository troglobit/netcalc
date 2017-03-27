EXEC            = netcalc
SYMLINK         = ipcalc
TESTS           = testit.sh
EXTRA_DIST      = configure README.md LICENSE testit.sh
doc_DATA        = README.md LICENSE ChangeLog.md
dist_man1_MANS  = $(EXEC).1
bin_PROGRAMS    = $(EXEC)
netcalc_SOURCES = netcalc.c netcalc.h output.c output.h \
		  misc.c strlcpy.c strlcat.c cprintf.c
netcalc_CFLAGS  = -W -Wall -Wextra

# Compat rule for Travis, default automake is "make check" for autotests
test: check

# Hook in to install to add netcalc --> ipcalc symlink
if SYMLINK
install-exec-hook:
	(cd $(DESTDIR)$(bindir); $(LN_S) $(EXEC) $(SYMLINK))
endif

package build-deb:
	dpkg-buildpackage -uc -us -B

## Target to run when building a release
release: dist build-deb
	@for file in $(DIST_ARCHIVES); do	\
		md5sum $$file > ../$$file.md5;	\
	done
	@mv $(DIST_ARCHIVES) ../
	@echo
	@echo "Resulting release files:"
	@echo "================================================================="
	@for file in $(DIST_ARCHIVES); do					\
		printf "$$file    \tDistribution tarball\n";			\
		printf "$$file.md5\t"; cat ../$$file.md5 | cut -f1 -d' ';	\
	done
	@for file in `ls ../$(PACKAGE)_$(VERSION)-*`; do			\
		printf "$$file    \tDebian/Ubuntu file\n";			\
	done
