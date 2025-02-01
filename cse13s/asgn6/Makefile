CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -gdwarf-4 -g 
OBJS = word.o io.o trie.o
DECODE = decode.o
ENCODE = encode.o
EXEC = encode decode


all: $(EXEC)

decode: $(OBJS) $(DECODE)
	$(CC) -o decode $(OBJS) $(DECODE)

encode: $(OBJS) $(ENCODE)
	$(CC) -o encode $(OBJS) $(ENCODE)

%.o: %.c 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS) $(DECODE) $(ENCODE)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
