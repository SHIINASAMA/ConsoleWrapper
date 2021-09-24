# Console Wrapper

## 描述

Windows 下的控制台程序包装器

## 配置

在可执行文件同级目录下新建<kbd>**config.ini**</kbd>。

模板如下：

```ini
title=xxx
icon=xxx.ico
cmd=xxx
```

| 字段  | 注释       |
| ----- | ---------- |
| title | 程序的标题 |
| icon  | 程序图标   |
| cmd   | 执行的指令 |

双击<kbd>**ConsoleWrapper.exe**</kbd>自动执行字段“cmd”当中的指令，并将程序窗口隐藏到系统托盘。