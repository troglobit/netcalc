/*
 * Copyright (c) 2003-2013  Simon Ekstrand
 * Copyright (c) 2010-2017  Joachim Nilsson
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
#include <ctype.h>		/* isxdigit() */
#include <stdio.h>
#include <stdlib.h>		/* atoi(), strtoul(), strtol() */
#include <string.h>
#include "netcalc.h"

#define INTLEN_MAX 10


int count(char *buf, char ch)
{
	int i, j;

	j = 0;
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i] == ch)
			j++;
	}

	return j;
}

/*
 * 0 error
 * 1 normal
 */
int validate_v4addr(char *addr)
{
	int i, j, k, m;
	char buf[16];

	if (strlen(addr) < 7 || strlen(addr) > 15)
		return 0;

	i = 0;
	j = 0;
	while (i < strlen(addr)) {
		if (addr[i++] == '.')
			j++;
	}
	if (j != 3)
		return 0;

	i = 0;
	j = 0;
	while (i < strlen(addr)) {
		k = 0;
		j = 0;
		while (k < strlen(V4ADDR_VAL) && !j) {
			if (addr[i] == V4ADDR_VAL[k])
				j = 1;
			k++;
		}
		if (!j)
			return 0;
		i++;
	}

	if (addr[0] == '.' || addr[strlen(addr) - 1] == '.')
		return 0;

	i = 0;
	while (i < (strlen(addr) - 1)) {
		if (addr[i] == '.' && addr[i + 1] == '.')
			return 0;
		i++;
	}

	j = 0;
	for (i = 0; i < 4; i++) {
		k = 0;
		memset(buf, 0, sizeof(buf));
		while (addr[j] != '.' && j < strlen(addr))
			buf[k++] = addr[j++];

		if (k > 3)
			return 0;

		m = atoi(buf);
		if (m < 0 || m > 255)
			return 0;
		j++;
	};

	return 1;
}

/*
 * 0 error
 * 1 normal
 * 2 /x
 * 3 hex
 */
int validate_netmask(char *in_addr)
{
	int i = 0, j = 0, k = 0, m;
	char addr[16];
	char *sl;
	uint32_t sm;

	if (strlen(in_addr) > 18)
		return 0;

	while (i < strlen(in_addr) && !j) {
		if (!isxdigit(in_addr[i]) && in_addr[i] != 'x' && in_addr[i] != 'X')
			j = 1;
		if (in_addr[i] == 'x' || in_addr[i] == 'X') {
			k++;
			j = 0;
		}
		i++;
	}

	i = 0;
	if (!j && k == 1) {
		i = 0;
		if (in_addr[0] == 'x' || in_addr[0] == 'X')
			i = 1;
		if (!i && in_addr[0] == '0' && (in_addr[1] == 'x' || in_addr[1] == 'X'))
			i = 1;
	}
	if (i == 1)
		return 3;

	memset(addr, 0, sizeof(addr));
	if (strstr(in_addr, "/")) {
		i = 0;
		while (in_addr[i] != '/' && i < 15) {
			addr[i] = in_addr[i];
			i++;
		}
	} else {
		strlcpy(addr, in_addr, sizeof(addr));
	}

	/* This also calls validate_v4addr() */
	if (quadtonum(addr, &sm))
		return 0;

	if (!(sl = strstr(in_addr, "/"))) {
		j = 0;
		for (i = 0; i < 32; i++) {
			if (!j && !((sm >> (31 - i)) & 1))
				j = 1;
			if (j == 1 && ((sm >> (31 - i)) & 1))
				j = 2;
		}
		if (j == 2)
			return -1;

		return 1;	/* Valid */
	}

	sl++;
	if (strlen(sl) < 1 || strlen(sl) > 2)
		return 0;

	if (sl[0] == '.' || sl[1] == '.')
		return 0;

	i = 0;
	j = 0;
	while (i < strlen(sl)) {
		k = 0;
		j = 0;
		while (k < strlen(V4ADDR_VAL) && !j) {
			if (sl[i] == V4ADDR_VAL[k])
				j = 1;
			k++;
		}
		if (!j)
			return 0;
		i++;
	}

	m = atoi(sl);
	if (m < 0 || m > 32)
		return 0;

	return 2;
}

