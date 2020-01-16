all:
	gcc nb.c loop.c event.c -o nb.out
	./nb.out

clear:
	rm *.out