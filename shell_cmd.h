#ifndef __CMD_H_
#define __CMD_H_

#include "list.h"
#include "readline.h"

typedef int32 (*cmd_func_t)(/*int32 flag, */int32 argc, int8* const argv[]);

struct cmd_tbl_s 
{
    int8*               name;       /* Command Name                 */
    int32               maxargs;    /* maximum number of arguments  */
    //int32               repeatable; /* autorepeat allowed?        */
    cmd_func_t          cmd;        /* command call function        */
    int8*               help;       /* Help  message    (short)     */
#ifdef	CONFIG_SYS_LONGHELP
    int8*               usage;      /* Usage message    (long)      */
#endif
#ifdef CONFIG_AUTO_COMPLETE
                        /* do auto completion on the arguments */
    int32               (*complete)(int32, int8* const, int8, int32, int8*);
#endif
#ifdef CONFIG_MULTILEVEL_CMD
    struct list_head    child;
#endif
    struct list_head    list;//命令列表
};

typedef struct cmd_tbl_s    cmd_tbl_t;

#ifdef CONFIG_AUTO_COMPLETE
#define _CMD_COMPLETE(x) .complete = x,
#else
#define _CMD_COMPLETE(x)
#endif
#ifdef CONFIG_SYS_LONGHELP
#define _CMD_USAGE(x) .usage = x,
#else
#define _CMD_USAGE(x)
#endif

#define CLI_REG_NAME_TYP(_NAME) CMD_LIST_##_NAME
#define CLI_REG_NAME_STR(_NAME) #_NAME

#define CLI_REG_CMD_COMPLETE(_name, _maxargs, _cmd, _help, _usage, _comp)   \
static cmd_tbl_t CLI_REG_NAME_TYP(_name) =                                  \
{                                                                           \
    .name       = CLI_REG_NAME_STR(_name),                                  \
    .maxargs    = _maxargs,                                                 \
    .cmd        = _cmd,                                                     \
    .help       = _help,                                                    \
    _CMD_USAGE(_usage)                                                      \
    _CMD_COMPLETE(_comp)                                                    \
}

/*****************************************************************************
 函 数 名  : CLI_REG_CMD
 功能描述  : 用于注册命令的宏
 输入参数  : _name     :命令名称(不带双引号)
             _maxargs  :命令参数个数
             _cmd      :命令回调函数
             _help     :命令帮助信息
 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
#define CLI_REG_CMD(_name, _maxargs, _cmd, _help)		                    \
do{                                                                         \
    CLI_REG_CMD_COMPLETE(_name, _maxargs, _cmd, _help, NULL, NULL);         \
    cli_reg_cmd(&(CLI_REG_NAME_TYP(_name)));                               \
}while(0)

/*****************************************************************************
 函 数 名  : cli_reg_cmd
 功能描述  : 向系统中注册一条命令
 输入参数  : cmd_tbl_t* cmd  :命令结构体
 输出参数  : 无
 返 回 值  : 0/1 - success/error
 *****************************************************************************/
extern int32 cli_reg_cmd          (cmd_tbl_t* cmd);

/*****************************************************************************
 函 数 名  : run_command
 功能描述  : Run a command using the selected parser.
 输入参数  : const int8 *cmd  :Command to run
             int32 flag         :Execution flags (CMD_FLAG_...)
 输出参数  : 无
 返 回 值  : 0 on success, or != 0 on error.
 *****************************************************************************/
extern int32 cli_run_cmd          (const int8 *cmd/*, int32 flag*/);

//extern cmd_tbl_t *cli_find_cmd    (const int8 *name);
//extern int32 cli_allcmd_print     (void);
//extern int32 cli_cmd_usage        (const cmd_tbl_t *cmdtp);
extern int32 cli_cmd_init         (void);

#endif