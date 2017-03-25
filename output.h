/*
 * Copyright (c) 2003-2013  Simon Ekstrand
 * Copyright (c) 2010-2017  Joachim Nilsson
 */

#ifndef NETCALC_OUTPUT_H_
#define NETCALC_OUTPUT_H_

void show_split_networks_v4(struct if_info *ifi, uint32_t splitmask, int v4args, struct misc_args m_argv4);
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
void show_split_networks_v6(struct if_info *ifi, struct sip_in6_addr splitmask, int v6args, struct misc_args m_argv6);

#endif /* NETCALC_OUTPUT_H_ */

/**
 * Local Variables:
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
