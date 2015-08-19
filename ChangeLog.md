[v2.0][] - UNRELEASED
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

<!--
  -- Local Variables:
  -- mode: markdown
  -- End:
  -->