int getrange_min_max(char *buf, uint32_t *range_min, uint32_t *range_max)
{
	int i, j;
	char min_buf[12], max_buf[12];
	uint32_t min, max;

	if (strlen(buf) < 1)
		return -1;

	if (count(buf, ':') != 1)
		return -1;

	for (i = 0; buf[i] != ':' && i < INTLEN_MAX; i++)
		min_buf[i] = buf[i];
	min_buf[i] = '\0';

	for (j = 0; buf[i] != '\0' && j < INTLEN_MAX; i++, j++)
		max_buf[j++] = buf[i++];
	max_buf[j] = '\0';

	i = atoi(min_buf);
	if (i < 1)
		return -1;
	min = i;

	i = atoi(max_buf);
	if (i < 1)
		return -1;
	max = i;

	if (min >= max)
		return -1;

	*range_min = min;
	*range_max = max;

	return 0;
}

int getsplitnumv4(char *buf, uint32_t *splitmask)
{
	int i, j;
	uint32_t sm;

	if (strlen(buf) < 1)
		return -1;

	if (strlen(buf) < 4) {
		if (buf[0] == '/')
			buf++;
		j = atoi(buf);
		if (j < 1 || j > 32)
			return -1;

		sm = 0;
		for (i = 0; i < j; i++)
			sm = sm | (1 << (31 - i));
	} else {
		if (!strstr(buf, "."))
			sm = strtoul(buf, (char **)NULL, 16);
		else
			if (quadtonum(buf, &sm))
				return -1;
	}

	j = 0;
	for (i = 0; i < 32; i++) {
		if (!j && !((sm >> (31 - i)) & 1))
			j = 1;
		if (j == 1 && ((sm >> (31 - i)) & 1))
			j = 2;
	}
	if (j == 2)
		return -1;

	*splitmask = sm;

	return 0;
}

int getsplitnumv6(char *buf, struct sip_in6_addr *splitmask, int *v6splitnum)
{
	int i;

	if (strlen(buf) < 1)
		return -1;
	if (strlen(buf) > 4)
		return -1;

	if (buf[0] == '/')
		buf++;

	i = atoi(buf);
	if (i < 1 || i > 128)
		return -1;

	*v6splitnum = i;
	v6masktonum(buf, &i, splitmask);

	return 0;
}

int quadtonum(char *quad, uint32_t *num)
{
	char buf[128];
	int i, j, k;

	if (!validate_v4addr(quad))
		return -1;

	memset(buf, 0, sizeof(buf));
	for (i = 0; quad[i] != '.'; i++)
		buf[i] = quad[i];

	*num = 0;
	i++;
	for (j = 0; j < 4; j++) {
		k = atoi(buf);
		if (k > 255 || k < 0)
			return -1;

		*num = *num | (k << (8 * (3 - j)));
		memset(buf, 0, sizeof(buf));
		for (k = 0; quad[i] != '.' && quad[i] != '\0' && i < strlen(quad); k++)
			buf[k] = quad[i++];
		i++;
	}

	return 0;
}

char *numtoquad(uint32_t num)
{
	int a[4], i;
	static char quad[17];

	for (i = 0; i < 4; i++)
		a[i] = num >> (8 * (3 - i)) & 0xff;

	snprintf(quad, sizeof(quad), "%d.%d.%d.%d", a[0], a[1], a[2], a[3]);

	return quad;
}

char *numtobitmap(uint32_t num, uint32_t prefix_len)
{
	static char bitmap[100];
	int i, j, k;

	memset(bitmap, 0, sizeof(bitmap));
	j = 1;
	k = 0;
	for (i = 0; i < 32; i++) {
		if (!((num >> (31 - i)) & 1))
			bitmap[k] = '0';
		else
			bitmap[k] = '1';
		if (j == 8 && k < 34) {
			bitmap[++k] = '.';
			j = 0;
		}
		if (i == prefix_len - 1)
			bitmap[++k] = ' ';
		j++;
		k++;
	}

	return bitmap;
}

