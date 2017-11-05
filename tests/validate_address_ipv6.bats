#!/usr/bin/env bats

# Test IPv6 address formats that should be valid

@test "netcalc -c IPv6 ordinary address" {
  run ../src/netcalc -c 1:2:3:4:5:6:7:8
  [ $status -eq 0 ]
  [[ $output = "1:2:3:4:5:6:7:8" ]]
}

@test "netcalc -c IPv6 ordinary address with some zeroes uncompressed" {
  run ../src/netcalc -c 1:2:0:4:5:0:0:8
  [ $status -eq 0 ]
  [[ $output = "1:2:0:4:5:0:0:8" ]]
}

@test "netcalc -c IPv6 ordinary address with ::" {
  run ../src/netcalc -c fe80::12ab:34cd:56:78d
  [ $status -eq 0 ]
  [[ $output = "fe80::12ab:34cd:56:78d" ]]
}

@test "netcalc -c IPv6 address with zero-filled components" {
  run ../src/netcalc -c fe80::12ab:34cd:0056:078d
  [ $status -eq 0 ]
  [[ $output = "fe80::12ab:34cd:0056:078d" ]]
}

@test "netcalc -c IPv6 address with 0 CIDR" {
  run ../src/netcalc -c fe80::a:b:c:1/0
  [ $status -eq 0 ]
  [[ $output = "fe80::a:b:c:1/0" ]]
}

@test "netcalc -c IPv6 address with 48 CIDR" {
  run ../src/netcalc -c fe80::a:b:c:1/48
  [ $status -eq 0 ]
  [[ $output = "fe80::a:b:c:1/48" ]]
}

@test "netcalc -c IPv6 address with 64 CIDR" {
  run ../src/netcalc -c fe80::a:b:c:1/64
  [ $status -eq 0 ]
  [[ $output = "fe80::a:b:c:1/64" ]]
}

@test "netcalc -c IPv6 address with 128 CIDR" {
  run ../src/netcalc -c fe80::a:b:c:1/128
  [ $status -eq 0 ]
  [[ $output = "fe80::a:b:c:1/128" ]]
}

@test "netcalc -c IPv6 zero address full form" {
  run ../src/netcalc -c 0:0:0:0:0:0:0:0
  [ $status -eq 0 ]
  [[ $output = "0:0:0:0:0:0:0:0" ]]
}

@test "netcalc -c IPv6 zero address 0::0" {
  run ../src/netcalc -c 0::0
  [ $status -eq 0 ]
  [[ $output = "0::0" ]]
}

@test "netcalc -c IPv6 zero address 0::" {
  run ../src/netcalc -c 0::
  [ $status -eq 0 ]
  [[ $output = "0::" ]]
}

@test "netcalc -c IPv6 zero address ::0" {
  run ../src/netcalc -c ::0
  [ $status -eq 0 ]
  [[ $output = "::0" ]]
}

@test "netcalc -c IPv6 zero address ::" {
  run ../src/netcalc -c ::
  [ $status -eq 0 ]
  [[ $output = "::" ]]
}

@test "netcalc -c IPv6 zero address :: with 0 CIDR" {
  run ../src/netcalc -c ::/0
  [ $status -eq 0 ]
  [[ $output = "::/0" ]]
}

@test "netcalc -c IPv6 zero address :: with 64 CIDR" {
  run ../src/netcalc -c ::/64
  [ $status -eq 0 ]
  [[ $output = "::/64" ]]
}

@test "netcalc -c IPv6 zero address :: with 128 CIDR" {
  run ../src/netcalc -c ::/128
  [ $status -eq 0 ]
  [[ $output = "::/128" ]]
}

@test "netcalc -c IPv6 all-ones address" {
  run ../src/netcalc -c ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff
  [ $status -eq 0 ]
  [[ $output = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff" ]]
}

@test "netcalc -c IPv6 all-ones address with 0 CIDR" {
  run ../src/netcalc -c ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/0
  [ $status -eq 0 ]
  [[ $output = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/0" ]]
}

@test "netcalc -c IPv6 all-ones address with 64 CIDR" {
  run ../src/netcalc -c ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/64
  [ $status -eq 0 ]
  [[ $output = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/64" ]]
}

@test "netcalc -c IPv6 all-ones address with 128 CIDR" {
  run ../src/netcalc -c ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128
  [ $status -eq 0 ]
  [[ $output = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128" ]]
}

@test "netcalc -c IPv6 address with ambiguous :: (1)" {
  run ../src/netcalc -c 1::4:5:0:0:8
  [ $status -eq 0 ]
  [[ $output = "1::4:5:0:0:8" ]]
}

@test "netcalc -c IPv6 address with ambiguous :: (2)" {
  run ../src/netcalc -c 1:0:0:4:5::8
  [ $status -eq 0 ]
  [[ $output = "1:0:0:4:5::8" ]]
}

@test "netcalc -c IPv6 address with zeroes and ::" {
  run ../src/netcalc -c 1:0:3::6:0:8
  [ $status -eq 0 ]
  [[ $output = "1:0:3::6:0:8" ]]
}

# Note: really this should be an invalid IPv6 address
# 1::4:0:0:0:8 => 1:0:0:4:0:0:0:8 which in compressed form
# should be rendered 1:0:0:4::8
@test "netcalc -c IPv6 address with non-minimal :: zero compression" {
  run ../src/netcalc -c 1::4:0:0:0:8
  [ $status -eq 0 ]
  [[ $output = "1::4:0:0:0:8" ]]
}

# Note: really this should be an invalid IPv6 address
# 1:2:3::0:7:8 should be compressed to 1:2:3::7:8
@test "netcalc -c IPv6 address with non-complete :: zero compression" {
  run ../src/netcalc -c 1:2:3::0:7:8
  [ $status -eq 0 ]
  [[ $output = "1:2:3::0:7:8" ]]
}

# Tests for things that look a bit like an IPv6 address
# but are actually invalid

@test "netcalc -c IPv6 CIDR out of range" {
  run ../src/netcalc -c fe80::12ab:34cd:56:78d/129
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 negative CIDR" {
  run ../src/netcalc -c fe80::12ab:34cd:56:78d/-1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 slash but no CIDR" {
  run ../src/netcalc -c fe80::12ab:34cd:56:78d/
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 with too many components" {
  run ../src/netcalc -c fe80:2:3:12ab:34cd:56:78d:8:1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 with alpha CIDR" {
  run ../src/netcalc -c fe80::12ab:34cd:56:78d/aa
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 with following non-hex" {
  run ../src/netcalc -c fe80::12ab:34cd:56:78dx
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 component 1 out of range" {
  run ../src/netcalc -c 11111:a:b::1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 mid component out of range" {
  run ../src/netcalc -c 1:a:33333::1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 last component out of range" {
  run ../src/netcalc -c 1:2::7:88888
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 with non-hex" {
  run ../src/netcalc -c 1:2:3g:4::1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 not enough octets" {
  run ../src/netcalc -c 1:2:3:4:5:6:7
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 ending single colon" {
  run ../src/netcalc -c 1:2:3::4:
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 extra ending single colon" {
  run ../src/netcalc -c 1:2:3:4:5:6:7:8:
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 leading single colon" {
  run ../src/netcalc -c :1:2:3::7:8
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 extra leading single colon" {
  run ../src/netcalc -c :1:2:3:4:5:6:7:8
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 triple colon" {
  run ../src/netcalc -c 1:2:3:::6:7:8
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv6 two double colons" {
  run ../src/netcalc -c 1:2::4:5::8
  [ $status -eq 0 ]
  [[ $output = "" ]]
}
