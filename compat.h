/* SPDX-License-Identifier: GPL-2.0-only
 *
 * This code is derived from the Linux Kernel sources intended
 * to maintain compatibility with different Kernel versions.
 * Copyright of original source is of respective Linux Kernel authors.
 */

#ifndef COMPAT_NETFLOW_H
#define COMPAT_NETFLOW_H
#include "compat_def.h"

#ifndef NIPQUAD
# define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
#endif
#ifndef HIPQUAD
# if defined(__LITTLE_ENDIAN)
#  define HIPQUAD(addr) \
	((unsigned char *)&addr)[3], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[0]
# elif defined(__BIG_ENDIAN)
#  define HIPQUAD NIPQUAD
# else
#  error "Please fix asm/byteorder.h"
# endif /* __LITTLE_ENDIAN */
#endif

#ifndef IPT_CONTINUE
# define IPT_CONTINUE XT_CONTINUE
# define ipt_target xt_target
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)
union nf_inet_addr {
	__be32		ip;
	__be32		ip6[4];
	struct in_addr	in;
	struct in6_addr	in6;
};
#endif

#ifndef list_first_entry
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
# define INIT_NET(x) x
#else
# define INIT_NET(x) init_net.x
#endif

#ifndef ETH_P_8021AD
# define ETH_P_8021AD	0x88A8	/* 802.1ad Service VLAN */
#endif

#ifndef ETH_P_QINQ1
# define ETH_P_QINQ1	0x9100	/* deprecated QinQ VLAN */
# define ETH_P_QINQ2	0x9200	/* deprecated QinQ VLAN */
# define ETH_P_QINQ3	0x9300	/* deprecated QinQ VLAN */
#endif

#ifndef IPPROTO_MH
# define IPPROTO_MH	135
#endif

#ifdef CONFIG_SYSCTL
# if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
#  define BEFORE2632(x,y) x,y
# else /* since 2.6.32 */
#  define BEFORE2632(x,y)
# endif

# if LINUX_VERSION_CODE >= KERNEL_VERSION(6,12,0)
#  pragma message "Kernel version >= 6.12.0 detected, using new ctl_table definitions."
#  define PROC_CTL_TABLE const struct ctl_table
# ifndef HAVE_GRSECURITY_H
#  define ctl_table_no_const struct ctl_table
# endif /* since 6.12.0 */
# else
#  pragma message "Kernel version < 6.12.0 detected, using old ctl_table definitions."
# define PROC_CTL_TABLE struct ctl_table
# ifndef HAVE_GRSECURITY_H
#  define ctl_table_no_const struct ctl_table
# endif
# endif
#endif /* CONFIG_SYSCTL */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,9,0)
# define compat_hlist_for_each_entry			hlist_for_each_entry
# define compat_hlist_for_each_entry_safe		hlist_for_each_entry_safe
#else /* since 3.9.0 */
# define compat_hlist_for_each_entry(a,pos,c,d)		hlist_for_each_entry(a,c,d)
# define compat_hlist_for_each_entry_safe(a,pos,c,d,e)	hlist_for_each_entry_safe(a,c,d,e)
#endif

#ifndef WARN_ONCE
#define WARN_ONCE(x,fmt...) ({ if (x) printk(KERN_WARNING fmt); })
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
# define IPPROTO_UDPLITE 136
#endif

#ifndef time_is_before_jiffies
# define time_is_before_jiffies(a) time_after(jiffies, a)
#endif
#ifndef time_is_after_jiffies
# define time_is_after_jiffies(a) time_before(jiffies, a)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,14,0)
# if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
#  define prandom_u32 get_random_int
# elif LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0)
#  define prandom_u32 random32
#endif
#define prandom_u32_max compat_prandom_u32_max
static inline u32 prandom_u32_max(u32 ep_ro)
{
	return (u32)(((u64) prandom_u32() * ep_ro) >> 32);
}
#endif

#ifndef min_not_zero
# define min_not_zero(x, y) ({			\
	typeof(x) __x = (x);			\
	typeof(y) __y = (y);			\
	__x == 0 ? __y : ((__y == 0) ? __x : min(__x, __y)); })
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)
static int __ethtool_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	ASSERT_RTNL();

	if (!dev->ethtool_ops->get_settings)
		return -EOPNOTSUPP;

	memset(cmd, 0, sizeof(struct ethtool_cmd));
	cmd->cmd = ETHTOOL_GSET;
	return dev->ethtool_ops->get_settings(dev, cmd);
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
# define ethtool_cmd_speed(x) (x)->speed
#endif

