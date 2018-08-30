/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* RFC 8367 IP filter module for IP tables */

#ifndef _XT_EQUALIZE_H
#define _XT_EQUALIZE_H

#include <linux/types.h>

enum {
  XT_EQUALIZE_SET = 0,
};

#define XT_EQUALIZE_MAXMODE XT_EQUALIZE_SET

struct xt_EQUALIZE_info {
};

#endif