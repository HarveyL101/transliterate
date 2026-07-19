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
#include <regex.h>

// TODO: Read the file and echo it back unchanged
// TODO: Find literal \ru{ occurrences
// TODO: Transliterate a single word, single letters only 
// TODO: Extend to digraphs with greedy longest-match
// TODO: Preserve capitalisation
// TODO: Combine all stages into one process
// TODO: Create edge cases and a test file

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

/* --------------- Buffer Functions --------------- */

char *read_file_to_buffer(const char *path, char *buffer)
{
	FILE *file = fopen(path, "r");
	long bufferSize;
	size_t newSize;

	if (file != NULL)
	{
		// Go to the end of the file
		if (fseek(file, 0L, SEEK_END) == 0)
		{
			// Get size of the file
			bufferSize = ftell(file);

			if (bufferSize == -1) { fputs("File empty or non existent, [buffer size == -1]", stderr); exit(1); }
			// Allocate new buffer to that size
			buffer = malloc(sizeof(char) * (bufferSize + 1));

			// Return to the start of the file
			if (fseek(file, 0L, SEEK_SET) != 0) { fputs("Error returning to the start of the file", stderr); exit(2); }

			// Read the file's contents into our memory.
			newSize = fread(buffer, sizeof(char), bufferSize, file);

			if (ferror(file) != 0)
			{
				fputs("Error reading file", stderr);
			} else {
				buffer[newSize++] = '\0';
			}
		}
		fclose(file);
	}
	printf("File: %s\n\tSize (bytes): %zu", path, newSize);

	return buffer;
}

void print_buffer(char *buffer)
{
	char *ptr = buffer;
	while (*ptr != '\0')
	{
		printf("%c", *ptr);
		ptr++;
	}
}

/* --------------- Transliteration Functions --------------- */

void transliterate(char *buffer)
{
	/*
	 * Multi-language catching regex: \\([a-z]{2})\{([^}]+)\}
	 * E.g. The capture group of \ru{kak dela} is "kak dela".
	 * While using a regex library is more extensible, a simple parser better fits this use case (russian only). This is open to expansion through minor changes to the function.
	 */
	
	
}

int main(void)
{
	/*
	 * IDEAS:
	 * Recursively transliterate everything within the src folder, allowing for larger resources to be compiled 
	 * and moved as a single grouping of content (think multi-file learning resource with chapters etc.). 
	 * This would produce an identical file structure in /build. (lookup `file-tree-walk`)
	 */
	
	char *buffer;
	// Test path (relative to the build location
	char *test_path = "src/test/test.src.md";

	// TODO: Read the contents of a given file ending in .src.md, returning a string buffer of its contents.
	// Points our buffer's contents at the address of the temporary string buffer after read_file does the heavy lifting.
	buffer = read_file_to_buffer(test_path, buffer);
	
	// read_file testing print
	print_buffer(buffer);

	// TODO: Apply transliteration to text proceeding '\ru{'. terminating at the closing '}'.
	
	// TODO: Write the transliterated content into build/FILE_NAME.md (touch or fwrite).
	
	free(buffer);

	return 0;
}
