# Calculate network address from IP and prefix len
# Tobias Waldekranz, 2017
#
#   $ echo "192.168.2.232/24" | awk -f ipcalc.awk
#   192.168.2.0/24
#
#   $ echo "192.168.2.232.24" | awk -f ipcalc.awk
#   192.168.2.0/24
#
#   $ echo "192.168.2.232 24" | awk -f ipcalc.awk
#   192.168.2.0/24
#
BEGIN { FS="[. /]" }
{ 
    ip  = lshift($1, 24) + lshift($2, 16) + lshift($3, 8) + $4;
    net = lshift(rshift(ip, 32 - $5), 32 - $5);

    printf("%d.%d.%d.%d/%d\n",
	   and(rshift(net, 24), 0xff),
	   and(rshift(net, 16), 0xff),
	   and(rshift(net,  8), 0xff),
	   and(net, 0xff), $5);
}
