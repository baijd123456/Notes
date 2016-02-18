[TOC]

##时间API
Unix环境下的时间获取相关函数定义在`/usr/include/time.h`头文件之中。

###获取当前的系统时间
使用`time()`函数获取当前系统时间。

```c
time_t time(time_t *t);
```

`t`参数为`time_t`类型变量的地址，函数会返回当前系统的时间，同时将当前系统的时间写入传入的地址中。
如果只是从返回值获取当前时间，不需要将时间写入传入参数中，则参数可以填`NULL`，函数正常运行。
需要注意的是`time_t`保存的是`1970-01-01 00:00:00 +0000 (UTC)`开始到**当前时间**的秒数的数值，一般不直接使用。

###将时间转化为可读格式
使用`localtime()`函数输出时间结构体`tm`类型。

```c
struct tm *localtime(const time_t *timep);
```

函数接受参数为`time()`返回的标准时间秒数，将其转换返回对应的可读时间结构体指针`tm*`类型。
结构体`tm`的定义为：

```c
struct tm {
	int tm_sec;    /* Seconds (0-60) */
	int tm_min;    /* Minutes (0-59) */
	int tm_hour;   /* Hours (0-23) */
	int tm_mday;   /* Day of the month (1-31) */
	int tm_mon;    /* Month (0-11) */
	int tm_year;   /* Year - 1900 */
	int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
	int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
	int tm_isdst;  /* Daylight saving time */
};
```

可以从`tm`结构体的对应成员中读取对应的时间信息。

###将时间转化为标准字符串表示

```c
char *asctime(const struct tm *tm);
```

对于`localtime()`函数获得的当前时间，可以使用该函数转化为可读的字符串形式，返回值为标准时间字符串的地址。
需要注意的是，该函数返回的地址指向的字符串内容中已经包含了换行符，不需要再额外添加。
一个典型的输出如：`Wed Jul 29 01:04:10 2015`
实例代码：

```c
#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t now = time(NULL);
	struct tm *tm_now = localtime(&now);
	printf("当前的时间是：%d点%d分%d秒。\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	printf("标准的时间输出：%s", asctime(tm_now));
	return 0;
}
```

运行结果：
当前的时间是：1点15分39秒。
标准的时间输出：Wed Jul 29 01:15:39 2015



##文件与基本IO
Unix环境下基本的文件操作为`open()``read()``write()``close()``ioctl()`。
相关的头文件在`/usr/include/unistd.h``/usr/include/fcntl.h``/usr/include/sys/ioctl.h`之中。

###创建/打开一个文件
创建/打开文件涉及的系统调用定义在文件`/usr/include/fcntl.h`中。

```c
int creat(const char *pathname, mode_t mode);
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
int openat(int dirfd, const char *pathname, int flags);
int openat(int dirfd, const char *pathname, int flags, mode_t mode);
```

以上函数在执行成功时返回新的文件描述符，失败时返回**-1**并置**errno**。

- `crate()`函数用于创建文件，`open()`函数既可用于创建文件(**flags**取`O_CREAT`)，也可用于打开文件，打开的对象也可以是**目录**。
- 对于`create()`和`open()`函数，参数`pathname`代表文件所在路径绝对地址的字符数组首地址，参数`mode`代表创建的文件文件带有的默认权限，可以用逻辑或操作符连接以下参数：`S_IRUSR``S_IWUSR``S_IXUSR``S_IRGRP``S_IWGRP``S_IXGRP``S_IROTH``S_IWOTH``S_IXOTH`，分别代表**拥有者**、**同组用户**、其他用户的**读**、**写**、**执行**权限。
- `open()`函数的`flags`参数表示文件打开时的参数，参数可取**多个**，以**逻辑或**操作符连接，常用的有`O_RDONLY``O_WRONLY``O_RDWR`，分别表示以**只读**、**只写**、**读写**的方式打开文件，默认每个文件描述符首次写操作会清除已有的数据，使用`O_APPEND`标志以追加的方式写入数据；`flags`参数取`O_CREAT | O_EXECL`时创建一个原先**不存在**的文件，如果需要被创建的文件已经存在了，则创建**失败**。
- `openat()`函数作用与`open()`函数完全相同，但是`openat()`函数允许使用多种路径表示方式；`dirfd`文件描述符表示的路径为父目录，而`pathname`中包含的字符串为相对路径；或是`dirfd`取特殊值`AT_FDCWD`，则父目录为**当前路径**；也可以像`open()`函数一样在`pathname`中写入**绝对路径**，但此时`dirfd`的取值会被**忽略**。

###读取文件中的内容
使用`read()`函数读取文件，函数定义在`/usr/include/unistd.h`中。

```c
ssize_t read(int fd, void *buf, size_t count);
```

- `fd`参数为要读取的文件描述符。
- `buf`参数为读取的内容的首地址。
- `count`参数为设定读取的字节数。

返回值为实际读取了的字节数。
失败时返回`-1`。

###向文件中写入内容
使用`write()`向文件中写入数据。

```c
ssize_t write(int fd, const void *buf, size_t count);
```

