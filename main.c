#include <stdio.h>
#include <string.h>
#include "shell_cmd.h"

void main(void) {
    char buff[CONFIG_SYS_CBSIZE];
    char *buff_t = NULL;
    cli_cmd_init();
    CLI_REG_CMD(ls, 0, NULL, "List directory contents.");
    CLI_REG_CMD(cd, 0, NULL, "Change directory.");
    CLI_REG_CMD(cat, 0, NULL,
                "Concatenate files and print on the standard output.");
    CLI_REG_CMD(pwd, 0, NULL, "Print name of current directory.");
    CLI_REG_CMD(man, 0, NULL, "An interface to the on-line reference manuals.");
    CLI_REG_CMD(man1, 0, NULL,
                "An interface to the on-line reference manuals.");
    CLI_REG_CMD(man2, 0, NULL,
                "An interface to the on-line reference manuals.");
    while (TRUE) {
        buff_t = readline(CONFIG_SYS_PROMPT);
        if (buff_t != NULL) {
            strcpy(buff, buff_t);
            cli_run_cmd(buff);
        } else {
            printf("ERROR!\r\n");
        }
    }
}
