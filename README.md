# uboot-shell

> ���Ǵ�ubootԴ������ȡ������shell�����н�����Դ�룬�������˼ӹ����޸ģ����Կ�ƽ̨��ֲ��ʹ��Ҳ�ܼ򵥡�����ֻ�򵥽���Դ����ɣ��޹�ϸ�ڣ����鿴Դ�롣��Դ��������¼��������ݣ�

## main.c��
>  һ���򵥵�ʾ�����롣
## type.h��
>  �����������͵��ض��塣
## list.h��
>  ��ȡ��linux�ں˵�˫�������Ѿ����Ż����������κε��������롣���ڴ洢�Ͳ������
## readline.c/readline.h��
>  ��ubootԴ������ȡ��һ��readlineʵ�֣�����������GNU Readline�⣬�����򵥡�����������ʷ��¼���ܺ�tab��ȫ���ܺ�����һЩ��ݼ����������ȫ������Ҫ�Լ�ʵ�֡��ò������޸Ĳ��Ż��������������⣬�ɵ���ʹ�ã�ֻ����Щ��ƽ̨��صĺ�����Ҫ�Լ������Լ���ƽ̨�޸ġ�
## shell_cmd.c/shell_cmd.h��
>  ��ubootԴ������ȡ�������н��������룬���ṩ��һ��������ϵͳ��ע������ĺꡣ�ò������޸Ĳ��Ż��������������⣬�ɵ���ʹ�ã�ֻ����Щ��ƽ̨��صĺ�����Ҫ�Լ������Լ���ƽ̨�޸ġ�Ĭ���Դ�һ��help���
## shell_config.h��
>  ����readline��������н������������ļ���



## ʾ��

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

