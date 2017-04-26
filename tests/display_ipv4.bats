#!/usr/bin/env bats

load helper_ipv4

# Address  : 192.168.1.0          11000000.10101000.00000001. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 192.168.1.0/24       11000000.10101000.00000001. 00000000
# HostMin  : 192.168.1.1          11000000.10101000.00000001. 00000001
# HostMax  : 192.168.1.254        11000000.10101000.00000001. 11111110
# Broadcast: 192.168.1.255        11000000.10101000.00000001. 11111111
# Hosts/Net: 254                   Class C, Private network (RFC1918)

@test "netcalc display IPv4 class C private address" {
  run ./netcalc -n 192.168.1.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11000000.10101000.00000001." \
    "192.168.1.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "192.168.1.0/24 00000000" \
    "192.168.1.1 00000001" \
    "192.168.1.254 11111110" \
    "192.168.1.255 11111111" \
    "254" \
    "Class C, Private network (RFC1918)" \
    lines
}

# Address  : 192.169.0.0          11000000.10101001.00000000. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 192.169.0.0/24       11000000.10101001.00000000. 00000000
# HostMin  : 192.169.0.1          11000000.10101001.00000000. 00000001
# HostMax  : 192.169.0.254        11000000.10101001.00000000. 11111110
# Broadcast: 192.169.0.255        11000000.10101001.00000000. 11111111
# Hosts/Net: 254                   Class C, Private network (RFC1918)

@test "netcalc display IPv4 just past class C private address" {
  run ./netcalc -n 192.169.0.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11000000.10101001.00000000." \
    "192.169.0.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "192.169.0.0/24 00000000" \
    "192.169.0.1 00000001" \
    "192.169.0.254 11111110" \
    "192.169.0.255 11111111" \
    "254" \
    "Class C" \
    lines
}

# Address  : 172.17.0.0           10101100.00010001. 00000000.00000000
# Netmask  : 255.255.0.0 = 16     11111111.11111111. 00000000.00000000
# Wildcard : 0.0.255.255          00000000.00000000. 11111111.11111111
# =>
# Network  : 172.17.0.0/16        10101100.00010001. 00000000.00000000
# HostMin  : 172.17.0.1           10101100.00010001. 00000000.00000001
# HostMax  : 172.17.255.254       10101100.00010001. 11111111.11111110
# Broadcast: 172.17.255.255       10101100.00010001. 11111111.11111111
# Hosts/Net: 65534                 Class B, Private network (RFC1918)

@test "netcalc display IPv4 class B private address" {
  run ./netcalc -n 172.17.0.0/16
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "10101100.00010001." \
    "172.17.0.0 00000000.00000000" \
    "255.255.0.0 16 11111111.11111111. 00000000.00000000" \
    "0.0.255.255 00000000.00000000. 11111111.11111111" \
    "172.17.0.0/16 00000000.00000000" \
    "172.17.0.1 00000000.00000001" \
    "172.17.255.254 11111111.11111110" \
    "172.17.255.255 11111111.11111111" \
    "65534" \
    "Class B, Private network (RFC1918)" \
    lines
}

# Address  : 172.32.0.0           10101100.00100000. 00000000.00000000
# Netmask  : 255.255.0.0 = 16     11111111.11111111. 00000000.00000000
# Wildcard : 0.0.255.255          00000000.00000000. 11111111.11111111
# =>
# Network  : 172.32.0.0/16        10101100.00100000. 00000000.00000000
# HostMin  : 172.32.0.1           10101100.00100000. 00000000.00000001
# HostMax  : 172.32.255.254       10101100.00100000. 11111111.11111110
# Broadcast: 172.32.255.255       10101100.00100000. 11111111.11111111
# Hosts/Net: 65534                 Class B

@test "netcalc display IPv4 just past class B private address" {
  run ./netcalc -n 172.32.0.0/16
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "10101100.00100000." \
    "172.32.0.0 00000000.00000000" \
    "255.255.0.0 16 11111111.11111111. 00000000.00000000" \
    "0.0.255.255 00000000.00000000. 11111111.11111111" \
    "172.32.0.0/16 00000000.00000000" \
    "172.32.0.1 00000000.00000001" \
    "172.32.255.254 11111111.11111110" \
    "172.32.255.255 11111111.11111111" \
    "65534" \
    "Class B" \
    lines
}

