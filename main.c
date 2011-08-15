 /************************************************************************
  * 
  * pwgen - The random string generator
  * Copyright (C) 2001  Marcus Jansson <mjansson256@yahoo.se>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  * 
  ************************************************************************/

#include <stdio.h>
#include <unistd.h>

#define VERSION "0.6"

#define YES 1
#define NO 0

#define NR_OF_CHARS_MIN 8
#define NR_OF_CHARS_MAX 16

#define CHAR_MIN 0x21
#define CHAR_MAX 0x7e


void help(void)
{
	printf("Usage: pwgen [options]\n \
Options:\n \
 -v         Verbose, must be first option.\n \
 -l <len> 	Length of random string to generate (default is random 8-16)\n \
 -n <nr>    Repeat random string generation <nr> of times\n \
 -e 		Exclude strange letters\n \
 -i			Include strange letters (default)\n \
 -o <file>	Output random strings to file\n \
 -a <file>  Append random strings to file\n \
 ");
}

void version(void)
{
	printf("pwgen %s (C) Copyright 2011 Marcus Jansson <mjansson256@gmail.com>\n", VERSION);
	printf("This program comes with ABSOLUTELY NO WARRANTY.\n \
    This is free software, and you are welcome to redistribute it\n \
    under certain conditions.\n");
};

unsigned char slump(char min, char max)
{
	return (rand() % (max - min + 1)) + min; 
}

int main(int argc, char * argv[])
{
	unsigned int i;
	unsigned char ch;
	unsigned int nr_of_chars;
	unsigned int repeat = 1;
	char repeat_random_len;
	char include_uncommon_chars;
	int c;
	FILE * fp;
	char filename[512];	//Oh, bad programming. :/
	char verbose = NO;
	
	//Default output to stdout
	fp = stdout;
	
	opterr = 0;
	
	srand(time(NULL));

	nr_of_chars = slump(NR_OF_CHARS_MIN, NR_OF_CHARS_MAX);
	repeat_random_len = YES;
	include_uncommon_chars = YES;	
		
	while((c = getopt(argc, argv, "ein:r:o:a:hvV")) != -1) {
		switch(c) {
			/* Exclude strange chars */
			case 'e':
				include_uncommon_chars = NO;	
				break;
			
			/* Include strange chars */
			case 'i':
				include_uncommon_chars = YES;	
				break;
			
			/* Number of chars */
			case 'l':
				nr_of_chars = atoi(optarg);
				repeat_random_len = NO;
				break;
			
			/* Repeat */
			case 'n':
				repeat = atoi(optarg);
				break;
			
			/* Output to file */
			case 'o':
				if(verbose == YES) {
					printf("Writing to file %s\n", optarg);
				}
				fp = fopen(optarg, "w");
				break;
			
			/* Append to file */
			case 'a':
				if(verbose == YES) {
					printf("Appending to file %s\n", optarg);
				}
				fp = fopen(optarg, "a");
				fprintf(fp, "\n");
				break;
			
			/* Help */
			case 'h':
				help();
				return -1;	//Not an error, but we probably want to signal something to the system
				break;
				
			/* Verbose */
			case 'v':
				verbose = YES;
				break;
				
			/* Version */
			case 'V':
				version();
				return -1;	//Not an error, but we probably want to signal something to the system
				break;
				
			/* Some argument is missing, try to understand what */
			case '?':
			
				/* Forgot to give number of chars in the argument list */
				if(optopt == 'l') {
					printf("Length of string: ");
					scanf("%i", &nr_of_chars);
				}
				
				/* Forgot to give number of repeats in the argument list */
				if(optopt == 'n') {
					printf("Repeat nr: ");
					scanf("%i", &repeat);
				}
				
				/* Forgot to give new filename in the argument list */
				if(optopt == 'o') {
					printf("Create file: ");
					scanf("%511[^\n]", filename);	//TODO: len
					fp = fopen(filename, "w");
				}
				
				/* Forgot to give append filename in the argument list */
				if(optopt == 'a') {
					printf("Append to file: ");
					scanf("%511[^\n]", filename);	//TODO: len
					fp = fopen(filename, "a");
				}
				break;
		}
	}
	
	if(verbose == YES) {
		printf("Generating %i chars.\n", nr_of_chars);
		printf("Generating %i random string", repeat);
		if(repeat > 1) { 
			printf("s");
		}
		printf(".\n");
		switch(include_uncommon_chars) {
			case YES:
				printf("Including strange chars\n");
				break;
			case NO:
				printf("Excluding strange chars\n");
				break;
			}
	}
	
	/* Generate 'repeat' nr of strings */
	while(repeat-- != 0) {
		/* Generate a random string */
		for(i = 0; i < nr_of_chars; i++) {
			switch(include_uncommon_chars) {
				case YES:
					ch = slump(CHAR_MIN, CHAR_MAX);				//Get a char (normally btwn 0x21 and 0x7e)
					break;
				case NO:
					do {
						ch = slump(CHAR_MIN, CHAR_MAX); 		//Get a char (normally btwn 0x21 and 0x7e)
					} while(!(((ch >= 0x30) && (ch <= 0x39)) 	//Allow numbers
							|| ((ch >= 0x41) && (ch <= 0x5a)) 	//Allow big letters
							|| ((ch >= 0x61) && (ch <= 0x7a)))	//Allow small letters
						);
					break;
			}
			fprintf(fp, "%c", ch);
		}
		fprintf(fp, "\n");
		if(repeat_random_len == YES) {
			nr_of_chars = slump(NR_OF_CHARS_MIN, NR_OF_CHARS_MAX);
		}
	} 
	
	/* Close the file, or print a newline if we wrote to stdout (screen) */
	if(fp != stdout) {
		fclose(fp);
	} 
	
	return 0;
}
