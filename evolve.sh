for ((1;1;1))
do
	rm "stats.txt"
	for i in $(cat generated.txt)
	do
		player env/simple.cfg & src/a.out $i
		pkill -SIGINT -f "player"
	done
	rm "generated.txt"
	src/selector.out
done
