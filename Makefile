
CC  = gcc

CFLAGS = -Wall -Wextra

EXEC = tema1

build:	$(EXEC)

tema1:	tema1.c
	$(CC) $(CFLAGS) tema1.c -o tema1

run: $(EXEC)
	./$(EXEC)

clean:
	rm	-f	$(OBJS)	$(EXEC)