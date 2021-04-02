#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;

/*
每个进程都有自己的虚拟地址空间，一般32为机器是4G大小（就是寻址大小2^32），其中1-3G是用户地址空间，3-4G是是内核地址空间。
当父进程创建子进程时，子进程复制父进程的代码段和数据段，即clone父进程的1-3G的内容，而3-4G的内核只需要重新映射下到物理地址的内核即可。
子进程几乎复制了所有的父进程内容，包括pcb，但是在复制时，系统会修改子进程的pid来和父进程区别。
因为子进程会把父进程的pcb完全拷贝过来，所以会记录程序运行到哪了，子进程会从fork之后的代码段运行。
*/

/*
孤儿进程：一个父进程退出，而子进程还在运行，那么这些子进程将成为孤儿进程。孤儿进程会被init进程（1号进程）收养，由init进程完成这些孤儿进程的善后工作。
僵尸进程：任何一个子进程(init除外)在exit()之后，并非马上就消失掉，而是留下一个称为僵尸进程(Zombie)的数据结构，等待父进程处理。父进程需要调用wait/waitpid来对子进程保留的信息处理

孤儿进程没什么危害，但是僵尸进程会占用进程号，但是系统所能使用的进程号是有限的，如果大量的产生僵死进程，将因为没有可用的进程号而导致系统不能产生新的进程. 此即为僵尸进程的危害，应当避免。
可使用SIGCHLD来捕获退出的子进程，并对其做善后处理
*/

static void sig_child(int signo)
{
    pid_t pid;
    int stat;
    //处理僵尸进程
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated.\n", pid);
}

int main()
{
    cout << "weclome to test fork process" << endl;
    pid_t pid;

    //捕获退出的子进程，sig_child对其处理善后，避免成为僵尸进程
    signal(SIGCHLD, sig_child);

    int beforeFork = 99;

    // fork a child process, once call, twice return. return child's pid if in parent process, return 0 if in child process
    pid = fork();

    if (pid > 0)
    { //it is child's pid, you can via "ps -aux | grep " find they are the same
        pid_t current = getpid();
        cout << "i am parent,pid:" << pid << ",currentPid =" << current << endl;
        beforeFork = 100;
        pid_t parentPid = getppid();
        cout << "parent's pid=" << parentPid << endl;
        sleep(3);
        system("ps -o pid,ppid,state,tty,command");
        // while(1);
    }
    else if (pid == 0)
    { //child process clone the pcb of parent, but OS modify the pid to distinguish them.
        pid_t current = getpid();
        cout << "i am child,pid:" << pid << ",currentPid = " << current << endl;
        //shared on read, copy on write.
        cout << "beforeFork = " << beforeFork << endl; //99
        exit(0);
        // while(1);
    }
    else
    {
        cout << "error" << endl;
    }

    return 0;
}