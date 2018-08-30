/*
 * EQUALIZE Target module
 * This program is distributed under the terms of GNU GPL
 */
#include "xt_EQUALIZE.h"
#include <stdio.h>
#include <xtables.h>

enum {
  O_EQUALIZE_SET = 0,
  F_EQUALIZE_SET = 1 << O_EQUALIZE_SET,
  F_ANY = F_EQUALIZE_SET,
};

static const struct xt_option_entry EQUALIZE_opts[] = {
    XTOPT_TABLEEND,
};

static void EQUALIZE_help(void) { printf("EQUALIZE target options\n"); }

static void EQUALIZE_parse(struct xt_option_call *cb) {}

static void EQUALIZE_check(struct xt_fcheck_call *cb) {}

static void EQUALIZE_save(const void *ip, const struct xt_entry_target *target) {}

static void EQUALIZE_print(const void *ip, const struct xt_entry_target *target,
                        int numeric) {
  printf(" EQUALIZE ");
}

static struct xtables_target equalize_tg_reg = {
    .name = "EQUALIZE",
    .version = XTABLES_VERSION,
    .family = NFPROTO_UNSPEC,
    .size = XT_ALIGN(sizeof(struct xt_EQUALIZE_info)),
    .userspacesize = XT_ALIGN(sizeof(struct xt_EQUALIZE_info)),
    .help = EQUALIZE_help,
    .print = EQUALIZE_print,
    .save = EQUALIZE_save,
    .x6_parse = EQUALIZE_parse,
    .x6_fcheck = EQUALIZE_check,
    .x6_options = EQUALIZE_opts,
};

static __attribute__((constructor)) void _init(void) {
  xtables_register_target(&equalize_tg_reg);
}