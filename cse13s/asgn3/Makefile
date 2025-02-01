CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -gdwarf-4
LDFLAGS = -lm
OBJS = set.o stats.o batcher.o shell.o heap.o quick.o sorting.o
EXEC = sorting


all: sorting

sorting: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
