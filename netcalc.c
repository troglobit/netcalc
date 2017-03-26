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
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#include <stdio.h>
#include <string.h>

#include "netcalc.h"
#include "output.h"

#define ARGLEN 128

char *ident = PACKAGE_NAME;
extern char *optarg;
extern int optind, opterr, optopt;

int
out_cmdline(struct if_info *ifarg_cur, int v4args, struct misc_args m_argv4,
	    int v6args, struct misc_args m_argv6, int recurse, int index)
{
	int ret = 0;

	if (ifarg_cur->type == IFT_V4)
		ret = get_addrv4(ifarg_cur);

	if (ifarg_cur->type == IFT_V6)
		ret = get_addrv6(ifarg_cur);

	if (ifarg_cur->type == IFT_INTV4 || ifarg_cur->type == IFT_INTV6) {
		if (ifarg_cur->errorstr[0] != 0) {
			warnx("Invalid address: %s", ifarg_cur->errorstr);
			return 0;
		}

		ret = get_addrv4(ifarg_cur);
	}

	if (ifarg_cur->type == IFT_UNKWN) {
		warnx("Unknown %s: %s", ifarg_cur->cmdstr, ifarg_cur->errorstr);
		return 0;
	}

	if (ret == -1) {
		warnx("Invalid address");
		return 0;
	}
	if (ret == -2) {
		warnx("Invalid netmask");
		return 0;
	}

	if (ifarg_cur->type == IFT_V4 || ifarg_cur->type == IFT_INTV4) {
		if (!v4args)
			v4args = V4_INFO;

		if ((v4args & V4_INFO) == V4_INFO)
			print_cf_info_v4(ifarg_cur);
		if ((v4args & V4SPLIT) == V4SPLIT)
			show_split_networks_v4(ifarg_cur, m_argv4.splitmask, v4args, m_argv4);

		printf("\n");
	}

	if (ifarg_cur->type == IFT_V6 || ifarg_cur->type == IFT_INTV6) {
		if (!v6args)
			v6args = V6_INFO;

		if ((v6args & V6_INFO) == V6_INFO)
			print_v6(ifarg_cur);
		if ((v6args & V4INV6) == V4INV6)
			print_v4inv6(ifarg_cur);
		if ((v6args & V6REV) == V6REV)
			print_rev_v6(ifarg_cur);
		if ((v6args & V6SPLIT) == V6SPLIT)
			show_split_networks_v6(ifarg_cur, m_argv6.v6splitmask, v6args, m_argv6);

		printf("\n");
	}

	return 0;
}

int cleanline(char *sbuf, char *dbuf)
{
	int x, y;

	for (x = 0; x < strlen(sbuf); x++) {
		if (sbuf[x] == '\n')
			sbuf[x] = ' ';
		if (sbuf[x] == '\t')
			sbuf[x] = ' ';
		if (sbuf[x] == '#')
			sbuf[x] = '\0';
	}
	x = strlen(sbuf) - 1;
	while (sbuf[x] == ' ' && x > -1) {
		sbuf[x] = '\0';
		x--;
	}
	if (!strlen(sbuf))
		return 0;

	x = 0;
	y = 0;
	while (x < strlen(sbuf)) {
		if (sbuf[x] == ' ' && x) {
			dbuf[y] = ' ';
			y++;
		}
		while (sbuf[x] == ' ' && x < strlen(sbuf))
			x++;
		while (sbuf[x] != ' ' && x < strlen(sbuf)) {
			dbuf[y] = sbuf[x];
			y++;
			x++;
		}
		if (dbuf[y - 1] == ' ')
			return 0;
	}
	if (dbuf[strlen(dbuf) - 1] == ' ')
		dbuf[strlen(dbuf) - 1] = '\0';

	y = 1;
	for (x = 0; x < strlen(dbuf); x++)
		if (dbuf[x] == ' ')
			y++;

	return y;
}