/* Note: Only applicable to netmasks */
uint32_t numtolen(uint32_t num)
{
	uint32_t len = 0;

	while ((num <<= 1))
		len++;

	return len + 1;
}

int parse_addr(struct if_info *ifi)
{
	char buf[128], buf2[128];
	char *s_find;
	int i, j, k;

	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	ifi->v4ad.n_nmaskbits = 0;

	/*
	 * netmask
	 */
	if (ifi->p_v4nmask[0] == '\0') {
		/*
		 * "/netmask"
		 */
		if (!(s_find = strstr(ifi->p_v4addr, "/"))) {
			ifi->v4ad.n_nmask = 0xffffffff;
			ifi->v4ad.n_nmaskbits = 32;
		} else {
			*s_find = '\0';
			s_find++;
			if (!*s_find)
				return -1;
			if (strlen(s_find) > 2)
				return -2;

			i = 0;
			j = 0;
			while (i < strlen(s_find)) {
				j = 0;
				for (k = 0; k < strlen(NETMASK_VAL) && !j; k++) {
					if (s_find[i] == NETMASK_VAL[k])
						j = 1;
				}
				if (!j)
					return -2;
				i++;
			}

			buf[0] = *s_find;
			*s_find = '\0';
			s_find++;
			if (*s_find) {
				buf[1] = *s_find;
				*s_find = '\0';
			}

			ifi->v4ad.n_nmaskbits = atoi(buf);

			ifi->v4ad.n_nmask = 0;
			for (i = 0; i < ifi->v4ad.n_nmaskbits; i++)
				ifi->v4ad.n_nmask = ifi->v4ad.n_nmask | (1 << (31 - i));
		}
	} else {
		/*
		 * hex netmask
		 */
		if (!strstr(ifi->p_v4nmask, "."))
			ifi->v4ad.n_nmask = strtoul(ifi->p_v4nmask, (char **)NULL, 16);
		/*
		 * "normal" netmask
		 */
		else {
			if (quadtonum(ifi->p_v4nmask, &ifi->v4ad.n_nmask))
				return -2;
		}
	}

	if (ifi->v4ad.n_nmaskbits < 0 || ifi->v4ad.n_nmaskbits > 32)
		return -2;

	/*
	 * host address
	 */
	if (quadtonum(ifi->p_v4addr, &ifi->v4ad.n_haddr))
		return -1;

	return 0;
}

/*
 * return
 * 0  ok
 * -1 invalid address
 * -2 invalid netmask
 */
