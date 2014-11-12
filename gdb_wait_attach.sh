NARGS=$#
PROCNAME=${1-"transceiver"}
shift
GARGS=${@-"transceiver-bladerf"}

if [ $NARGS -lt 2 ]; then
	cat <<EOF
$(basename $0) syntax error: $(basename $0) <procname> <GDB arguments>
    Where <GDB arguments> is usually just one: binary-name
EOF
	exit 1
fi


echo "GDB-attach (PID=$$) waiting for $PROCNAME"

while true; do
	PID=$(ps -Al | grep $PROCNAME | awk '{print $4}')
	if [ "x$PID" != "x" ]; then
		echo
		echo "$PROCNAME detected at: $PID"
		echo "Attaching using: ${GARGS}"
		echo ""

		#gdb $GARGS $PID
		sudo kdbg -p $PID $GARGS
	else
		echo -n "."
		sleep 1
	fi
done
