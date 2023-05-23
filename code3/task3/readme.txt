# make
# make install
insmod kthread_sleep.ko
# lsmod | grep kthread_sleep
kthread_sleep         262144  0
# dmesg | tail -10
[11258.590494] hello kernel thread kthreadtest, cpu = 1
[11259.873950] Kill new kthread.
[12003.007302] the pid of the sub_thread is:11316
[12003.007829] the current pid is:11315
[12003.008306] the result of wake_up_process for new thread is: 1
[12003.008311] the sub_thrread pid is:11316
[12003.009338] the state of the init function is :2
[12003.009835] the state of the init function after wake_up_process is :0
[12003.009841] the result of wake_up_process for init thread is: 0
[12003.010441] the result of the wake_up_process is:1
# make uninstall
rmmod kthread_sleep
# dmesg | tail -10
[11259.873950] Kill new kthread.
[12003.007302] the pid of the sub_thread is:11316
[12003.007829] the current pid is:11315
[12003.008306] the result of wake_up_process for new thread is: 1
[12003.008311] the sub_thrread pid is:11316
[12003.009338] the state of the init function is :2
[12003.009835] the state of the init function after wake_up_process is :0
[12003.009841] the result of wake_up_process for init thread is: 0
[12003.010441] the result of the wake_up_process is:1
[12068.046451] Goodbye wake_up_process