int get_stdin(char *args[])
{
	char buf[2], sbuf[ARGLEN], dbuf[ARGLEN], *arg1, *arg2;
	int x, y, z, argmax;

	memset(buf, 0, sizeof(buf));

	argmax = ARGLEN - 1;
	arg1 = (char *)calloc(1, ARGLEN);
	arg2 = (char *)calloc(1, ARGLEN);
	memset(sbuf, 0, sizeof(sbuf));
	memset(dbuf, 0, sizeof(dbuf));

	while (!sbuf[0]) {
		x = 0;
		y = 0;
		memset(sbuf, 0, sizeof(sbuf));
		do {
			x = read(0, buf, 1);
			if (x == 1)
				sbuf[y] = buf[0];
			y++;
		} while (x > 0 && buf[0] != '\n' && y < (sizeof(sbuf) - 1));
		if (x < 0) {
			free(arg1);
			free(arg2);
			return -1;
		}
		if (!x)
			break;

		while (buf[0] != '\n' && x == 1)
			x = read(0, buf, 1);
		if (x < 0) {
			free(arg1);
			free(arg2);
			return -1;
		}
		if (!x)
			break;
	};
	if (!sbuf[0]) {
		free(arg1);
		free(arg2);
		return -2;
	}

	x = cleanline(sbuf, dbuf);
	if (x < 1) {
		free(arg1);
		free(arg2);
		return x;
	}

	y = 0;
	while (y < strlen(dbuf) && y < argmax && dbuf[y] != ' ') {
		arg1[y] = dbuf[y];
		y++;
	}
	y++;
	z = 0;
	while (y < strlen(dbuf) && z < argmax && dbuf[y] != ' ') {
		arg2[z] = dbuf[y];
		y++;
		z++;
	}

	strlcpy(args[0], arg1, ARGLEN);
	strlcpy(args[1], arg2, ARGLEN);

	free(arg1);
	free(arg2);

	return x;
}

struct argbox *new_arg(struct argbox *abox)
{
	abox->next = (struct argbox *)calloc(1, sizeof(struct argbox));
	abox = abox->next;

	return abox;
}

void free_boxargs(struct argbox *abox)
{
	struct argbox *old;

	while (abox) {
		old = abox;
		abox = abox->next;
		free(old);
	}
}

/*
 * This function will try to populate an argumentbox.
 * This is slightly difficult due to the numerous different possible
 * input types, ie. v4addr, dotted quad netmask, /netmask, hex netmask,
 * v6addr, interface name etc.
 * This forces to have to try to guess what a user means in some cases.
 * This method can be fairly unforgiving with typos.
 */
struct argbox *get_boxargs(int argc, char *argv[], int argcount, struct argbox *abox_cur)
{
	char expaddr[ARGLEN];
	int x, y;

