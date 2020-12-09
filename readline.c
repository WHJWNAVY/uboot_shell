/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * 这是一个跨平台的简单的readline库,实现了类似GNU Readline库的大部分功能.
 * 本代码提取修改自UBOOT源码(具体位于"u-boot-2013.01-fs4412\common\main.c"),
 * 版权归原作者所有!请尊重作者劳动成果!---20160121(胡较小兄弟/www.wnavy.com)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"

#define CTL_CH(c) ((c) - 'a' + 1)
#define CTL_BACKSPACE ('\b')
#define DEL ((int8)255)
#define DEL7 ((int8)127)
#define CREAD_HIST_CHAR ('!')

#define HIST_MAX 20
#define HIST_SIZE CONFIG_SYS_CBSIZE

#define BEGINNING_OF_LINE()                                                    \
    {                                                                          \
        while (num) {                                                          \
            getcmd_putch(CTL_BACKSPACE);                                       \
            num--;                                                             \
        }                                                                      \
    }

#define ERASE_TO_EOL()                                                         \
    {                                                                          \
        if (num < eol_num) {                                                   \
            rl_printf("%*s", (int32)(eol_num - num), "");                      \
            do {                                                               \
                getcmd_putch(CTL_BACKSPACE);                                   \
            } while (--eol_num > num);                                         \
        }                                                                      \
    }

#define REFRESH_TO_EOL()                                                       \
    {                                                                          \
        if (num < eol_num) {                                                   \
            wlen = eol_num - num;                                              \
            putnstr(buf + num, wlen);                                          \
            num = eol_num;                                                     \
        }                                                                      \
    }

#define putnstr(str, n)                                                        \
    do {                                                                       \
        rl_printf("%.*s", (int32)n, str);                                      \
    } while (0)

static int32 hist_max;
static int32 hist_add_idx;
static int32 hist_cur = -1;
static uint32 hist_num;

static int8 *hist_list[HIST_MAX];
static int8 hist_lines[HIST_MAX][HIST_SIZE + 1]; /* Save room for NULL */
int8 console_buffer[CONFIG_SYS_CBSIZE + 1];      /* console I/O buffer	*/

/*****************************************************************************
 以下函数需要自己根据自己的平台实现的函数，这里的示例适用于linux平台
*****************************************************************************/
/* 用于跨平台的函数接口 */
#define getcmd_putch(ch) putch(ch)
#define getcmd_getch() getch()
#define getcmd_cbeep() getcmd_putch('\a')
#define rl_printf printf
#define rl_strlen strlen
#define rl_strcpy strcpy
#define rl_memset memset
#define rl_memmove memmove
/*****************************************************************************
 函 数 名  : getch
 功能描述  : 从控制台无回显地取一个字符
 输入参数  : void
 输出参数  : 无
 返 回 值  : 读取到的字符
*****************************************************************************/
static int32 getch(void) {
    int8 c;
    system("stty -echo");
    system("stty -icanon");
    c = getchar();
    system("stty icanon");
    system("stty echo");
    return c;
}

/*****************************************************************************
 函 数 名  : putch
 功能描述  : 在当前光标处向屏幕输出字符ch，然后光标自动右移一个字符位置
 输入参数  : ch: 要输出的字符
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
static int32 putch(int8 ch) { return putchar(ch); }

/*****************************************************************************
 函 数 名  : cmd_auto_complete
 功能描述  : 按tab键命令自动补全功能需要的函数
 输入参数  :
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
#ifdef CONFIG_AUTO_COMPLETE
int32 cmd_auto_complete(const int8 *const prompt, int8 *buf, int32 *np,
                        int32 *colp) {
    return 1;
}
#endif

/*****************************************************************************
 以下函数是实现readline功能的库函数，平台无关，不建议修改其内容
*****************************************************************************/
static void hist_init(void) {
    int32 i = 0;

    hist_max = 0;
    hist_add_idx = 0;
    hist_cur = -1;
    hist_num = 0;

    for (i = 0; i < HIST_MAX; i++) {
        hist_list[i] = hist_lines[i];
        hist_list[i][0] = '\0';
    }
}

