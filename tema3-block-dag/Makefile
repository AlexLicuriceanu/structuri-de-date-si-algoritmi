build:
	gcc *.c -o blockdag

test: build
	valgrind --leak-check=full ./blockdag

clean:
	rm blockdag
	rm *.out