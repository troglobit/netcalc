/*
 * Copyright (c) 2003-2013  Simon Ekstrand
 * Copyright (c) 2010-2020  Joachim Nilsson
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

#ifndef NETCALC_H_
#define NETCALC_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
#include <sys/types.h>		/* u_intN_t */
#endif
#include <err.h>		/* warnx() et al */
#include <net/if.h>		/* IFNAMSIZ */

/*** Fallback to older types before stdint.h was defined ***/
#ifndef HAVE_UINT8_T
#define uint8_t u_int8_t
#endif

#ifndef HAVE_UINT16_T
#define uint16_t u_int16_t
#endif

#ifndef HAVE_UINT32_T
#define uint32_t u_int32_t
#endif
/*** End of fallback ***/

#define V4ADDR_VAL "0123456789."
#define V6ADDR_VAL "0123456789ABCDEFabcdef:"
#define NETMASK_VAL "0123456789"

#define AT_V4 1
#define AT_V6 2
#define AT_INT 3
#define AT_UNKWN 4

#define IFT_V4 1
#define IFT_V6 2
#define IFT_INTV4 3
#define IFT_INTV6 4
#define IFT_UNKWN 5

#define ARGLEN 128

/* v4 args */
#define V4_INFO     0x01
#define V4RANGE     0x08
#define V4CHECK     0x10
#define V4SPLIT     0x20
#define C_WILDCARD  0x40

/* v6 args */
#define V6_INFO     0x01
#define V4INV6      0x02
#define V6SPLIT     0x04
#define V6REV       0x08
#define V6CHECK     0x10

#define V6TYPE_STANDARD 1
#define V6TYPE_V4INV6 2

/*
 * Easier to define this ourselves than to use all the different
 * versions from different platforms.
 */
struct sip_in6_addr {
	union {
		uint8_t u6_addr8[16];
		uint16_t u6_addr16[8];
		uint32_t u6_addr32[4];
	} sip_in6_u;
#define sip6_addr	sip_in6_u.u6_addr8
#define sip6_addr8	sip_in6_u.u6_addr8
#define sip6_addr16	sip_in6_u.u6_addr16
#define sip6_addr32	sip_in6_u.u6_addr32
};

struct v4addr {
	char class;
	char class_remark[64];
	char pres_bitmap[36];
	int n_nmaskbits;
	uint32_t n_cbroadcast;
	uint32_t n_broadcast;
	uint32_t n_cnaddr;
	uint32_t n_naddr;
	uint32_t n_cnmask;
	uint32_t n_nmask;
	uint32_t n_haddr;
	uint32_t i_broadcast;
};

/*
 * Broadcast in this structure is sort of misleading, since ipv6 networks
 * don't have broadcast addresses, but it's as good a name as any for the
 * top address of a subnet.
 *
 * Prefix can also be directly translated to a ipv4 network address.
 */
struct v6addr {
	char class_remark[64];
	char comment[64];
	struct sip_in6_addr haddr;
	int nmaskbits;
	struct sip_in6_addr nmask;
	struct sip_in6_addr prefix;
	struct sip_in6_addr suffix;
	struct sip_in6_addr broadcast;
	int type;
	int real_v4;
};

struct if_info {
	char name[IFNAMSIZ + 1];
	char p_v4addr[19], p_v4nmask[16];
	char p_v6addr[44];
	char errorstr[64];
	char cmdstr[128];
	short flags;
	short type;
	struct v4addr v4ad;
	struct v6addr v6ad;
	struct if_info *next;
};

struct misc_args {
	int numnets;
	uint32_t splitmask;
	struct sip_in6_addr v6splitmask;
	int v6splitnum;
	uint32_t range_min;
	uint32_t range_max;
};

struct ipv6_split {
	char ipv6addr[40];
	char ipv4addr[16];
	char nmask[4];
};

struct argbox {
	char str[2 * ARGLEN + 2];
	int type;
	int resolv;
	struct argbox *next;
};

extern int   colorize;		/* cprintf.c */
extern char *ident;

/*
 * netcalc.c
 */
int out_int(struct if_info *if_cur, struct if_info *ifarg_cur, int v4args,
	    struct misc_args m_argv4, int v6args, struct misc_args m_argv6);
int out_cmdline(struct if_info *ifarg_cur, int v4args, struct misc_args m_argv4, int v6args, struct misc_args m_argv6);
int cleanline(char *sbuf, char *dbuf);
int get_stdin(char *args[]);

/*
 * misc.c
 */
int count(char *buf, char ch);

int validate_v4addr (char *addr);
int validate_netmask(char *in_addr);

int getrange_min_max(char *buf, uint32_t *range_min, uint32_t *range_max);
int getsplitnumv4(char *buf, uint32_t * splitmask);
int getsplitnumv6(char *buf, struct sip_in6_addr *splitmask, int *v6splitnum);

int quadtonum(char *quad, uint32_t * num);
char *numtoquad(uint32_t num);
char *numtobitmap(uint32_t num, uint32_t prefix_len);
uint32_t numtolen(uint32_t num);

int parse_addr(struct if_info *ifi);

int get_addrv4(struct if_info *ifi);
int get_addrv6(struct if_info *ifi);

int split_ipv6addr(char *addr, struct ipv6_split *spstr);
int validate_s_v6addr(char *addr, int type);

int getcolon(char *addr, int pos, int type);

int v6addrtonum(struct ipv6_split spstr, struct v6addr *in6_addr, int type);
int v6masktonum(char *nmask, int *nmaskbits, struct sip_in6_addr *in6_addr);

int validate_v6addr(char *addr);

int v6addrtoprefix(struct v6addr *in6_addr);
int v6addrtosuffix(struct v6addr *in6_addr);
int v6addrtobroadcast(struct v6addr *in6_addr);

void v6_type(struct v6addr *in6_addr);
void v6_comment(struct v6addr *in6_addr);
int v6verifyv4(struct sip_in6_addr addr);

char *get_comp_v6(struct sip_in6_addr addr);
int mk_ipv6addr(struct v6addr *in6_addr, char *addr);

/*
 * strlcpy.c strlcat.c
 */
#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t siz);
#endif
#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t siz);
#endif

/*
 * cprintf.c
 */
extern int cprintf(const char *fmt, ...);

#endif /* NETCALC_H_ */

/**
 * Local Variables:
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