static void cread_add_to_hist(int8 *line) {
    rl_strcpy(hist_list[hist_add_idx], line);

    if (++hist_add_idx >= HIST_MAX)
        hist_add_idx = 0;

    if (hist_add_idx > hist_max)
        hist_max = hist_add_idx;

    hist_num++;
}

static int8 *hist_prev(void) {
    int8 *ret = NULL;
    int32 old_cur = 0;

    if (hist_cur < 0)
        return NULL;

    old_cur = hist_cur;
    if (--hist_cur < 0)
        hist_cur = hist_max;

    if (hist_cur == hist_add_idx) {
        hist_cur = old_cur;
        ret = NULL;
    } else
        ret = hist_list[hist_cur];

    return (ret);
}

static int8 *hist_next(void) {
    int8 *ret = NULL;

    if (hist_cur < 0)
        return NULL;

    if (hist_cur == hist_add_idx)
        return NULL;

    if (++hist_cur > hist_max)
        hist_cur = 0;

    if (hist_cur == hist_add_idx) {
        ret = "";
    } else
        ret = hist_list[hist_cur];

    return (ret);
}

static void cread_add_char(int8 ichar, int32 insert, uint32 *num,
                           uint32 *eol_num, int8 *buf, uint32 len) {
    uint32 wlen = 0;

    if (insert || *num == *eol_num) {
        if (*eol_num > len - 1) {
            getcmd_cbeep();
            return;
        }
        (*eol_num)++;
    }

    if (insert) {
        wlen = *eol_num - *num;
        if (wlen > 1) {
            rl_memmove(&buf[*num + 1], &buf[*num], wlen - 1);
        }

        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
        while (--wlen) {
            getcmd_putch(CTL_BACKSPACE);
        }
    } else {
        /* echo the character */
        wlen = 1;
        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
    }
}

static void cread_add_str(int8 *str, int32 strsize, int32 insert, uint32 *num,
                          uint32 *eol_num, int8 *buf, uint32 len) {
    while (strsize--) {
        cread_add_char(*str, insert, num, eol_num, buf, len);
        str++;
    }
}