	/*
	 * We use goto's here *gasp*.
	 */
	while (argv[argcount]) {
		/*
		 * Baaad argument. Error out if this happens.
		 */
		if (strlen(argv[argcount]) > sizeof(expaddr) - 1)
			errx(1, "Invalid argument %s", expaddr);

		strlcpy(expaddr, argv[argcount], sizeof(expaddr));

		/*
		 * Is this a v6 address?
		 */
		x = validate_v6addr(expaddr);
		if (x) {
			strlcpy(abox_cur->str, expaddr, sizeof(abox_cur->str));
			abox_cur->type = AT_V6;
			abox_cur->resolv = 0;
			abox_cur = new_arg(abox_cur);
			goto complete;
		}

		/*
		 * Nope, is it an ipv4 address with a /xx mask?
		 *
		 * NOTE: validate_netmask returns different values if it finds
		 * other types of netmasks to, but we only match on the above
		 * here.
		 */
		x = validate_netmask(expaddr);
		if (x == 2) {
			strlcpy(abox_cur->str, expaddr, sizeof(abox_cur->str));
			abox_cur->type = AT_V4;
			abox_cur->resolv = 0;
			abox_cur = new_arg(abox_cur);
			goto complete;
		}

		/*
		 * No, so is it a plain ipv4 address?
		 */
		x = validate_v4addr(expaddr);
		if (x) {
			y = 0;
			/*
			 * It is, does that mean the next argument is a
			 * netmask?
			 */
			if (argcount + 1 < argc)
				y = validate_netmask(argv[argcount + 1]);
			/*
			 * 1 == 'normal' netmask
			 * 3 == hex netmask
			 */
			if (y == 1 || y == 3) {
				snprintf(abox_cur->str, 34, "%s %s", expaddr, argv[argcount + 1]);
				argcount++;
			} else
				snprintf(abox_cur->str, 18, "%s", expaddr);
			abox_cur->type = AT_V4;
			abox_cur->resolv = 0;
			abox_cur = new_arg(abox_cur);
			goto complete;
		}

		y = 0;
		if (argcount + 1 < argc)
			y = validate_netmask(argv[argcount + 1]);
		if (y == 1 || y == 3) {
			snprintf(abox_cur->str, sizeof(abox_cur->str), "%s %s", expaddr, argv[argcount + 1]);
			argcount++;
		} else
			strlcpy(abox_cur->str, expaddr, sizeof(abox_cur->str));
		abox_cur->type = AT_UNKWN;
		abox_cur->resolv = 1;
		abox_cur = new_arg(abox_cur);

 complete:
		argcount++;
	}

	return abox_cur;
}

struct if_info *new_if(struct if_info *ifarg_cur)
{
	struct if_info *n_if;

	n_if = (struct if_info *)calloc(1, sizeof(struct if_info));
	ifarg_cur->next = n_if;

	return n_if;
}

void free_if(struct if_info *if_cur)
{
	struct if_info *if_old;

	while (if_cur) {
		if_old = if_cur;
		if_cur = if_cur->next;
		free(if_old);
	}
}

struct if_info *parse_abox(struct argbox *abox, struct if_info *if_start)
{
	struct if_info *ifarg_start, *ifarg_cur, *ifarg_old;
	struct if_info *if_cur;
	struct dnsresp *d_resp_start, *d_resp_cur;
	char *tmpstr;
	int x, if_found;

	ifarg_old = ifarg_cur = ifarg_start = (struct if_info *)calloc(1, sizeof(struct if_info));

	while (abox) {
		if (abox->type == AT_V4 && !abox->resolv) {
			tmpstr = strstr(abox->str, " ");
			if (tmpstr != NULL && (strlen(tmpstr) > 0)) {
				tmpstr++;
				x = 0;
				while (x < 15 && tmpstr[x] != ' ' && x < strlen(tmpstr)) {
					ifarg_cur->p_v4nmask[x] = tmpstr[x];
					x++;
				}
			}

			x = 0;
			while (x < 18 && abox->str[x] != ' ') {
				ifarg_cur->p_v4addr[x] = abox->str[x];
				x++;
			}
			ifarg_cur->type = IFT_V4;
			strlcpy(ifarg_cur->cmdstr, abox->str, sizeof(ifarg_cur->cmdstr));
		}

		if (abox->type == AT_V6 && !abox->resolv) {
			strlcpy(ifarg_cur->p_v6addr, abox->str, sizeof(ifarg_cur->p_v6addr));
			strlcpy(ifarg_cur->cmdstr, abox->str, sizeof(ifarg_cur->cmdstr));

			mk_ipv6addr(&ifarg_cur->v6ad, ifarg_cur->p_v6addr);
			ifarg_cur->type = IFT_V6;
		}

		if (abox->type == AT_UNKWN && !abox->resolv) {
			strlcpy(ifarg_cur->name, abox->str, IFNAMSIZ);
			strlcpy(ifarg_cur->cmdstr, abox->str, sizeof(ifarg_cur->cmdstr));
			snprintf(ifarg_cur->errorstr, sizeof(ifarg_cur->errorstr), "Unable to retrieve interface information");
			ifarg_cur->type = IFT_INTV4;
		}

		abox = abox->next;
		ifarg_old = ifarg_cur;
		ifarg_cur = new_if(ifarg_cur);
	}

