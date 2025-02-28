这是一个使用 POSIX IPC 机制实现的跨进程生产者-消费者模型，两个程序通过共享内存和信号量协同工作，具体功能如下：

process_a.c (生产者进程)

内存管理

创建/初始化共享内存 (shm_open + mmap)
实现动态扩容机制 (容量翻倍策略)
维护内存头部元数据 (容量/就绪标志/数据尺寸)
同步机制

初始化跨进程互斥锁 (pthread_mutexattr_setpshared)
创建并管理信号量 (SEM_AVAIL/SEM_DONE)
使用互斥锁保护共享内存访问
数据生产

安全写入数据到共享内存
通过信号量通知消费者
等待消费者处理完成信号
process_b.c (消费者进程)

内存接入

连接已存在的共享内存
动态检测内存扩容 (通过 fstat 检测)
自动重映射扩容后的内存
数据处理

等待生产者通知信号
使用互斥锁安全读取数据
清空就绪标志位
发送处理完成信号
协同工作流程 (通过信号量 SEM_AVAIL/SEM_DONE)


plaintext
Apply
process_a                     process_b
=========                     =========
写入数据 ->                     
sem_post(SEM_AVAIL) ----> 等待 sem_wait(SEM_AVAIL)
                         处理数据
                         sem_post(SEM_DONE)
等待 sem_wait(SEM_DONE) <--- 
关键技术特征：

线程安全的共享内存访问 (通过 PTHREAD_PROCESS_SHARED 属性的互斥锁)
智能内存扩容策略 (避免频繁扩容)
原子操作保证 (ready_flag 的置零操作)
完善的错误处理机制 (所有系统调用均带错误检查)
资源生命周期管理 (通过 cleanup 函数统一释放)
这两个程序共同实现了一个可靠的生产者-消费者 IPC 系统，process_a 负责动态生成和发送数据，process_b 负责实时接收和处理数据，通过共享内存和信号量的组合实现了高效的进程间通信。