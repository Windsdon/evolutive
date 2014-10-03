for ((1;1;1))
do
	player env/simple.cfg & src/a.out
	pkill -SIGINT -f "player"

done
