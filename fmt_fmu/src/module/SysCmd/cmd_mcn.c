/******************************************************************************
 * Copyright 2020 The Firmament Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <firmament.h>
#include <string.h>

#include "module/syscmd/syscmd.h"

static int _name_maxlen(const char* title, const McnList_t list)
{
    int max_len = strlen(title);

    for (McnList_t cp = list; cp != NULL; cp = cp->next) {
        int len = strlen(cp->hub_t->obj_name);

        if (len > max_len) {
            max_len = len;
        }
    }

    return max_len;
}

static void _list_topics(void)
{
    McnList list = mcn_get_list();
    char* title_1 = "Topic";
    char* title_2 = "#SUB";
    char* title_3 = "Freq(Hz)";
    char* title_4 = "Echo";
    uint32_t title1_len = _name_maxlen(title_1, &list) + 2;
    uint32_t title2_len = strlen(title_2) + 2;
    uint32_t title3_len = strlen(title_3) + 2;
    uint32_t title4_len = strlen(title_4) + 2;

    syscmd_printf(' ', title1_len, SYSCMD_ALIGN_MIDDLE, title_1);
    syscmd_putc(' ', 1);
    syscmd_printf(' ', title2_len, SYSCMD_ALIGN_MIDDLE, title_2);
    syscmd_putc(' ', 1);
    syscmd_printf(' ', title3_len, SYSCMD_ALIGN_MIDDLE, title_3);
    syscmd_putc(' ', 1);
    syscmd_printf(' ', title4_len, SYSCMD_ALIGN_MIDDLE, title_4);
    console_printf("\n");

    syscmd_putc('-', title1_len);
    syscmd_putc(' ', 1);
    syscmd_putc('-', title2_len);
    syscmd_putc(' ', 1);
    syscmd_putc('-', title3_len);
    syscmd_putc(' ', 1);
    syscmd_putc('-', title4_len);
    console_printf("\n");

    for (McnList_t cp = &list; cp != NULL; cp = cp->next) {
        syscmd_printf(' ', title1_len, SYSCMD_ALIGN_LEFT, cp->hub_t->obj_name);
        syscmd_putc(' ', 1);
        syscmd_printf(' ', title2_len, SYSCMD_ALIGN_MIDDLE, "%d", (int)cp->hub_t->link_num);
        syscmd_putc(' ', 1);
        syscmd_printf(' ', title3_len, SYSCMD_ALIGN_MIDDLE, "%.1f", cp->hub_t->freq);
        syscmd_putc(' ', 1);
        syscmd_printf(' ', title4_len, SYSCMD_ALIGN_MIDDLE, "%s", cp->hub_t->echo ? "true" : "false");
        console_printf("\n");
    }
}

static void _echo_topic(const char* topic_name, int optc, optv_t* optv)
{
    McnList_t cp;
    McnList list = mcn_get_list();
    uint32_t cnt = 0xFFFFFFFF;
    uint32_t period = 500;

    /* handle operation */
    for (uint16_t i = 0; i < optc; i++) {
        if (STRING_COMPARE(optv[i].opt, "-c") || STRING_COMPARE(optv[i].opt, "--cnt")) {
            if (!syscmd_is_num(optv[i].val))
                continue;

            cnt = atoi(optv[i].val);
        }

        if (STRING_COMPARE(optv[i].opt, "-p") || STRING_COMPARE(optv[i].opt, "--period")) {
            if (!syscmd_is_num(optv[i].val))
                continue;

            period = atoi(optv[i].val);
        }
    }

    for (cp = &list; cp != NULL; cp = cp->next) {
        if (strcmp(cp->hub_t->obj_name, topic_name) == 0) {
            break;
        }
    }

    if (cp == NULL) {
        console_printf("can not find topic %s\n", topic_name);
        return;
    }

    if (cp->hub_t->echo == NULL) {
        console_printf("there is no topic echo function defined!\n");
        return;
    }

    McnNode_t node = mcn_subscribe(cp->hub_t, NULL, NULL);

    if (node == NULL) {
        console_printf("mcn subscribe fail\n");
        return;
    }

    while (cnt) {
        /* type any key to exit */
        if (syscmd_has_input()) {
            syscmd_flush();
            break;
        }

        if (mcn_poll(node)) {
            /* call custom echo function */
            cp->hub_t->echo(cp->hub_t);
            mcn_node_clear(node);
            cnt--;
        }

        if (period) {
            rt_thread_delay(period);
        }
    }

    mcn_unsubscribe(cp->hub_t, node);
}

static void show_usage(void)
{
    PRINT_USAGE(mcn, [OPTION] ACTION[ARGS]);

    PRINT_STRING("\nAction:\n");
    PRINT_ACTION("list", 4, "List all uMCN topics.");
    PRINT_ACTION("echo", 4, "Echo a uMCN topic.");

    PRINT_STRING("\nOption:\n");
    PRINT_ACTION("-c, --cnt", 12, "Set topic echo count, e.g, -c=10 will echo 10 times.");
    PRINT_ACTION("-p, --period", 12, "Set topic echo period(ms), -p=0 inherits topic period. The default period is 500ms.");
}

static int handle_cmd(int argc, char** argv, int optc, optv_t* optv)
{
    /* handle operation */
    for (uint16_t i = 0; i < optc; i++) {
        if (STRING_COMPARE(optv[i].opt, "-h") || STRING_COMPARE(optv[i].opt, "--help")) {
            show_usage();
            return 0;
        }
    }

    if (argc < 2) {
        show_usage();
        return -1;
    }

    if (strcmp(argv[1], "list") == 0) {
        _list_topics();
    } else if (strcmp(argv[1], "echo") == 0) {
        if (argc < 3) {
            console_printf("usage: mcn echo <topic>\n");
            return -1;
        }

        _echo_topic(argv[2], optc, optv);
    } else {
        show_usage();
    }

    return 0;
}

int cmd_mcn(int argc, char** argv)
{
    return syscmd_process(argc, argv, handle_cmd);
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_mcn, __cmd_mcn, uMCN topics operations);