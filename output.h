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

#ifndef NETCALC_OUTPUT_H_
#define NETCALC_OUTPUT_H_

void show_split_networks_v4(struct if_info *ifi, uint32_t splitmask);
void show_network_ranges_v4(struct if_info *ifi, uint32_t range_min, uint32_t range_max);
void print_cf_info_v4(struct if_info *ifi);
char *print_comp_v6(struct sip_in6_addr addr, char *buf, size_t len);
void print_exp_v4inv6(struct sip_in6_addr addr);
void print_comp_v4inv6(struct sip_in6_addr addr);
void print_exp_v6(struct sip_in6_addr addr);
char *print_mixed_v6(struct sip_in6_addr addr, char *buf, size_t len);
void print_rev_v6(struct if_info *ifi);
void print_v6(struct if_info *ifi);
void print_v4inv6(struct if_info *ifi);
int v6plus(struct sip_in6_addr *a, struct sip_in6_addr *b);
void show_split_networks_v6(struct if_info *ifi, struct sip_in6_addr splitmask);

#endif /* NETCALC_OUTPUT_H_ */

/**
 * Local Variables:
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
