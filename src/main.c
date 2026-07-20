/*
 * *****************************************************************************************
 * Name: Transliterate
 * Purpose: Provides a lightweight method of adding in-line cyrillic text to markdown files, 
 * 	    avoiding tedious copy pasting from browser pages or third party keyboard inputs. 
 * Author: Harvey Lopez
 * *****************************************************************************************
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Excessive backslashes are caused by the two layers of escaping (C strings & regex escaping)
#define FIND_RU "\\\\ru\\{([^}]+)\\}"

// Simple macro for finding the number of array items.
#define NUM_ITEMS(arr) sizeof(arr) / sizeof(arr[0])
// 
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
				exit(3);
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

char *transliterate(char *source, char *pattern)
{
	/*
	 * Multi-language catching regex: \\([a-z]{2})\{([^}]+)\}
	 * E.g. The capture group of \ru{kak dela} is "kak dela".
	 * This program will use regular expressions instead of a simple parser, making the program more extensible and open to supporting other language modules in the future.
	 */
	regex_t regex; 		// Location of compiled regex
	unsigned int ret_val;   // Return value of the regex.h function(s)
	regmatch_t pmatch[1];
	regoff_t off, len;
	char *ptr = source;

	// 1. Compile the regex
	ret_val = regcomp(&regex, pattern, REG_EXTENDED);
	
	if (ret_val != 0) { puts("Error compiling regular expression"); exit(4); }

	// 2. Execute it on the buffer's contents (regexec)
	printf("String = \"%s\"\n", source);
	printf("Matches:\n");

	for (unsigned int i = 0; ; i++)
	{
		if (regexec(&regex, source, NUM_ITEMS(pmatch), pmatch, 0))
		{
			break;
		}

		off = pmatch[0].rm_so + (ptr - source);
		len = pmatch[0].rm_eo - pmatch[0].rm_so;
		printf("#%u:\n", i);
		printf("offset = %jd; length = %jd\n", (intmax_t) off, (intmax_t) len);
		printf("substring = \"%.*s\"\n", len, ptr  + pmatch[0].rm_so);

		ptr += pmatch[0].rm_eo;
	}

	// 3. Read the matches (regmatch_t)
	// 4. Free any used memory (regfree)
	regfree(&regex);

	return source;
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