# Address  : 10.0.0.0             00001010. 00000000.00000000.00000000
# Netmask  : 255.0.0.0 = 8        11111111. 00000000.00000000.00000000
# Wildcard : 0.255.255.255        00000000. 11111111.11111111.11111111
# =>
# Network  : 10.0.0.0/8           00001010. 00000000.00000000.00000000
# HostMin  : 10.0.0.1             00001010. 00000000.00000000.00000001
# HostMax  : 10.255.255.254       00001010. 11111111.11111111.11111110
# Broadcast: 10.255.255.255       00001010. 11111111.11111111.11111111
# Hosts/Net: 16777214              Class A, Private network (RFC1918)

@test "netcalc display IPv4 class A private address" {
  run ./netcalc -n 10.0.0.0/8
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "00001010." \
    "10.0.0.0 00000000.00000000.00000000" \
    "255.0.0.0 8 11111111. 00000000.00000000.00000000" \
    "0.255.255.255 00000000. 11111111.11111111.11111111" \
    "10.0.0.0/8 00000000.00000000.00000000" \
    "10.0.0.1 00000000.00000000.00000001" \
    "10.255.255.254 11111111.11111111.11111110" \
    "10.255.255.255 11111111.11111111.11111111" \
    "16777214" \
    "Class A, Private network (RFC1918)" \
    lines
}

@test "netcalc display IPv4 just past class A private address" {
  run ./netcalc -n 11.0.0.0/8
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "00001011." \
    "11.0.0.0 00000000.00000000.00000000" \
    "255.0.0.0 8 11111111. 00000000.00000000.00000000" \
    "0.255.255.255 00000000. 11111111.11111111.11111111" \
    "11.0.0.0/8 00000000.00000000.00000000" \
    "11.0.0.1 00000000.00000000.00000001" \
    "11.255.255.254 11111111.11111111.11111110" \
    "11.255.255.255 11111111.11111111.11111111" \
    "16777214" \
    "Class A" \
    lines
}

# Address  : 199.42.7.0           11000111.00101010.00000111. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 199.42.7.0/24        11000111.00101010.00000111. 00000000
# HostMin  : 199.42.7.1           11000111.00101010.00000111. 00000001
# HostMax  : 199.42.7.254         11000111.00101010.00000111. 11111110
# Broadcast: 199.42.7.255         11000111.00101010.00000111. 11111111
# Hosts/Net: 254                   Class C

@test "netcalc display IPv4 class C address" {
  run ./netcalc -n 199.42.7.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11000111.00101010.00000111." \
    "199.42.7.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "199.42.7.0/24 00000000" \
    "199.42.7.1 00000001" \
    "199.42.7.254 11111110" \
    "199.42.7.255 11111111" \
    "254" \
    "Class C" \
    lines
}

# Address  : 133.45.0.0           10000101.00101101. 00000000.00000000
# Netmask  : 255.255.0.0 = 16     11111111.11111111. 00000000.00000000
# Wildcard : 0.0.255.255          00000000.00000000. 11111111.11111111
# =>
# Network  : 133.45.0.0/16        10000101.00101101. 00000000.00000000
# HostMin  : 133.45.0.1           10000101.00101101. 00000000.00000001
# HostMax  : 133.45.255.254       10000101.00101101. 11111111.11111110
# Broadcast: 133.45.255.255       10000101.00101101. 11111111.11111111
# Hosts/Net: 65534                 Class B

@test "netcalc display IPv4 class B address" {
  run ./netcalc -n 133.45.0.0/16
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "10000101.00101101." \
    "133.45.0.0 00000000.00000000" \
    "255.255.0.0 16 11111111.11111111. 00000000.00000000" \
    "0.0.255.255 00000000.00000000. 11111111.11111111" \
    "133.45.0.0/16 00000000.00000000" \
    "133.45.0.1 00000000.00000001" \
    "133.45.255.254 11111111.11111110" \
    "133.45.255.255 11111111.11111111" \
    "65534" \
    "Class B" \
    lines
}

