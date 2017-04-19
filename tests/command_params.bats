#!/usr/bin/env bats

@test "netcalc -c" {
  regexp="^Usage:[[:blank:]]netcalc"
  run ./netcalc -c
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $regexp ]]
}

@test "netcalc -h" {
  regexp="^Usage:[[:blank:]]netcalc"
  run ./netcalc -h
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $regexp ]]
}

@test "netcalc -n" {
  regexp="^Usage:[[:blank:]]netcalc"
  run ./netcalc -n
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $regexp ]]
}

@test "netcalc -v" {
  regexp="^[[:digit:]]+[.][[:digit:]]+[.][[:digit:]]+$"
  run ./netcalc -v
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ $regexp ]]
}

@test "netcalc invalid option" {
  regexp="^Usage:[[:blank:]]netcalc"
  run ./netcalc -d
  [ $status -eq 0 ]
  [[ ${lines[0]} =~ "invalid option" ]]
  [[ ${lines[1]} =~ $regexp ]]
}

@test "netcalc bare command" {
  regexp="^Usage:[[:blank:]]netcalc"
  run ./netcalc
  [ $status -eq 1 ]
  [[ ${lines[0]} =~ $regexp ]]
}
