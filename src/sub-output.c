/*
 * sipcalc, sub-output.c
 * 
 * $Id: sub-output.c,v 1.32 2003/03/19 12:33:56 simius Exp $
 * 
 * -
 * Copyright (c) 2003 Simon Ekstrand
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *  
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

static const char rcsid[] = "$Id: sub-output.c,v 1.32 2003/03/19 12:33:56 simius Exp $";

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sub.h"

void
show_split_networks_v4 (struct if_info *ifi, u_int32_t splitmask, int v4args, struct misc_args m_argv4)
{
	u_int32_t diff, start, end;
	struct if_info ifi_tmp;
	int v4args_tmp;
        size_t maxlen;

	v4args_tmp = 0;

	if ((v4args & V4VERBSPLIT) == V4VERBSPLIT)
		printf ("[Split network/%d - verbose]\n", numtolen (splitmask));
	else
		printf ("[Split network/%d]\n", numtolen (splitmask));

	if (splitmask < ifi->v4ad.n_nmask) {
		fprintf (stderr, "Cannot subnet to /%d with this base network, use a prefix len > /%d\n", 
                         numtolen (splitmask), numtolen (ifi->v4ad.n_nmask));
		return;
	}
	diff = 0xffffffff - splitmask + 1;
	start = ifi->v4ad.n_naddr;
	end = ifi->v4ad.n_naddr + diff - 1;

	if ((v4args & V4VERBSPLIT) == V4VERBSPLIT) {
		memcpy ((struct if_info *) &ifi_tmp, (struct if_info *) ifi, sizeof(struct if_info));
		v4args_tmp = v4args ^ V4SPLIT;
		v4args_tmp = v4args_tmp ^ V4VERBSPLIT;
		if (!v4args_tmp)
			v4args_tmp = V4_INFO;
		ifi_tmp.next = NULL;
	}
        else {
                /* Figure out max width of a network string. */
                while (1) {
                        char buf[30];
                        size_t len;

                        len = snprintf (buf, sizeof(buf), "%s", numtoquad (start));
                        if (len > maxlen)
                                maxlen = len;
                        
                        start += diff;
                        if (end == 0xffffffff || end >= ifi->v4ad.n_broadcast)
                                break;
                        end += diff;
                }
        }

	start = ifi->v4ad.n_naddr;
	end = ifi->v4ad.n_naddr + diff - 1;
	while (1) {
		if ((v4args & V4VERBSPLIT) != V4VERBSPLIT) {
                        char buf[30];

                        snprintf (buf, sizeof(buf), "%s", numtoquad (start));
                        printf ("Network  : \e[34m%-*s - %-15s\e[0m  ", maxlen, buf, numtoquad (end));
                        printf ("Netmask: \e[34m%s\e[0m\n", numtoquad (splitmask));
		}
		if ((v4args & V4VERBSPLIT) == V4VERBSPLIT) {
			safe_bzero (ifi_tmp.p_v4addr);
			safe_bzero (ifi_tmp.p_v4nmask);
			safe_bzero (ifi_tmp.p_v6addr);
			safe_snprintf (ifi_tmp.p_v4addr, "%s", numtoquad (start));
			safe_snprintf (ifi_tmp.p_v4nmask, "%s", numtoquad (splitmask));
		}
		start += diff;
		if (end == 0xffffffff || end >= ifi->v4ad.n_broadcast)
                        break;
		end += diff;

		if ((v4args & V4VERBSPLIT) == V4VERBSPLIT)
			out_cmdline (&ifi_tmp, v4args_tmp, m_argv4, 0, m_argv4, 1, 0);
	}

	printf ("\n");

	return;
}