# Address  : 69.0.0.0             01000101. 00000000.00000000.00000000
# Netmask  : 255.0.0.0 = 8        11111111. 00000000.00000000.00000000
# Wildcard : 0.255.255.255        00000000. 11111111.11111111.11111111
# =>
# Network  : 69.0.0.0/8           01000101. 00000000.00000000.00000000
# HostMin  : 69.0.0.1             01000101. 00000000.00000000.00000001
# HostMax  : 69.255.255.254       01000101. 11111111.11111111.11111110
# Broadcast: 69.255.255.255       01000101. 11111111.11111111.11111111
# Hosts/Net: 16777214              Class A

@test "netcalc display IPv4 class A address" {
  run ./netcalc -n 69.0.0.0/8
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "01000101." \
    "69.0.0.0 00000000.00000000.00000000" \
    "255.0.0.0 8 11111111. 00000000.00000000.00000000" \
    "0.255.255.255 00000000. 11111111.11111111.11111111" \
    "69.0.0.0/8 00000000.00000000.00000000" \
    "69.0.0.1 00000000.00000000.00000001" \
    "69.255.255.254 11111111.11111111.11111110" \
    "69.255.255.255 11111111.11111111.11111111" \
    "16777214" \
    "Class A" \
    lines
}

# Address  : 192.0.0.0            11000000.00000000.00000000. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 192.0.0.0/24         11000000.00000000.00000000. 00000000
# HostMin  : 192.0.0.1            11000000.00000000.00000000. 00000001
# HostMax  : 192.0.0.254          11000000.00000000.00000000. 11111110
# Broadcast: 192.0.0.255          11000000.00000000.00000000. 11111111
# Hosts/Net: 254                   Class C, Reserved, IANA (RFC5735)

@test "netcalc display IPv4 class C first address" {
  run ./netcalc -n 192.0.0.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11000000.00000000.00000000." \
    "192.0.0.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "192.0.0.0/24 00000000" \
    "192.0.0.1 00000001" \
    "192.0.0.254 11111110" \
    "192.0.0.255 11111111" \
    "254" \
    "Class C, Reserved, IANA (RFC5735)" \
    lines
}

@test "netcalc display IPv4 class C first ordinary address" {
  run ./netcalc -n 192.0.1.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11000000.00000000.00000001." \
    "192.0.1.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "192.0.1.0/24 00000000" \
    "192.0.1.1 00000001" \
    "192.0.1.254 11111110" \
    "192.0.1.255 11111111" \
    "254" \
    "Class C" \
    lines
}

# Address  : 192.0.0.0            11000000.00000000.0000000 0.00000000
# Netmask  : 255.255.254.0 = 23   11111111.11111111.1111111 0.00000000
# Wildcard : 0.0.1.255            00000000.00000000.0000000 1.11111111
# =>
# Network  : 192.0.0.0/23         11000000.00000000.0000000 0.00000000
# HostMin  : 192.0.0.1            11000000.00000000.0000000 0.00000001
# HostMax  : 192.0.1.254          11000000.00000000.0000000 1.11111110
# Broadcast: 192.0.1.255          11000000.00000000.0000000 1.11111111
# Hosts/Net: 510                   Class C, In Part Reserved, IANA (RFC5735)

@test "netcalc display IPv4 class C first addresses overlapped" {
  run ./netcalc -n 192.0.0.0/23
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11000000.00000000.0000000" \
    "192.0.0.0 0.00000000" \
    "255.255.254.0 23 11111111.11111111.1111111 0.00000000" \
    "0.0.1.255 00000000.00000000.0000000 1.11111111" \
    "192.0.0.0/23 0.00000000" \
    "192.0.0.1 0.00000001" \
    "192.0.1.254 1.11111110" \
    "192.0.1.255 1.11111111" \
    "510" \
    "Class C, In Part Reserved, IANA (RFC5735)" \
    lines
}

