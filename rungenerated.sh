while true; do
	src/remaker ev && break;
	for i in $(src/remaker); do
		player -p 10001 env/simple.cfg & 
		killPID1=$!
		echo "\n\nProcess: $!\n\n"
		rm lock.*
		src/a.out $i 10001
		echo "\n\n#### Killing $killPID1\n\n"
		kill $killPID1
		rm "lock.$i"
	done
done
