while true; do
	./rungenerated.sh
	./select.sh
	rm stats.txt
	echo "" > stats.txt
done
