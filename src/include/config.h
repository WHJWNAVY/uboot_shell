#ifndef __UBOOT_SHELL_CONFIG__
#define __UBOOT_SHELL_CONFIG__

#define CONFIG_IS_ENABLED(option) defined (CONFIG_ ## option)

#define CONFIG_CMDLINE 1
#define CONFIG_CMD_RUN 1
#define CONFIG_HUSH_PARSER 1
#define CONFIG_AUTO_COMPLETE 1
#define CONFIG_CMDLINE_EDITING 1

#endif
