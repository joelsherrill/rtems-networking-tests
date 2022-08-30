#! /bin/sh
#
# Simple wrapper for qemu which does not check for timeouts
#
# Output has characters which can be removed with:
#
#  sed -e 's/\x0//g' -e 's/\r//g'

if [ ${#} -ne 1 ] ; then
  echo usage: $0 Executable
  exit 1
fi

if [ ! -r ${1} ] ; then
  echo ${1} does not exist
  exit 1
fi

ulimit -t 300
echo
echo
echo ====== Starting ${1}
qemu-system-arm -no-reboot -serial null -serial mon:stdio \
  -net nic,model=cadence_gem,macaddr=0e:b0:ba:5e:ba:11 \
  -net user,hostfwd=tcp:127.0.0.1:2000-:23,hostfwd=udp:225.0.0.25:12345-:12345 \
  -nographic -M xilinx-zynq-a9 -m 256M \
  -kernel ${1}
echo ====== End of ${1}
exit $?
