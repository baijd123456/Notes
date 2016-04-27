OS X源于BSD系列，在使用方式上与传统的Linux发行版有较大的不同。

========


OS X下的一些常用功能 >>

常用快捷键 >
锁屏: Control + Shift + Power
全屏截图: Shift + Command + 3
指定位置截图: Shift + Command + 4
指定窗口截图: Shift + Command + 4 然后将光标放在需要截图的窗口上，按空格键确认
保存: Command + S
全选: Command + A
行首/行尾: Control + A/E
Home/End/PageDown/PageUp: Fn + Left/Right/Down/Up
复制/粘贴文件: Command + C/V
剪切文件: Option + Command + V
切换程序: Command + Tab
切换当前程序的窗口: Command + `
退出程序: Command + Q
关闭当前窗口: Command + W
最小化窗口: Command + M
立即息屏进入睡眠: Command + Alt + [电源键]

----

常用命令行指令 >
# scutil --set HostName [主机名]				//设置主机名称
# scutil --set ComputerName [主机共享名称]	//设置主机共享名称
$ defaults write com.apple.finder AppleShowAllFiles YES/NO			//显示/取消显示隐藏文件，需要重新登陆账户才会生效
$ defaults write com.apple.notificationcenterui bannerTime [数值]	//控制通知中心点的通知停留时间，单位为秒
$ defaults delete com.apple.notificationcenterui bannerTime			//恢复默认的通知停留时间(一直显示)

----

常用软件 >
The Unarchiver								//解压软件
VLC											//多媒体播放器
AppCleaner									//APP卸载工具
MacDown										//所见即所得的Markdown编辑工具
Xcode										//OS X下的集成开发环境
iTerm2										//OS X下的终端模拟器，比自带的功能更强大

----

一些特殊目录 >
~/Library/Fonts								//用户字体目录
~/Library/Caches							//缓存目录
~/Library/Application Support				//用户程序配置目录
~/Library/Preferences						//用户程序配置目录
~/Library/Saved Application State			//用户程序状态目录
~/Library/VirtualBox						//VirtualBox的配置文件目录，删除则VirtualBox恢复初始状态，需要重新添加虚拟机
/Library/Application Support				//系统程序配置目录
/System/Library/Fonts						//系统默认字体目录
/Library/Fonts								//系统可选字体目录

========


与常规PC的不同之处 >>
Mac机与常规的PC有较大的差异，需要一个适应过程。

----

基本操作差异 >
1.缺少一些常用按键，且按键逻辑不统一。
没有Home/End/PageDown/PageUp等按键，需要通过Fn搭配方向键等组合键才能实现行首/行尾/翻页等操作。
同时，在部分编辑器以及终端中，行首/行尾操作需要通过Control加A/E等按键完成。
在部分开发环境中，行首行尾的快捷键不使用系统方案，而是有程序自身定义，比如在Eclipse中，行首/行尾为"Command + 方向键左/右"。
2.Finder少一些常用的功能。
Finder右键菜单中没有创建文件的选项，甚至创建文本都需要使用Terminal中的touch指令。
Finder右键菜单没有剪切功能，通过组合键能实现类似效果。
3.Windows/Linux中以Control作为组合键触发的一些快捷操作在OS X中全部使用Command键进行触发。

----

Drawin与GNU/Linux的差异 >
1.基础UID与Linux中不同，Linux中普通用户UID为1000开始，OS X中UID从500开始。
2.家目录与Linux/BSD中不同，OS X中的/home目录默认是空的，用户家目录的位置为/Users。
3.root用户目录与Linux中不同，位于/var/root。
4.Darwin没有默认的包管理器。
5.环境变量记录在文件/etc/paths中。

----

文件系统 >
在OS X中，默认文件系统为HFS+，此类文件系统同时支持区分大小写(Case-Sensitive)和忽略大小写两种类型，在格式化时可以进行选择。
需要注意的是，如果选择了区分大小写形式的HFS+文件系统，则部分软件将无法安装(比如PhotoShop等)。
文件系统类型在安装了OS X之后除了重装系统之外无法更改，需要慎重选择。

========


包管理 >>
OS X中当下最流行、最主流的包管理器为Homebrew。另外，MacPorts也是不错的选择。

----

Homebrew与MacPorts的区别 >
Homebrew基于Git，是比较轻量级的一个包管理器。
与传统的包管理器不同，使用Homebrew并不需要使用root用户，只需要一个普通的OS X管理员用户即可。
Homebrew将软件包安装在/usr/local/目录下，在OS X中该目录默认情况下为空，因此当用户不再需要使用Homebrew时，只需完整删除/usr/local/目录下的所有内容即可(需要注意的是，有些非Bundle形式安装的软件也是会将一些内容安装在/usr/local目录下的，比如VirtualBox。如果安装了此类软件，清理/usr/local目录时需要仔细辨别)。
默认情况下，在OS X中，/usr/local的所有者为root，用户组为wheel，而安装了Homebrew之后，该目录所有者会被更改为当前用户，用户组被改为admin。

----

安装Homebrew >
Homebrew采用Ruby语言开发，OS X中已经带有ruby开发环境。
Homebrew需要用到Git等工具，在安装Homebrew之前需要先安装Xcode命令行工具(或者直接完整安装Xcode)。
之后在Terminal中执行：
$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

----

Homebrew基本指令 >
$ brew update								//更新源
$ brew ugrade								//升级包
$ brew install [package_name]				//安装包
$ brew leaves								//查看没有其它包被依赖的包
$ brew deps [package_name]					//显示指定包的依赖

----

安装OS X缺失的一些命令行工具和开发环境 >
$ brew install aria2 wget python3 gcc screenfetch nmap scala p7zip mariadb boost sbcl go mono gdb
需要注意的是，通过Homebrew安装的包文件全部保存在/usr/local/Cellar目录下。
与Linux下的常规包管理器不同，Homebrew在安装包和删除包的时候，不会有多余的确认提示，输入指令就会直接执行。
对于带有头文件的包(如boost、gcc等)，会在/usr/local/include目录下创建符号链接，指向/usr/local/Cellar目录中的具体包内容。

----

使用通过HomeBrew安装的mysql/mariadb >
通过HomeBrew安装的mysql/mariadb使用时不需要root权限。
mariadb与mysql数据库的操作指令相同，因此mariadb与mysql软件包只能选择一个进行安装。
mariadb与mysql数据库存储位置相同，路径为/usr/local/var/mysql/。
启动服务使用指令：
$ mysql.server start
停止mysql服务：
$ mysql.server stop

----

安装Qt >
与常规的Unix程序不同，Qt不推荐使用Homebrew安装，因为通过Homebrew安装的Qt并没有像其他Unix软件包一样创建符号连接到/usr/local/bin目录下，同时也不带有QtCreator程序。
从Qt官网下载Qt的dmg安装镜像，按步骤安装到自定义路径之后，创建符号链接到~/Application目录下，即可在Launchpad中看到Qt的相关应用。

----

删除JDK >
JDK需要自行手工删除，JDK的相关文件位于以下目录：
/Library/Internet Plug-Ins/JavaAppletPlugin.plugin
/System/Library/Java/Support/CoreDeploy.bundle/Contents/JavaAppletPlugin.plugin
/Library/Java/JavaVirtualMachines/＊
删除JDK时需要移除这些目录。
当安装了新版本的JDK时，旧版本的JDK不会自动卸载，其包文件依然位于/Library/Java/JavaVirtualMachines路径下，文件夹名称即为对应的JDK版本，手动删除不需要的版本即可。

========


OS X下的软件管理 >>
在OS X中，软件包常以dmg格式的镜像封存，而安装软件的方式分为Bundle以及pkg形式。

----

Bundle >
在OS X中最常见的软件包是以Bundle的形式存在的，Bundle饰一个以 ".app" 为后缀的目录，外观为可执行程序的图标，封装了程序执行所需的一些必要资源以及真正的可执行文件。
dmg镜像中若直接包含Bundle软件，则将其复制到/Application或是~/Application目录下即可(推荐存放在用户的Application目录下)。
常规的Bundle程序所有者为当前用户。

----

pkg >
一些大型的软件包如Adobe系列、Office系列等的dmg镜像中保存的是pkg格式的安装包，双击进行安装，步骤与Windows系统下的安装流程类似。
通过pkg安装的软件最终也会在/Application目录下创建软件的Bundle，但通过此中方式安装的软件往往会在系统的其它目录创建程序所需的其它文件。
通过pkg安装的软件所有者一般为root，不能随意移动到~/Application/路径下。

----

软件存放路径 >
默认情况下，Bundle形式的软件一般存在于/Application/目录或是~/Application/目录下。
OS X的系统默认Bundle应用存放在/Application/目录下，一些pkg形式或事通过AppStore安装的应用也在该目录下。
默认情况下~/Application目录为空。
用户自行安装的Bundle应用推荐存放在~/Application目录下，避免与系统程序混淆。

========


一些问题的处理方案 >>

切换分辨率/语言时，OS X下的登陆界面的分辨率/语言依然不变 >
可以尝试更改登录界面的选项。
也可以尝试以下指令:
# languagesetup
登陆界面的分辨率/语言未发生变化是由于登陆界面的数据未更新，使用root权限执行languagesetup重设语言即会刷新登陆界面信息。

----

更改默认应用程序 >
使用 Command + i 查看一个文件的详细信息，在"Open With:"条目中可以选择打开此文件使用的默认程序，修改为需要的程序之后，选择 Change All... 即可将所有此类文件全部修改为自定义的程序。

----

在BootCamp安装的Windows系统中调整了分区，重启后Mac分区在启动页中消失 >
发生此种情况的原因是Windows下的一些分区管理工具将Mac分区的分区Type UUID改成了Windows的Type UUID，只需将分区类型ID改回来即可恢复Mac分区。
具体解决方法：
1.使用Windows的diskpart分区工具更改Type UUID，在CMD下执行以下指令：
> diskpart													#进入diskpart分区工具
> list disk													#列出所有磁盘
> select disk [磁盘号]										#指定正在使用的磁盘
> list partition											#列出所选磁盘中的所有分区
> select partition [分区号]									#指明OS X所在的分区号
> set id=48465300-0000-11AA-AA11-00306543ECAC				#设置分区的Type UUID
2.也可以使用Linux/Unix中的parted工具进行分区类型ID变更，推荐使用parted的图形化前端gparted，只需要在分区标志中去掉msdata即可使Mac分区正常启动。

----

使用默认bootloader引导Linux系统 >
需要创建一个100MB左右的分区(其实可以更小)，在分区的中创建System/Library/CoreServices目录：
$ mkdir -p /Volumes/[启动分区名称]/System/Library/CoreServices/
并在该目录中放入Linux的efi启动文件。
同时创建系统描述文件：
$ nano /Volumes/[启动分区名称]/System/Library/CoreServices/SystemVersion.plist
在描述文件中添加以下内容：
<?xml version="1.0" encoding="utf-8"?>
<plist version="1.0">
<dict>
	<key>ProductBuildVersion</key>
	<string></string>
	<key>ProductName</key>
	<string>Linux</string>
	<key>ProductVersion</key>
	<string>Arch Linux</string>
</dict>
</plist>
然后使用OS X系统的启动管理器bless来创建启动项，执行指令：
# bless --folder=/Volumes/[启动分区名称]/System/Library/CoreServices/ --file=/Volumes/[启动分区名称]/System/Library/CoreServices/boot.efi --setBoot

----

重置Launchpad中的图标 >
Launchpad中数据保存在~/Library/Application Support/Dock/路径下，如果Launchpad中图标出现异常(比如已删除的软件图标依然存在)，可以尝试清空Launchpad中的数据。
删除该目录之后，Launchpad会在下次开机之后重置图标布局，恢复成默认的样式(Apple自带的软件占一页，用户自行安装的软件从第二页开始)。

----

设置Xcode路径 >
Xcode中包含了一系列的命令行工具如clang、git等，Homebrew的安装也依赖于这些命令行工具。
默认情况下，安装Xcode的同时会自动设置Xcode的对应路径。
但如果用户改变了Xcode.app的位置，则需要重新设定Xcode的路径，否则会出现找不到命令行工具的情况。
使用xcode-select可以设定Xcode的安装位置：
# xcode-select --switch [Your Xcode Path]/Xcode.app/Contents/Developer
需要注意的是，如果该变了Xcode.app的位置，即使使用了xcode-select重新设定了Xocde.app的路径，一些通过Homebrew安装的编译器(gcc)也依然会出现找不到头文件的情况，此时需要重新编译安装编译器的包。

----

使用gdb调试器 >
在新版的OS X系统中，clang作为默认编译器取代了gcc，lldb作为默认编译器取代了gdb。
默认情况下，使用Homebrew安装的gdb调试器是不能在普通用户下正常调试代码的，需要对其进行签名后才能使其正常调试代码。
使用KeyChain Access.app创建一个证书(certificate)，证书的Certificate Type要设为Code Signing， KeyChain要设为System，Trust列表中的CodeSigning属性要设置为Always Trust。
成功创建了证书之后，使用codesign命令对gdb进行签名：
$ codesign -s [证书名称] [gdb安装路径]
证书需要在系统重启之后才会生效。
需要注意的是，每次gdb包升级，都需要重新使用证书对其进行签名，否则不能正常调试代码。