static int32 cread_line(const int8 *const prompt, int8 *buf, uint32 *len) {
    uint32 num = 0;
    uint32 eol_num = 0;
    uint32 wlen = 0;
    int8 ichar = 0;
    int32 insert = 1;
    int32 esc_len = 0;
    int8 esc_save[8] = {0};
    int32 init_len = rl_strlen(buf);
    int32 first = 1;

    if (init_len)
        cread_add_str(buf, init_len, 1, &num, &eol_num, buf, *len);

    while (1) {
        ichar = getcmd_getch();

        if ((ichar == '\n') || (ichar == '\r')) {
            getcmd_putch('\n');
            break;
        }

        /*
         * handle standard linux xterm esc sequences for arrow key, etc.
         */
        if (esc_len != 0) {
            if (esc_len == 1) {
                if (ichar == '[') {
                    esc_save[esc_len] = ichar;
                    esc_len = 2;
                } else {
                    cread_add_str(esc_save, esc_len, insert, &num, &eol_num,
                                  buf, *len);
                    esc_len = 0;
                }
                continue;
            }

            switch (ichar) {

            case 'D': /* <- key */
                ichar = CTL_CH('b');
                esc_len = 0;
                break;
            case 'C': /* -> key */
                ichar = CTL_CH('f');
                esc_len = 0;
                break; /* pass off to ^F handler */
            case 'H':  /* Home key */
                ichar = CTL_CH('a');
                esc_len = 0;
                break; /* pass off to ^A handler */
            case 'A':  /* up arrow */
                ichar = CTL_CH('p');
                esc_len = 0;
                break; /* pass off to ^P handler */
            case 'B':  /* down arrow */
                ichar = CTL_CH('n');
                esc_len = 0;
                break; /* pass off to ^N handler */
            default:
                esc_save[esc_len++] = ichar;
                cread_add_str(esc_save, esc_len, insert, &num, &eol_num, buf,
                              *len);
                esc_len = 0;
                continue;
            }
        }

        switch (ichar) {
        case 0x1b:
            if (esc_len == 0) {
                esc_save[esc_len] = ichar;
                esc_len = 1;
            } else {
                rl_printf("impossible condition #876\n");
                esc_len = 0;
            }
            break;

        case CTL_CH('a'):
            BEGINNING_OF_LINE();
            break;
        case CTL_CH('c'): /* ^C - break */
            *buf = '\0';  /* discard input */
            return (-1);
        case CTL_CH('f'):
            if (num < eol_num) {
                getcmd_putch(buf[num]);
                num++;
            }
            break;
        case CTL_CH('b'):
            if (num) {
                getcmd_putch(CTL_BACKSPACE);
                num--;
            }
            break;
        case CTL_CH('d'):
            if (num < eol_num) {
                wlen = eol_num - num - 1;
                if (wlen) {
                    rl_memmove(&buf[num], &buf[num + 1], wlen);
                    putnstr(buf + num, wlen);
                }

                getcmd_putch(' ');
                do {
                    getcmd_putch(CTL_BACKSPACE);
                } while (wlen--);
                eol_num--;
            }
            break;
        case CTL_CH('k'):
            ERASE_TO_EOL();
            break;
        case CTL_CH('e'):
            REFRESH_TO_EOL();
            break;
        case CTL_CH('o'):
            insert = !insert;
            break;
        case CTL_CH('x'):
        case CTL_CH('u'):
            BEGINNING_OF_LINE();
            ERASE_TO_EOL();
            break;
        case DEL:
        case DEL7:
        case 8:
            if (num) {
                wlen = eol_num - num;
                num--;
                rl_memmove(&buf[num], &buf[num + 1], wlen);
                getcmd_putch(CTL_BACKSPACE);
                putnstr(buf + num, wlen);
                getcmd_putch(' ');
                do {
                    getcmd_putch(CTL_BACKSPACE);
                } while (wlen--);
                eol_num--;
            }
            break;
        case CTL_CH('p'):
        case CTL_CH('n'): {
            int8 *hline = NULL;

            esc_len = 0;

            if (ichar == CTL_CH('p'))
                hline = hist_prev();
            else
                hline = hist_next();

            if (!hline) {
                getcmd_cbeep();
                continue;
            }

            /* nuke the current line */
            /* first, go home */
            BEGINNING_OF_LINE();

            /* erase to end of line */
            ERASE_TO_EOL();

            /* copy new line into place and display */
            rl_strcpy(buf, hline);
            eol_num = rl_strlen(buf);
            REFRESH_TO_EOL();
            continue;
        }
#ifdef CONFIG_AUTO_COMPLETE
        case '\t': {
            int32 num2 = 0, col = 0;

            /* do not autocomplete when in the middle */
            if (num < eol_num) {
                getcmd_cbeep();
                break;
            }

            buf[num] = '\0';
            col = rl_strlen(prompt) + eol_num;
            num2 = num;
            if (cmd_auto_complete(prompt, buf, &num2, &col)) {
                col = num2 - num;
                num += col;
                eol_num += col;
            }
            break;
        }
#endif
        default:
            cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
            break;
        }
    }
    *len = eol_num;
    buf[eol_num] = '\0'; /* lose the newline */

    if (buf[0] && buf[0] != CREAD_HIST_CHAR)
        cread_add_to_hist(buf);
    hist_cur = hist_add_idx;

    return 0;
}

static int32 readline_into_buffer(const int8 *const prompt, int8 *buffer) {
    int8 *p = buffer;
    uint32 len = CONFIG_SYS_CBSIZE;
    int32 rc = 0;
    static int32 initted = 0;

    if (!initted) {
        hist_init();
        initted = 1;
    }

    if (prompt)
        rl_printf("%s", prompt);

    rc = cread_line(prompt, p, &len);
    return rc < 0 ? rc : len;
}

/*****************************************************************************
 函 数 名  : readline
 功能描述  : readline是一个开源的跨平台程序库，提供了交互式的文本编辑功能。
 输入参数  : prompt     :命令行提示符
 输出参数  : readline   :读取到的内容
 返 回 值  :
*****************************************************************************/
int8 *readline(const int8 *const prompt) {
    int32 ret = 0;
    // console_buffer[0] = '\0';
    rl_memset(console_buffer, 0, sizeof(console_buffer));

    ret = readline_into_buffer(prompt, console_buffer);
    return (ret < 0) ? NULL : console_buffer;
}