# Address  : 128.0.0.0            10000000.00000000. 00000000.00000000
# Netmask  : 255.255.0.0 = 16     11111111.11111111. 00000000.00000000
# Wildcard : 0.0.255.255          00000000.00000000. 11111111.11111111
# =>
# Network  : 128.0.0.0/16         10000000.00000000. 00000000.00000000
# HostMin  : 128.0.0.1            10000000.00000000. 00000000.00000001
# HostMax  : 128.0.255.254        10000000.00000000. 11111111.11111110
# Broadcast: 128.0.255.255        10000000.00000000. 11111111.11111111
# Hosts/Net: 65534                 Class B

@test "netcalc display IPv4 class B first address" {
  run ./netcalc -n 128.0.0.0/16
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "10000000.00000000." \
    "128.0.0.0 00000000.00000000" \
    "255.255.0.0 16 11111111.11111111. 00000000.00000000" \
    "0.0.255.255 00000000.00000000. 11111111.11111111" \
    "128.0.0.0/16 00000000.00000000" \
    "128.0.0.1 00000000.00000001" \
    "128.0.255.254 11111111.11111110" \
    "128.0.255.255 11111111.11111111" \
    "65534" \
    "Class B" \
    lines
}

# Address  : 128.0.0.0            10000000.0000000 0.00000000.00000000
# Netmask  : 255.254.0.0 = 15     11111111.1111111 0.00000000.00000000
# Wildcard : 0.1.255.255          00000000.0000000 1.11111111.11111111
# =>
# Network  : 128.0.0.0/15         10000000.0000000 0.00000000.00000000
# HostMin  : 128.0.0.1            10000000.0000000 0.00000000.00000001
# HostMax  : 128.1.255.254        10000000.0000000 1.11111111.11111110
# Broadcast: 128.1.255.255        10000000.0000000 1.11111111.11111111
# Hosts/Net: 131070                Class B

@test "netcalc display IPv4 class B first addresses overlapped" {
  run ./netcalc -n 128.0.0.0/15
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "10000000.0000000" \
    "128.0.0.0 0.00000000.00000000" \
    "255.254.0.0 15 11111111.1111111 0.00000000.00000000" \
    "0.1.255.255 00000000.0000000 1.11111111.11111111" \
    "128.0.0.0/15 0.00000000.00000000" \
    "128.0.0.1 0.00000000.00000001" \
    "128.1.255.254 1.11111111.11111110" \
    "128.1.255.255 1.11111111.11111111" \
    "131070" \
    "Class B" \
    lines
}

# Address  : 0.0.0.0              00000000. 00000000.00000000.00000000
# Netmask  : 255.0.0.0 = 8        11111111. 00000000.00000000.00000000
# Wildcard : 0.255.255.255        00000000. 11111111.11111111.11111111
# =>
# Network  : 0.0.0.0/8            00000000. 00000000.00000000.00000000
# HostMin  : 0.0.0.1              00000000. 00000000.00000000.00000001
# HostMax  : 0.255.255.254        00000000. 11111111.11111111.11111110
# Broadcast: 0.255.255.255        00000000. 11111111.11111111.11111111
# Hosts/Net: 16777214              Class A, Current network (RFC1700)

@test "netcalc display IPv4 class A first address" {
  run ./netcalc -n 0.0.0.0/8
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "00000000." \
    "0.0.0.0 00000000.00000000.00000000" \
    "255.0.0.0 8 11111111. 00000000.00000000.00000000" \
    "0.255.255.255 00000000. 11111111.11111111.11111111" \
    "0.0.0.0/8 00000000.00000000.00000000" \
    "0.0.0.1 00000000.00000000.00000001" \
    "0.255.255.254 11111111.11111111.11111110" \
    "0.255.255.255 11111111.11111111.11111111" \
    "16777214" \
    "Class A, Current network (RFC1700)" \
    lines
}

@test "netcalc display IPv4 class A first ordinary address" {
  run ./netcalc -n 1.0.0.0/8
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "00000001." \
    "1.0.0.0 00000000.00000000.00000000" \
    "255.0.0.0 8 11111111. 00000000.00000000.00000000" \
    "0.255.255.255 00000000. 11111111.11111111.11111111" \
    "1.0.0.0/8 00000000.00000000.00000000" \
    "1.0.0.1 00000000.00000000.00000001" \
    "1.255.255.254 11111111.11111111.11111110" \
    "1.255.255.255 11111111.11111111.11111111" \
    "16777214" \
    "Class A" \
    lines
}

