#!/usr/bin/env bats

# Test IPv4 address formats that should be valid

@test "netcalc -c 1.2.3.4" {
  run ./netcalc -c 1.2.3.4
  [ $status -eq 0 ]
  [[ $output = "1.2.3.4" ]]
}

@test "netcalc -c 192.168.1.1" {
  run ./netcalc -c 192.168.1.1
  [ $status -eq 0 ]
  [[ $output = "192.168.1.1" ]]
}

@test "netcalc -c 192.168.1.1/0" {
  run ./netcalc -c 192.168.1.1/0
  [ $status -eq 0 ]
  [[ $output = "192.168.1.1" ]]
}

@test "netcalc -c 192.168.1.1/24" {
  run ./netcalc -c 192.168.1.1/24
  [ $status -eq 0 ]
  [[ $output = "192.168.1.1" ]]
}

@test "netcalc -c 192.168.1.1/32" {
  run ./netcalc -c 192.168.1.1/32
  [ $status -eq 0 ]
  [[ $output = "192.168.1.1" ]]
}

@test "netcalc -c IPv4 zero address" {
  run ./netcalc -c 0.0.0.0
  [ $status -eq 0 ]
  [[ $output = "0.0.0.0" ]]
}

@test "netcalc -c IPv4 all-ones address" {
  run ./netcalc -c 255.255.255.255
  [ $status -eq 0 ]
  [[ $output = "255.255.255.255" ]]
}

# Tests for things that look a bit like an IPv4 address
# but are actually invalid

@test "netcalc -c IPv4 CIDR out of range" {
  run ./netcalc -c 192.168.1.1/33
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 negative CIDR" {
  run ./netcalc -c 192.168.1.1/-1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 slash but no CIDR" {
  run ./netcalc -c 192.168.1.1/
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 with colon and number (looks like a port number)" {
  run ./netcalc -c 192.168.1.1:22
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 with alpha CIDR" {
  run ./netcalc -c 192.168.1.1/aa
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 with following alpha" {
  run ./netcalc -c 192.168.1.1x
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 octet 1 out of range" {
  run ./netcalc -c 256.1.1.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 octet 2 out of range" {
  run ./netcalc -c 1.256.1.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 octet 3 out of range" {
  run ./netcalc -c 1.1.256.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 octet 4 out of range" {
  run ./netcalc -c 1.1.1.256
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 multi out of range" {
  run ./netcalc -c 1.456.1.300
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 with non-digit" {
  run ./netcalc -c 1.1.a.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 single number no dots" {
  run ./netcalc -c 123
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 not enough octets" {
  run ./netcalc -c 1.1.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 ending dot" {
  run ./netcalc -c 1.1.1.
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 extra ending dot" {
  run ./netcalc -c 1.1.1.1.
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 leading dot" {
  run ./netcalc -c .1.1.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}

@test "netcalc -c IPv4 extra leading dot" {
  run ./netcalc -c .1.1.1.1
  [ $status -eq 0 ]
  [[ $output = "" ]]
}
