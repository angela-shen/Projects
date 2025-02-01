CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LDFLAGS = -lm
MATHLIB_OBJS = bbp.o e.o euler.o madhava.o newton.o viete.o mathlib-test.o
EXEC = mathlib-test


all: mathlib-test

mathlib-test: $(MATHLIB_OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(MATHLIB_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(MATHLIB_OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
