# make
# make install
insmod kth_bindCPU.ko
# lsmod | grep kth_bindCPU
kth_bindCPU           262144  0
# dmesg | tail -n6
[11240.446740] hello kernel thread kthreadtest, cpu = 1
[11242.462708] hello kernel thread kthreadtest, cpu = 1
[11244.478683] hello kernel thread kthreadtest, cpu = 1
[11246.494652] hello kernel thread kthreadtest, cpu = 1
[11248.510633] hello kernel thread kthreadtest, cpu = 1
[11250.526610] hello kernel thread kthreadtest, cpu = 1
# make uninstall
rmmod kth_bindCPU
# dmesg | tail -n6
[11250.526610] hello kernel thread kthreadtest, cpu = 1
[11252.542578] hello kernel thread kthreadtest, cpu = 1
[11254.558547] hello kernel thread kthreadtest, cpu = 1
[11256.574510] hello kernel thread kthreadtest, cpu = 1
[11258.590494] hello kernel thread kthreadtest, cpu = 1
[11259.873950] Kill new kthread.
# make clean
