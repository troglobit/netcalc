#!/bin/sh
# Extremely simplistic test ...

TMP=`mktemp /tmp/XXXXXX`
VERBOSE=0
if [ x"$1" = x"-v" ]; then
    VERBOSE=1
fi

testit()
{
    grep "$1 $2" $TMP >/dev/null
    if [ $? -ne 0 ]; then
	if [ $VERBOSE -eq 1 ]; then
	    echo "Invalid output from netcalc `grep \"$1\" $TMP |head -1`"
	    echo "Should be: $2"
	    echo "Test FAIL! :-("
	fi
	exit 1
    fi
}

# Filter out ANSI escape codes so testit() doesn't barf on those
./src/netcalc 192.168.1.0/24 -s 25 | sed -r "s:\x1B\[[0-9;]*[mK]::g" > $TMP

testit "Netmask  :" "255.255.255.0 = 24"
testit "HostMin  :" "192.168.1.1"
testit "HostMax  :" "192.168.1.254"
testit "Network  :" "192.168.1.128 - 192.168.1.255    Netmask  : 255.255.255.128"

rm $TMP
if [ $VERBOSE -eq 1 ]; then
    echo "Test OK! :-)"
fi
exit 0
