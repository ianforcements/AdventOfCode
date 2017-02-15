/******************************************************************************
*
*	Advent of Code
*	Day 7, Exercise 2
*	by Ian Hutchinson
*	ian.hutchinson@gmail.com
*
*	github.com/ianforcements
*
*	This code will:
*	for a supplied data file, read each line for three
*	character 'ABA" sequences outside of the brackets. If it finds one, it then
*	searches within the brackets for a corresponding inverted sequence.
*	(eg. aba and bab, or zxz and xzx)
*
*	I have tried to eschew the use of functions in this one as I was 
*	running into issues with variable scope and passing pointers.
*	I'm intending to avoid them until I've covered that section of Zed's
*	book.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h> //for isalpha

#define MAX_LINE_SIZE 512

// determines whether the line closes with incomplete brackets and is long enough
int lineContainsErrors(char *lineBuffer) 
{
	int length = strlen(lineBuffer);
	char currentChar = '\0';
	int withinBrackets = 0;
	if (length < 8) { //three characters for ABA outside brackets, 2 for brackets, 3 for character inside brackets
		return 1;
	}
	for (int i = 0; i < length; i++) {
		currentChar = lineBuffer[i];
		if (currentChar == '[') {
			withinBrackets++;
		} else if (currentChar == ']') {
			withinBrackets--;
		}
	}
	if (withinBrackets == 0) {
		return 0;
	} else {
		return 1;
	}
}

int ABABABisAlphaOnly (char ABA[], char BAB[])
{
	for (int i = 0; i < 3; i++) {
		if((isalpha(ABA[i]) == 0) || (isalpha(BAB[i] == 0))) {
			return 0;
		}
	}
	return 1;
}

int containsBAB (char BAB[], char lineBuffer[])
{
	int withinBrackets = 0;
	int length = strlen(lineBuffer);
	int shortLength = length - 2;
	for (int i = 0; i < shortLength; i++) {
		if (lineBuffer[i] == '[') {
			withinBrackets++;
		} else if (lineBuffer[i] == ']') {
			withinBrackets--;
		} else if (withinBrackets == 1) {
			if ((lineBuffer[i] == BAB[0]) && (lineBuffer[i+1] == BAB[1]) && (lineBuffer[i+2] == BAB[2])) {
				return 1;
			}
		}
	}
	return 0;
}

int main (int argc, char *argv[])
{

	FILE *inputFile;
	char lineBuffer[MAX_LINE_SIZE] = "\0";
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
	char ABA[4] = {0};
	char BAB[4] = {0};
	int lineBufferLength = 0;
	int withinBrackets = 0;
	
	while(newLinesAreFound > 0) {
		newLinesAreFound = fscanf(inputFile, "%s", lineBuffer);
		lineBufferLength = strlen(lineBuffer);
		if(newLinesAreFound == 1) {
			linesRead++;
			if(lineContainsErrors(lineBuffer)) {
				errorLines++;
			} else {
				for (int i = 0; i < lineBufferLength - 2; i++) {
					if (lineBuffer[i] == '[') {
						withinBrackets++;
					} else if (lineBuffer[i] == ']') {
						withinBrackets--;
					} else if (withinBrackets == 0) {
						if ((lineBuffer[i] == lineBuffer[i+2]) && (lineBuffer[i] != lineBuffer[i+1])) {
							ABA[0] = lineBuffer[i];
							ABA[1] = lineBuffer[i+1];
							ABA[2] = lineBuffer[i+2];
							BAB[0] = ABA[1];
							BAB[1] = ABA[0];
							BAB[2] = ABA[1];
							if (ABABABisAlphaOnly(ABA, BAB)) {
								if (containsBAB(BAB, lineBuffer)) {
									validLines++;
									i = lineBufferLength; //end loop
								}
							}
						}
					}
				}
			}
		}
	}
	printf("Complete.\n");
	printf("Read %d lines and found %d errors.\n", linesRead, errorLines);
	printf("There are %d valid lines.\n", validLines);

	return 0;
}