	ifarg_old->next = NULL;
	free(ifarg_cur);
	ifarg_cur = NULL;

	if (ifarg_start == ifarg_cur) {
		free(ifarg_start);
		return NULL;
	}

	return ifarg_start;
}

static int usage(int code)
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
	       "Copyright (C) 2010-2017  Joachim Nilsson\n"
	       "\n"
	       "This is free software, under the 3-clause BSD license: you are free to change\n"
	       "and redistribute it.  There is NO WARRANTY, to the extent permitted by law\n", ident);

	return code;
}

static char *progname(char *arg0)
{
       char *nm;

       nm = strrchr(arg0, '/');
       if (nm)
	       nm++;
       else
	       nm = arg0;

       return nm;
}

int main(int argc, char *argv[])
{
	int x, y, z, m, v4args, v6args, argcount, first_err;
	struct if_info *if_start, *if_cur;
	struct if_info *ifarg_start, *ifarg_cur, *ifarg_old;
	int ch, parse_stdin, index;
	struct misc_args m_argv4, m_argv6;
	int split_errv4, split_errv6;
	char expaddr[ARGLEN] = "";
	char oldcmdstr[ARGLEN] = "";
	struct argbox *abox_start, *abox_cur, *abox_tmp;
	char *stdinarg[3];

	ident = progname(argv[0]);
	if (argc < 2)
		return usage(1);

	parse_stdin = 0;
	/*
	 * Our default v4 and v6 options, hopefully what's mostly used.
	 */
	v4args = V4_INFO;
	v6args = V6_INFO;
	m_argv4.splitmask = 0;
	m_argv4.numnets = 0;
	m_argv6.splitmask = 0;
	m_argv6.numnets = 0;
	split_errv4 = 0;
	split_errv6 = 0;
	first_err = 1;
	ifarg_start = NULL;
	ifarg_old = NULL;

	/*
	 * abox == argument box == a box that holds (commandline) arguments :)
	 * This is the structure we use to store all user input parsed into
	 * (hopefully) managable chunks.
	 * This excludes most of the -[a-z] flags, they're generally handled by
	 * v[4,6]args.
	 */
	abox_start = abox_cur = (struct argbox *)calloc(1, sizeof(struct argbox));

	/*
	 * v[4,6]args holds flags based on commandline arguments for what we
	 * want to output.
	 */
	while ((ch = getopt(argc, argv, "ehrs:S:v")) != -1) {
		switch (ch) {
		case 'e':
			v6args |= V4INV6;
			break;

		case 'r':
			v6args |= V6REV;
			break;

		case 's':
			y = getsplitnumv4(optarg, &m_argv4.splitmask);
			if (!y) {
				v4args |= V4SPLIT;
			} else {
				warnx("Invalid IPv4 splitmask, unable to split.");
				split_errv4 = 1;
			}
			break;

		case 'S':
			y = getsplitnumv6(optarg, &m_argv6.v6splitmask, &m_argv6.v6splitnum);
			if (!y) {
				v6args |= V6SPLIT;
			} else {
				warnx("Invalid IPv6 splitmask, unable to split.");
				split_errv6 = 1;
			}
			break;

		case 'v':
			printf("%s\n", VERSION);
			return 0;

		case '?':
		case 'h':
			return usage(0);

		default:
			return usage(1);
		}
	}

	if (split_errv4 || split_errv6) {
 nothing:
		warnx("No (valid) commands received, nothing to do.");
		free_boxargs(abox_start);
		return 1;
	}

	argcount = optind;
	if (m_argv4.numnets < 1)
		m_argv4.numnets = -1;

	if (argv[argcount]) {
		if (argv[argcount][0] == '-' && argv[argcount][1] == '\0')
			parse_stdin = 1;
	} else {
		if (abox_start->str[0] == '\0') {
			free_boxargs(abox_start);
			return usage(0);
		}
	}

	/*
	 * Populate our argumentbox.
	 * (Ie., see what's on the commandline).
	 */
	if (!parse_stdin && argv[argcount]) {
		abox_cur = get_boxargs(argc, argv, argcount, abox_cur);

		abox_tmp = abox_start;
		while (abox_tmp->next != abox_cur) {
			abox_tmp = abox_tmp->next;
		}
		abox_tmp->next = NULL;
		free(abox_cur);
		abox_cur = NULL;
	}

	/*
	 * This will try to gather information about the network interfaces
	 * present on the local machine.
	 */
	if_start = NULL;
	if_cur = NULL;

	if (!parse_stdin)
		ifarg_cur = ifarg_start = parse_abox(abox_start, if_start);

	if (!ifarg_start && !parse_stdin)
		goto nothing;

	index = 0;
	ifarg_cur = ifarg_start;
	while (ifarg_cur && !parse_stdin) {
		if (strlen(ifarg_cur->cmdstr) > 0) {
			if (!strcmp(ifarg_cur->cmdstr, oldcmdstr))
				index++;
			else
				index = 0;
		} else {
			index = 0;
		}
		out_cmdline(ifarg_cur, v4args, m_argv4, v6args, m_argv6, 0, index);
		strlcpy(oldcmdstr, ifarg_cur->cmdstr, sizeof(oldcmdstr));
		ifarg_cur = ifarg_cur->next;
	}

	z = 0;
	y = 1;
	while (parse_stdin && y > -1) {
		stdinarg[0] = (char *)calloc(1, ARGLEN);
		stdinarg[1] = (char *)calloc(1, ARGLEN);
		stdinarg[2] = NULL;
		y = get_stdin(stdinarg);
		if (y > 0) {
			m = 2;
			if (stdinarg[1][0] == '\0') {
				free(stdinarg[1]);
				stdinarg[1] = NULL;
				m = 1;
			}
			abox_cur = get_boxargs(m, stdinarg, 0, abox_cur);
			abox_tmp = abox_start;
			while (abox_tmp->next != abox_cur && abox_tmp != abox_cur) {
				abox_tmp = abox_tmp->next;
			}
			abox_tmp->next = NULL;
			free(abox_cur);
			abox_cur = NULL;

			ifarg_cur = ifarg_start = parse_abox(abox_start, if_start);
			if (ifarg_start) {
				index = 0;
				ifarg_cur = ifarg_start;
				while (ifarg_cur) {
					if (strlen(ifarg_cur->cmdstr) > 0) {
						if (!strcmp(ifarg_cur->cmdstr, oldcmdstr))
							index++;
						else
							index = 0;
					} else {
						index = 0;
					}
					out_cmdline(ifarg_cur, v4args, m_argv4, v6args, m_argv6, 0, index);
					strlcpy(oldcmdstr, ifarg_cur->cmdstr, sizeof(oldcmdstr));
					ifarg_cur = ifarg_cur->next;
				}
			}

			z = 1;
			free_if(ifarg_start);
			free_boxargs(abox_start);
			abox_start = abox_cur = (struct argbox *)calloc(1, sizeof(struct argbox));
		}
		for (x = 0; x < 2; x++) {
			if (stdinarg[x]) {
				free(stdinarg[x]);
				stdinarg[x] = NULL;
			}
		}
		if (y == -1)
			warnx("Problem parsing stdin");
	}
	if (parse_stdin) {
		free(stdinarg[0]);
		free(stdinarg[1]);
	}

	if (!z && parse_stdin)
		warnx("No arguments found on stdin");

	if (!parse_stdin)
		free_if(ifarg_start);
	free_if(if_start);

	return 0;
}

/**
 * Local Variables:
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
