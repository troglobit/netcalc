#!/usr/bin/env bats

# Note: line 3 is just a separator with "=>"
#    It is of no consequence to this testing
@test "netcalc display IPv4 ordinary address" {
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
