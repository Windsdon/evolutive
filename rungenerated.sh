for i in $(cat generated.txt)
do
	player env/simple.cfg & src/a.out $i
	pkill -SIGINT -f "player"
done
