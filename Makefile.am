SUBDIRS         = src tests
EXTRA_DIST      = README.md LICENSE ChangeLog.md
doc_DATA        = README.md LICENSE
dist_man1_MANS  = netcalc.1

## Generate .deb package
package:
	@debuild -uc -us -B --lintian-opts --profile debian -i -I --show-overrides

## Target to run when building a release
release: distcheck
	@for file in $(DIST_ARCHIVES); do		\
		md5sum    $$file > ../$$file.md5;	\
		sha256sum $$file > ../$$file.sha256;	\
	done
	@mv $(DIST_ARCHIVES) ../
	@echo
	@echo "Resulting release files:"
	@echo "================================================================="
	@for file in $(DIST_ARCHIVES); do						\
		printf "%-32s Distribution tarball\n" $$file;				\
		printf "%-32s " $$file.md5;    cat ../$$file.md5    | cut -f1 -d' ';	\
		printf "%-32s " $$file.sha256; cat ../$$file.sha256 | cut -f1 -d' ';	\
	done