int get_addrv4(struct if_info *ifi)
{
	uint32_t i, j, k, len;
	char *rfc = NULL;
	size_t sz;

	i = 0;
	if (!ifi->name[0])
		i = parse_addr(ifi);

	if (i)
		return i;

	/*
	 * count netmask bits
	 */
	ifi->v4ad.n_nmaskbits = 0;
	for (i = 0; i < 32; i++) {
		if ((ifi->v4ad.n_nmask >> i) & 1)
			ifi->v4ad.n_nmaskbits++;
	}
	if (ifi->v4ad.n_nmaskbits < 0 || ifi->v4ad.n_nmaskbits > 32)
		return -2;

	/*
	 * validate netmask
	 */
	j = 0;
	for (i = 0; i < 32; i++) {
		if (!j && !((ifi->v4ad.n_nmask >> (31 - i)) & 1))
			j = 1;
		if (j == 1 && ((ifi->v4ad.n_nmask >> (31 - i)) & 1))
			j = 2;
	}
	if (j == 2)
		return -2;

	/*
	 * network class, class remark and classful netmask
	 * For details, and a possible RFC URL extension, see http://en.wikipedia.org/wiki/IPv4
	 */
	len = ifi->v4ad.n_nmaskbits;
	memset(ifi->v4ad.class_remark, 0, sizeof(ifi->v4ad.class_remark));
	ifi->v4ad.n_cnaddr = 0;

	i = ifi->v4ad.n_haddr >> 24;
	j = (ifi->v4ad.n_haddr >> 16) & 0xFF;
	k = (ifi->v4ad.n_haddr >> 8) & 0xFF;
	if (!(i & 0x80)) {
		ifi->v4ad.class = 'A';
		ifi->v4ad.n_cnmask = 0xff000000;

		if (i == 0)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sCurrent network",
				 len < 8 ? "In Part " : ""), rfc = "RFC1700";
		if (i == 10)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sPrivate network",
				 len < 8 ? "In Part " : ""), rfc = "RFC1928";
		if (i == 127)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sLoopback network",
				 len < 8 ? "In Part " : ""), rfc = "RFC5735";
	}
	if ((i & 0xc0) == 0x80) {
		ifi->v4ad.class = 'B';
		ifi->v4ad.n_cnmask = 0xffff0000;

		if (i == 169 && j == 254)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sLink-Local",
				 len < 16 ? "In Part " : ""), rfc = "RFC3927";
		if (i == 172 && (j & 0xF0) == 16)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sPrivate network",
				 len < 12 ? "In Part " : ""), rfc = "RFC1918";
	}
	if ((i & 0xe0) == 0xc0) {
		ifi->v4ad.class = 'C';
		ifi->v4ad.n_cnmask = 0xffffff00;

		if (i == 192 && (j & 0xF8) == 168)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sPrivate network",
				 len < 16 ? "In Part " : ""), rfc = "RFC1918";
		if (i == 192 && j == 88 && k == 99)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sIPv6 to IPv4 relay",
				 len < 24 ? "In Part " : ""), rfc = "RFC3068";
		if (i == 192 && j == 0 && k == 0)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sReserved, IANA",
				 len < 24 ? "In Part " : ""), rfc = "RFC5735";
		if (i == 192 && j == 0 && k == 2)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark),
				 ", %sTEST-NET-1, Documentation and examples", len < 24 ? "In Part " : ""), rfc = "RFC5735";
		if (i == 198 && j == 18 && k == 0)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", %sNetwork benchmark tests",
				 len < 15 ? "In Part " : ""), rfc = "RFC2544";
		if (i == 198 && j == 51 && k == 100)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark),
				 ", %sTEST-NET-2, Documentation and examples", len < 24 ? "In Part " : ""), rfc = "RFC5737";
		if (i == 203 && j == 0 && k == 113)
			snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark),
				 ", %sTEST-NET-3, Documentation and examples", len < 24 ? "In Part " : ""), rfc = "RFC5737";
	}
	if ((i & 0xf0) == 0xe0) {
		ifi->v4ad.class = 'D';
		ifi->v4ad.n_cnmask = ifi->v4ad.n_nmask;

		snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", Multicast"), rfc = "RFC3171";
	}
	if ((i & 0xf8) == 0xf0) {
		ifi->v4ad.class = 'E';
		ifi->v4ad.n_cnmask = ifi->v4ad.n_nmask;

		snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", Reserved for future use"), rfc = "RFC1700";
	}
	if (i == 255) {
		ifi->v4ad.class = 'I';
		ifi->v4ad.n_cnmask = ifi->v4ad.n_nmask;

		snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", Broadcast"), rfc = "RFC919";
	}

	if (ifi->v4ad.class == '\0') {
		ifi->v4ad.class = 'I';	/* MAX Classes! */
		ifi->v4ad.n_cnmask = ifi->v4ad.n_nmask;
//              ifi->v4ad.n_cnmask = 0xffffffff;
		snprintf(ifi->v4ad.class_remark, sizeof(ifi->v4ad.class_remark), ", Nonexistent");
	}

	sz = strlen(ifi->v4ad.class_remark);
	if (len == 31 && rfc)
		snprintf(&ifi->v4ad.class_remark[sz], 64 - sz, ", PtP Link (%s, RFC3021)", rfc);
	else if (rfc)
		snprintf(&ifi->v4ad.class_remark[sz], 64 - sz, " (%s)", rfc);

	/*
	 * network address (classful + cidr)
	 */
	ifi->v4ad.n_naddr = ifi->v4ad.n_haddr & ifi->v4ad.n_nmask;
	ifi->v4ad.n_cnaddr = ifi->v4ad.n_haddr & ifi->v4ad.n_cnmask;

	/*
	 * cidr broadcast address
	 */
	ifi->v4ad.n_broadcast = 0xffffffff - ifi->v4ad.n_nmask + ifi->v4ad.n_naddr;

	return 0;
}

