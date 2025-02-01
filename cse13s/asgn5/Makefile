CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -gdwarf-4 `pkg-config --cflags gmp`
LDFLAGS = `pkg-config --libs gmp` 
OBJS = randstate.o numtheory.o ss.o
DECRYPT = decrypt.o
KEYGEN = keygen.o
ENCRYPT = encrypt.o
EXEC = keygen decrypt encrypt


all: $(EXEC)

keygen: $(OBJS) $(KEYGEN)
	$(CC) $(LDFLAGS) -o keygen $(OBJS) $(KEYGEN)

decrypt: $(OBJS) $(DECRYPT)
	$(CC) $(LDFLAGS) -o decrypt $(OBJS) $(DECRYPT)

encrypt: $(OBJS) $(ENCRYPT)
	$(CC) $(LDFLAGS) -o encrypt $(OBJS) $(ENCRYPT)

%.o: %.c 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS) $(KEYGEN) $(DECRYPT) $(ENCRYPT)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
