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

int main(int argc, char * argv[])
{
	char i;
	unsigned char ch;
	int nr_of_chars;
	char include_uncommon_chars;
	int c;
	FILE * fp;
	char filename[1024];	//Oh, bad programming. :/
	
	fp = stdout;
	
	opterr = 0;
	
	srand(time(NULL));

	nr_of_chars = slump(NR_OF_CHARS_MIN, NR_OF_CHARS_MAX);
	include_uncommon_chars = YES;	
	
	while((c = getopt(argc, argv, "ein:o:h")) != -1) {
		switch(c) {
			case 'e':
				printf("Excluding strange chars\n");
				include_uncommon_chars = NO;	
				break;
			
			case 'i':
				printf("Including strange chars\n");
				include_uncommon_chars = YES;	
				break;
			
			case 'n':
				if(optarg != NULL) {
					nr_of_chars = atoi(optarg);
					printf("Generating %i chars\n", nr_of_chars);
				} else {
					printf("optarg == NULL!\n");
					return;
				}
				break;
			
			case 'o':
				printf("Writing to file %s\n", optarg);
				fp = fopen(optarg, "w");
				break;
			
			case 'h':
				help();
				return;
				break;
			case '?':
			
				/* Forgot to give number of chars in the argument list */
				if(optopt == 'n') {
					printf("Give a number for how many chars should be generated\nn: ");
					scanf("%i", &nr_of_chars);
				}
				
				/* Forgot to give filename in the argument list */
				if(optopt == 'o') {
					printf("Give a filename: ");
					scanf("%[^\n]", filename);
					fp = fopen(filename, "w");
				}
				break;
		}
	}
	
	//~ /* Help requested? */
	//~ if((argv > 1) && strncmp(argc[1], "--help", 6) == 0) {
		//~ version();
		//~ help();
		//~ return;
	//~ } 
//~ 
	//~ /* First parameter is the nr of chars to generate
	 //~ * Otherwise default to a random nr of chars */
	//~ if(argv > 1) {
		//~ nr_of_chars = atoi(argc[1]);
	//~ } else {
		//~ nr_of_chars = slump(NR_OF_CHARS_MIN, NR_OF_CHARS_MAX);
	//~ }
//~ 
	//~ include_uncommon_chars = YES;	
	//~ if(argv >= 3) {
		//~ if(strncmp(argc[2], "exclude", 7) == 0) {
			//~ printf("Excluding strange chars\n");
			//~ include_uncommon_chars = NO;
		//~ } 
	//~ }
	 
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
		
		fprintf(fp, "%c", ch);
	}
	
	/* Close the file, or print a newline if we wrote to stdout (screen) */
	if(fp != stdout) {
		fclose(fp);
	} else {
		printf("\n");
	}
	
	return 0;
}
