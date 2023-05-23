# make
# make install
insmod proc_parser.ko
# cat /home/kernel_file
你好
# dmesg | tail -n10
[  180.388919] p name:cycle_print_kth, pid is:2843
[  180.389803] the cpu load is: 0.37
[  180.390485] total memory is: 6975232 KB
[  180.391172] free memory is: 6166656 KB
[  180.391822] occupy memory is: 808576 KB
[  183.428874] p name:cycle_print_kth, pid is:2843
[  183.429666] the cpu load is: 0.42
[  183.430279] total memory is: 6975232 KB
[  183.430907] free memory is: 6166656 KB
[  183.431513] occupy memory is: 808576 KB
# make uninstall
rmmod proc_parser
# make clean
