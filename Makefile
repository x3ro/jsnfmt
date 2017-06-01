CFLAGS=-pipe -march=native -mtune=native
CC=clang

build:
	$(CC) $(CFLAGS) -O3 -o jsnfmt src/jsnfmt.c

profile:
	$(CC) $(CFLAGS) -lprofiler -g -o jsnfmt src/jsnfmt.c

format:
	clang-format -i --style="WebKit" src/jsnfmt.c