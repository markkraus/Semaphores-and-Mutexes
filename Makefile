CC=gcc

CFLAGS=-Wall -Werror -Iinclude

OBJS=src/main.o src/speak.o

EXEC=exec

all: $(EXEC)

$(EXEC): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

src/%.o: src/%.c include/speak.h
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS) $(EXEC)