#ifndef ARPHRD_PHONET
# define ARPHRD_PHONET		820
# define ARPHRD_PHONET_PIPE	821
#endif
#ifndef ARPHRD_IEEE802154
# define ARPHRD_IEEE802154	804
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
# define for_each_netdev_ns(net, dev) for (dev = dev_base; dev; dev = dev->next)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
# define for_each_netdev_ns(net, d) for_each_netdev(d)
#else
# define for_each_netdev_ns(net, d) for_each_netdev(net, d)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35)
# define CHECK_FAIL	0
# define CHECK_OK	1
#else
# define CHECK_FAIL	-EINVAL
# define CHECK_OK	0
#endif

#ifndef NF_IP_LOCAL_IN /* 2.6.25 */
# define NF_IP_PRE_ROUTING	NF_INET_PRE_ROUTING
# define NF_IP_LOCAL_IN		NF_INET_LOCAL_IN
# define NF_IP_FORWARD		NF_INET_FORWARD
# define NF_IP_LOCAL_OUT	NF_INET_LOCAL_OUT
# define NF_IP_POST_ROUTING	NF_INET_POST_ROUTING
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
/* net/netfilter/x_tables.c */
static void xt_unregister_targets(struct xt_target *target, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		xt_unregister_target(&target[i]);
}
static int xt_register_targets(struct xt_target *target, unsigned int n)
{
	unsigned int i;

	int err = 0;
	for (i = 0; i < n; i++)
		if ((err = xt_register_target(&target[i])))
			goto err;
	return err;
err:
	if (i > 0)
		xt_unregister_targets(target, i);
	return err;
}
#endif

#ifdef HAVE_TOTALRAM_PAGES
#define num_physpages	totalram_pages()
#elif defined HAVE_TOTALRAM_PAGES_REF
#define num_physpages	totalram_pages
#endif

#ifndef HAVE_TIMEVAL
/* timeval is only used internally, so we can use anything for it. */
struct timeval {
	long tv_sec;
	long tv_usec; /* microseconds */
};

unsigned long timeval_to_jiffies(const struct timeval *tv);
unsigned long timeval_to_jiffies(const struct timeval *tv)
{
	return timespec64_to_jiffies(&(struct timespec64){
				     tv->tv_sec,
				     tv->tv_usec * NSEC_PER_USEC
				     });
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35)
# ifdef ktime_to_timeval
/* ktime_to_timeval is defined on 64bit and inline on 32bit cpu */
/* when it's defined it calls ns_to_timeval, which is not exported */
struct timeval portable_ns_to_timeval(const s64 nsec)
{
	struct timespec ts = ns_to_timespec(nsec);
	struct timeval tv;

	tv.tv_sec = ts.tv_sec;
	tv.tv_usec = (suseconds_t) ts.tv_nsec / 1000;

	return tv;
}
# define ns_to_timeval portable_ns_to_timeval
# endif

static inline s64 portable_ktime_to_ms(const ktime_t kt)
{
	struct timeval tv = ktime_to_timeval(kt);
	return (s64) tv.tv_sec * MSEC_PER_SEC + tv.tv_usec / USEC_PER_MSEC;
}
# define ktime_to_ms portable_ktime_to_ms
#endif /* before 2.6.35 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
static inline s64 portable_ktime_to_us(const ktime_t kt)
{
	struct timeval tv = ktime_to_timeval(kt);
	return (s64) tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
}
#define ktime_to_us portable_ktime_to_us
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)
static inline void put_unaligned_be16(u16 val, void *p)
{
	put_unaligned(cpu_to_be16(val), (__be16 *)p);
}
static inline void put_unaligned_be32(u32 val, void *p)
{
	put_unaligned(cpu_to_be32(val), (__be32 *)p);
}
static inline void put_unaligned_be64(u64 val, void *p)
{
	put_unaligned(cpu_to_be64(val), (__be64 *)p);
}
#endif

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24) && !defined(RHEL_MAJOR)
static void *__seq_open_private(struct file *f, struct seq_operations *ops,
    int psize)
{
	int rc;
	void *private;
	struct seq_file *seq;

	private = kzalloc(psize, GFP_KERNEL);
	if (private == NULL)
		goto out;

	rc = seq_open(f, ops);
	if (rc < 0)
		goto out_free;

	seq = f->private_data;
	seq->private = private;
	return private;

out_free:
	kfree(private);
out:
	return NULL;
}
#endif

