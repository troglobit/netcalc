SUBDIRS         = src
TESTS           = testit.sh
EXTRA_DIST      = README.md LICENSE ChangeLog.md testit.sh
doc_DATA        = README.md LICENSE
dist_man1_MANS  = netcalc.1

# Compat rule for Travis, default automake is "make check" for autotests
test: check

package build-deb:
	dpkg-buildpackage -uc -us -B

## Target to run when building a release
release: distcheck dist build-deb
	@for file in $(DIST_ARCHIVES); do	\
		md5sum $$file > ../$$file.md5;	\
	done
	@mv $(DIST_ARCHIVES) ../
	@echo
	@echo "Resulting release files in ../"
	@echo "================================================================="
	@for file in $(DIST_ARCHIVES); do					\
		printf "%-32s Distribution tarball\n" $$file;			\
		printf "%-32s " $$file.md5; cat ../$$file.md5 | cut -f1 -d' ';	\
	done
	@for file in `cd ..; ls $(PACKAGE)_$(VERSION)-*`; do			\
		printf "%-32s Debian/Ubuntu file\n" $$file;			\
	done
