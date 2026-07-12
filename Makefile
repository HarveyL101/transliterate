CC = gcc

CFLAGS = -Wall -Wextra -pedantic 

SRC = src/main.c

TARGET = build/transliterate.exe

translit:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

clean:
	rm  -f build/*.exe build/*.o
