PROCNAME="transceiver"
#GDB_CMD="sudo gdb.tap.sh ${@}"
GDB_CMD="sudo gdb ${@}"
#GDB_CMD="sudo kdbg ${@} -p"

echo "GDB-attach waiting for $PROCNAME"

while true; do
	PID=$(ps -Al | grep $PROCNAME | awk '{print $4}')
	if [ "x$PID" != "x" ]; then
		echo
		echo "$PROCNAME detected at: $PID"
		echo ""
		#$GDB_CMD $PID
		
		sudo kdbg -p $PID ${@}
	else
		echo -n "."
		sleep 1
	fi
done