- `fd`参数为要写入的文件描述符。
- `buf`参数为要写入内容的首地址。
- `count`参数指定写入数据的长度。

`buf`指向的内容中超过`count`长度的内容**不会**被写入。
返回值为**实际写入内容的大小**。

对于同一个文件描述符，连续进行读写操作，每一次函数调用都会在上一次结束的位置进行，因此想要重复读取某个文件的内容，需要创建新的文件描述符。
同一个文件可以同时拥有多个文件描述符，且各个文件描述符之间的文件读取是相互独立的。

###标准输入/输出
在Unix哲学中，秉承**一切皆文件**思想，因而，在终端中进行输入/输出与读写文件操作类似，使用`read()/write()`调用即可。
**标准输入**对应的文件描述符为`0`，**标准输出**对应的文件描述符为`1`，**标准错误输出**对应的文件描述符为`2`，使用`read()/write()`调用对这些特殊的文件描述符进行读写操作即可实现**终端输入/输出**的效果。



##Unix进程控制(fork)
进程相关的几个函数定义在头文件`/usr/include/unistd.h`中：

```c
pid_t fork(void);								//创建当前进程的一个拷贝
pid_t getpid(void);								//得到当前进程的pid
pid_t getppid(void); 							//得到当前进程的ppid
unsigned int sleep(unsigned in seconds);		//让当前进程睡眠seconds秒
void _exit(int status)							//终止程序，立即进入内核(不会调用atexit注册的清理函数)，参数status为程序退出码，一般可以设为0，有特殊用途时可以设为其它数值
```

###标准库进程函数
标准库中的进程相关函数定义在`/usr/include/stdlib.h`中：

```c
int system(const char *string);					//创建进程
void exit(int status);							//终止程序，属于C标准库，先执行一些清理处理(调用atexit注册的清理函数，并刷新输出流，关闭已打开的流，删除通过标注I/O函数tmpfile创建的临时文件)，再返回内核
void _Exit(int status);							//终止程序，属于C标准库，等价于系统调用中的_exit，立即进入内核
int atexit(void (*function)(void));				//用该函数注册终止处理程序function，在exit时将自动调用，成功时返回0，失败返回非0
```

return语句向函数**提供返回值**，只有main函数中return才会结束进程，而exit函数在一个进程的**任意函数**中调用都将**终止当前进程**。
`system()`函数的作用是，运行以字符串参数的形式传递给它的命令并等待该命令的完成(效果类似于在shell中使用对应命令)。
与exec()系统调用不同，system()函数会新建一个进程来执行命令。
如果无法启动shell来运行这个命令，system()函数将返回错误代码127；其它错误返回-1，否则system()函数将返回该命令的退出码(一般命令都是0)。
需要注意的是，在实际的Linux开发中，system()函数往往是很少被使用的，因为使用system()函数必须启动一个shell来执行需要的指令，使得system()函数的效率并不高。

### *fork()* 调用
在Unix环境下，`fork()`系统调用是最常见的创建进程方式，函数定义在`/usr/include/unistd.h`中，函数原型为：

```c
pid_t fork(void);
```

`fork()`函数的作用是，为当前进程创建一个相同的**拷贝**，原进程为**父进程**，新进程为**子进程**，原进程的`fork()`函数返回子进程的`pid`，新进程的`fork()`函数返回`0`。新进程与原进程有着相同的**运行状态**和**代码**，即从`fork()`函数开始(包括`fork()`函数本身)接下来的代码原进程和新进程将会各执行一遍，但是，新的进程有**独立**的数据空间、环境、和文件描述符(父进程中已经打开的文件描述符在子进程中依然会存在)，子进程不继承父进程的文件锁，父进程中未处理的信号集在子进程中被置为空集。
多进程并行执行时，各个进程是**异步乱序**执行的，因此你并不能确定各个进程各段代码的执行先后顺序，所以不要尝试编写依赖于其它进程执行结果的代码。

实例代码：

```c
#include <stdlib.h>		//包含system()系统调用
#include <stdio.h>
#include <unistd.h>		//包含fork()、sleep()系统调用

int main(void)
{
	if (!system("whoami"))		//system()函数只执行了一次，可知在fork()函数拷贝的是fork()运行此刻运行状态，但fork()函数之前的内容依然只执行一次
		printf("Run the system call successful!\n");
	pid_t num = fork();
	if (num)	//fork()系统调用对于父进程返回子进程的pid，对于子进程返回0，fork()有可能执行失败，失败返回-1，并且不创建子进程
	{
		printf("This is parent process!\n");
		printf("The PID is %d\n", getpid());
		printf("The PPID is %d\n", getppid());
		sleep(2);		//程序运行到此暂停2秒
		system("ps");
	}
	else
	{
		printf("This is child process!\n");
		printf("The PID is %d\n", getpid());
		printf("Ths PPID is %d\n", getppid());
	}
	printf("End!\n");	//fork()之后的内容父进程子进程各执行一次
	return 0;
}
```