/* disappeared in v3.19 */
#ifndef __get_cpu_var
#define __get_cpu_var(var)	(*this_cpu_ptr(&(var)))
#endif

#ifndef MPLS_HLEN
#define MPLS_HLEN 4
static inline int eth_p_mpls(__be16 eth_type)
{
	return eth_type == htons(ETH_P_MPLS_UC) ||
		eth_type == htons(ETH_P_MPLS_MC);
}
#endif
#ifndef MPLS_LS_S_MASK
struct mpls_label {
	__be32 entry;
};
#define MPLS_LS_S_MASK		0x00000100

#endif

/* sockaddr comparison functions is from fs/nfs/client.c */
static int sockaddr_match_ipaddr6(const struct sockaddr *sa1, const struct sockaddr *sa2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sa1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sa2;

	if (!ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr))
		return 0;
#if 0
	else if (ipv6_addr_type(&sin1->sin6_addr) & IPV6_ADDR_LINKLOCAL)
		return sin1->sin6_scope_id == sin2->sin6_scope_id;
#endif
	return 1;
}

static int sockaddr_match_ipaddr4(const struct sockaddr *sa1, const struct sockaddr *sa2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sa1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sa2;

	return sin1->sin_addr.s_addr == sin2->sin_addr.s_addr;
}

static int sockaddr_cmp_ip6(const struct sockaddr *sa1, const struct sockaddr *sa2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sa1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sa2;

	return sockaddr_match_ipaddr6(sa1, sa2) &&
		(sin1->sin6_port == sin2->sin6_port);
}

static int sockaddr_cmp_ip4(const struct sockaddr *sa1, const struct sockaddr *sa2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sa1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sa2;

	return sockaddr_match_ipaddr4(sa1, sa2) &&
		(sin1->sin_port == sin2->sin_port);
}

static int sockaddr_cmp(const struct sockaddr_storage *sa1, const struct sockaddr_storage *sa2)
{
	const struct sockaddr *s1 = (const struct sockaddr *)sa1;
	const struct sockaddr *s2 = (const struct sockaddr *)sa2;

	if (sa1->ss_family != sa2->ss_family)
		return 0;

	switch (sa1->ss_family) {
	case AF_INET:
		return sockaddr_cmp_ip4(s1, s2);
	case AF_INET6:
		return sockaddr_cmp_ip6(s1, s2);
	}
	return 0;
}

#ifndef IN6PTON_XDIGIT

#define hex_to_bin compat_hex_to_bin
/* lib/hexdump.c */
int hex_to_bin(char ch);
int hex_to_bin(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - '0';
	ch = tolower(ch);
	if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	return -1;
}

/* net/core/utils.c */
#define IN6PTON_XDIGIT		0x00010000
#define IN6PTON_DIGIT		0x00020000
#define IN6PTON_COLON_MASK	0x00700000
#define IN6PTON_COLON_1		0x00100000	/* single : requested */
#define IN6PTON_COLON_2		0x00200000	/* second : requested */
#define IN6PTON_COLON_1_2	0x00400000	/* :: requested */
#define IN6PTON_DOT		0x00800000	/* . */
#define IN6PTON_DELIM		0x10000000
#define IN6PTON_NULL		0x20000000	/* first/tail */
#define IN6PTON_UNKNOWN		0x40000000

static inline int xdigit2bin(char c, int delim)
{
	int val;

	if (c == delim || c == '\0')
		return IN6PTON_DELIM;
	if (c == ':')
		return IN6PTON_COLON_MASK;
	if (c == '.')
		return IN6PTON_DOT;

	val = hex_to_bin(c);
	if (val >= 0)
		return val | IN6PTON_XDIGIT | (val < 10 ? IN6PTON_DIGIT : 0);

	if (delim == -1)
		return IN6PTON_DELIM;
	return IN6PTON_UNKNOWN;
}

