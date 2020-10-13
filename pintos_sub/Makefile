CC=gcc
OBJS=bitmap.o hash.o debug.o hex_dump.o list.o
TARGET=testlib

$(TARGET) : main.c $(OBJS)
	$(CC) -o $(TARGET) main.c $(OBJS)

bitmap.o : bitmap.c
	$(CC) -c -o bitmap.o bitmap.c

hash.o : hash.c
	$(CC) -c -o hash.o hash.c

debug.o : debug.c
	$(CC) -c -o debug.o debug.c

hex_dump.o : hex_dump.c
	$(CC) -c -o hex_dump.o hex_dump.c

list.o : list.c
	$(CC) -c -o list.o list.c 

clean :
	rm *.o $(TARGET)