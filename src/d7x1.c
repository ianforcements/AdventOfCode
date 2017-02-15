/******************************************************************************
*
*	Advent of Code
*	Day 7, Exercise 1
*	by Ian Hutchinson
*	ian.hutchinson@gmail.com
*
*	github.com/ianforcements
*
*	This code will:
*	for a supplied data file, read each line and determine if there are
*	any four character sequences which consist of a reverse two character
*	pair (eg. abba or zxxz)
*	input lines may include square brackets, if the sequence is within
*	brackets, then the line is considered invalid.
*
*	I have tried to eschew the use of functions in this one as I was 
*	running into issues with variable scope and passing pointers.
*	I'm intending to avoid them until I've covered that section of Zed's
*	book.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h> //for isalpa

#define MAX_LINE_SIZE 512

int main (int argc, char *argv[])
{

	FILE *inputFile;
	char lineBuffer[MAX_LINE_SIZE] = "\0";
	int lineBufferSize = 0;
	if(argc != 2) {
		printf("Usage: d7x1 inputfile\n");
		return 1;
	}
	
	inputFile = fopen(argv[1], "r");
	if(!inputFile) {
		printf("Error opening file! %s\n", strerror(errno));
		return 1;
	}

	int newLinesAreFound = 1;
	int linesRead = 0;
	int errorLines = 0;
	int validLines = 0;
	int currentStringContainsABBA = 0;
	int currentStringContainsBracketABBA = 0;
	int withinBrackets = 0;
	char currentChar = '\0';
	char nextFourChars[4] = {'\0', '\0', '\0', '\0' };
	
	while(newLinesAreFound > 0) {
		newLinesAreFound = fscanf(inputFile, "%s", lineBuffer);
		if(newLinesAreFound == 1) {
			linesRead++;
			currentStringContainsABBA = 0;
			currentStringContainsBracketABBA = 0;
			withinBrackets = 0;
			lineBufferSize = strlen(lineBuffer);
			for (int i = 0; i < lineBufferSize; i++) {
				currentChar = lineBuffer[i];
				if(currentChar == '[') {
					withinBrackets++;
				} else if(currentChar == ']') {
					withinBrackets--;
				} else if((lineBufferSize - i) >= 4) {
					nextFourChars[0] = lineBuffer[i];
					nextFourChars[1] = lineBuffer[i + 1];
					nextFourChars[2] = lineBuffer[i + 2];
					nextFourChars[3] = lineBuffer[i + 3];
					if ((nextFourChars[0] == nextFourChars[3]) && (nextFourChars[1] == nextFourChars[2]) && (nextFourChars[0] != nextFourChars[1])) {
						if(withinBrackets == 0) {
							currentStringContainsABBA = 1;
						} else {
							currentStringContainsBracketABBA = 1;
						}
					}
				}
			}
			if (withinBrackets > 0) {
				printf("Error: open bracket on line %d\n", linesRead);
				errorLines++;
			}
			if ((currentStringContainsABBA == 1) && (currentStringContainsBracketABBA == 0)) {	
				validLines++;
			}
		}
	}
	printf("Complete.\n");
	printf("Read %d lines and found %d errors.\n", linesRead, errorLines);
	printf("There are %d valid lines.\n", validLines);

	return 0;
}
