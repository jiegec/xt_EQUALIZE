#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/checksum.h>

#include "xt_EQUALIZE.h"
#include <linux/netfilter/x_tables.h>

MODULE_AUTHOR("Jiajie Chen <jiegec@qq.com>");
MODULE_DESCRIPTION("Xtables: RFC 8367 compliant IP filter target");
MODULE_LICENSE("GPL");

static unsigned int equalize_tg(struct sk_buff *skb,
                                const struct xt_action_param *par) {
  struct iphdr *iph;
  __u32 sum;

  // RFC8367 3.1 Color-Based Termination
  iph = ip_hdr(skb);
  if (iph->check <= 0x23) {
    printk(pr_fmt("Dropped due to color-based termination"));
    return NF_DROP;
  }

  // RFC8367 3.2 Age-Based Termination
  iph = ip_hdr(skb);
  if (iph->ttl <= 23) {
    printk(pr_fmt("Dropped due to age-based termination"));
    return NF_DROP;
  }

  // RFC8367 3.3 Origin-Based Termination
  sum = ((iph->saddr & 0xFF000000) >> 24) + ((iph->saddr & 0x00FF0000) >> 16) +
        ((iph->saddr & 0x0000FF00) >> 8) + (iph->saddr & 0x000000FF);
  if (sum <= 23) {
    printk(pr_fmt("Dropped due to origin-based termination"));
    return NF_DROP;
  }

  // RFC8367 3.4 Length-Based Termination
  if (iph->tot_len <= 0x23) {
    printk(pr_fmt("Dropped due to length-based termination"));
    return NF_DROP;
  }

  // RFC8367 3.6 Flag-Based Termination
  if ((iph->frag_off & 0x2000) == 1) {
    printk(pr_fmt("Dropped due to flag-based termination"));
    return NF_DROP;
  }

  return XT_CONTINUE;
}

static unsigned int equalize_tg6(struct sk_buff *skb,
                                 const struct xt_action_param *par) {
  // RFC8367 3.5 IP-Version-Based Termination
  printk(pr_fmt("Dropped due to ip-version-based termination"));
  return NF_DROP;
}

static int equalize_tg_check(const struct xt_tgchk_param *par) { return 0; }

static struct xt_target equalize_tg_reg[] __read_mostly = {
    {
        .name = "EQUALIZE",
        .revision = 0,
        .family = NFPROTO_IPV4,
        .target = equalize_tg,
        .targetsize = sizeof(struct xt_EQUALIZE_info),
        .table = "filter",
        .checkentry = equalize_tg_check,
        .me = THIS_MODULE,
    },
    {
        .name = "EQUALIZE",
        .revision = 0,
        .family = NFPROTO_IPV6,
        .target = equalize_tg6,
        .targetsize = sizeof(struct xt_EQUALIZE_info),
        .table = "filter",
        .checkentry = equalize_tg_check,
        .me = THIS_MODULE,
    },
};

static int __init equalize_tg_init(void) {
  return xt_register_targets(equalize_tg_reg, ARRAY_SIZE(equalize_tg_reg));
}

static void __exit equalize_tg_exit(void) {
  xt_unregister_targets(equalize_tg_reg, ARRAY_SIZE(equalize_tg_reg));
}

module_init(equalize_tg_init);
module_exit(equalize_tg_exit);
MODULE_ALIAS("ipt_EQUALIZE");
MODULE_ALIAS("ip6t_EQUALIZE");