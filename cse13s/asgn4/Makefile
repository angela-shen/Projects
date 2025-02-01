CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -gdwarf-4
LDFLAGS = -lncurses
OBJS = universe.o life.o
EXEC = life


all: life

life: $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
