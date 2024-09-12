HEADERS = systick.h systick_simulator.h unit_tests.h

default: test

test:	main.o systick.o systick_simulator.o unit_tests.o
	gcc -g -o test main.o systick.o systick_simulator.o unit_tests.o
	
systick.o: systick.c $(HEADERS)
	gcc -g -c -Wall systick.c

systick_simulator.o: systick_simulator.c $(HEADERS)
	gcc -g -c -Wall systick_simulator.c

main.o: main.c $(HEADERS)
	gcc -g -c -Wall main.c
	
unit_tests.o: unit_tests.c $(HEADERS)
	gcc -g -c -Wall unit_tests.c

clean:
	-rm -f systick.o
	-rm -f systick_simulator.o
	-rm -f main.o
	-rm -f unit_tests.o
	-rm -f test


