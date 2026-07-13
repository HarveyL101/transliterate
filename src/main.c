/*
 * *****************************************************************************************
 * Name: Transliterate
 * Purpose: Provides a lightweight method of adding in-line cyrillic text to markdown files, 
 * 	    avoiding tedious copy pasting from browser pages or third party keyboard inputs. 
 * Author: Harvey Lopez
 * *****************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// TODO: Read the file and echo it back unchanged
// TODO: Find literal \ru{ occurrences
// TODO: Transliterate a single word, single letters only 
// TODO: Extend to digraphs with greedy longest-match
// TODO: Preserve capitalisation
// TODO: Combine all stages into one process
// TODO: Create edge cases and a test file


/* --------------- String Buffer --------------- */
typedef struct
{
	const char[] *contents;
	long int size;
} Buffer;

/* --------------- Transliteration Table --------------- */

// The table is ordered longest-match-first, aiming to prevent single letter matches stealing from multi-letter sounds produced by single characters (E.g. 'zh').

typedef struct
{
	const char *latin; /* lowercase latin word to be matched. */
	const char *lowercase; /* cyrillic replacement, in lowercase */
	const char *uppercase; /* cyrillic replacement, in uppercase */
} Rule;

static const Rule RULES[] =
{
	// Four letter matches
	{"shch", "щ", "Щ"},
	// Two letter matches
	{"zh",  "ж", "Ж"},
    	{"kh",  "х", "Х"},
    	{"ts",  "ц", "Ц"},
    	{"ch",  "ч", "Ч"},
    	{"sh",  "ш", "Ш"},
    	{"ya",  "я", "Я"},
    	{"yu",  "ю", "Ю"},
    	{"yo",  "ё", "Ё"},
	// One letter matches
    	{"a", "а", "А"}, {"b", "б", "Б"}, {"v", "в", "В"}, {"g", "г", "Г"},
    	{"d", "д", "Д"}, {"e", "е", "Е"}, {"z", "з", "З"}, {"i", "и", "И"},
   	{"j", "й", "Й"}, {"k", "к", "К"}, {"l", "л", "Л"}, {"m", "м", "М"},
    	{"n", "н", "Н"}, {"o", "о", "О"}, {"p", "п", "П"}, {"r", "р", "Р"},
    	{"s", "с", "С"}, {"t", "т", "Т"}, {"u", "у", "У"}, {"f", "ф", "Ф"},
    	{"h", "х", "Х"}, {"c", "к", "К"}, {"y", "ы", "Ы"}, {"x", "кс", "Кс"},
    	{"'", "ь", "Ь"}, {"\"", "ъ", "Ъ"},
};

#define NUM_RULES (sizeof(RULES) / sizeof(RULES[0]))

char *read_file(const char *path, size_t *output_size)
{
	// Reads the contents of the given file and returns a pointer to a temporary string buffer.
	FILE *file;
	long int size; // ftell() returns a long int
	size_t result;
	char *temp;

	file = fopen(path, "r");
	if (file == NULL) { fputs("Error reading file.", stderr); exit (1); }

	// Walks to the end of the file and counts its steps, giving the total size of the file (bytes)
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file); // Resets the walking position to the start of the file.
	
	*output_size = size;
	printf("File: %s\n\tSize (bytes): %zu", *path, *output_size);

	// Allocate a temporary buffer of (size + 1) to include a null terminator.
	temp = (char *) malloc(sizeof(char) * size + 1);
	if (temp == NULL) { fputs("Error allocating to the heap", stderr); exit (2); }
	buffer[size] = "\0"; 

	// Copy the read contents into the temp buffer.
	temp = fread(temp, 1, size, file);
	if (temp != size) { fputs("Error copying file-data into the buffer", stderr); exit (3); }

	fclose(file);

	return temp;
}

int main(void)
{
	/*
	 * IDEAS:
	 * Recursively transliterate everything within the src folder, allowing for larger resources to be compiled 
	 * and moved as a single grouping of content (think multi-file learning resource with chapters etc.). 
	 * This would produce an identical file structure in /build. (lookup `file-tree-walk`)
	 */
	
	Buffer buffer;
	// Test path (relative to the build location
	char *test_path = "src/test/test.src.md";

	// TODO: Read the contents of a given file ending in .src.md, returning a string buffer of its contents.
	// Points our buffer's contents at the address of the temporary string buffer after read_file does the heavy lifting.
	buffer->contents = read_file(test_path, &buffer->size);
	
	// read_file testing print
	char *ptr = buffer->contents;
	while (*ptr != "\0")
	{
		printf("%c", *ptr);
		ptr++;
	}
	
	// TODO: Apply transliteration to text proceeding '\ru{'. terminating at the closing '}'.
	
	// TODO: Write the transliterated content into build/FILE_NAME.md (touch or fwrite).
	
	free(buffer->contents);

	return 0;
}
