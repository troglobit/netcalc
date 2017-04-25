# Helper functions that are called by tests of IPv4 addresses

# check_ipv4_display_output
#   The parts of the expected output that vary based on the input
#   IPv4 address are passed in. Fixed parts of the expected output
#   are hard-coded in this function so that they only need to appear
#   once.
#
# Input Parameters:
#   $1 - binary netmask (typically appears 5 times in the output lines)
#   $2 - expected values on line 1 (Address)
#   $3 - expected values on line 2 (Netmask)
#   $4 - expected values on line 3 (Wildcard)
#   $5 - expected values on line 5 (Network)
#   $6 - expected values on line 6 (HostMin)
#   $7 - expected values on line 7 (HostMax)
#   $8 - expected values on line 8 (Broadcast)
#   $9 - expected value of Hosts/Net on line 9
#  $10 - expected value of network class description on line 9
#  $11 - array of lines of output to be checked, passed by reference

# The typical output format being checked is like:
# Address  : 192.168.1.0          11000000.10101000.00000001. 00000000
# Netmask  : 255.255.255.0 = 24   11111111.11111111.11111111. 00000000
# Wildcard : 0.0.0.255            00000000.00000000.00000000. 11111111
# =>
# Network  : 192.168.1.0/24       11000000.10101000.00000001. 00000000
# HostMin  : 192.168.1.1          11000000.10101000.00000001. 00000001
# HostMax  : 192.168.1.254        11000000.10101000.00000001. 11111110
# Broadcast: 192.168.1.255        11000000.10101000.00000001. 11111111
# Hosts/Net: 254                   Class C, Private network (RFC1918)

check_ipv4_display_output() {
  if [ -z "$1" ]
  then
    echo "Netmask not found in param 1"
    return 1
  fi

  if [ -z "$2" ]
  then
    echo "Expected Address values not found in param 2"
    return 1
  fi

  if [ -z "$3" ]
  then
    echo "Expected Netmask values not found in param 3"
    return 1
  fi

  if [ -z "$4" ]
  then
    echo "Expected Wildcard values not found in param 4"
    return 1
  fi

  if [ -z "$5" ]
  then
    echo "Expected Network values not found in param 5"
    return 1
  fi

  if [ -z "$6" ]
  then
    echo "Expected HostMin values not found in param 6"
    return 1
  fi

  if [ -z "$7" ]
  then
    echo "Expected HostMax values not found in param 7"
    return 1
  fi

  if [ -z "$8" ]
  then
    echo "Expected Broadcast values not found in param 8"
    return 1
  fi

  if [ -z "$9" ]
  then
    echo "Expected Hosts/Net not found in param 9"
    return 1
  fi

  if [ -z "${10}" ]
  then
    echo "Expected class description not found in param 10"
    return 1
  fi

  if [ -z "${11}" ]
  then
    echo "Output values not found in param 11"
    return 1
  fi

  local -n output_lines=${11}

  netmask=$1
  netmask="${netmask//./[.]}"

  paramdata=$2
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^Address[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+${netmask}[[:blank:]]+${paramdata_arr[1]}$"
  if ! [[ ${output_lines[0]} =~ $paramdata_exp ]]
  then
    echo "Error in address line ${output_lines[0]} ${paramdata_exp}"
	return 1
  fi

  paramdata=$3
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^Netmask[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+=[[:blank:]]+${paramdata_arr[1]}[[:blank:]]+${paramdata_arr[2]}[[:blank:]]+${paramdata_arr[3]}$"
  if ! [[ ${output_lines[1]} =~ $paramdata_exp ]]
  then
    echo "Error in netmask line ${output_lines[1]} ${paramdata_exp}"
	return 1
  fi

  paramdata=$4
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^Wildcard[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+${paramdata_arr[1]}[[:blank:]]+${paramdata_arr[2]}$"
  if ! [[ ${output_lines[2]} =~ $paramdata_exp ]]
  then
    echo "Error in wildcard line ${output_lines[2]} ${paramdata_exp}"
	return 1
  fi

  paramdata=$5
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^Network[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+${netmask}[[:blank:]]+${paramdata_arr[1]}$"
  if ! [[ ${output_lines[4]} =~ $paramdata_exp ]]
  then
    echo "Error in network line ${output_lines[4]} ${paramdata_exp}"
	return 1
  fi

  paramdata=$6
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^HostMin[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+${netmask}[[:blank:]]+${paramdata_arr[1]}$"
  if ! [[ ${output_lines[5]} =~ $paramdata_exp ]]
  then
    echo "Error in HostMin line ${output_lines[5]} ${paramdata_exp}"
	return 1
  fi

  paramdata=$7
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^HostMax[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+${netmask}[[:blank:]]+${paramdata_arr[1]}$"
  if ! [[ ${output_lines[6]} =~ $paramdata_exp ]]
  then
    echo "Error in HostMax line ${output_lines[6]} ${paramdata_exp}"
	return 1
  fi

  paramdata=$8
  paramdata="${paramdata//./[.]}"
  read -ra paramdata_arr <<<$paramdata
  paramdata_exp="^Broadcast[[:blank:]]*:[[:blank:]]+${paramdata_arr[0]}[[:blank:]]+${netmask}[[:blank:]]+${paramdata_arr[1]}$"
  if ! [[ ${output_lines[7]} =~ $paramdata_exp ]]
  then
    echo "Error in broadcast line ${output_lines[7]} ${paramdata_exp}"
	return 1
  fi

  # Values for this last line are passed separately and are not split into "words"
  hostspernet=$9
  classdescr=${10}
  # The class description is text that may have brackets () or full stops that need to be matched literally
  classdescr="${classdescr//./[.]}"
  classdescr="${classdescr//(/[(]}"
  classdescr="${classdescr//)/[)]}"
  paramdata_exp="^Hosts/Net[[:blank:]]*:[[:blank:]]+${hostspernet}[[:blank:]]+${classdescr}$"
  if ! [[ ${output_lines[8]} =~ $paramdata_exp ]]
  then
    echo "Error in Hosts-Net line ${output_lines[8]} ${paramdata_exp}"
	return 1
  fi

  return 0
}
