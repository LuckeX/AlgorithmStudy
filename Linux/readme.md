# 编译程序加上-g选项
# 启动gdb
1. gdb调试可执行文件  
`gdb <program>`
2. gdb调试coredump文件  
`ulimit  -c unlimited //设置core文件的大小`  
`gdb <program> <core dump file> // 进入gdbtiaoshi`
3. gdb调试服务程序  
`gdb <program> <PID>`
`gdb hello 11127`

# 一般调试命令
`run(r)`
`break(b) n //在n行上打断点`  
`continue(c) //执行到下一个断点`  
`next(n) // 单步跟踪程序，当遇到函数调用时，也不进入此函数体`  
`step(s) // 单步调试如果有函数调用，则进入函数；与命令n不同，n是不进入调用的函数的`  
`delete n //删除第n个断点`    
`clear n //清除第n行的断点`  
`info b (info breakpoints)//显示当前程序的断点设置情况`  
`delete breakpoints //清除所有断点`  
`where/bt //当前运行的堆栈列表`  
`info threads //显示所有线程信息`  
`thread apply all bt //显示所有线程堆栈信息`  
`thread 1 // 切换到1号线程`     
`layout src：显示源代码窗口`    
`layout asm：显示反汇编窗口`