int
show_networks_v4 (struct if_info *ifi, int count)
{
        int lcount = count;
        size_t startlen = 0, endlen = 0;
	u_int32_t diff, start, end, len;
        u_int32_t lstart, lend;

	diff = 0xffffffff - ifi->v4ad.n_nmask + 1;
        len = ifi->v4ad.n_nmaskbits;
	if (len >= 31) {
		start = ifi->v4ad.n_naddr & ifi->v4ad.n_nmask;
                end = start;
                if (len == 31) end++;
	} else {
		start = ifi->v4ad.n_naddr;
		end = ifi->v4ad.n_broadcast;
	}
        lstart = start;
        lend = end;

        /* Figure out max width of a network string. */
	while (lcount) {
                char buf[30];
                size_t len;

                len = snprintf (buf, sizeof(buf), "%s", numtoquad (lstart));
                if (len > startlen)
                        startlen = len;

                len = snprintf (buf, sizeof(buf), "%s", numtoquad (lend));
                if (len > endlen)
                        endlen = len;

		lstart += diff;
		if (lend == 0xffffffff)
			break;
		if ((lend & 0x000000ff) == 0xff && lcount == -1)
                        break;
		lend += diff;
		if (lcount > 0)
			lcount--;
	}

	printf ("[Networks/%d]\n", len);
	while (count) {
                printf ("Network  : \e[34m%-*s - ", startlen, numtoquad (start));
                printf ("%-*s\e[0m", endlen, numtoquad (end));
                printf ("  Netmask: \e[34m%s\e[0m\n", numtoquad (ifi->v4ad.n_nmask));

		start += diff;
		if (end == 0xffffffff)
			break;
		if ((end & 0x000000ff) == 0xff && count == -1)
                        break;
		end += diff;
		if (count > 0)
			count--;
	}

	printf ("\n");

	return 0;
}

