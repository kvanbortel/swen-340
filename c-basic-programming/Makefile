HEADERS = c_language_test.h unit_tests.h

test:	build/obj/main.o build/obj/c_language_test.o build/obj/unit_tests.o
	mkdir -p build/bin
	gcc -g -o build/bin/test build/obj/main.o build/obj/c_language_test.o build/obj/unit_tests.o
	
build/obj/c_language_test.o: c_language_test.c $(HEADERS)
	mkdir -p build/obj
	gcc -g -o build/obj/c_language_test.o -c -Wall c_language_test.c

build/obj/main.o: main.c $(HEADERS)
	mkdir -p build/obj
	gcc -g -o build/obj/main.o -c -Wall main.c
	
build/obj/unit_tests.o: unit_tests.c $(HEADERS)
	mkdir -p build/obj
	gcc -g -o build/obj/unit_tests.o -c -Wall unit_tests.c

clean:
	-rm -rf build


