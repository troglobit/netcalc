/*
 * Copyright (c) 2003-2013  Simon Ekstrand
 * Copyright (c) 2010-2015  Joachim Nilsson
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <string.h>

#include "netcalc.h"


void show_split_networks_v4(struct if_info *ifi, uint32_t splitmask, int v4args, struct misc_args m_argv4)
{
	uint32_t diff, start, end;
	size_t maxlen = 0;

	if (splitmask < ifi->v4ad.n_nmask) {
		warnx("Cannot subnet to /%d with this base network, use a prefix len > /%d",
		      numtolen(splitmask), numtolen(ifi->v4ad.n_nmask));
		return;
	}

	printf("[Split network/%d]\n", numtolen(splitmask));

	diff  = 0xffffffff - splitmask + 1;
	start = ifi->v4ad.n_naddr;
	end   = ifi->v4ad.n_naddr + diff - 1;

	/* Figure out max width of a network string. */
	while (1) {
		char buf[30];
		size_t len;

		len = snprintf(buf, sizeof(buf), "%s", numtoquad(start));
		if (len > maxlen)
			maxlen = len;

		start += diff;
		if (end == 0xffffffff || end >= ifi->v4ad.n_broadcast)
			break;
		end += diff;
	}

	start = ifi->v4ad.n_naddr;
	end = ifi->v4ad.n_naddr + diff - 1;
	while (1) {
		char buf[30];

		snprintf(buf, sizeof(buf), "%s", numtoquad(start));
		printf("Network  : \e[34m%-*s - %-15s\e[0m  ", (int)maxlen, buf, numtoquad(end));
		printf("Netmask  : \e[34m%s\e[0m\n", numtoquad(splitmask));

		start += diff;
		if (end == 0xffffffff || end >= ifi->v4ad.n_broadcast)
			break;
		end += diff;
	}

	printf("\n");

	return;
}

void print_cf_info_v4(struct if_info *ifi)
{
	uint32_t num, bcast, len, min, max;
	char temp[21];

	num = ifi->v4ad.n_broadcast - ifi->v4ad.n_naddr - 1;
	len = ifi->v4ad.n_nmaskbits;
	if (len > 30) {
		if (len == 31) {
			num = 2;
			min = ifi->v4ad.n_naddr;
			max = min + 1;
		} else
			num = 1;
	} else {
		min = ifi->v4ad.n_naddr + 1;
		max = ifi->v4ad.n_broadcast - 1;
	}

	printf("Address  : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad(ifi->v4ad.n_haddr), numtobitmap(ifi->v4ad.n_haddr, len));
//      printf ("Address (decimal): %u\n", ifi->v4ad.n_haddr);
//      printf ("Address (hex)    : %X\n", ifi->v4ad.n_haddr);
	snprintf(temp, sizeof(temp), "%s = %d", numtoquad(ifi->v4ad.n_nmask), ifi->v4ad.n_nmaskbits);
	printf("Netmask  : \e[34m%-20s\e[0m \e[31m%s\e[0m\n", temp, numtobitmap(ifi->v4ad.n_nmask, len));
//      printf ("Netmask (hex)  - %X\n", ifi->v4ad.n_cnmask);
	printf("Wildcard : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad(ifi->v4ad.n_nmask ^ 0xffffffff),
	       numtobitmap(ifi->v4ad.n_nmask ^ 0xffffffff, len));

	printf("=>\n");

	snprintf(temp, sizeof(temp), "%s/%d", numtoquad(ifi->v4ad.n_cnaddr), ifi->v4ad.n_nmaskbits);
	if (num >= 2) {
		size_t clen = ifi->v4ad.class - 'A' + 1;
		char buf['Z' - 'A'] = { 0 };
		char *net = numtobitmap(ifi->v4ad.n_cnaddr, len);

		strncpy(buf, net, clen);
		printf("Network  : \e[34m%-20s\e[0m \e[35m%s\e[0m\e[33m%s\e[0m\n", temp, buf, &net[clen]);
		printf("HostMin  : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad(min), numtobitmap(min, len));
		printf("HostMax  : \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad(max), numtobitmap(max, len));
		if (len < 31) {
			bcast = ifi->v4ad.n_broadcast;
			printf("Broadcast: \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad(bcast), numtobitmap(bcast, len));
		}
	} else {
		printf("HostRoute: \e[34m%-20s\e[0m \e[33m%s\e[0m\n", numtoquad(ifi->v4ad.n_cnaddr),
		       numtobitmap(ifi->v4ad.n_cnaddr, len));
	}

	printf("Hosts/Net: \e[34m%-20u\e[0m  ", num);
	if (ifi->v4ad.class == 'I')	/* Invalid */
		printf("\e[35mClass Invalid");
	else
		printf("\e[35mClass %c", ifi->v4ad.class);
	printf("\e[0m%s\n", ifi->v4ad.class_remark);
	printf("\n");
}

void print_comp_v6(struct sip_in6_addr addr)
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
				printf(":");
			printf(":");
			x += num - 1;
		} else {
			printf("%x", addr.sip6_addr16[x]);
			if (x != 7)
				printf(":");
		}
	}

	return;
}

void print_exp_v4inv6(struct sip_in6_addr addr)
{
	unsigned char num;

	printf("%04x:%04x:%04x:%04x:%04x:%04x:",
	       addr.sip6_addr16[0],
	       addr.sip6_addr16[1], addr.sip6_addr16[2], addr.sip6_addr16[3], addr.sip6_addr16[4], addr.sip6_addr16[5]);

	num = (addr.sip6_addr16[6] >> 8) & 0xff;
	printf("%d.", num);
	num = addr.sip6_addr16[6] & 0xff;
	printf("%d.", num);
	num = (addr.sip6_addr16[7] >> 8) & 0xff;
	printf("%d.", num);
	num = addr.sip6_addr16[7] & 0xff;
	printf("%d", num);

	return;
}

void print_comp_v4inv6(struct sip_in6_addr addr)
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
				printf(":");
			printf(":");
			x += num - 1;
		} else {
			printf("%x:", addr.sip6_addr16[x]);
		}
	}

	v4num = (addr.sip6_addr16[6] >> 8) & 0xff;
	printf("%d.", v4num);
	v4num = addr.sip6_addr16[6] & 0xff;
	printf("%d.", v4num);
	v4num = (addr.sip6_addr16[7] >> 8) & 0xff;
	printf("%d.", v4num);
	v4num = addr.sip6_addr16[7] & 0xff;
	printf("%d", v4num);

	return;
}

