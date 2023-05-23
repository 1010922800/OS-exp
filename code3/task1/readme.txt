# make
# make install
insmod kthread.ko
# lsmod | grep kthread
kthread               262144  0
# dmesg | tail -n6
[10118.702603] New kthread is running.
[10120.718568] New kthread is running.
[10122.734536] New kthread is running.
[10124.750521] New kthread is running.
[10126.766488] New kthread is running.
[10128.782458] New kthread is running.
# make uninstall
rmmod kthread
# dmesg | tail -n6
[10144.910237] New kthread is running.
[10146.926200] New kthread is running.
[10148.942166] New kthread is running.
[10150.958139] New kthread is running.
[10152.974109] New kthread is running.
[10153.422119] Kill new kthread.
# make clean
