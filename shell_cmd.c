#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"
#include "shell_cmd.h"

//#define __CMD_DEBUG__

#ifdef __CMD_DEBUG__
#define CMD_DEBUG_LOG(fmt, args...)                                            \
    cli_printf("DEBUG: [%s:%d] DbgMsg:" fmt "\r\n", __FUNCTION__, __LINE__,    \
               ##args)
#define CMD_DEBUG_ERR(errno, fmt, args...)                                     \
    cli_printf("ERROR: [%s:%d] ErrNum:%d ErrMsg:" fmt "\r\n", __FUNCTION__,    \
               __LINE__, errno, ##args)
#else
#define CMD_DEBUG_LOG(fmt, args...)
#define CMD_DEBUG_ERR(errno, fmt, args...)
#endif

/* 用于跨平台的函数接口 */
#define cli_printf printf
#define cli_strcmp strcmp
#define cli_strncmp strncmp
#define cli_strlen strlen
#define cli_strcpy strcpy
#define cli_memcpy memcpy
#define cli_memset memset

typedef enum _command_ret_t {
    CMD_RET_SUCCESS,    /* 0 = Success */
    CMD_RET_FAILURE,    /* 1 = Failure */
    CMD_RET_USAGE = -1, /* Failure, please report 'usage' error */
} command_ret_t;

cmd_tbl_t CMD_HEAD; //全局变量，保存命令列表

static int32 __cmd_init_head(cmd_tbl_t *head) {
    ARG_CHK_EQ(head, NULL, ERROR);
    INIT_LIST_HEAD(&(head->list));
    return OK;
}

static int32 __cmd_init_child(cmd_tbl_t *child) {
#ifdef CONFIG_MULTILEVEL_CMD
    ARG_CHK_EQ(child, NULL, ERROR);
    INIT_LIST_HEAD(&(child->child));
    return OK;
#else
    return OK;
#endif
}

static int32 __cmd_add(cmd_tbl_t *head, cmd_tbl_t *cmd) {
    ARG_CHK_EQ(head, NULL, ERROR);
    ARG_CHK_EQ(cmd, NULL, ERROR);
    list_add(&(cmd->list), &(head->list));
    __cmd_init_child(cmd);
    return OK;
}

static int32 __cmd_add_prev(cmd_tbl_t *head, cmd_tbl_t *cmd) {
    ARG_CHK_EQ(head, NULL, ERROR);
    ARG_CHK_EQ(cmd, NULL, ERROR);
    list_add_tail(&(cmd->list), &(head->list));
    __cmd_init_child(cmd);
    return OK;
}

static int32 __cmd_add_child(cmd_tbl_t *f, cmd_tbl_t *c) {
#ifdef CONFIG_MULTILEVEL_CMD
    ARG_CHK_EQ(f, NULL, ERROR);
    ARG_CHK_EQ(c, NULL, ERROR);
    list_add_tail(&(c->list), &(f->child));
    __cmd_init_child(c);
    return OK;
#else
    return OK;
#endif
}

static int32 __cli_reg_cmd(cmd_tbl_t *cmdhead, cmd_tbl_t *regcmd) {
    cmd_tbl_t *pos = NULL;
    int32 ret = 0;
    ARG_CHK_EQ(cmdhead, NULL, ERROR);
    ARG_CHK_EQ(regcmd, NULL, ERROR);

    if (list_empty(&(cmdhead->list)) == TRUE) //链表为空
    {
        CMD_DEBUG_LOG("List is empty!\r\n");
        return (__cmd_add(cmdhead, regcmd));
    } else {
        list_for_each_entry(pos, &(cmdhead->list), list) {
            ret = cli_strcmp(regcmd->name, pos->name);
            if (ret == 0) {
                CMD_DEBUG_ERR(ERROR, "Command:%s already exist!\r\n",
                              regcmd->name);
                return ERROR; //命令已经存在
            } else if (ret > 0) {
                if (list_is_last(&(pos->list), &(cmdhead->list)) == TRUE) {
                    return (__cmd_add(pos, regcmd));
                } else {
                    continue;
                }
            } else {
                return (__cmd_add_prev(pos, regcmd));
            }
        }
    }
}

/*****************************************************************************
 函 数 名  : cli_reg_cmd
 功能描述  : 向系统中注册一条命令
 输入参数  : cmd_tbl_t* cmd  :命令结构体
 输出参数  : 无
 返 回 值  : 0/1 - success/error
 *****************************************************************************/
int32 cli_reg_cmd(cmd_tbl_t *cmd) { return __cli_reg_cmd(&CMD_HEAD, cmd); }

static cmd_tbl_t *__cmd_find_byname(const cmd_tbl_t *cmdhead,
                                    const int8 *name) {
    cmd_tbl_t *pos = NULL;
    ARG_CHK_EQ(cmdhead, NULL, NULL);
    ARG_CHK_EQ(name, NULL, NULL);
    list_for_each_entry(pos, &(cmdhead->list), list) {
        ARG_CHK_EQ(cli_strcmp(pos->name, name), 0, pos);
    }
    return NULL;
}

cmd_tbl_t *cli_find_cmd(const int8 *name) {
    return __cmd_find_byname(&CMD_HEAD, name);
}

int32 cli_cmd_usage(const cmd_tbl_t *cmdtp) {
    ARG_CHK_EQ(cmdtp, NULL, ERROR);
    ARG_CHK_EQ(cmdtp->name, NULL, ERROR);
    ARG_CHK_EQ(cmdtp->help, NULL, ERROR);
    cli_printf("%s - %s\r\n", cmdtp->name, cmdtp->help);

#ifdef CONFIG_SYS_LONGHELP
    cli_printf("Usage:\r\n%s ", cmdtp->name);

    if (!cmdtp->usage) {
        cli_printf("- No additional help available.\r\n\r\n");
        return OK;
    }

    cli_printf(cmdtp->usage);
    cli_printf('\r\n\r\n');
#endif /* CONFIG_SYS_LONGHELP */
    return OK;
}

static int32 __cli_allcmd_print(const cmd_tbl_t *cmdhead) {
    cmd_tbl_t *pos = NULL;
    ARG_CHK_EQ(cmdhead, NULL, ERROR);
    list_for_each_entry(pos, &(cmdhead->list), list) {
        ARG_CHK_EQ(cli_cmd_usage(pos), ERROR, ERROR);
    }
    return OK;
}

int32 cli_allcmd_print(void) { return __cli_allcmd_print(&CMD_HEAD); }

static int32 cli_cmd_call(cmd_tbl_t *cmdtp,
                          // int32       flag,
                          int32 argc, int8 *const argv[]) {
    int32 result = 0;

    result = (cmdtp->cmd)(/*flag, */ argc, argv);
    if (result != OK)
        CMD_DEBUG_ERR(result, "Command failed");
    return result;
}

command_ret_t cli_cmd_process(
    // int32           flag,
    int32 argc, int8 *const argv[] /*,
             int32*          repeatable*/
) {
    command_ret_t rc = CMD_RET_SUCCESS;
    cmd_tbl_t *cmdtp = NULL;

    /* Look up command in command table */
    cmdtp = cli_find_cmd(argv[0]);
    if (cmdtp == NULL) {
        cli_printf("Unknown command '%s' - try 'help'\n", argv[0]);
        return ERROR;
    }

    /* found - check max args */
    if (argc > cmdtp->maxargs)
        rc = CMD_RET_USAGE;

    /* If OK so far, then do the command */
    if (rc == CMD_RET_SUCCESS) {
        rc = cli_cmd_call(cmdtp, /*flag, */ argc, argv);
        //*repeatable &= cmdtp->repeatable;
    } else {
        // if (rc == CMD_RET_USAGE)
        rc = cli_cmd_usage(cmdtp);
    }
    return rc;
}

int32 cli_parse_line(int8 *line, int8 *argv[]) {
    int32 nargs = 0;

    CMD_DEBUG_LOG("parse_line: \"%s\"\n", line);
    while (nargs < CONFIG_SYS_MAXARGS) {
        /* skip any white space */
        while (isblank(*line))
            ++line;

        if (*line == '\0') /* end of line, no more args    */
        {
            argv[nargs] = NULL;
            CMD_DEBUG_LOG("parse_line: nargs=%d\n", nargs);
            return (nargs);
        }

        argv[nargs++] = line; /* begin of argument string */

        /* find end of string */
        while (*line && !isblank(*line))
            ++line;

        if (*line == '\0') /* end of line, no more args    */
        {
            argv[nargs] = NULL;
            CMD_DEBUG_LOG("parse_line: nargs=%d\n", nargs);
            return (nargs);
        }

        *line++ = '\0'; /* terminate current arg     */
    }

    cli_printf("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

    CMD_DEBUG_LOG("parse_line: nargs=%d\n", nargs);
    return (nargs);
}

static void cli_process_macros(const int8 *input, int8 *output) {
    cli_memset(output, 0, sizeof(output));
    cli_memcpy(output, input, cli_strlen(input));
}

static int32 builtin_run_cmd(const int8 *cmd /*, int32 flag*/) {
    int8 cmdbuf[CONFIG_SYS_CBSIZE] = {0}; /* working copy of cmd      */
    int8 *token = NULL;                   /* start of token in cmdbuf */
    int8 *sep = NULL; /* end of token (separator) in cmdbuf */
    // int8 finaltoken[CONFIG_SYS_CBSIZE];
    int8 *str = cmdbuf;
    int8 *argv[CONFIG_SYS_MAXARGS + 1] = {NULL}; /* NULL terminated  */
    int32 argc = 0, inquotes = 0;
    // int32 repeatable = 1;
    int32 rc = 0;

    CMD_DEBUG_LOG("[RUN_COMMAND] cmd[%p]=\"", cmd);
    if (cmd == NULL)
        CMD_DEBUG_LOG("NULL");
    else
        CMD_DEBUG_LOG("%s", cmd);

    CMD_DEBUG_LOG("\"\n");

    if (!cmd || !*cmd) {
        return ERROR; /* empty command */
    }

    if (cli_strlen(cmd) >= CONFIG_SYS_CBSIZE) {
        cli_printf("## Command too long!\n");
        return ERROR;
    }

    cli_strcpy(cmdbuf, cmd);

    /* Process separators and check for invalid
     * repeatable commands
     */
    CMD_DEBUG_LOG("[PROCESS_SEPARATORS] %s\n", cmd);
    while (*str) {
        /*多条命令中间用"\;分割"*/
        for (inquotes = 0, sep = str; *sep; sep++) {
            if ((*sep == '\'') && (*(sep - 1) != '\\'))
                inquotes = !inquotes;

            if (!inquotes && (*sep == ';') && /* separator        */
                (sep != str) &&               /* past string start    */
                (*(sep - 1) != '\\'))         /* and NOT escaped  */
                break;
        }

        /*
         * Limit the token to data between separators
         */
        token = str;
        if (*sep) {
            str = sep + 1; /* start of command for next pass */
            *sep = '\0';
        } else
            str = sep; /* no more commands for next pass */

        CMD_DEBUG_LOG("token: \"%s\"\n", token);

        /* 查找并替换命令中包含的宏 */
        // cli_process_macros(token, finaltoken);

        /* 分解命令行参数 */
        if ((argc = cli_parse_line(/*finaltoken*/ token, argv)) == 0) {
            rc = ERROR; /* no command at all */
            continue;
        }

        if (cli_cmd_process(/*flag, */ argc, argv /*, &repeatable*/) != OK)
            rc = ERROR;
    }

    return rc /*? rc : repeatable*/;
}

/*****************************************************************************
 函 数 名  : run_command
 功能描述  : Run a command using the selected parser.
 输入参数  : const int8 *cmd  :Command to run
             int32 flag         :Execution flags (CMD_FLAG_...)
 输出参数  : 无
 返 回 值  : 0 on success, or != 0 on error.
 *****************************************************************************/
int32 cli_run_cmd(const int8 *cmd /*, int32 flag*/) {
    if (builtin_run_cmd(cmd /*, flag*/) != OK)
        return ERROR;

    return OK;
}

static int32 do_help(int32 argc, int8 *const argv[]) {
    if (cli_strcmp(argv[0], "help") == 0) {
        if (argc == 1)
            cli_allcmd_print();
        else {
            if (cli_cmd_usage(cli_find_cmd(argv[1])) != OK) {
                cli_printf("Command: %s - Not found!\r\n", argv[1]);
            }
        }
    } else {
        cli_printf("Command Error!\r\n");
    }
    return OK;
}

int32 cli_cmd_init(void) {
    __cmd_init_head(&CMD_HEAD); //初始化命令列表
    CLI_REG_CMD(help, 2, do_help, "View the command help information.");
    return OK;
}