void
print_cf_info_v4 (struct if_info *ifi)
{
        u_int32_t num, bcast, len, min, max;
        char temp[21];

        num = ifi->v4ad.n_broadcast - ifi->v4ad.n_naddr - 1;
        len = ifi->v4ad.n_nmaskbits;
        if (len > 30) {
                if (len == 31) {
                        num = 2;
                        min = ifi->v4ad.n_naddr;
                        max = min + 1;
                }
                else 
                        num = 1;
        }
        else {
                min = ifi->v4ad.n_naddr + 1;
                max = ifi->v4ad.n_broadcast - 1;
        }

	printf ("Address  : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", 
                numtoquad (ifi->v4ad.n_haddr), numtobitmap (ifi->v4ad.n_haddr, len));
//	printf ("Address (decimal): %u\n", ifi->v4ad.n_haddr);
//	printf ("Address (hex)    : %X\n", ifi->v4ad.n_haddr);
        snprintf (temp, sizeof(temp), "%s = %d", numtoquad (ifi->v4ad.n_nmask), ifi->v4ad.n_nmaskbits);
	printf ("Netmask  : \e[34m%-20s\e[0m \e[31m%s\e[0m\n", temp, numtobitmap (ifi->v4ad.n_nmask, len));
//	printf ("Netmask (hex)	- %X\n", ifi->v4ad.n_cnmask);
	printf ("Wildcard : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad (ifi->v4ad.n_nmask ^ 0xffffffff),
		numtobitmap (ifi->v4ad.n_nmask ^ 0xffffffff, len));

        printf ("=>\n");

        snprintf (temp, sizeof(temp), "%s/%d", numtoquad (ifi->v4ad.n_cnaddr), ifi->v4ad.n_nmaskbits);
	if (num >= 2) {
                size_t clen = ifi->v4ad.class - 'A' + 1;
                char buf['Z' - 'A'] = { 0 };
                char *net = numtobitmap (ifi->v4ad.n_cnaddr, len);

                strncpy (buf, net, clen);
                printf ("Network  : \e[34m%-20s\e[0m \e[35m%s\e[0m\e[33m%s\e[0m\n", temp, buf, &net[clen]);
                printf ("HostMin  : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad (min), numtobitmap (min, len));
		printf ("HostMax  : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad (max), numtobitmap (max, len));
                if (len < 31) {
                        bcast = ifi->v4ad.n_broadcast;
                        printf ("Broadcast: \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad (bcast), numtobitmap (bcast, len));
                }
	}
        else {
                printf ("HostRoute: \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad (ifi->v4ad.n_cnaddr),
                       numtobitmap (ifi->v4ad.n_cnaddr, len));
        }

        printf ("Hosts/Net: \e[34m%-20u\e[0m  ", num);
        if (ifi->v4ad.class == 'I') /* Invalid */
                printf ("\e[35mClass Invalid", ifi->v4ad.class);
        else
                printf ("\e[35mClass %c", ifi->v4ad.class);
        printf ("\e[0m%s\n", ifi->v4ad.class_remark);
	printf ("\n");
}

void
print_comp_v6 (struct sip_in6_addr addr)
{
	int x, y, z;
	int start, num;

	start = -1;
	num = 0;
	y = 0;
	z = 0;
	for (x = 0; x < 8; x++) {
		if (addr.sip6_addr16[x] == 0) {
			if (y == -1)
				y = x;
			z++;
		} else {
			if (z > num && z > 1) {
				start = y;
				num = z;
			}
			y = -1;
			z = 0;
		}
	}

	if (z > num && z > 1) {
		start = y;
		num = z;
	}

	for (x = 0; x < 8; x++) {
		if (x == start) {
			if (!x)
				printf (":");
			printf (":");
			x += num - 1;
		} else {
			printf ("%x", addr.sip6_addr16[x]);
			if (x != 7)
				printf (":");
		}
	}

	return;
}

void
print_exp_v4inv6 (struct sip_in6_addr addr)
{
	unsigned char num;

	printf ("%04x:%04x:%04x:%04x:%04x:%04x:",
		addr.sip6_addr16[0],
		addr.sip6_addr16[1],
		addr.sip6_addr16[2],
		addr.sip6_addr16[3], addr.sip6_addr16[4], addr.sip6_addr16[5]);

	num = (addr.sip6_addr16[6] >> 8) & 0xff;
	printf ("%d.", num);
	num = addr.sip6_addr16[6] & 0xff;
	printf ("%d.", num);
	num = (addr.sip6_addr16[7] >> 8) & 0xff;
	printf ("%d.", num);
	num = addr.sip6_addr16[7] & 0xff;
	printf ("%d", num);

	return;
}

void
print_comp_v4inv6 (struct sip_in6_addr addr)
{
	unsigned char v4num;
	int x, y, z;
	int start, num;

	start = -1;
	num = 0;
	y = 0;
	z = 0;
	for (x = 0; x < 6; x++) {
		if (addr.sip6_addr16[x] == 0) {
			if (y == -1)
				y = x;
			z++;
		} else {
			if (z > num && z > 1) {
				start = y;
				num = z;
			}
			y = -1;
			z = 0;
		}
	}

	if (z > num && z > 1) {
		start = y;
		num = z;
	}

	for (x = 0; x < 6; x++) {
		if (x == start) {
			if (!x)
				printf (":");
			printf (":");
			x += num - 1;
		} else {
			printf ("%x:", addr.sip6_addr16[x]);
		}
	}

	v4num = (addr.sip6_addr16[6] >> 8) & 0xff;
	printf ("%d.", v4num);
	v4num = addr.sip6_addr16[6] & 0xff;
	printf ("%d.", v4num);
	v4num = (addr.sip6_addr16[7] >> 8) & 0xff;
	printf ("%d.", v4num);
	v4num = addr.sip6_addr16[7] & 0xff;
	printf ("%d", v4num);

	return;
}

void
print_exp_v6 (struct sip_in6_addr addr)
{
	printf
	    ("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
	     addr.sip6_addr16[0],
	     addr.sip6_addr16[1],
	     addr.sip6_addr16[2],
	     addr.sip6_addr16[3],
	     addr.sip6_addr16[4],
	     addr.sip6_addr16[5], addr.sip6_addr16[6], addr.sip6_addr16[7]);
}

void
print_mixed_v6 (struct sip_in6_addr addr)
{
	printf
	    ("%x:%x:%x:%x:%x:%x:%x:%x",
	     addr.sip6_addr16[0],
	     addr.sip6_addr16[1],
	     addr.sip6_addr16[2],
	     addr.sip6_addr16[3],
	     addr.sip6_addr16[4],
	     addr.sip6_addr16[5], addr.sip6_addr16[6], addr.sip6_addr16[7]);
}

void
print_revdns_v6 (struct sip_in6_addr addr) {
	char inbuf[40], outbuf[256];
	int x, y;

	safe_bzero (inbuf);
	safe_bzero (outbuf);

	safe_snprintf
	(inbuf, "%04x%04x%04x%04x%04x%04x%04x%04x",
	 addr.sip6_addr16[0],
	 addr.sip6_addr16[1],
	 addr.sip6_addr16[2],
	 addr.sip6_addr16[3],
	 addr.sip6_addr16[4],
	 addr.sip6_addr16[5],
	 addr.sip6_addr16[6], addr.sip6_addr16[7]);

	y = 0;
	for (x = (strlen (inbuf) - 1); x >= 0; x--) {
		outbuf[y] = inbuf[x];
		outbuf[y + 1] = '.';
		y += 2;
	}

	safe_strncat (outbuf, "ip6.arpa.");

	printf("%s", outbuf);
}

void
print_rev_v6 (struct if_info *ifi)
{
	printf("[IPV6 DNS]\n");
	printf ("Reverse DNS (ip6.arpa)	-\n");
	print_revdns_v6 (ifi->v6ad.haddr);
	printf("\n");

	printf ("\n");
}

void
print_v6 (struct if_info *ifi)
{
	printf ("[IPV6 INFO]\n");
	printf ("Expanded Address	- ");
	print_exp_v6 (ifi->v6ad.haddr);
	printf ("\n");
	printf ("Compressed address	- ");
	print_comp_v6 (ifi->v6ad.haddr);
	printf ("\n");
	printf ("Subnet prefix (masked)	- ");
	print_mixed_v6 (ifi->v6ad.prefix);
	printf ("/%d\n", ifi->v6ad.nmaskbits);
	printf ("Address ID (masked)	- ");
	print_mixed_v6 (ifi->v6ad.suffix);
	printf ("/%d\n", ifi->v6ad.nmaskbits);
	printf ("Prefix address		- ");
	print_mixed_v6 (ifi->v6ad.nmask);
	printf ("\n");
	printf ("Prefix length		- %d\n", ifi->v6ad.nmaskbits);
	printf ("Address type		- %s\n", ifi->v6ad.class_remark);
	if (ifi->v6ad.comment[0])
		printf ("Comment			- %s\n",
			ifi->v6ad.comment);
	printf ("Network range		- ");
	print_exp_v6 (ifi->v6ad.prefix);
	printf (" -\n			  ");
	print_exp_v6 (ifi->v6ad.broadcast);
	printf ("\n");

	printf ("\n");

	return;
}

void
print_v4inv6 (struct if_info *ifi)
{
	printf ("[V4INV6]\n");
	if (ifi->v6ad.type == V6TYPE_V4INV6 && !ifi->v6ad.real_v4) {
		printf
		    ("-[INFO : Address was submitted as an IPv4-compatible IPv6 address]\n");
		printf
		    ("-[INFO : The Address does not qualify as this as per the guidelines]\n");
		printf ("-[INFO : in RFC2373]\n\n");
	}

	printf ("Expanded v4inv6 address	- ");
	print_exp_v4inv6 (ifi->v6ad.haddr);
	printf ("\n");
	printf ("Compr. v4inv6 address	- ");
	print_comp_v4inv6 (ifi->v6ad.haddr);
	printf ("\n");
	if (ifi->v6ad.type == V6TYPE_V4INV6 && ifi->v6ad.real_v4 == 1)
		printf
		    ("Comment			- IPv4-compatible IPv6 address\n");
	if (ifi->v6ad.type == V6TYPE_V4INV6 && ifi->v6ad.real_v4 == 2)
		printf
		    ("Comment			- IPv4-mapped IPv6 address\n");

	printf ("\n");

	return;
}

int
v6plus (struct sip_in6_addr *a, struct sip_in6_addr *b)
{
	int x, y, z;

	for (x = 7; x >= 0; x--) {
		if (a->sip6_addr16[x] + b->sip6_addr16[x] > 0xffff) {
			y = x - 1;
			z = 0;
			while (y >= 0 && !z) {
				z = 1;
				if (a->sip6_addr16[y] + 1 > 0xffff) {
					a->sip6_addr16[y] = 0;
					z = 0;
				}
				else {
					a->sip6_addr16[y]++;
				}
					
				y--;				
			}

			a->sip6_addr16[x] = a->sip6_addr16[x] + b->sip6_addr16[x] - 0x10000;
		}
		else {
			a->sip6_addr16[x] += b->sip6_addr16[x];
		}
	}

	return 0;
}

void
show_split_networks_v6 (struct if_info *ifi, struct sip_in6_addr splitmask, int v6args, struct misc_args m_argv6)
{
	struct sip_in6_addr sdiff, ediff, start, end, tmpaddr;
	int x, y, z;
	struct if_info ifi_tmp;
	int v6args_tmp;

	v6args_tmp = 0;

	if ((v6args & V6VERBSPLIT) == V6VERBSPLIT)
		printf ("[Split network - verbose]\n");
	else
		printf ("[Split network]\n");

	x = 0;
	y = 0;
	do {
		if (splitmask.sip6_addr16[x] > ifi->v6ad.nmask.sip6_addr16[x])
			y = 1;
		if (ifi->v6ad.nmask.sip6_addr16[x] > splitmask.sip6_addr16[x])
			y = 2;
		x++;
	} while (x < 8 && !y);
	if (y == 2) {
		printf ("-[ERR : Oversized splitmask]\n\n");
		return;
	}

	for (x = 0; x < 8; x++) {
		if(splitmask.sip6_addr16)
			sdiff.sip6_addr16[x] = 0xffffffff - splitmask.sip6_addr16[x];
		start.sip6_addr16[x] = ifi->v6ad.prefix.sip6_addr16[x];
		end.sip6_addr16[x] =
		    ifi->v6ad.prefix.sip6_addr16[x] + sdiff.sip6_addr16[x];
		ediff.sip6_addr16[x] = sdiff.sip6_addr16[x];
	}
	for (x = 0; x < 8; x++)
		tmpaddr.sip6_addr16[x] = 0;
	tmpaddr.sip6_addr16[7] = 1;
	v6plus (&sdiff, &tmpaddr);

	if ((v6args & V6VERBSPLIT) == V6VERBSPLIT) {
		memcpy ((struct if_info *) &ifi_tmp, (struct if_info *) ifi, sizeof(struct if_info));
		v6args_tmp = v6args ^ V6SPLIT;
		v6args_tmp = v6args_tmp ^ V6VERBSPLIT;
		if (!v6args_tmp)
			v6args_tmp = V6_INFO;
		ifi_tmp.next = NULL;
	}

	x = 0;
	while (!x) {
		if ((v6args & V6VERBSPLIT) != V6VERBSPLIT) {
			printf ("Network			- ");
			print_exp_v6 (start);
			printf (" -\n\t\t\t  ");
			print_exp_v6 (end);
			printf ("\n");
		}

		if ((v6args & V6VERBSPLIT) == V6VERBSPLIT) {
			safe_bzero (ifi_tmp.p_v4addr);
			safe_bzero (ifi_tmp.p_v4nmask);
			safe_bzero (ifi_tmp.p_v6addr);
			safe_snprintf (ifi_tmp.p_v6addr, "%s/%d", get_comp_v6 (start), m_argv6.v6splitnum);
		}

		v6plus (&start, &sdiff);

		y = 0;
		for (z = 0; z < 8; z++)
			if (end.sip6_addr16[z] != 0xffff)
				y = 1;
		if (!y)
			x = 1;

		y = 0;
		z = 0;
		do {
			if (end.sip6_addr16[z] >
			    ifi->v6ad.broadcast.sip6_addr16[z])
				y = 1;
			if (ifi->v6ad.broadcast.sip6_addr16[z] >
			    end.sip6_addr16[z])
				y = 2;
			z++;
		} while (z < 8 && !y);

		if (!y || y == 1)
			x = 1;

		for (z = 0; z < 8; z++)
			end.sip6_addr16[z] = 0;

		v6plus (&end, &start);
		v6plus (&end, &ediff);

		if ((v6args & V6VERBSPLIT) == V6VERBSPLIT)
			out_cmdline (&ifi_tmp, v6args_tmp, m_argv6, v6args_tmp, m_argv6, 1, 0);
	}

	printf ("\n");

	return;
}

#ifdef HAVE_GETOPT_LONG
void
print_help ()
{
	printf ("Usage: %s [OPTIONS]... <[ADDRESS]... [INTERFACE]... | [-]>\n\n", NAME);
	printf ("Global options:\n");
	printf ("  -a, --all\t\t\tAll possible information.\n");
	printf ("  -d, --resolve\t\t\tEnable name resolution.\n");
	printf ("  -h, --help\t\t\tDisplay this help.\n");
	printf ("  -I, --addr-int=INT\t\tAdded an interface.\n");
	printf ("  -n, --subnets=NUM\t\tDisplay NUM extra subnets (starting from\n");
	printf ("\t\t\t\tthe current subnet). Will display all subnets\n");
	printf ("\t\t\t\tin the current /24 if NUM is 0.\n");
	printf ("  -u, --split-verbose\t\tVerbose split.\n");
	printf ("  -v, --version\t\t\tVersion information.\n");
	printf ("  -4, --addr-ipv4=ADDR\t\tAdd an ipv4 address.\n");
	printf ("  -6, --addr-ipv6=ADDR\t\tAdd an ipv6 address.\n");
	printf ("\n");
	printf ("IPv4 options:\n");
	printf ("  -b, --cidr-bitmap\t\tCIDR bitmap.\n");
	printf ("  -c, --classful-addr\t\tClassful address information.\n");
	printf ("  -i, --cidr-addr\t\tCIDR address information. (default)\n");
	printf
	    ("  -s, --v4split=MASK\t\tSplit the current network into subnets\n");
	printf ("\t\t\t\tof MASK size.\n");
	printf ("  -w, --wildcard\t\tDisplay information for a wildcard\n");
	printf ("\t\t\t\t(inverse mask).\n");
	printf ("  -x, --classful-bitmap\tClassful bitmap.\n");
	printf ("\n");
	printf ("IPv6 options:\n");
	printf ("  -e, --v4inv6\t\t\tIPv4 compatible IPv6 information.\n");
	printf ("  -r, --v6rev\t\t\tIPv6 reverse DNS output.\n");
	printf ("  -S, --v6split=MASK\t\tSplit the current network into subnets\n\t\t\t\tof MASK size.\n");
	printf ("  -t, --v6-standard\t\tStandard IPv6. (default)\n");
	printf ("\n");
	printf ("Address must be in dotted quad forma, but the netmask can be given in three\n"
                "different ways:\n"
                " - Number of bits    [/nn]\n"
                " - Dotted quad       [nnn.nnn.nnn.nnn]\n"
                " - Hex               [0xnnnnnnnn | nnnnnnnn]\n"
                "\n"
                "Interface must be a valid network interface on the system.  Replacing address,\n"
                "or interface, with '-' will use stdin for reading further arguments.\n");

	return;
}
#else				/* ! HAVE_GETOPT_LONG */
void
print_help ()
{
	printf ("Usage: %s [OPTIONS]... <[ADDRESS]... [INTERFACE]... | [-]>\n\n", NAME);
	printf ("Global options:\n");
	printf ("  -a\t\tAll possible information.\n");
	printf ("  -d\t\tEnable name resolution.\n");
	printf ("  -h\t\tDisplay this help.\n");
	printf ("  -I\t\tAdded an interface.\n");
	printf
	    ("  -n n\t\tDisplay n extra subnets (starting from current subnet).\n");
	printf ("\t\tWill display all subnets in the current /24 if n is 0.\n");
	printf ("  -u\t\tVerbose split.\n");
	printf ("  -v\t\tVersion information.\n");
	printf ("  -4\t\tAdd an ipv4 address.\n");
	printf ("  -6\t\tAdd an ipv6 address.\n");
	printf ("\n");
	printf ("IPv4 options:\n");
	printf ("  -b\t\tCIDR bitmap.\n");
	printf ("  -c\t\tClassful address information.\n");
	printf ("  -i\t\tCIDR address information. (default)\n");
	printf
	    ("  -s\t\tSplit the current network into subnets of MASK size.\n");
	printf ("  -w\t\tDisplay information for a wildcard (inverse mask).\n");
	printf ("  -x\t\tClassful bitmap.\n");
	printf ("\n");
	printf ("IPv6 options:\n");
	printf ("  -e\t\tIPv4 compatible IPv6 information.\n");
	printf ("  -r\t\tIPv6 reverse DNS output.\n");
	printf ("  -S\t\tSplit the current network into subnets of MASK size.\n");
	printf ("  -t\t\tStandard IPv6. (default)\n");
	printf ("\n");
	printf ("Address must be in dotted quad forma, but the netmask can be given in three\n"
                "different ways:\n"
                " - Number of bits    [/nn]\n"
                " - Dotted quad       [nnn.nnn.nnn.nnn]\n"
                " - Hex               [0xnnnnnnnn | nnnnnnnn]\n"
                "\n"
                "Interface must be a valid network interface on the system.  Replacing address,\n"
                "or interface, with '-' will use stdin for reading further arguments.\n");

	return;
}
#endif  /* HAVE_GETOPT_LONG */

void
print_short_help ()
{
	printf ("Usage: %s [OPTIONS]... <[ADDRESS]... [INTERFACE]... | [-]>\n",
		NAME);
	printf ("Try '%s -h' for more information.\n", NAME);
}

void
print_version ()
{
	printf ("%s %s\n", NAME, VERSION);
	printf ("Copyright (C) 2003-2013 Simon Ekstrand\n"
                "Copyright (C) 2010-2015 Joachim Nilsson\n\n"
                "Licensed under the 3-Clause BSD <http://en.wikipedia.org/wiki/BSD_license>\n"
                "This is free software: you are free to change and redistribute it.\n"
                "There is NO WARRANTY, to the extent permitted by law.\n");
}