运行结果：
dainslef
Run the system call successful!
This is parent process!
The PID is 13722
The PPID is 10480
This is child process!
The PID is 13724
Ths PPID is 13722
End!
PID TTY          TIME CMD
10480 pts/0    00:00:00 zsh
13722 pts/0    00:00:00 a.out
13724 pts/0    00:00:00 a.out <defunct>
13725 pts/0    00:00:00 ps
End!

由结果可知，`fork()`函数之前的`system("whoami")`函数只执行了一遍，因此shell指令`whoami`也只执行一遍。但在`fork()`函数之后的代码都执行了两遍，分别来自父进程和子进程的`printf()`函数向屏幕打印了两次`End!`。
由`system("ps")`函数中执行的shell指令`ps`向屏幕中输出的结果可以看出，父进程的`ppid`是启动这个进程的shell的`pid`，而**子进程**的`ppid`就是**父进程**的`pid`。

### *exec()* 系统调用
`exec()`系统调用，由一系列的相关函数组成，函数定义在`/usr/include/unistd.h`中，函数原型为：

```c
extern char **environ;
int execl(const char *path, const char *arg0, ... /*, (char *)0 */);
int execle(const char *path, const char *arg0, ... /*, (char *)0, char *const envp[]*/);
int execlp(const char *file, const char *arg0, ... /*, (char *)0 */);
int execv(const char *path, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
int execvp(const char *file, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp[]);
```

`exec()`函数为**系统调用**，执行后，会将当前的进程**完全替换**为执行新程序的进程(即这个进程`exec()`调用成功之后的代码都不再运行)，但`PID`不变，`exec()`系统调用比`system()`函数要**高效**，`exec()`与`fork()`搭配是Unix系统中最**常用**的系统进程创建组合。
一般情况下，exec()系统调用是不会返回的，除非发生了错误。出现错误时，exec()会返回-1，并且设置错误变量errno，同时继续执行余下的代码。
在exec()函数组中，只有execve()函数是真正的系统调用，其它的几个函数都是execve()封装而成的库函数。
参数中的path代表绝对路径，file代表命令名称。
这些函数主要可以归为两类，其中execl、execlp、execle三个函数接收的参数个数是可变的，参数以一个空指针结束((chasqlserver 导出 mysqlr*)0或是NULL)，用多个字符数组*arg来传递要执行的程序的参数，而execv、execp、execve等函数参数个数是固定的，将要传递给要执行的程序的参数放在二维字符数组*argv[]中(对应main函数参数中的*argv[])，而二维字符数组*envp[]中保存exec()函数要运行的程序的环境变量无论是传递给被执行程序的参数字符数组*argv[]或是环境变量字符数组*envp[]都要以一个空指针结尾。

实例代码：

```c
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	char *envp[] = { "LC_ALL=zh_CN.UTF-8", NULL };
	char *argv[] = { "ls", "-l", NULL };		//不要想当然地认为*argv[]的第一个参数是没用的，第一个参数不能为NULL，否则exec()系统调用执行失败，而且exec()执行新指令时如果指令参数不正确时指令在终端上显示的错误信息会将argv[0]作为输入的程序名！
	printf("The PID is %d\n", getpid());
	execve("/usr/bin/ls", argv, envp);		//execve()系统调用的envp如果不需要设置可以填NULL，与system()函数不同，用exec系统调用在执行程序时，如果参数中的envp为NULL，则程序就在无环境变量的状态运行，即系统当前的环境变量不会对exec()系统调用产生影响，但会对依赖shell的system()函数产生影响。在这段代码中，如果用户为中文环境且exec()系统调用没有设置环境变量则ls命令显示的中文目录会为问号，但system()函数执行ls命令则能正常显示。
	/*
		上一句代码等价于
		execle("/usr/bin/ls", "ls", "-l", NULL, envp);
		如果运行一些不需要环境变量的程序，可以有更简单的方式，比如：
		execlp("ps", "ps", "-l", NULL);
	*/
	return 0;
}
```

###等待进程
可以在父进程中调用`wait()`函数让父进程等待子进程结束，还可以使用`waitpid()`函数来等待某个**特定进程**结束，函数定义在 `/usr/include/sys/wait.h`中，函数原型为：

```c
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

`wait()/waitpid()`函数都将状态信息写到`status`所指向的内容。`wait()`函数返回子进程的`pid`，`waitpid()`运行成功返回等待进程的`pid`，失败返回`-1`并置`errno`。`waitpid()`函数第一个参数为要等待的进程的`pid`，如果该参数为`-1`，则返回任一子进程的信息，第三个参数`options`可用来改变函数的行为。
`/sys/wait.h`头文件中定义了宏来解释状态信息：

```c
WIFEXITED(status);		//returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
WEXITSTATUS(status);	//returns  the  exit  status  of  the  child.  This consists of the least significant 8 bits of the status argument that the child specified in a call to exit(3) or _exit(2) or as the argument for a return statement in main().  This macro should be employed only if WIFEXITED returned true.
WIFSIGNALED(status);	//returns true if the child process was terminated by a signal.
WTERMSIG(status);		//returns the number of the signal that caused the child process to terminate.  This macro should be employed only if WIFSIGNALED returned true.
```

...还有很多相关宏在手册中。

实例代码：

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	printf("The PID is %d\n\n", getpid());
	pid_t pid = fork();
	if (pid)
	{
		int status;
		int child_pid = waitpid(pid, &status, 0);	//等效于 int child_pid = wait(&status);
		printf("This is the parent process!\n");
		printf("The PID is %d\n", getpid());
		if (pid == child_pid)
			printf("The child's PID is %d\n", child_pid);
		if (WIFEXITED(status))		//如果等待进程正常结束，WIFEXITED宏返回非0值
			printf("The child process's exit_code is %d\nParent process END!\n", WEXITSTATUS(status));		//如果WIFEXITED非0，返回等待进程的退出码
		_exit(0);
	}
	if (!pid)
	{
		printf("This is the child process!\n");
		sleep(3);
		printf("The PID is %d\nChild process END!\n\n", getpid());
		_exit(100);
	}
}
```

