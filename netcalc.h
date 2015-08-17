/*
 * Copyright (c) 2003-2013  Simon Ekstrand
 * Copyright (c) 2010-2015  Joachim Nilsson
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
#include <net/if.h>		/* IFNAMSIZ */

#ifdef PACKAGE
#define NAME PACKAGE
#else
#define NAME "netcalc"
#endif

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

/*
 * Easier to define this ourselves then to use all the different
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
 * Broadcast in this structure is sort of missleading, since ipv6 networks
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
};

struct ipv6_split {
	char ipv6addr[40];
	char ipv4addr[16];
	char nmask[4];
};

struct argbox {
	char str[128];
	int type;
	int resolv;
	struct argbox *next;
};

#define AT_V4 1
#define AT_V6 2
#define AT_INT 3
#define AT_UNKWN 4

#define IFT_V4 1
#define IFT_V6 2
#define IFT_INTV4 3
#define IFT_INTV6 4
#define IFT_UNKWN 5

/* v4 args */
#define V4_INFO     0x01
#define NET_INFO    0x10
#define V4SPLIT     0x20
#define V4VERBSPLIT 0x40
#define C_WILDCARD  0x80

/* v6 args */
#define V6_INFO     0x01
#define V4INV6      0x02
#define V6SPLIT     0x04
#define V6REV       0x08
#define V6VERBSPLIT 0x10

#define V6TYPE_STANDARD 1
#define V6TYPE_V4INV6 2

/*
 * netcalc.c
 */
int out_int(struct if_info *if_cur, struct if_info *ifarg_cur, int v4args,
	    struct misc_args m_argv4, int v6args, struct misc_args m_argv6);
int out_cmdline(struct if_info *ifarg_cur, int v4args,
		struct misc_args m_argv4, int v6args, struct misc_args m_argv6, int recurse, int index);
int cleanline(char *sbuf, char *dbuf);
int get_stdin(char *args[]);

/*
 * misc.c
 */
int count(char *buf, char ch);

int validate_v4addr (char *addr);
int validate_netmask(char *in_addr);

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
size_t strlcpy(char *dst, const char *src, size_t siz);
size_t strlcat(char *dst, const char *src, size_t siz);

#endif /* NETCALC_H_ */

/**
 * Local Variables:
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
