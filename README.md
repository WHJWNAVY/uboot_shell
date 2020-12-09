# uboot-shell

> 这是从uboot源码中提取出来的shell命令行解释器源码，并进行了加工和修改，可以跨平台移植。使用也很简单。这里只简单介绍源码组成，无关细节，详情看源码。该源码包含以下及部分内容：

## main.c：
>  一个简单的示例代码。
## type.h：
>  基本数据类型的重定义。
## list.h：
>  提取自linux内核的双向链表，已经过优化，不依赖任何第三方代码。用于存储和查找命令。
## readline.c/readline.h：
>  从uboot源码中提取的一个readline实现，功能类似于GNU Readline库，但更简单。包含命令历史纪录功能和tab补全功能和其他一些快捷键。但是命令补全部分需要自己实现。该部分已修改并优化，不依赖其他库，可单独使用，只是有些跨平台相关的函数需要自己根据自己的平台修改。
## shell_cmd.c/shell_cmd.h：
>  从uboot源码中提取的命令行解释器代码，并提供了一个用于向系统中注册命令的宏。该部分已修改并优化，不依赖其他库，可单独使用，只是有些跨平台相关的函数需要自己根据自己的平台修改。默认自带一条help命令。
## shell_config.h：
>  关于readline库和命令行解释器的配置文件。



## 示例

```bash
$ ./main
root# ?
Unknown command '?' - try 'help'
root# help
cat - Concatenate files and print on the standard output.
cd - Change directory.
help - View the command help information.
ls - List directory contents.
man - An interface to the on-line reference manuals.
man1 - An interface to the on-line reference manuals.
man2 - An interface to the on-line reference manuals.
pwd - Print name of current directory.
root# pwd
pwd - Print name of current directory.
root# cd
cd - Change directory.
root# ls
ls - List directory contents.
root# man
man - An interface to the on-line reference manuals.
root#
```

