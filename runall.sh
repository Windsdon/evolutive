for ((1;1;1))
do
	player env/simple.cfg & src/a.out
	player env/simple2.cfg & src/a.out
	player env/simple3.cfg & src/a.out
	player env/simple4.cfg & src/a.out
	player env/simple5.cfg & src/a.out
	pkill -SIGINT -f "player"

done