int in4_pton(const char *src, int srclen,
	     u8 *dst,
	     int delim, const char **end)
{
	const char *s;
	u8 *d;
	u8 dbuf[4];
	int ret = 0;
	int i;
	int w = 0;

	if (srclen < 0)
		srclen = strlen(src);
	s = src;
	d = dbuf;
	i = 0;
	while(1) {
		int c;
		c = xdigit2bin(srclen > 0 ? *s : '\0', delim);
		if (!(c & (IN6PTON_DIGIT | IN6PTON_DOT | IN6PTON_DELIM | IN6PTON_COLON_MASK))) {
			goto out;
		}
		if (c & (IN6PTON_DOT | IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
			if (w == 0)
				goto out;
			*d++ = w & 0xff;
			w = 0;
			i++;
			if (c & (IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
				if (i != 4)
					goto out;
				break;
			}
			goto cont;
		}
		w = (w * 10) + c;
		if ((w & 0xffff) > 255) {
			goto out;
		}
cont:
		if (i >= 4)
			goto out;
		s++;
		srclen--;
	}
	ret = 1;
	memcpy(dst, dbuf, sizeof(dbuf));
out:
	if (end)
		*end = s;
	return ret;
}

int in6_pton(const char *src, int srclen,
	     u8 *dst,
	     int delim, const char **end)
{
	const char *s, *tok = NULL;
	u8 *d, *dc = NULL;
	u8 dbuf[16];
	int ret = 0;
	int i;
	int state = IN6PTON_COLON_1_2 | IN6PTON_XDIGIT | IN6PTON_NULL;
	int w = 0;

	memset(dbuf, 0, sizeof(dbuf));

	s = src;
	d = dbuf;
	if (srclen < 0)
		srclen = strlen(src);

	while (1) {
		int c;

		c = xdigit2bin(srclen > 0 ? *s : '\0', delim);
		if (!(c & state))
			goto out;
		if (c & (IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
			/* process one 16-bit word */
			if (!(state & IN6PTON_NULL)) {
				*d++ = (w >> 8) & 0xff;
				*d++ = w & 0xff;
			}
			w = 0;
			if (c & IN6PTON_DELIM) {
				/* We've processed last word */
				break;
			}
			/*
			 * COLON_1 => XDIGIT
			 * COLON_2 => XDIGIT|DELIM
			 * COLON_1_2 => COLON_2
			 */
			switch (state & IN6PTON_COLON_MASK) {
			case IN6PTON_COLON_2:
				dc = d;
				state = IN6PTON_XDIGIT | IN6PTON_DELIM;
				if (dc - dbuf >= sizeof(dbuf))
					state |= IN6PTON_NULL;
				break;
			case IN6PTON_COLON_1|IN6PTON_COLON_1_2:
				state = IN6PTON_XDIGIT | IN6PTON_COLON_2;
				break;
			case IN6PTON_COLON_1:
				state = IN6PTON_XDIGIT;
				break;
			case IN6PTON_COLON_1_2:
				state = IN6PTON_COLON_2;
				break;
			default:
				state = 0;
			}
			tok = s + 1;
			goto cont;
		}

		if (c & IN6PTON_DOT) {
			ret = in4_pton(tok ? tok : s, srclen + (int)(s - tok), d, delim, &s);
			if (ret > 0) {
				d += 4;
				break;
			}
			goto out;
		}

		w = (w << 4) | (0xff & c);
		state = IN6PTON_COLON_1 | IN6PTON_DELIM;
		if (!(w & 0xf000)) {
			state |= IN6PTON_XDIGIT;
		}
		if (!dc && d + 2 < dbuf + sizeof(dbuf)) {
			state |= IN6PTON_COLON_1_2;
			state &= ~IN6PTON_DELIM;
		}
		if (d + 2 >= dbuf + sizeof(dbuf)) {
			state &= ~(IN6PTON_COLON_1|IN6PTON_COLON_1_2);
		}
cont:
		if ((dc && d + 4 < dbuf + sizeof(dbuf)) ||
		    d + 4 == dbuf + sizeof(dbuf)) {
			state |= IN6PTON_DOT;
		}
		if (d >= dbuf + sizeof(dbuf)) {
			state &= ~(IN6PTON_XDIGIT|IN6PTON_COLON_MASK);
		}
		s++;
		srclen--;
	}

	i = 15; d--;

	if (dc) {
		while(d >= dc)
			dst[i--] = *d--;
		while(i >= dc - dbuf)
			dst[i--] = 0;
		while(i >= 0)
			dst[i--] = *d--;
	} else
		memcpy(dst, dbuf, sizeof(dbuf));

	ret = 1;
out:
	if (end)
		*end = s;
	return ret;
}
#endif /* IN6PTON_XDIGIT */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,2,0)
# define sock_create_kern(f, t, p, s) sock_create_kern(&init_net, f, t, p, s)
#endif

#if !defined(vlan_tx_tag_get) && defined(skb_vlan_tag_get)
# define vlan_tx_tag_get skb_vlan_tag_get
# define vlan_tx_tag_present skb_vlan_tag_present
#endif

#ifndef SPEED_UNKNOWN
# define SPEED_UNKNOWN		-1
#endif

#if !defined __GNUC_PREREQ && defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif

/* ktime is not union anymore, since 2456e855354415bfaeb7badaa14e11b3e02c8466 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,10,0)
# define first_tv64	first.tv64
# define last_tv64	last.tv64
#else
# define first_tv64	first
# define last_tv64	last
#endif

#ifndef SK_CAN_REUSE
# define SK_CAN_REUSE   1
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,13,0)
# define compat_refcount_read atomic_read
#else
# define compat_refcount_read refcount_read
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
# define timer_setup setup_timer
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,15,0)
static int dev_get_alias(const struct net_device *dev, char *name, size_t len)
{
	return snprintf(name, len, "%s", dev->ifalias);
}
#else
/* no static because defined in include/linux/netdevice.h,
 * but forgot to create EXPORT_SYMBOL,
 * probably will collide with some future kernel */
int dev_get_alias(const struct net_device *dev, char *name, size_t len)
{
	const struct dev_ifalias *alias;
	int ret = 0;

	rcu_read_lock();
	alias = rcu_dereference(dev->ifalias);
	if (alias)
		ret = snprintf(name, len, "%s", alias->ifalias);
	rcu_read_unlock();

	return ret;
}
#endif

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,39) && !defined(RHEL_MAJOR)
static inline int is_vlan_dev(struct net_device *dev)
{
	return dev->priv_flags & IFF_802_1Q_VLAN;
}
#endif

#ifdef CONFIG_BRIDGE_NETFILTER
# ifndef HAVE_NF_BRIDGE_INFO_GET
static inline struct nf_bridge_info *
nf_bridge_info_get(const struct sk_buff *skb)
{
	return skb->nf_bridge;
}
# endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
static inline void do_gettimeofday(struct timeval *tv)
{
	struct timespec64 ts64;
	ktime_get_real_ts64(&ts64);
	tv->tv_sec = ts64.tv_sec;
	tv->tv_usec = ts64.tv_nsec/1000;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,12,0)
/*
 * find_module() is unexported in v5.12:
 *   089049f6c9956 ("module: unexport find_module and module_mutex")
 * and module_mutex is replaced with RCU in
 *   a006050575745 ("module: use RCU to synchronize find_module")
 */
#include <linux/rcupdate.h>
struct module *find_module(const char *name)
{
	struct module *mod;

	rcu_read_lock_sched();
	/* Yes this is crazy, but should work. */
	list_for_each_entry_rcu(mod, &THIS_MODULE->list, list) {
		if (!strcmp(mod->name, name)) {
			rcu_read_unlock_sched();
			return mod;
		}
	}
	rcu_read_unlock_sched();
	return NULL;
}
#endif

/* Copy from 294f69e662d1 ("compiler_attributes.h: Add 'fallthrough' pseudo
 * keyword for switch/case use") */
#ifndef fallthrough
# if defined __has_attribute && __has_attribute(__fallthrough__)
#  define fallthrough			__attribute__((__fallthrough__))
# else
#  define fallthrough			do {} while (0)  /* fallthrough */
# endif
#endif

#ifndef HAVE_NF_CT_EVENT_NOTIFIER_CT_EVENT
/*
 * nat event callback parameter is constified in 5.15+
 * but it prevents module building with previous kernel versions
 */
# define NF_CT_EVENT struct nf_ct_event
# define ct_event fcn
#else
# define NF_CT_EVENT const struct nf_ct_event
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,0)
static inline bool netif_is_bridge_port(const struct net_device *dev)
{
	return dev->priv_flags & IFF_BRIDGE_PORT;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,15,0)
# define del_timer_sync timer_delete_sync
#endif

#endif /* COMPAT_NETFLOW_H */