/*
 * return
 * 0  ok
 * -1 invalid address
 * -2 invalid netmask
 */
int get_addrv6(struct if_info *ifi)
{
	int i;

	i = mk_ipv6addr(&ifi->v6ad, ifi->p_v6addr);
	if (i < 0)
		return -1;

	return 0;
}

int split_ipv6addr(char *addr, struct ipv6_split *spstr)
{
	char *split;
	int i, j, k;

	split = strstr(addr, "/");
	if (split && (count(addr, '/') == 1)) {
		if (strlen(split) > 1 && strlen(split) < 5) {
			split++;
			strlcpy(spstr->nmask, split, sizeof(spstr->nmask));
		}
	}

	i = 0;
	j = 0;
	split = strstr(addr, ".");
	if (split)
		i = strlen(split);
	split = strstr(addr, ":");
	if (split)
		j = strlen(split);
	if (i < j)
		i = 1;
	else
		i = 0;

	if (count(addr, '.') == 3 && i) {
		split = strstr(addr, ".");
		i = strlen(addr) - strlen(split);
		while (addr[i] != ':')
			i--;
		i++;
		split = strstr(addr, "/");
		if (split)
			j = strlen(addr) - strlen(split);
		else
			j = strlen(addr);
		if ((j - i >= 7) && (j - i <= 15)) {
			for (k = 0; k < j - i; k++)
				spstr->ipv4addr[k] = addr[i + k];
		}
	}

	i = strlen(addr) - (strlen(spstr->ipv4addr) + strlen(spstr->nmask));
	if (strlen(spstr->nmask) > 0)
		i--;
	if (i > 1 && i < 40)
		strncpy(spstr->ipv6addr, addr, i);

	return 0;
}

int validate_s_v6addr(char *addr, int type)
{
	int i, j, k;
	int numcolon;
	int compressed;

	if (strlen(addr) < 2)
		return -1;

	if (strlen(addr) > 39)
		return -1;

	i = 0;
	j = 0;
	while (i < strlen(addr)) {
		j = 0;
		
		for (k = 0; k < strlen(V6ADDR_VAL) && !j; k++) {
			if (addr[i] == V6ADDR_VAL[k])
				j = 1;
		}
		if (!j)
			return -1;
		i++;
	}

	i = 0;
	j = 0;
	while (i < strlen(addr)) {
		if (addr[i] == ':')
			j++;
		else
			j = 0;

		if (j == 3)
			return -1;

		i++;
	}

	if (addr[0] == ':' && addr[1] != ':')
		return -1;

	if (type == V6TYPE_STANDARD && addr[strlen(addr) - 1] == ':' && addr[strlen(addr) - 2] != ':')
		return -1;

	numcolon = 0;
	for (i = 0; i < strlen(addr); i++) {
		if (addr[i] == ':')
			numcolon++;
	}

	compressed = 0;
	i = 0;
	while (i < strlen(addr) - 1) {
		if (addr[i] == ':' && addr[i + 1] == ':')
			compressed++;
		i++;
	}

	if (compressed > 1)
		return -1;

	if (!compressed && numcolon != 7 && type == V6TYPE_STANDARD)
		return -1;

	if (!compressed && numcolon != 6 && type == V6TYPE_V4INV6)
		return -1;

	if (compressed && type == V6TYPE_STANDARD) {
		if (numcolon > 7)
			return -1;
	}

	if (compressed && type == V6TYPE_V4INV6) {
		if (numcolon > 6)
			return -1;
	}

	j = 0;
	for (i = 0; i < strlen(addr); i++) {
		if (addr[i] != ':')
			j++;
		else
			j = 0;

		if (j > 4)
			break;
	}

	if (j > 4)
		return -1;

	return 0;
}

