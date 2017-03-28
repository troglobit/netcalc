Change Log
==========

All notable changes to the project are documented in this file.


[v2.1][] - 2017-03-28
---------------------

### Changes
- Added basic Debian packaging, no ext. library dependencies
- Support for disabling `ipcalc` symlink in `configure` script
- Add Tobias Waldekranz's awesome little AWK script to GIT
- Add `-n` command line option to disable colorization of output
- Add `-c` command line option to validate IP addreses.  Feature
  courtesy of Lonnie Abelbeck
- Auto-disable colorization if run over a pipe, by Lonnie Abelbeck
- Add `-R MIN:MAX` command line option to show an offset IPv4 network
  range, from MIN to MAX.  Feature courtesy of Lonnie Abelbeck

### Fixes
- Don't prefix "Compressed IPv6" with previous output, by Vincent Bernat
- Issue #4: Unit test script missing from released tarball
- Don't enforce automake v1.11, only check >= v1.11
- Adaptions for musl libc
- Fix issue #7: not all arguments supported on `stdin`, too small input
  buffer(s), by Lonnie Abelbeck
- Fix issue #13: Add IPv6 *Unique Local Address* address types


[v2.0][] - 2015-08-19
---------------------

First release of netcalc, cloned from sipcalc.

### Changes
- Project cloned from sipcalc into netcalc
- Greatly simplified and reduced feature set, no more interfaces ...
- Output format changed to that of the famous ipcalc tool
- File names changed to protect innocent children
- Coding style changed to pure Linux KNF, to protect
  future generations of programmers
- Lots of bugs introduced

[v1.1.6][] - 2013-01-14
-----------------------

### Fixes
- Fix multiple buffer overflows. Patch from Jaromir Capik.
- Fix multiple spelling errors. Suggestions from Ross Richardson.

[v1.1.5][] - 2009-07-20
-----------------------

### Fixes
- Compilation fix for GCC 4.4, patch from Artem Zolochevskiy.
- Compilation fix Sun Studio 11 on Solaris 8 x86, patch from Ian Dickinson.

[v1.1.4][] - 2009-11-10
-----------------------

### Fixes
- Fixed a bunch of compiler warnings when building with `-Wall`.  Thanks
  to the Debian package maintainer Adriaan Peeters for the heads up.

[v1.1.3][] - 2005-11-02
-----------------------

### Changes
- Add `-w ipv4` option to display some inverse mask information.

### Fixes
- Fix typos in man page, thanks to A Costa <agcosta@gis.net> for patch.

[v1.1.2][] - 2003-03-19
-----------------------

### Changes
- Changed stuff around to be compatible with newer version of autoconf.
- Updated ipv6 reverse dns output to use the domain ip6.arpa instead of
  ip6.int, thanks to Måns Nilsson for bringing this to my attention.
- Change license from GPL to BSD, see COPYING for details.

[v1.1.1][] - 2002-05-24
-----------------------

### Changes
- Removed web (cgi) support for now.
- Rewrote much of the argument handling.
- Rewrote much of the DNS parsing.
- Multiple addresses for a resolved host are now supported, both with
  ipv4 and ipv6.

### Fixes
- Some minor bug fixes.
- Verified MacOSX support, no ipv6 dns resolution though.
- DNS resolution now works with stdin parsing.
- Fixed improper parsing of subnetmask in some cases where dns
  resolution and the -4 argument was used.

[v1.1.0][] - 2002-03-11
-----------------------

### Changes
- IPv6 support.
- Updated the output to try to get it more orderly.
- Hostname resolution (-d).
- Added explicit adding of v4/v6/interface addresses on the commandline
  (-4, -6, -I).
- Verbose network split, enables recursive runs through sipcalc of split
  networks (-u).

### Fixes
- Lots of bugfixes.
- Fixed sipcalc cgi-script.

[v1.0.3][] - 2001-12-13
-----------------------

### Changes
- Finally added a manpage.
- Several minor changes to documentation

[v1.0.2][] - 2001-07-13
-----------------------

This should really be good enough for a stable release, though
sipcalc-www needs more cleaning.

### Changes
- Some minor cosmetic changes.
- Added split-network to sipcalc-www.
- Split-network commandline parsing bugfixes.
- Build process now uses configure.
- Support for stdin parsing complete.
- Multiple addresses/interfaces on commandline supported.
- Changed to getopt for argument handling.

### Fixes
- Lots of Solaris compile fixes: `u_int32_t`, `libsocket`,
  `SIOCGIFADDR`, ..., sipcalc compiles fine on Solaris now.
- Fix `getopt_long()` support on FreeBSD.
- Interface detection was broken on Linux-alpha due to one of the unions
  in struct ifreq being larger on 64bit then 32bit architectures.
  Should now hopefully be fixed in a completely portable manner.
- Lots of minor fixes for multiple architectures (made good use of the
  sourceforge compile farm).
- The interface code now (hopefully) handles interfaces with multiple
  addresses correctly.

[v1.0.1][] - 2001-05-02
-----------------------

### Changes
- Use `u_int32_t` for portability, this also fixed alpha support (tru64)
- Add support for "reverse engineering" of interfaces.
- First commit of web (cgi) support.
- Initial commit, 2001-02-10 16:55:14 CEST!

[UNRELEASED]: https://github.com/troglobit/netcalc/compare/v2.1...HEAD
[v2.1]: https://github.com/troglobit/netcalc/compare/v2.0...v2.1
[v2.0]: https://github.com/troglobit/netcalc/compare/v1.1.6...v2.0
[v1.1.6]: https://github.com/troglobit/netcalc/compare/v1.1.5...v1.1.6
[v1.1.5]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.1.5.tar.gz
[v1.1.4]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.1.4.tar.gz
[v1.1.3]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.1.3.tar.gz
[v1.1.2]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.1.2.tar.gz
[v1.1.1]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.1.1.tar.gz
[v1.1.0]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.1.0.tar.gz
[v1.0.3]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.0.3.tar.gz
[v1.0.2]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.0.2.tar.gz
[v1.0.1]: http://www.routemeister.net/projects/sipcalc/files/sipcalc-1.0.1.tar.gz

<!--
  -- Local Variables:
  -- mode: markdown
  -- End:
  -->
