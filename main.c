 /************************************************************************
  * 
  * pwgen - The random letter generator
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
	printf("Usage: pwgen [-n, nr of chars (default 8 - 16)] [-i, include all chars (default)] [-e,  exclude strange chars] [-o filename, output to a file]\n");
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
	char include_uncommon_chars;
	int c;
	FILE * fp;
	char filename[1024];	//Oh, bad programming. :/
	
	//Default output to stdout
	fp = stdout;
	
	opterr = 0;
	
	srand(time(NULL));

	nr_of_chars = slump(NR_OF_CHARS_MIN, NR_OF_CHARS_MAX);
	include_uncommon_chars = YES;	
	
	while((c = getopt(argc, argv, "ein:o:hv")) != -1) {
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
				nr_of_chars = atoi(optarg);
				printf("Generating %i chars\n", nr_of_chars);
				break;
			
			case 'o':
				printf("Writing to file %s\n", optarg);
				fp = fopen(optarg, "w");
				break;
			
			case 'h':
				help();
				return -1;	//Not an error, but we probably want to signal something to the system
				break;
				
			case 'v':
				version();
				return -1;	//Not an error, but we probably want to signal something to the system
				break;
				
			/* Some argument is missing, try to understand what */
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
	
	/* Generate a random pw */
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
						|| ((ch >= 0x61) && (ch <= 0x7a)))	//Allow small
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
