#include <stdio.h>
#include <unistd.h>

#define VERSION "0.5"

#define YES 1
#define NO 0

#define NR_OF_CHARS_MIN 8
#define NR_OF_CHARS_MAX 16

#define CHAR_MIN 0x21
#define CHAR_MAX 0x7e

void help(void)
{
	printf("Usage: pwgen [-n, nr_of_random_chars (default 8 - 16)] [-i, include all chars (default)] [-e,  exclude uncommon chars]\n");
}

void version(void)
{
	printf("pwgen %s (C) Copyright 2011 Marcus Jansson <mjansson256@gmail.com>\n", VERSION);
};

unsigned char slump(char min, char max)
{
	return (rand() % (max - min + 1)) + min; 
}

int main(int argv, char * argc[])
{
	char i;
	unsigned char ch;
	char nr_of_chars;
	char include_uncommon_chars;
	
	srand(time(NULL));

	//~ for(i = 0; i < argv; i++) {
		//~ printf("argc[%i]: %s\n", i, argc[i]);
	//~ }

	/* Help requested? */
	if((argv > 1) && strncmp(argc[1], "--help", 6) == 0) {
		version();
		help();
		return;
	} 

	/* First parameter is the nr of chars to generate
	 * Otherwise default to a random nr of chars */
	if(argv > 1) {
		nr_of_chars = atoi(argc[1]);
	} else {
		nr_of_chars = slump(NR_OF_CHARS_MIN, NR_OF_CHARS_MAX);
	}

	include_uncommon_chars = YES;	
	if(argv >= 3) {
		if(strncmp(argc[2], "exclude", 7) == 0) {
			printf("Excluding strange chars\n");
			include_uncommon_chars = NO;
		} 
	}
	
	for(i = 0; i < nr_of_chars; i++) {
		switch(include_uncommon_chars) {
			case YES:
				ch = slump(CHAR_MIN, CHAR_MAX);	//Get a char btwn 0x21 and 0x7e
				break;
			case NO:
				do {
					ch = slump(CHAR_MIN, CHAR_MAX); //Get a char btwn 0x21 and 0x7e
				} while(!(((ch >= 0x30) && (ch <= 0x39)) 
						|| ((ch >= 0x41) && (ch <= 0x5a)) 
						|| ((ch >= 0x61) && (ch <= 0x7a)))
					);
				break;
		}
		
		printf("%c", ch);
	}
	
	printf("\n");
	
	return 0;
}