@test "netcalc display IPv4 class A first addresses overlapped" {
  run ./netcalc -n 0.0.0.0/7
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "0000000" \
    "0.0.0.0 0.00000000.00000000.00000000" \
    "254.0.0.0 7 1111111 0.00000000.00000000.00000000" \
    "1.255.255.255 0000000 1.11111111.11111111.11111111" \
    "0.0.0.0/7 0.00000000.00000000.00000000" \
    "0.0.0.1 0.00000000.00000000.00000001" \
    "1.255.255.254 1.11111111.11111111.11111110" \
    "1.255.255.255 1.11111111.11111111.11111111" \
    "33554430" \
    "Class A, In Part Current network (RFC1700)" \
    lines
}

# Address  : 6.7.8.0              00000110.00000111.00001000. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 6.7.8.0/24           00000110.00000111.00001000. 00000000
# HostMin  : 6.7.8.1              00000110.00000111.00001000. 00000001
# HostMax  : 6.7.8.254            00000110.00000111.00001000. 11111110
# Broadcast: 6.7.8.255            00000110.00000111.00001000. 11111111
# Hosts/Net: 254                   Class A

@test "netcalc display IPv4 24 CIDR in class A address space" {
  run ./netcalc -n 6.7.8.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "00000110.00000111.00001000." \
    "6.7.8.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "6.7.8.0/24 00000000" \
    "6.7.8.1 00000001" \
    "6.7.8.254 11111110" \
    "6.7.8.255 11111111" \
    "254" \
    "Class A" \
    lines
}

# Address  : 156.43.14.0          10011100.00101011.00001110. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 156.43.14.0/24        10011100.00101011.00001110. 00000000
# HostMin  : 156.43.14.1          10011100.00101011.00001110. 00000001
# HostMax  : 156.43.14.254        10011100.00101011.00001110. 11111110
# Broadcast: 156.43.14.255        10011100.00101011.00001110. 11111111
# Hosts/Net: 254                   Class B

@test "netcalc display IPv4 24 CIDR in class B address space" {
  run ./netcalc -n 156.43.14.0/24
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "10011100.00101011.00001110." \
    "156.43.14.0 00000000" \
    "255.255.255.0 24 11111111.11111111.11111111. 00000000" \
    "0.0.0.255 00000000.00000000.00000000. 11111111" \
    "156.43.14.0/24 00000000" \
    "156.43.14.1 00000001" \
    "156.43.14.254 11111110" \
    "156.43.14.255 11111111" \
    "254" \
    "Class B" \
    lines
}

# Address  : 207.99.11.16         11001111.01100011.00001011.0001 0000
# Netmask  : 255.255.255.240 = 28 11111111.11111111.11111111.1111 0000
# Wildcard : 0.0.0.15             00000000.00000000.00000000.0000 1111
# =>
# Network  : 207.99.11.16/28       11001111.01100011.00001011.0001 0000
# HostMin  : 207.99.11.17         11001111.01100011.00001011.0001 0001
# HostMax  : 207.99.11.30         11001111.01100011.00001011.0001 1110
# Broadcast: 207.99.11.31         11001111.01100011.00001011.0001 1111
# Hosts/Net: 14                    Class C

@test "netcalc display IPv4 28 CIDR in class C address space" {
  run ./netcalc -n 207.99.11.16/28
  [ $status -eq 0 ]
  check_ipv4_display_output \
    "11001111.01100011.00001011.0001" \
    "207.99.11.16 0000" \
    "255.255.255.240 28 11111111.11111111.11111111.1111 0000" \
    "0.0.0.15 00000000.00000000.00000000.0000 1111" \
    "207.99.11.16/28 0000" \
    "207.99.11.17 0001" \
    "207.99.11.30 1110" \
    "207.99.11.31 1111" \
    "14" \
    "Class C" \
    lines
}

