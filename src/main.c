/*
 * *****************************************************************************************
 * Name: Transliterate
 * Purpose: Provides a lightweight method of adding in-line cyrillic text to markdown files, 
 * 	    avoiding tedious copy pasting from browser pages or third party keyboard inputs. 
 * Author: Harvey Lopez
 * *****************************************************************************************
 *
 * Process:
 * 	1. Read a target file (*.src.md) into a heap buffer.
 * 	2. Pass through the buffer until reaching a `\ru{` call.
 * 	3. Transliterate each character (Largest match first) using the RULES[] table.
 * 	4. Append these new characters into a new output buffer, growing as needed.
 * 	5. Write the finished output buffer into a new file (*.md).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

// Simple macro for finding the number of array items.
#define NUM_ITEMS(arr) sizeof(arr) / sizeof(arr[0])

/*
 * Two layers of escaping (\) are needed, one for C strings and another for regex.
 * The original regex is `\ru\{([^}]+)\}`.
 *
 * 	"\ru\{"		Finds literal matches to "\ru{".
 * 	"([^}]+)"	captures all characters that are not "}".
 * 	"\}"		Matches the closing "}" of the "\ru{" call.
 *
 */

#define FIND_RU "\\\\ru\\{([^}]+)\\}"

/* =====================
 * Transliteration Table 
 * ===================== */

typedef struct
{
	const char *latin; /* lowercase latin word to be matched. */
	const char *lowercase; /* cyrillic replacement, in lowercase */
	const char *uppercase; /* cyrillic replacement, in uppercase */
} Rule;

// The table is ordered longest-match-first, aiming to prevent single letter matches stealing from multi-letter sounds produced by single characters (E.g. 'zh').
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


/* ================
 * Buffer Functions 
 * ================ */
typedef struct {
	char *data; 
	size_t length;
	size_t capacity;
} Buffer;

static void buffer_init(Buffer *buf)
{
	buf->capacity = 64; /* Initial starting size, grows as required. */
	buf->length = 0;
	buf->data = malloc(buf->capacity);

	if (!buf->data) { fputs("Out of memory\n", stderr); exit(1); }
	buf->data[0] = '\0';
}

static void buffer_append(Buffer *buf, const char *src, size_t n)
{
	if (buf->length + n + 1 > buf->capacity)
	{
		size_t new_capacity = buf->capacity;
		while (buf->length + n + 1 > new_capacity)
		{
			new_capacity *= 2;
		}

		char *temp = realloc(buf->data, new_capacity);

		if (!temp) { fputs("Out of memory\n", stderr); exit(1); }

		buf->data = temp;
		buf->capacity = new_capacity;
	}
	memcpy(buf->data + buf->length, src, n);
	buf->length += n;
	buf->data[buf->length] = '\0';
}

/* Simple function for convenience when appending an already null-terminated C string. */
static void buffer_append_str(Buffer *buf, const char *str)
{
	buffer_append(buf, str, strlen(str));
}

/* =========================
 * Transliteration Functions
 * ========================= */
static char *transliterate_word(const char *word, size_t length)
{
	Buffer output;
	buffer_init(&output);

	size_t i = 0;
	while (i < length)
	{
		int matched = 0;

		/* Since RULES[] is ordered largest char to smallest, 
		 * greedy matching can be done by scanning top to bottom. */
		for (size_t rule = 0; rule < NUM_ITEMS(RULES); rule++)
		{
			size_t pos_length = strlen(RULES[rule].latin);
			if (pos_length > length - i) continue; /* Insufficient bytes for a match. */

			int ok = 1;
			for (size_t k = 0; k < pos_length; k++)
			{
				if (tolower((unsigned char)word[i +k]) != RULES[rule].latin[k])
				{
					ok = 0;
					break;
				}
			}
			if (!ok) continue;

			int is_upper = isupper((unsigned char)word[i]);
			buffer_append_str(&output, is_upper ? RULES[rule].uppercase : RULES[rule].lowercase);
			i += pos_length;
			matched = 1;
			break;
		}

		if (!matched)
		{
			/* Catches letters unable to be transliterated,
			 * E.g. numbers, spaces & punctuation. */
			buffer_append(&output, &word[i], 1);
			i++;
		}
	}

	return output.data; /* The function caller is responsible for freeing this heap memory. */
}

/* ===========================
 * File Manipulation Functions 
 * =========================== */
static char *read_file(const char *path, size_t *output_length)
{
	FILE *file = fopen(path, "rb");
	if (!file) { perror("Could not open the target file\n"); exit(1); }

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	char *buffer = malloc((size_t)size + 1);
	if (!buffer) { fputs("Out of memory\n", stderr); exit(1); }

	size_t got = fread(buffer, 1, (size_t)size, file);
	buffer[got] = '\0';
	fclose(file);

	if (output_length) *output_length = got;
	
	return buffer;
}

static char *find_and_replace(const char *source, const char *pattern)
{
	regex_t regex;
	/* [0] = whole \ru{...} contents match
	 * [1] = captured word */
	regmatch_t pmatch[2];

	if (regcomp(&regex, pattern, REG_EXTENDED) != 0) 
	{
		fputs("Failed to compile regex\n", stderr);
		exit(1);
	}

	Buffer output;
	buffer_init(&output);

	const char *cursor = source; /* Sliding pointer to track progress through `source`. */

	while (regexec(&regex, cursor, NUM_ITEMS(pmatch), pmatch, 0) == 0)
	{
		/* 1. Everything before the first match is plaintext and should be copied without alteration. */
		buffer_append(&output, cursor, (size_t)pmatch[0].rm_so);

		/* 2. Find the captured word's start and end using `cursor` and regmatch_t's "start of" / "end of". */
		const char *word = cursor + pmatch[1].rm_so; /* Word starting point relative to the cursor. */
		size_t word_length = (size_t)(pmatch[1].rm_eo - pmatch[1].rm_so); 
		
		/* 3. Transliterate the found slice and append the result to the output buffer. */
		char *cyrillic = transliterate_word(word, word_length);
		buffer_append_str(&output, cyrillic);
		free(cyrillic); /* Taking responsibility of memory handling since ownership was given by transliterate_word(). */
		
		/* 4. Shift cursor past the entire match (including closing brace) to avoid regexec() being stuck in the same \ru{...} call. */
		cursor += pmatch[0].rm_eo;
	}

	// Whatever remains after the last match is all plaintext, append to output buffer.
	buffer_append_str(&output, cursor);

	regfree(&regex);
	return output.data;
}

static void write_file(const char *path, const char *data, size_t length)
{
	FILE *file = fopen(path, "wb");
	if (!file) { perror("fopen failed.\n"); exit(1); }
	fwrite(data, 1, length, file);
	fclose(file);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input.src.md> <output.md>\n", argv[0]);
		return 1;
	}

	size_t input_length;
	char *source = read_file(argv[1], &input_length);

	char *result = find_and_replace(source, FIND_RU);

	write_file(argv[2], result, strlen(result));

	printf("Wrote %s (%zu bytes) from %s (%zu bytes)\n", argv[2], strlen(result), argv[1], input_length);

	free(source);
	free(result);

	return 0;
}