int getcolon(char *addr, int pos, int type)
{
	int i, j;
	int compressed;
	int cstart, cend;
	int max;
	char str[5];

	if (type == V6TYPE_STANDARD)
		max = 7;
	if (type == V6TYPE_V4INV6)
		max = 5;

	compressed = 0;
	for (i = 0; i < strlen(addr) - 1; i++) {
		if (addr[i] == ':' && addr[i + 1] == ':')
			compressed++;
	}

	if (compressed) {
		cstart = 0;
		for (i = 0; i < strlen(addr) - 1; i++) {
			if (addr[i] == ':' && addr[i + 1] == ':')
				break;

			if (addr[i] == ':')
				cstart++;
		}
		i += 2;

		cend = 0;
		while (i < strlen(addr)) {
			if (addr[i] == ':')
				cend++;
			i++;
		}
		if (addr[strlen(addr) - 1] == ':' && addr[strlen(addr) - 2] != ':')
			cend--;
	}

	if (!compressed) {
		i = 0;
		for (j = 0; i < pos; j++) {
			if (addr[j] == ':')
				i++;
		}

		memset(str, 0, sizeof(str));
		i = 0;
		while (j < strlen(addr) && addr[j] != ':')
			str[i++] = addr[j++];
	}

	if (compressed) {
		memset(str, 0, sizeof(str));
		if (pos <= cstart) {
			i = 0;
			
			for (j = 0; i < pos; j++) {
				if (addr[j] == ':')
					i++;
			}

			i = 0;
			while (j < strlen(addr) && addr[j] != ':')
				str[i++] = addr[j++];
		}

		if ((pos > cstart) && (pos < (max - cend)))
			str[0] = '0';

		if (pos >= (max - cend)) {
			
			for (j = 0; j < strlen(addr) - 1; j++) {
				if (addr[j] == ':' && addr[j + 1] == ':')
					break;
			}
			j += 2;

			i = max - cend;
			while (i < pos) {
				if (addr[j++] == ':')
					i++;
			}

			memset(str, 0, sizeof(str));
			for (i = 0; j < strlen(addr) && addr[j] != ':'; i++)
				str[i] = addr[j++];
		}

	}

	if (str[0] == '\0')
		str[0] = '0';

	return strtol(str, NULL, 16);
}

int v6addrtonum(struct ipv6_split spstr, struct v6addr *in6_addr, int type)
{
	int colon;
	int i, j, k, n;
	char buf[128];

	colon = 0;
	if (type == V6TYPE_STANDARD)
		colon = 8;
	if (type == V6TYPE_V4INV6)
		colon = 6;

	for (i = 0; i < 4; i++)
		in6_addr->haddr.sip6_addr32[i] = 0;

	for (i = 0; i < colon; i++)
		in6_addr->haddr.sip6_addr16[i] = getcolon(spstr.ipv6addr, i, type);

	if (type == V6TYPE_V4INV6) {
		memset(buf, 0, sizeof(buf));
		for (i = 0; spstr.ipv4addr[i] != '.'; i++)
			buf[i] = spstr.ipv4addr[i];
		i++;

		for (j = 0; j < 4; j++) {
			if (j == 1)
				in6_addr->haddr.sip6_addr16[6] = (n << 8) | atoi(buf);

			if (j == 3)
				in6_addr->haddr.sip6_addr16[7] = (n << 8) | atoi(buf);

			n = atoi(buf);

			memset(buf, 0, sizeof(buf));
			
			for (k = 0; spstr.ipv4addr[i] != '.' && spstr.ipv4addr[i] != '\0' && i < strlen(spstr.ipv4addr); k++)
				buf[k] = spstr.ipv4addr[i++];
			i++;
		}
	}

	return 0;
}