void print_exp_v6(struct sip_in6_addr addr)
{
	printf("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
	       addr.sip6_addr16[0], addr.sip6_addr16[1], addr.sip6_addr16[2], addr.sip6_addr16[3],
	       addr.sip6_addr16[4], addr.sip6_addr16[5], addr.sip6_addr16[6], addr.sip6_addr16[7]);
}

void print_mixed_v6(struct sip_in6_addr addr)
{
	printf("%x:%x:%x:%x:%x:%x:%x:%x",
	       addr.sip6_addr16[0], addr.sip6_addr16[1], addr.sip6_addr16[2], addr.sip6_addr16[3],
	       addr.sip6_addr16[4], addr.sip6_addr16[5], addr.sip6_addr16[6], addr.sip6_addr16[7]);
}

void print_revdns_v6(struct sip_in6_addr addr)
{
	char inbuf[40], outbuf[256];
	int x, y;

	memset(outbuf, 0, sizeof(outbuf));
	snprintf(inbuf, sizeof(inbuf), "%04x%04x%04x%04x%04x%04x%04x%04x",
		 addr.sip6_addr16[0], addr.sip6_addr16[1], addr.sip6_addr16[2], addr.sip6_addr16[3],
		 addr.sip6_addr16[4], addr.sip6_addr16[5], addr.sip6_addr16[6], addr.sip6_addr16[7]);

	y = 0;
	for (x = (strlen(inbuf) - 1); x >= 0; x--) {
		outbuf[y] = inbuf[x];
		outbuf[y + 1] = '.';
		y += 2;
	}

	strlcat(outbuf, "ip6.arpa.", sizeof(outbuf));

	printf("%s", outbuf);
}

void print_rev_v6(struct if_info *ifi)
{
	printf("[IPV6 DNS]\n");
	printf("Reverse DNS (ip6.arpa)	-\n");
	print_revdns_v6(ifi->v6ad.haddr);
	printf("\n");

	printf("\n");
}

void print_v6(struct if_info *ifi)
{
	printf("[IPV6 INFO]\n");
	printf("Expanded Address	- ");
	print_exp_v6(ifi->v6ad.haddr);
	printf("\n");
	printf("Compressed address	- ");
	print_comp_v6(ifi->v6ad.haddr);
	printf("\n");
	printf("Subnet prefix (masked)	- ");
	print_mixed_v6(ifi->v6ad.prefix);
	printf("/%d\n", ifi->v6ad.nmaskbits);
	printf("Address ID (masked)	- ");
	print_mixed_v6(ifi->v6ad.suffix);
	printf("/%d\n", ifi->v6ad.nmaskbits);
	printf("Prefix address		- ");
	print_mixed_v6(ifi->v6ad.nmask);
	printf("\n");
	printf("Prefix length		- %d\n", ifi->v6ad.nmaskbits);
	printf("Address type		- %s\n", ifi->v6ad.class_remark);
	if (ifi->v6ad.comment[0])
		printf("Comment			- %s\n", ifi->v6ad.comment);
	printf("Network range		- ");
	print_exp_v6(ifi->v6ad.prefix);
	printf(" -\n			  ");
	print_exp_v6(ifi->v6ad.broadcast);
	printf("\n");

	printf("\n");

	return;
}

