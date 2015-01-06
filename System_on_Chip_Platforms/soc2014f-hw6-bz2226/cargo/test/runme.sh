#!/sbin/ash

/sbin/mark_app

/sbin/insmod debayer.ko
/sbin/mdev -s
./debayer-mmap ./inout &
wait
/sbin/rmmod debayer

/sbin/qsim_exit