运行结果：
The PID is 9411

This is the child process!
The PID is 9412
Child process END!

This is the parent process!
The PID is 9411
The child's PID is 9412
The child process's exit_code is 100
Parent process END!



##信号(signal)
信号是Unix系统响应某些条件而产生的的一个事件，进程接收到信号会采取一些相应的行动。
信号的相关函数定义在头文件`/usr/include/signal.h`中。
常用的信号有`SIGINT`(中断程序，用`CTRL + C`触发)，`SIGQUIT`(退出，用`CTRL + \`触发)。

### *signal()* 函数
可以用`signal()`函数处理信号，函数原型为：

```c
void* (*signal(int sig, void (*func)(int)))(int);
```

- `sig`参数为要处理的信号。
- `func()`参数为要绑定的信号处理函数。

`signal()`函数是传统Unix的信号处理方式。
对于信号处理函数`func(int)`，有两个特殊的值`SIG_INT`和`SIG_DFL`，将其绑定到一个信号上则表示**忽略信号**或是将信号的处理方式恢复为**默认动作**。

### *sigaction()* 函数
现代Unix中，使用更加健壮的信号编程接口`sigaction()`函数，函数的原型为：

```c
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
```

其中，结构体`sigaction`至少包含这几个成员：

```c
void (*)(int) sa_handler;		//指向信号处理函数的函数指针
sigset_t sa_mask;				//屏蔽信号字，可以使用sigemptyset(*sigset_t)函数将信号集置空
int sa_flags;					//设置信号处理选项，没有特别要求可以设为NULL
```

结构体`sigaction`指针`act`指向包含信号处理函数的结构体，而指针`oact`用于把原先的动作写到指向的位置(可以取`NULL`)。



##多线程(pthread)
Linux下线程的实现为`NPTL`，即本地POSIX线程库(Native POSIX Thread Library)。
在Unix系统中，多线程开发相关函数定义在头文件`/usr/include/pthread.h`中。
在`Linux`中编译使用了线程库的程序时，需要链接`pthread`库，编译指令如下：

```sh
$ cc -lpthread [源码文件]
```

在`FreeBSD`以及`OS X`中，编译使用了线程库的程序无需链接`pthread`库。

###创建线程
创建线程使用`pthread_create()`函数。

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
```

- `thread`参数为将新建线程的线程标志符写入所给的地址(注意线程标识符的类型为`pthread_t`，使用`int`型编译器会提示**不兼容**)。
- `attr`参数为启动线程时设置**特殊属性**(一般情况下用不到，填`NULL`即可)。
- `start_routine`参数为一个指向返回值和参数都为void*类型的函数的函数指针，该指针指向的函数即为新线程要执行的函数。
- `arg`参数为要传递到`start_routine`所指向函数的值(如果没有需要传递的参数，可以填**NULL**)。

需要注意的是，`thread`参数必须要填入**有效**的地址，填`NULL`会引起程序崩溃。
创建新线程成功则返回`0`，未创建成功返回**错误代码**(**不一定**是`-1`)，可根据man手册查看错误代码判断错误类型。

###等待线程
等待线程使用`pthread_join()`函数。

```c
int pthread_join(pthread_t thread, void **retval);
```

- `thread`参数为要等待的线程的**线程描述符**。
- `retval`参数为要等待的线程的返回值的地址(不使用线程返回值则可以填`NULL`)。

该函数为**阻塞**函数。
需要注意的是，`pthread_join()`函数只会等待指定线程标识符对应的线程，对其它线程不会造成影响，依然是并发执行。
默认情况下，主程序是不会等待线程执行的，无论线程是否执行完毕，主程序都会依次执行直到结束。由于线程是共享资源的，一旦主程序结束了，该程序创建的线程无论是否执行完毕都会立即被关闭。如果需要主程序等待某个线程执行完毕，即可以使用`pthread_join()`函数。

###取消线程
取消线程使用`pthread_cacnel()`函数。

```c
int pthread_cancel(pthread_t thread);
```

参数为需要取消的线程的**线程描述符**。
取消线程成功返回值为`0`，取消线程失败返回一个非`0`的**错误代码**(不一定是-1)。

###终止线程
退出、终止一个线程使用`pthread_exit()`函数。

```c
void pthread_exit(void *retval);
```

线程调用该函数终止自身，如同进程的`exit(num)`函数一样。
`pthread_exit()`函数的参数为线程的返回内容，需要注意的是，不要将`retval`指向一个**局部变量**，因为调用`pthread_exit()`函数之后线程会结束，线程函数内的局部变量(栈变量)将会被**删除**。
与其它函数一样，线程也可以使用`return`提供返回值。

###用互斥量进行线程同步
互斥量相关的函数也定义在头文件`/usr/include/pthread.h`文件中，常用的函数有：

```c
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

互斥量可以让程序员锁住某个对象，使得每次只有一个线程能对其进行访问。
`pthread_mutex_lock()`在未成功加锁时会一直阻塞线程。
使用`pthread_mutex_init()`函数初始化一个互斥量，`mutex`参数为要初始化的互斥量，`attr`参数为互斥量属性，没有特殊要求可以设置为`NULL`(默认为**线程同步**)。

实例代码：

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MYMES1 SIGUSR1 + 1			//自定义信号，不能设置太大，Linux中32～64信号为用户可用
#define MYMES2 SIGUSR1 + 2

pthread_t thread_fd = 0;
pthread_t thread_fd1 = 0;
pthread_mutex_t mutex;

void deal_signal(int signal)
{
	switch(signal)
	{
	case SIGQUIT:		//捕获SIGQUIT信号，用 "ctrl+\" 组合键触发
		pthread_cancel(thread_fd);					//取消一号线程
		pthread_cancel(thread_fd1);					//取消二号线程
		printf("结束一、二号线程，接下来三、四号线程没有互斥量，输出内容会相互交错！\n");
		if (pthread_mutex_destroy(&mutex) == -1)		//删除锁，实际删除锁没什么用。。。
			printf("删除失败！\n");
		break;
	case MYMES1:
		printf("有互斥变量的时候，内容不会交错。\n");
		break;
	case MYMES2:
		printf("没有互斥变量的时候，内容会相互交错。\n");
		break;
	}
}

void* thread_func(void* arg)
{
	while (1)
	{
		if (!strcmp(arg, "三号线程") || !strcmp(arg, "四号线程"))
			kill(getpid(), MYMES2);		//三号、四号线程发送自定义信号MYMES2
		else if (pthread_mutex_lock(&mutex) == -1)
			printf("加锁失败！\n");
		else
			kill(getpid(), MYMES1);		//一号、二号线程发送自定义信号MYMES1

		printf("%s开始。\n", (char*)arg);

		int num = 0;
		while (num++ < 3)
		{
			sleep(2);
			printf("%s循环第%d次\n", (char*)arg, num);
		}

		if (pthread_mutex_unlock(&mutex) == -1)
			printf("解锁失败！\n");

		sleep(2);			//线程休眠一秒，否则资源会一直被一个线程占有
	}
}

int main(void)
{
	char *str1 = "一号线程";
	char *str2 = "二号线程";
	char *str3 = "三号线程";
	char *str4 = "四号线程";

	struct sigaction sig;
	sig.sa_handler = deal_signal;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGQUIT, &sig, 0);		//现代Unix信号捕捉函数

	signal(MYMES1, deal_signal);		//传统Unix信号捕捉函数
	signal(MYMES2, deal_signal);

	pthread_mutex_init(&mutex, NULL);
	pthread_create(&thread_fd, NULL, thread_func, str1);
	pthread_create(&thread_fd1, NULL, thread_func, str2);
	pthread_join(thread_fd1, NULL);			//等待二号线程结束
	pthread_create(&thread_fd, NULL, thread_func, str3);		//创建三号线程
	pthread_create(&thread_fd, NULL, thread_func, str4);		//创建四号线程
	pthread_join(thread_fd1, NULL);

	return 0;
}
```

###使用互斥量进行进程同步
互斥量用于**进程同步**时，会用到下列函数，头文件同样在`/usr/include/pthread.h`中：

```c
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr, int *restrict pshared);
```

通过设置`pthread_mutex_init()`函数的参数`attr`来实现**进程间互斥**。
使用以下方式初始化`pthread_mutexattr_t`结构体。

```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);		//第二个参数如果取PTHREAD_PROCESS_PRIVATE则互斥量仅为进程内部所使用，这是默认情况，即pthread_mutex_init()函数的第二个参数取NULL时的情况
```

需要注意的是，由于每个进程的地址空间是独立的，每个进程定义的变量无法被其它进程所访问。因此，要想使用互斥量进行进程间的同步，仅仅是设置其共享属性是不够的，需要使用Unix的**共享内存**机制，开辟一块共享内存，将互斥量定义在**共享内存**中，以此来保证一个互斥量能被多个进程访问，实现真正的进程互斥。

实例代码：

文件 `Process_Mutex_Parent.c`：

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/shm.h>				//包含共享内存的相关函数

int shm_id;							//共享内存标志，类似于文件标识符
pthread_mutexattr_t attr;

void deal_signal(int signal)
{
	if (signal == SIGQUIT
			&& !pthread_mutex_unlock((pthread_mutex_t*)shmat(shm_id, NULL, 0)))
	{
		printf("解锁互斥量成功！\n父进程即将结束！\n");
		for (int i = 5; i > 0; i--)
		{
			printf("倒数计时：%d\n", i);
			sleep(1);
		}
		printf("父进程结束！\n");
		_exit(0);
	}
}

int main(void)
{
	shm_id = shmget((key_t)666, sizeof(pthread_mutex_t), IPC_CREAT | 0600);
	pthread_mutex_t *mutex = (pthread_mutex_t*)shmat(shm_id, NULL, 0);

	pthread_mutexattr_init(&attr);				//初始化权限结构体attr
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

	if (!pthread_mutex_init(mutex, &attr))
		printf("成功创建了互斥量！\n");			//创建了一个进程互斥的互斥量

	signal(SIGQUIT, deal_signal);
	printf("父进程启动：\n");

	if (!pthread_mutex_lock((pthread_mutex_t*)shmat(shm_id, NULL, 0)))
		printf("父进程互斥量加锁成功！");

	while (1)
	{
		printf("父进程在执行！\n");
		sleep(2);
	}

	return 0;
}
```

文件 `Process_Mutex_Child.c`：

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/shm.h>				//包含共享内存的相关函数

int shm_id;							//共享内存标志，类似于文件标识符
pthread_mutexattr_t attr;

void deal_signal(int signal)
{
	if (signal == SIGQUIT
			&& !pthread_mutex_unlock((pthread_mutex_t*)shmat(shm_id, NULL, 0)))
	{
		printf("解锁互斥量成功！\n父进程即将结束！\n");
		for (int i = 5; i > 0; i--)
		{
			printf("倒数计时：%d\n", i);
			sleep(1);
		}
		printf("父进程结束！\n");
		_exit(0);
	}
}

int main(void)
{
	shm_id = shmget((key_t)666, sizeof(pthread_mutex_t), IPC_CREAT | 0600);
	pthread_mutex_t *mutex = (pthread_mutex_t*)shmat(shm_id, NULL, 0);

	pthread_mutexattr_init(&attr);				//初始化权限结构体attr
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

	if (!pthread_mutex_init(mutex, &attr))
		printf("成功创建了互斥量！\n");		//创建了一个进程互斥的互斥量

	signal(SIGQUIT, deal_signal);
	printf("父进程启动：\n");

	if (!pthread_mutex_lock((pthread_mutex_t*)shmat(shm_id, NULL, 0)))
		printf("父进程互斥量加锁成功！");

	while (1)
	{
		printf("父进程在执行！\n");
		sleep(2);
	}

	return 0;
}
```

运行结果：(文字描述)
`Process_Mutex_Parent`先运行，创建互斥量并锁住，然后一直循环。
`Process_Mutex_Child`后执行，进程阻塞在`pthread_mutex_lock()`函数上。
在`Process_Mutex_Parent`按下`CTRL + \`键，触发`SIGQUIT`信号，由信号处理函数释放锁，倒数计时后结束进程。
`Process_Mutex_Child`在`Process_Mutex_Parent`释放互斥量锁之后立即加锁成功，开始循环。



##共享内存
共享内存是Unix系统中常用的**进程间通信**(IPC, Inter-Process Communication)的一种机制，3大主要IPC之一。
相比信号量等IPC机制，共享内存有着最高的效率，因为共享内存并不涉及复制操作。
共享内存的相关函数定义在`/usr/include/sys/shm.h`中。

###创建/获取共享内存
使用`shmget()`函数创建共享内存或获取已经存在的**key值**的共享内存。

```c
int shmget(key_t key, size_t size, int shmflg);
```

- `key`参数为共享参数的**key值**，为要使用的共享内存**命名**，所有使用这个共享内存的进程都应使用同一个**key值**。
- `size`参数为共享内存的大小。
- `shmflg`参数为特殊标识，取`0`时获取**key值**对应的共享内存，若传入的**key值**对应的共享内存**未创建**，则调用**失败**。

创建共享内存使用`IPC_CREAT`标识，创建的同时可以手动设定共享内存的读写权限如`IPC_CREAT | 0660`。
使用`IPC_CREAT`标识时，若传入**key值**对应的共享内存已经存在，不会调用失败，而是忽略该标识，返回已存在的共享内存的描述符。
如果需要创建一块**唯一**的共享内存，则使用`IPC_CREAT | IPC_EXCL`。
使用`IPC_CREAT | IPC_EXCL`标识时，若传入`key`参数对应的共享内存已存在，则创建**失败**。
`IPC_PRIVATE`标志用于创建一个只属于创建进程的共享内存。
共享内存创建成功时返回**共享内存描述符**(非负整数)，失败时返回`-1`。

###获得共享内存地址
使用`shmat()`函数获取共享内存的**地址**。

```c
void *shmat(int shmid, const void *shmaddr, int shmflg);
```

- `shmid`参数为目标共享内存描述符。
- `shmaddr`参数为将共享连接到当前进程中的地址，如果不需要指定共享内存连接到进程中的**指定地址**，则该参数可以为`NULL`(让系统分配可用的地址)。
- `shmflg`参数为一组标志，如果`shmaddr`参数手动指定了地址，则搭配`shmflg`参数取`SHM_RND`，如果**只读**共享内存则设定`SHM_RDONLY`，不使用此参数可设为`0`。

函数运行成功返回**共享内存**的**首地址**，运行失败返回数值`-1`。

###分离共享内存
使用`shmdt()`函数分离共享内存。

```c
int shmdt(const void *shmaddr);
```

- `shmaddr`参数为共享内存的**首地址**，运行成功之后共享内存在调用此函数的进程中将**不再可用**(但其它进程中依然**正常**)。

调用成功时返回`0`，失败返回`-1`。

###控制共享内存
使用`shmctl()`函数设置共享内存的**标识**。

```c
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

- `shmid`参数为共享内存描述符。
- `command`参数为要对共享内存发出的指令，常用的指令为`IPC_RMID`，用于**删除**共享内存，执行删除操作时- `buf`参数可以取值`NULL`。

函数调用成功返回`0`,失败返回`-1`。
与信号量机制类似，如果`shmget()`函数以`IPC_CREAT | IPC_EXCL`的形式创建**唯一**共享内存的话，如果没有在进程结束前将共享内存删除，则下次程序执行时将**不能**够再以`IPC_CREAT | IPC_EXCL`的形式创建**key值**相同的共享内存！



##信号量(semaphore)
信号量是Unix系统中常用的进程间通信(IPC, Inter-Process Communication)的一种机制。
信号量用于控制进程对资源的访问，但信号量也可以用于线程。
在进程开发中，常用的信号量函数定义在`/usr/include/sys/sem.h`文件中。

###创建/获取信号量
使用`semget()`函数创建一个新的信号量或获取一个已经存在的信号量。

```c
int semget(key_t key, int num_sems, int sem_flags);
```

- `key`参数为整数值，代表信号量的值，不同进程使用相同的**key值**就可以通过信号量进行通信。
- `num_sems`参数为需要的信号量数目，一般为`1`。
- `sem_flags`参数为信号标识，可接收多个标识，通过逻辑或操作符`|`相连。

函数调用成功时返回信号量描述符，失败时返回`-1`。
`sem_flags`参数上常用的信号标识有`IPC_CREAT`，用于**创建**新的信号量，但如果**key值**对应的信号量已被创建，并不会调用失败，而是**忽略**该标志。
`IPC_CREAT | IPC_EXCL`标识，用于创建一个**新的**、**唯一**的信号量，如果**key值**对应的信号量已被创建，则调用**失败**。
使用`IPC_CREAT | IPC_EXCL`标识需要注意，`key`的值不能设置太小，较小的`key`值对应的信号量可能**已被系统使用**而导致调用失败。此外，使用此种方式创建信号量在使用完毕后需要调用`semctl()`函数释放，否则下次运行同样的程序会由于信号量已经存在(没被释放)而造成调用失败。
`IPC_CRAET | 0666`标识，用于创建有**读写权限**的信号量。信号量在Unix系统也是文件，默认情况下，创建的信号量没有读写权限，需要搭配权限描述字段才能有读写权限。

###改变信号量的值
使用`semop()`函数修改信号量的值。

```c
int semop(int sem_id, struct sembuf *sem_ops, size_t num_sem_ops);
```

- `sem_id`参数为信号描述符，由`semget()`函数得到。
- `sem_ops`参数为指向`sembuf`结构体的指针(有多个信号量时可以指向`sembuf`结构体数组)。
- `num_sem_ops`参数为`sembuf`结构体的数量，一般为`1`。

`sembuf`结构体的定义为：

```c
struct sembuf {
	unsigned short sem_num;		//信号量的编号，在没有使用多个信号量的情况下，一般为0
	short sem_op;				//信号量操作，一般可以取-1或是+1，分别对应P(请求)、V(释放)操作
	short sem_flg;				//操作标志符，一般取SEM_UNDO
}
```

函数调用成功返回`0`，调用失败返回`-1`并置**errno**。

###设置信号量信息
使用`semctl()`函数来执行信号量集上的控制操作。

```c
int semctl(int sem_id, int sem_num, int command, ...);
```

- `sem_id`参数为信号量描述符。
- `sem_num`参数为信号量编号，一般没有多个信号量时取`0`。
- `command`参数为要执行的操作的标识符。

`command`参数可以有很多不同的值，常用的有`IPC_RMID`，用于删除一个信号量(如果信号创建方式是`IPC_CREAT | IPC_EXCL` ，则务必要在程序结束时删除信号量)。
`command`设置为`SETVAL`，则用于**初始化**一个信号量，此时函数需要有第四个参数，联合体`union semun`，通过设置`semun`的`val`成员的值来初始化信号量。
`semun`联合体的定义为：

```c
union semun {
	int     val;            /* value for SETVAL */
	struct  semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
	u_short *array;         /* array for GETALL & SETALL */
};
```

函数失败时返回`-1`，成功是有多种返回值，参数为`SETVAL`或`IPC_RMID`时运行成功返回`0`。

实例代码：

文件 `Semaphore_Before.c`：

```c
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int sem_id = 0;
struct sembuf sem_wait;
struct sembuf sem_ok;

void deal_signal(int sig)
{
	semop(sem_id, &sem_ok, 1);		//将信号量+1，释放资源
	_exit(0);
}

int main(void)
{
	signal(SIGINT, deal_signal);

	sem_id = semget(9999, 1, IPC_CREAT | IPC_EXCL | 0600);
	if (sem_id == -1)
	{
		printf("创建信号量失败！\n");
		return 0;
	}
	else
		printf("信号量创建成功！\n");

	sem_wait.sem_num = sem_ok.sem_num = 0;
	sem_wait.sem_op = -1;		//设置操作数，等待时-1
	sem_ok.sem_op = 1;			//等待完毕+1
	sem_wait.sem_flg = sem_ok.sem_flg = SEM_UNDO;
	semctl(sem_id, 0, SETVAL, 1);		//初始化信号量时可以不自定义联合体直接赋值。此外，只有创建信号量的进程才需要初始化信号量，获取信号量的进程不需要
	semop(sem_id, &sem_wait, 1);	//信号量-1，锁住资源

	while (1)		//由于信号量被锁，因此A在执行此段代码时，B在等待
	{
		sleep(3);
		printf("正在执行\n");
	}

	return 0;
}
```

文件 `Semaphore_After.c`：

```c
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int sem_id = 0;

void deal_signal(int sig)
{
	semctl(sem_id, 0, IPC_RMID);		//删除信号量
	_exit(0);
}

int main(void)
{
	signal(SIGINT, deal_signal);

	sem_id = semget(9999, 1, 0600);		//需要保证进程有读写信号量的权限
	if (sem_id == -1)
	{
		printf("打开信号量失败！\n");
		return 0;
	}
	else
		printf("信号量创建成功！\n");

	struct sembuf sem_wait;
	sem_wait.sem_num = 0;
	sem_wait.sem_op = -1;
	sem_wait.sem_flg = SEM_UNDO;
	semop(sem_id, &sem_wait, 1);

	while (1)
	{
		sleep(3);
		printf("正在执行\n");
	}

	return 0;
}
```

运行结果：(文字描述)
`Semaphore_Before`先运行，创建信号量成功(一直循环)。
`Semaphore_After`后运行，获取信号量成功，然后进程阻塞在`semop()`函数上，等待`Semaphore_Before`释放资源。
向`Semaphore_Before`发送`SIGINT`信号，让其释放资源，结束进程，然后`Semaphore_After`获得资源，`semop()`函数不再阻塞，也开始循环。
最后向`Semphore_After`发送`SIGINT`信号，让其删除信号量并结束进程。



##IO多路复用
在Unix中，posix定义了一系列IO多路复用机制，如`select()``pselect()``poll()`等调用。
Linux和BSD还分别提供了增强的IO复用机制，在Linux中为`epoll`，在BSD中为`kqueue`。

### *select()* 调用
在Unix环境下，`select()`是常用的IO多路复用机制之一，函数定义在`/usr/include/sys/select.h`中。
`select()`函数定义如下所示：

```c
int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout);
```

- `nfds`参数为需要监视的文件描述符的最大值加`1`。
- `readfds`参数为要监视的可读检测文件描述符集合。
- `writefds`参数为要监视的可写检测文件描述符集合。
- `errorfds`参数为要监视的错误检测文件描述符集合。
- `timeout`参数为超时等待的时间，可以精确到**微秒**，取`NULL`时为`select()`为阻塞函数，超时为`0`是为非阻塞轮询。

结构`timeval`的定义为：

```c
struct timeval {
	time_t tv_sec;			/* seconds */
	suseconds_t tv_usec;	/* microseconds */
};
```

描述符集合`fd_set`可以使用下列宏进行操作：

```c
void FD_ZERO(fd_set *fdset);
void FD_SET(fd, fd_set *fdset);
void FD_CLR(fd, fd_set *fdset);
int FD_ISSET(fd, fd_set *fdset);
```

- 使用`FD_ZERO`初始化描述符集合。
- 使用`FD_SET`向描述符集合中添加描述符。
- 使用`FD_CLR`从描述符集合中移除描述符。
- 使用`FD_ISSET`检测描述符，若`fd`参数在描述符集合参数`fdset`中，返回非零值，否则返回`0`。

每次调用`select()`前都需要重设描述符集合(执行`FD_ZERO`和`FD_SET`宏)。
函数执行成功返回变化的描述符数量，监视的描述符无变化则返回`0`，调用失败返回`-1`。

基本的`select()`函数代码框架为：

```c
int fd[fdcount];
struct fd_set fdset;

while(TRUE)
{
	//初始化并设置描述符集合，每次调用select()前都需要类似操作
	FD_ZERO(&fdset);
	FD_SET(fd[0], &fdset);
	...		//设置需要监视的描述符

	if (select(maxfd, &fdset, NULL, NULL, timeout) > 0)
	{
		for (int i = 0; i < fdcount; i++)
		{
			//使用宏FD_ISSET判断文件描述符是否发生变化
			if (FD_ISSET(fd[i], &fdset))
			{
				/* do something */
			}
			else
			{
				/* do something */
			}
		}
	}
}
```