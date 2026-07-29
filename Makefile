CC = gcc

CFLAGS = -Wall -Wextra -pedantic 

SRC = src/main.c

TARGET = build/transliterate

make:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

# translit:
# WIP command for translating files within the 'input' folder into the 'output' folder.
clean:
	rm  -f build/*.exe build/*.o

run:
	./$(TARGET)

test:
	./$(TARGET) src/test/test.src.md output/test.md
