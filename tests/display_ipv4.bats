#!/usr/bin/env bats

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
  line0exp="^Address[[:blank:]]*:[[:blank:]]+192.168.1.0[[:blank:]]+11000000.10101000.00000001.[[:blank:]]+00000000"
  line1exp="^Netmask[[:blank:]]*:[[:blank:]]+255.255.255.0[[:blank:]]+=[[:blank:]]+24[[:blank:]]+11111111.11111111.11111111.[[:blank:]]+00000000"
  line2exp="^Wildcard[[:blank:]]*:[[:blank:]]+0.0.0.255[[:blank:]]+00000000.00000000.00000000.[[:blank:]]+11111111"
  line4exp="^Network[[:blank:]]*:[[:blank:]]+192.168.1.0/24[[:blank:]]+11000000.10101000.00000001.[[:blank:]]+00000000"
  line5exp="^HostMin[[:blank:]]*:[[:blank:]]+192.168.1.1[[:blank:]]+11000000.10101000.00000001.[[:blank:]]+00000001"
  line6exp="^HostMax[[:blank:]]*:[[:blank:]]+192.168.1.254[[:blank:]]+11000000.10101000.00000001.[[:blank:]]+11111110"
  line7exp="^Broadcast[[:blank:]]*:[[:blank:]]+192.168.1.255[[:blank:]]+11000000.10101000.00000001.[[:blank:]]+11111111"
  line8exp="^Hosts/Net[[:blank:]]*:[[:blank:]]+254[[:blank:]]+Class[[:blank:]]*C[[:punct:]][[:blank:]]*Private[[:blank:]]*network[[:blank:]]*[[:punct:]]RFC1918[[:punct:]]"
  run ./netcalc -n 192.168.1.0/24
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $line0exp ]]
  [[ ${lines[1]} =~ $line1exp ]]
  [[ ${lines[2]} =~ $line2exp ]]
  [[ ${lines[4]} =~ $line4exp ]]
  [[ ${lines[5]} =~ $line5exp ]]
  [[ ${lines[6]} =~ $line6exp ]]
  [[ ${lines[7]} =~ $line7exp ]]
  [[ ${lines[8]} =~ $line8exp ]]
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
  line0exp="^Address[[:blank:]]*:[[:blank:]]+172.17.0.0[[:blank:]]+10101100.00010001.[[:blank:]]+00000000.00000000"
  line1exp="^Netmask[[:blank:]]*:[[:blank:]]+255.255.0.0[[:blank:]]+=[[:blank:]]+16[[:blank:]]+11111111.11111111.[[:blank:]]+00000000.00000000"
  line2exp="^Wildcard[[:blank:]]*:[[:blank:]]+0.0.255.255[[:blank:]]+00000000.00000000.[[:blank:]]+11111111.11111111"
  line4exp="^Network[[:blank:]]*:[[:blank:]]+172.17.0.0/16[[:blank:]]+10101100.00010001.[[:blank:]]+00000000.00000000"
  line5exp="^HostMin[[:blank:]]*:[[:blank:]]+172.17.0.1[[:blank:]]+10101100.00010001.[[:blank:]]+00000000.00000001"
  line6exp="^HostMax[[:blank:]]*:[[:blank:]]+172.17.255.254[[:blank:]]+10101100.00010001.[[:blank:]]+11111111.11111110"
  line7exp="^Broadcast[[:blank:]]*:[[:blank:]]+172.17.255.255[[:blank:]]+10101100.00010001.[[:blank:]]+11111111.11111111"
  line8exp="^Hosts/Net[[:blank:]]*:[[:blank:]]+65534[[:blank:]]+Class[[:blank:]]*B[[:punct:]][[:blank:]]*Private[[:blank:]]*network[[:blank:]]*[[:punct:]]RFC1918[[:punct:]]"
  run ./netcalc -n 172.17.0.0/16
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $line0exp ]]
  [[ ${lines[1]} =~ $line1exp ]]
  [[ ${lines[2]} =~ $line2exp ]]
  [[ ${lines[4]} =~ $line4exp ]]
  [[ ${lines[5]} =~ $line5exp ]]
  [[ ${lines[6]} =~ $line6exp ]]
  [[ ${lines[7]} =~ $line7exp ]]
  [[ ${lines[8]} =~ $line8exp ]]
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
  line0exp="^Address[[:blank:]]*:[[:blank:]]+10.0.0.0[[:blank:]]+00001010.[[:blank:]]+00000000.00000000"
  line1exp="^Netmask[[:blank:]]*:[[:blank:]]+255.0.0.0[[:blank:]]+=[[:blank:]]+8[[:blank:]]+11111111.[[:blank:]]+00000000.00000000.00000000"
  line2exp="^Wildcard[[:blank:]]*:[[:blank:]]+0.255.255.255[[:blank:]]+00000000.[[:blank:]]+11111111.11111111.11111111"
  line4exp="^Network[[:blank:]]*:[[:blank:]]+10.0.0.0/8[[:blank:]]+00001010.[[:blank:]]+00000000.00000000.00000000"
  line5exp="^HostMin[[:blank:]]*:[[:blank:]]+10.0.0.1[[:blank:]]+00001010.[[:blank:]]+00000000.00000000.00000001"
  line6exp="^HostMax[[:blank:]]*:[[:blank:]]+10.255.255.254[[:blank:]]+00001010.[[:blank:]]+11111111.11111111.11111110"
  line7exp="^Broadcast[[:blank:]]*:[[:blank:]]+10.255.255.255[[:blank:]]+00001010.[[:blank:]]+11111111.11111111.11111111"
  line8exp="^Hosts/Net[[:blank:]]*:[[:blank:]]+16777214[[:blank:]]+Class[[:blank:]]*A[[:punct:]][[:blank:]]*Private[[:blank:]]*network[[:blank:]]*[[:punct:]]RFC1918[[:punct:]]"
  run ./netcalc -n 10.0.0.0/8
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $line0exp ]]
  [[ ${lines[1]} =~ $line1exp ]]
  [[ ${lines[2]} =~ $line2exp ]]
  [[ ${lines[4]} =~ $line4exp ]]
  [[ ${lines[5]} =~ $line5exp ]]
  [[ ${lines[6]} =~ $line6exp ]]
  [[ ${lines[7]} =~ $line7exp ]]
  [[ ${lines[8]} =~ $line8exp ]]
}
