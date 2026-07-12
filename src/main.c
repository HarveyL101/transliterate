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
	const char[] data;
	size_t size;
	size_t length;
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

int main(void)
{
	FILE *f = fopen("test.txt", "rb");

	if (!f)
	{
		perror("fopen failed");
		return 1;
	}

	// Finds the filesize by jumping to the end and measuring distance between that and its startpoint.
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);

	char *buffer = malloc(size + 1);
	size_t = got fread(buffer, 1, size, f);
	buffer[got] = '\0';

	print("Read %zu bytes\n", got);
	fclose(f);
	free(buffer);

	return 0;
}