void print_v4inv6(struct if_info *ifi)
{
	printf("[V4INV6]\n");
	if (ifi->v6ad.type == V6TYPE_V4INV6 && !ifi->v6ad.real_v4) {
		printf("-[INFO : Address was submitted as an IPv4-compatible IPv6 address]\n");
		printf("-[INFO : The Address does not qualify as this as per the guidelines]\n");
		printf("-[INFO : in RFC2373]\n\n");
	}

	printf("Expanded v4inv6 address	- ");
	print_exp_v4inv6(ifi->v6ad.haddr);
	printf("\n");
	printf("Compr. v4inv6 address	- ");
	print_comp_v4inv6(ifi->v6ad.haddr);
	printf("\n");
	if (ifi->v6ad.type == V6TYPE_V4INV6 && ifi->v6ad.real_v4 == 1)
		printf("Comment			- IPv4-compatible IPv6 address\n");
	if (ifi->v6ad.type == V6TYPE_V4INV6 && ifi->v6ad.real_v4 == 2)
		printf("Comment			- IPv4-mapped IPv6 address\n");

	printf("\n");

	return;
}

int v6plus(struct sip_in6_addr *a, struct sip_in6_addr *b)
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
				} else {
					a->sip6_addr16[y]++;
				}

				y--;
			}

			a->sip6_addr16[x] = a->sip6_addr16[x] + b->sip6_addr16[x] - 0x10000;
		} else {
			a->sip6_addr16[x] += b->sip6_addr16[x];
		}
	}

	return 0;
}

void show_split_networks_v6(struct if_info *ifi, struct sip_in6_addr splitmask, int v6args, struct misc_args m_argv6)
{
	struct sip_in6_addr sdiff, ediff, start, end, tmpaddr;
	int x, y, z;

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
		warnx("Oversized splitmask");
		return;
	}

	printf("[Split network]\n");

	for (x = 0; x < 8; x++) {
		if (splitmask.sip6_addr16)
			sdiff.sip6_addr16[x] = 0xffffffff - splitmask.sip6_addr16[x];
		start.sip6_addr16[x] = ifi->v6ad.prefix.sip6_addr16[x];
		end.sip6_addr16[x] = ifi->v6ad.prefix.sip6_addr16[x] + sdiff.sip6_addr16[x];
		ediff.sip6_addr16[x] = sdiff.sip6_addr16[x];
	}
	for (x = 0; x < 8; x++)
		tmpaddr.sip6_addr16[x] = 0;
	tmpaddr.sip6_addr16[7] = 1;
	v6plus(&sdiff, &tmpaddr);

	x = 0;
	while (!x) {
		printf("Network			- ");
		print_exp_v6(start);
		printf(" -\n\t\t\t  ");
		print_exp_v6(end);
		printf("\n");

		v6plus(&start, &sdiff);

		y = 0;
		for (z = 0; z < 8; z++)
			if (end.sip6_addr16[z] != 0xffff)
				y = 1;
		if (!y)
			x = 1;

		y = 0;
		z = 0;
		do {
			if (end.sip6_addr16[z] > ifi->v6ad.broadcast.sip6_addr16[z])
				y = 1;
			if (ifi->v6ad.broadcast.sip6_addr16[z] > end.sip6_addr16[z])
				y = 2;
			z++;
		} while (z < 8 && !y);

		if (!y || y == 1)
			x = 1;

		for (z = 0; z < 8; z++)
			end.sip6_addr16[z] = 0;

		v6plus(&end, &start);
		v6plus(&end, &ediff);
	}

	printf("\n");

	return;
}

int usage(int code)
{
	printf("Usage: %s [OPTIONS] <NETWORK/LEN | - | NETWORK NETMASK>\n"
	       "\n"
	       "Global options:\n"
	       "  -h       This help text\n"
	       "  -v       Show version information\n"
	       "\n"
	       "IPv4 options:\n"
	       "  -s MASK  Split the IPv4 network into subnets of MASK size\n"
	       "\n"
	       "IPv6 options:\n"
	       "  -e       IPv4 compatible IPv6 information\n"
	       "  -r       IPv6 reverse DNS output\n"
	       "  -S MASK  Split the IPv6 network into subnets of MASK size\n"
	       "\n"
	       "Copyright (C) 2003-2013  Simon Ekstrand\n"
	       "Copyright (C) 2010-2015  Joachim Nilsson\n"
	       "\n"
	       "This is free software, under the 3-clause BSD license: you are free to change\n"
	       "and redistribute it.  There is NO WARRANTY, to the extent permitted by law\n", __progname);

	return code;
}

/**
 * Local Variables:
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