int v6masktonum(char *nmask, int *nmaskbits, struct sip_in6_addr *in6_addr)
{
	int i, j, k;

	if (nmask[0] == '\0')
		*nmaskbits = 128;
	else
		*nmaskbits = atoi(nmask);

	for (i = 0; i < 4; i++)
		in6_addr->sip6_addr32[i] = 0;

	j = 0;
	k = 0;
	for (i = 0; i < *nmaskbits; i++) {
		in6_addr->sip6_addr16[j] = in6_addr->sip6_addr16[j] | (1 << (15 - k));
		k++;
		if (k == 16) {
			k = 0;
			j++;
		}
	}

	return 0;
}

/*
 * 0 error
 * 1 normal
 */
int validate_v6addr(char *addr)
{
	int i;
	struct ipv6_split spstr;

	memset(spstr.ipv6addr, 0, sizeof(spstr.ipv6addr));
	memset(spstr.ipv4addr, 0, sizeof(spstr.ipv4addr));
	memset(spstr.nmask, 0, sizeof(spstr.nmask));

	split_ipv6addr(addr, &spstr);

	if (!spstr.ipv6addr[0])
		return 0;

	i = V6TYPE_STANDARD;
	if (spstr.ipv4addr[0])
		i = V6TYPE_V4INV6;

	if (validate_s_v6addr(spstr.ipv6addr, i) < 0)
		return 0;

	if (spstr.ipv4addr[0]) {
		if (!validate_v4addr(spstr.ipv4addr))
			return 0;
	}

	return 1;
}

int v6addrtoprefix(struct v6addr *in6_addr)
{
	int i;

	for (i = 0; i < 8; i++)
		in6_addr->prefix.sip6_addr16[i] = in6_addr->haddr.sip6_addr16[i] & in6_addr->nmask.sip6_addr16[i];

	return 0;
}

int v6addrtosuffix(struct v6addr *in6_addr)
{
	int i;

	for (i = 0; i < 8; i++)
		in6_addr->suffix.sip6_addr16[i] = in6_addr->haddr.sip6_addr16[i] & (in6_addr->nmask.sip6_addr16[i] ^ 0xffff);

	return 0;
}

int v6addrtobroadcast(struct v6addr *in6_addr)
{
	int i;

	for (i = 0; i < 8; i++)
		in6_addr->broadcast.sip6_addr16[i] = 0xffff - in6_addr->nmask.sip6_addr16[i] + in6_addr->prefix.sip6_addr16[i];

	return 0;
}

void v6_type(struct v6addr *in6_addr)
{
	uint16_t a = in6_addr->haddr.sip6_addr16[0];
	char  *buf = in6_addr->class_remark;
	size_t len = sizeof(in6_addr->class_remark);

	if (a == 0)
		snprintf(buf, len, "Reserved");
	if (a == 2 || a == 3)
		snprintf(buf, len, "Reserved for NSAP Allocation");
	if (a == 4 || a == 5)
		snprintf(buf, len, "Reserved for IPX Allocation");
	if ((a & 0xe000) == 0x2000)
		snprintf(buf, len, "Aggregatable Global Unicast Addresses");
	if ((a | 0x00ff) == 0x00ff)
		snprintf(buf, len, "Reserved");
	if ((a & 0xff00) == 0xff00)
		snprintf(buf, len, "Multicast Addresses");
	if ((a & 0xff80) == 0xfe80)
		snprintf(buf, len, "Link-Local Unicast Addresses");
	if ((a & 0xffc0) == 0xfec0)
		snprintf(buf, len, "Site-Local Unicast Addresses");

	if (buf[0] == '\0')
		snprintf(buf, len, "Unassigned");

	return;
}

void v6_comment(struct v6addr *in6_addr)
{
	int i, j;

	j = 0;
	for (i = 0; i < 8; i++) {
		if (in6_addr->haddr.sip6_addr16[i])
			j = 1;
	}
	if (!j)
		snprintf(in6_addr->comment, sizeof(in6_addr->comment), "Unspecified");

	j = 0;
	for (i = 0; i < 7; i++) {
		if (in6_addr->haddr.sip6_addr16[i])
			j = 1;
	}
	if (!j) {
		if (in6_addr->haddr.sip6_addr16[7] == 1)
			snprintf(in6_addr->comment, sizeof(in6_addr->comment), "Loopback");
	}
}

