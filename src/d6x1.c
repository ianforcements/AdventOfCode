/******************************************************************************
*	Advent of Code
*	Day 6, Both exercises.
*	by Ian Hutchinson
*	ian.hutchinson@gmail.com
*
*	This code will:
*	Given an input file of 8-character strings, find the distribution
*	of letters appearing in each position, then compose a string featuring
*	the most or least common character in each position
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	if(argc == 3) {
		if((argv[2] != "most") || (argv[2] != "least")) {
			printf("Second parameter must be 'most' or 'least'.\n");
			return 1;
		} 
	} else {
	 	printf("Usage: d6x1 filename most/least\n");
		return 1;
	}

	FILE *inFile = fopen(argv[1], "r");
	if(!inFile) {
		printf("Error opening file! %s\n", strerror(errno));
		return 1;
	}
	int newLinesAreFound = 1;
	int linesRead = 0;
	int errorLines = 0;
	char lineBuffer[9];
	int lineBufferSize = 0;
	int messageCharFrequency[8][26];
	char currentChar = '\0';
	char reconstructedMessage[] = "\0\0\0\0\0\0\0\0";
	
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 26; j++) {
			messageCharFrequency[i][j] = 0;
		}
	}

	while(newLinesAreFound > 0) {
		newLinesAreFound = fscanf(inFile, "%s", lineBuffer);
		if(newLinesAreFound == 1) {
			linesRead++;
			lineBufferSize = strlen(lineBuffer);
			if(lineBufferSize != 8) {
				errorLines++;
				continue;
			}

			for(int i = 0; i < lineBufferSize; i++) {
				currentChar = lineBuffer[i];
				if(!isalpha(currentChar)) {
					errorLines++;
					break;
				}
				currentChar = tolower(currentChar);
				messageCharFrequency[i][currentChar-'a']++;
			}
		}
	}
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 26; j++) {
			if(argv[2] == "least") {
				if(messageCharFrequency[i][reconstructedMessage[i]] > messageCharFrequency[i][j]) {
					reconstructedMessage[i] = j;
				}
			} else {
				if(messageCharFrequency[i][reconstructedMessage[i]] < messageCharFrequency[i][j]) {
				reconstructedMessage[i] = j;
				}
			}
		}
		reconstructedMessage[i] += 'a';
	}
	printf("Done.\n");
	printf("Read %d lines and found %d lines with errors.\n", linesRead, errorLines);
	printf("Reconstructed message:\t%s\n", reconstructedMessage);
	return 0;			
}	
