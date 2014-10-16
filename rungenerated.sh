while true; do
	src/remaker ev && break;
	for i in $(src/remaker); do
		player env/simple.cfg & src/a.out $i
		pkill -SIGINT -f "player"
	done
done