int v6verifyv4(struct sip_in6_addr addr)
{
	int i, j;

	j = 0;
	for (i = 0; i < 5; i++) {
		if (addr.sip6_addr16[i])
			j = 1;
	}

	if (!j) {
		if (!addr.sip6_addr16[5])
			return 1;

		if (addr.sip6_addr16[5] == 0xffff)
			return 2;
	}

	return 0;
}

char *get_comp_v6(struct sip_in6_addr addr)
{
	static char outad[44];
	char tmpad[5];
	int i, j, k;
	int start, num;

	memset(outad, 0, sizeof(outad));

	start = -1;
	num = 0;
	j = 0;
	k = 0;
	for (i = 0; i < 8; i++) {
		if (addr.sip6_addr16[i] == 0) {
			if (j == -1)
				j = i;
			k++;
		} else {
			if (k > num && k > 1) {
				start = j;
				num = k;
			}
			j = -1;
			k = 0;
		}
	}

	if (k > num && k > 1) {
		start = j;
		num = k;
	}

	for (i = 0; i < 8; i++) {
		if (i == start) {
			if (!i)
				strlcat(outad, ":", sizeof(outad));
			strlcat(outad, ":", sizeof(outad));
			i += num - 1;
		} else {
			memset(tmpad, 0, sizeof(tmpad));
			snprintf(tmpad, sizeof(tmpad), "%x", addr.sip6_addr16[i]);
			strlcat(outad, tmpad, sizeof(outad));
			if (i != 7)
				strlcat(outad, ":", sizeof(outad));
		}
	}

	return outad;
}

int mk_ipv6addr(struct v6addr *in6_addr, char *addr)
{
	int i, j, k;
	struct ipv6_split spstr;

	memset(spstr.ipv6addr, 0, sizeof(spstr.ipv6addr));
	memset(spstr.ipv4addr, 0, sizeof(spstr.ipv4addr));
	memset(spstr.nmask, 0, sizeof(spstr.nmask));

	split_ipv6addr(addr, &spstr);

	if (!spstr.ipv6addr[0])
		return -1;

	i = V6TYPE_STANDARD;
	if (spstr.ipv4addr[0])
		i = V6TYPE_V4INV6;

	if (validate_s_v6addr(spstr.ipv6addr, i) < 0)
		return -1;

	if (spstr.ipv4addr[0] && !validate_v4addr(spstr.ipv4addr))
		return -1;

	i = 0;
	j = 0;
	while (i < strlen(spstr.nmask)) {
		j = 0;
		k = 0;
		while (k < strlen(NETMASK_VAL) && !j) {
			if (spstr.nmask[i] == NETMASK_VAL[k])
				j = 1;
			k++;
		}
		if (!j)
			return -1;

		i++;
	}

	i = atoi(spstr.nmask);
	if (i < 0 || i > 128)
		return -1;

	if (spstr.ipv4addr[0] == '\0')
		in6_addr->type = V6TYPE_STANDARD;
	else
		in6_addr->type = V6TYPE_V4INV6;

	v6addrtonum(spstr, in6_addr, in6_addr->type);
	v6masktonum(spstr.nmask, &in6_addr->nmaskbits, &in6_addr->nmask);
	v6addrtoprefix(in6_addr);
	v6addrtosuffix(in6_addr);
	v6addrtobroadcast(in6_addr);
	in6_addr->real_v4 = v6verifyv4(in6_addr->haddr);

	memset(in6_addr->class_remark, 0, sizeof(in6_addr->class_remark));
	v6_type(in6_addr);
	memset(in6_addr->comment, 0, sizeof(in6_addr->comment));
	v6_comment(in6_addr);

	return 0;
}

/**
 * Local Variables:
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
