## ScreenLocker

software screen locker made for E6 

### Introduction 

![screenlocker](https://raw.githubusercontent.com/jemyzhang/E6ScreenLocker/master/screenshot/preview.png)

### Usage 

- [安装包文件说明]

sl.lin                启动用bash script
screenlock.desktop        桌面文件
Lock.png            程序大图标
Lock_sm.png            程序小图标
screenlocker.cfg        程序配置文件
screenlocker            主程序
skin/skin.cfg            皮肤配置文件
skin/skin.png            皮肤

- [安装方法]

下载pkg安装包后，使用mpkg软件打开/关联pkg，然后按照提示安装。【mpkg下载】

- [操作方法]

菜单中点击相应图标
![lock](https://raw.githubusercontent.com/jemyzhang/E6ScreenLocker/master/screenshot/Lock_sm.png) 
即可打开锁屏软件。
点击屏幕右上方小黑点
![dot](https://raw.githubusercontent.com/jemyzhang/E6ScreenLocker/master/screenshot/Dot.png) 
即可关闭锁屏软件。
点击屏幕上自动锁图标
![control](https://raw.githubusercontent.com/jemyzhang/E6ScreenLocker/master/screenshot/Control.bmp) 
，可以选择打开（彩色）/关闭（灰色）自动锁，同时配置文件记录当前自动锁状态，下次打开软件时，保留上次状态。
解锁/上锁，先按挂机键+然后向右方向键 
- [配 置文件]

`/mmc/mmca1/.system/QTDownLoad/screenlock/screenlocker.cfg`

```
#screenlocker configuration file

#path for background image                                  设定背景图片路径
BackgroundImage = 
#autolock onoff switch                                          自动锁开关
#1:ON,0:OFF
AutoLockONOFF = 1
#autolock period(seconds)                                      自动上锁时间间隔
AutoLockPeriod = 20
#light on timeout while screenlock is on(seconds) 上锁后屏亮时间（秒）
LockLightOnTimeout = 3
#screen brightness while screenlock is on             上锁后屏幕亮度
LockOnBrightness = 10
#if show unlock help instruction                           是否显示解锁提示
#1:show,0:not show
ShowInstruction = 1
#if show autolock icon on screen                         是否显示自动上锁开关图标
ShowAutoLockIcon = 1
#if unlock while sms/alert come                          当短信/闹钟到达时，是否自动解锁
HideWhileSMSCome = 1
#language id                                                        语言，0为英文，其他为中文
#0:English,other:Chinese
LANGUAGE = 1
```

- [自定义皮肤]

    -->\[配置文件/mmc/mmca1/.system/QTDownLoad/screenlock/skin.cfg\]

```
[SKIN CONFIG FOR SCREENLOCKER]                    [Screenlock 配置文件]
#format:                                                              格式：
#name = {width,height,picture-postion-x,picture-postion-y,OSD-position-x,OSD-position-y}

#名称 = {宽度，高度，在skin.png中的横坐标位置，skin.png中的纵坐标位置，显示在屏幕上是横坐标的位置，显示在屏幕上纵坐标的位置}

TimeDigi = {33,35,0,0,50,140}                            时间的位置
TimeColon = {8,35,330,0,116,140}                      小时和分之间冒号的位 置
DateDigi = {11,11,0,35,78,180}                            日期的位置
DateHori = {8,11,110,35,100,180}                        年 月日之间横杠的位置
AMPM = {28,11,338,0,198,165}                            ampm的位置
Weekday = {33,11,0,46,165,180}                          星期的位置
Logo = {40,50,33,46,32,130}                                logo （图标）的位置
Bluetooth = {13,18,97,78,22,175}                        蓝牙图标的位置
CloseClick = {10,10,97,96,225,2}                        关 闭软件的位置
AutoLock = {16,16,97,46,110,300}                      自动锁开关的位置
MissCall = {24,14,73,46,32,180}                          未接电话的位置
MissMail = {24,28,73,62,21,132}                            未接短信的位置
LockIcon = {24,20,73,90,110,243}                        锁的位置
```

    -->[皮肤图像文件/mmc/mmca1/.system/QTDownLoad/screenlock/skin.png]

注意：自己修改或者制作皮肤的时候，请使用png格式以保证透明的效果

![default skin](https://raw.githubusercontent.com/jemyzhang/E6ScreenLocker/master/screenshot/skin.png) 

 
    -->[皮肤配置文件和图像的对应关系说明]

![instructions](https://raw.githubusercontent.com/jemyzhang/E6ScreenLocker/master/screenshot/instruction.png) 


### ChangeLog

Ver1.0.Build 00:49:29, Jul 24 2007

修复自动锁开启时，接听/拨打电话屏幕背景灯常亮的bug

Ver1.0.Build 19:07:40, Jul 16 2007

1.当锁屏两倍autolock时间后，系统进入休眠状态，此时点击屏幕，会亮屏显示时间（由于LCDSleepTime的问题，进入休眠状态是，屏幕会不能锁定，只能如此尽量美化）
2.更新整理部分程序，主要为显示部分

Ver1.0 Build 14:49:57, Jul 13 2007

关闭PM_setupLcdSleepTime，测试是否因为LcdSleepTime设置成不休眠导致浪费电力(测试成功)

*版本存在的问题：由于lcdsleeptime没有被关闭，所以当pmd自动黑屏后，屏幕锁定会被解除，虽然已经判断QCopMessage，但是触摸屏幕会出现短暂的亮屏现象，一闪而过

Ver1.0 Build 17:53:16, Jul 12 2007

1.锁屏超时（两倍自动锁时间）后，永久睡眠，等待事件/键盘激活，测试是否节电

2.修正misscall图标位置问题

*当超时setitimer设置为0，不发送alarm信号

*将软件置于内存中，同时在SysRegistry的Autorun中添加自启动，完美自动运行

Ver1.0 Build 10:25:20, Jul 11 2007

1.更改版本命名方式
2.修正自定义皮肤的一些bug
3.屏蔽debug输出信息
4.准备编写使用方法
注意：如果已经修改过皮肤设置，并且想保留皮肤的话，请解压缩安装包，将其中的screenlocker直接覆盖到安装目录 （/mmc/mmca1/.system/QTDownLoad/screenlock/）

20070710-02.6

自定义皮肤功能
皮肤在安装目录的skin/skin.cfg和skin.png两个文件 

20070707-01.6

去除来电状态判断，测试是否省电

20070706.01.3

修正自动亮屏后屏幕不黑的问题

20070705-01.8

+配置文件增加Language,是否显示图像，当短消息来，是否解锁
*判断屏幕亮暗的方式改变 

20070704-01.2

+增加短信/闹铃等亮屏解锁(beta)

20070702-01.14

+对有限错误的处理
*时间显示自动更新，避免时间显示错误 

20070701-01.2

修正通话过程中屏幕灯不关闭的bug

20070630-01.6

1.修正ampm的bug
2.更改程序结构，使用信号机制，不再使用多线程，节省系统资源

20070629-01.12

1.改进界面显示不完整的bug。。。有时候还是会出现，不过次数较以往少很多
2.界面下方新增一个小按钮，点击后可以切换彩色/黑白，彩色表示autolock打开，黑白表示autolock关闭
3.新增配置文件screenlocker.cfg
4.修复退出屏锁后，背光会比自己设置的更亮的bug

20070627-01.20

1.修正界面时有显示不完整的bug
2.修正使用硬键锁后，触摸屏会发生无法锁定的现象（触摸后屏幕点亮）
3.锁屏时降低屏幕亮度以省电（退出/解锁后屏幕亮度回复到25，也就是50）

20070627-01.16

1.增加 小黑点 在屏幕右上方，点击后可以退出锁屏软件
2.将锁机/解锁键改成 挂机+向右，避免不必要的误操作
3.修正自动锁屏有时候不起作用的bug
4.修正程序正常关闭时，屏幕灯常亮的bug
5.重写部分代码，精简代码长度，以减少cpu利用率

20070626-01.4

1. 屏幕锁定的同时关闭键盘灯
2. 修正接听电话是屏幕会锁定的问题
3. 接通电话之后再挂断，屏幕不自动锁定，而是等待没有操作的自动锁屏（表达起来比较困难，希望能理解），如果拒绝接听或对方挂断，则立即锁屏

很失望，没有找到新消息到达通知，以及闹铃通知，正在研究中。。。所以短信到达和闹钟是屏幕不会亮，希望大家能理解

20070625-02-16

1.增加超时自动锁屏功能，不需要再等系统黑屏啦~~
超时设置方法：
在sl.lin中更改./screenlocker &一行，改为./screenlocker n &
n表示超时秒数，比如说60，那么就是60s后自动锁屏，0为关闭自动锁屏，不设置任何值，则默认为30秒自动锁屏
2.修正屏幕没有完全锁定的bug，现在包括背光和图像全部不显示
注：经测试，某些情况下，当刚开启锁屏软件时，会出现触摸屏幕背光亮的情况，但是只会出现一次，接下来就不再会了
3.修改部分程序结构，更加节省cpu和内存资源

20070625-01.1

1.将解锁、锁定键统一为 挂机键，然后确定键（中 间圆形）
2.在屏亮，锁机状态下，按屏幕右上角可以退出（关闭）锁屏软件，方便更新

20070624-04.6

重新打开自动锁屏功能：当屏幕黑掉以后自动锁屏

20070624-04.5

增加背景图片功能
修正来电屏幕锁定的bug
修正一些内存泄露的bug(继续测试中)


[关于背景图片]
由于现在没有设置界面，为了方便，将背景图片统一更名为screenlock.gb
背景图片支持 gif,jpeg,bmp,png等E6支持的图片格式
设置背景图片的方法：

将喜欢的图片改成screenlock.bg，放入/mmc/mmca1/.system/QTDownLoad/screenlock目录中，然后重新 再开一次screenlocker即可，注意后缀以及大小写 

20070623-02

有信来电时，屏幕锁自动关闭，屏亮
未接通或者挂机后，屏幕锁自动锁上（如果接电话前是锁定状态）

20070623-01.2

1.增加未接来电，未读短消息提示（由于还没有找到区分未接来电还是未读短消息的方法，两者暂时合一）
2.如果蓝牙打开，则在屏幕上显示蓝牙图标


20070622-014

1.彻底关闭自动锁屏。。。等待完美方法后再开启
2.实现屏幕锁定，背光控制，当锁屏启动后，3秒屏幕背光关闭，按键背光开启

下一步目标：

待机屏幕上显示 未读短信，以及未接电话提示，一目了然
注意：如若出现按键屏幕背光不开启的情况，使用硬锁键，打开关闭后即可恢复
造成这种现象的原因，背光关闭状态下，可能screenlocker崩溃退出了


20070622-013：

1.重写部分代码，提高运行效率
2.关闭10秒自动锁屏。现在当屏幕暗掉时自动锁屏

6.22：

1.运行更加稳定，使用内存更少
2.新增pkg安装包，安装后即可点击图标使用 

 

仅供测试用途，代码尚在完善中

按键：先按绿色的拨号键，然后按中间键解锁；侧面的快进（上），然后快退（下）锁屏
自动锁屏时间，10秒

注意事项，如果是从telnet运行次程序，时间显示为标准UTS时间，从手机运行是正确的当地时间

使用方法，用linloader打开sl.lin即可 

