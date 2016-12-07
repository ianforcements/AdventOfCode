/******************************************************************************
	Advent of Code
	Day 4, Exercise 2
	by Ian Hutchinson
	ian.hutchinson@gmail.comi

	This code will:
	Given an input of encrypted room data containing decoy
	entries, use a provided method to remove the decoy data.
	Split the data into its components.
 	Decrypt the room data into plaintext.
	
	The output can be grepped for the room you require.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // used for strtol
#include <errno.h> // error handling when opening files
#include <ctype.h> // for isalpha and isdigit

#define MAX_LINE_SIZE 99

/*
	Data is passed through here line by line
	data has three components, name, sector ID and room checksum.
	only the length of the checksum is known (5 characters)
	we do know that the lines are formatted like so:
	room-name-sectorID[roomchecksum]
	we can use the position of the brackets and final dash to delimit the substrings
	Returns 1 if successful, 0 otherwise.
*/
int cleanAndSplitInput(char lineBuffer[], char *roomName, long *sectorID, char *roomChecksum)
{
	//variables for data validation
	int returnValue = 1;
	int closeBracketIndex = -1;
	int openBracketIndex = -1;
	int lastDashIndex = -1;
	int illegalCharacterFound = 0;
	
	char sectorIDSubstring[MAX_LINE_SIZE];

//	printf("LineBuffer: %s\n", lineBuffer);
	int length = strlen(lineBuffer);

	for (int i = length-1; i >= 0; i--) {
		if(lineBuffer[i] == ']') {
			if(closeBracketIndex == -1) {
				closeBracketIndex = i;
			}
			else {
				illegalCharacterFound++;
				break;
			}
		}
		else if(lineBuffer[i] == '[') {
			if(openBracketIndex == -1) {
				openBracketIndex = i;
			}
			else {
				illegalCharacterFound++;
				break;
			}
		}
		else if(lineBuffer[i] == '-') {
			if(lastDashIndex == -1) {
				lastDashIndex = i;
			}
		}
		else if(!((isalpha(lineBuffer[i])) || (isdigit(lineBuffer[i])))) {
			illegalCharacterFound++;
			break;
		}
	}
	//do some tests
	if(illegalCharacterFound != 0) {returnValue = 0;}
	if(closeBracketIndex != length - 1) {returnValue = 0;}
	if((closeBracketIndex - openBracketIndex) != 6) {returnValue = 0;}
	if((lastDashIndex == -1) || (lastDashIndex > openBracketIndex)) {returnValue = 0;}
	
	//scrape off the checksum
	char currentChar = '\0';
	for(int i = 0; i < 5; i++) {
		currentChar = lineBuffer[i+openBracketIndex+1];
		if(isalpha(currentChar)) {
			roomChecksum[i+1] = '\0';
			roomChecksum[i] = currentChar;
		} else {
			returnValue = 0;
		}
	}

	//scrape off the sector ID
	currentChar = '\0';
	int sectorIDSize = openBracketIndex - lastDashIndex -1;
	for(int i = 0; i < sectorIDSize; i++) {
		currentChar = lineBuffer[i+lastDashIndex+1];
		if(isdigit(currentChar)) {
			sectorIDSubstring[i+1] = '\0';
			sectorIDSubstring[i] = currentChar;
		} else {
			returnValue = 0;
		}
	}
	*sectorID = strtol(sectorIDSubstring, NULL, 10);
	
	//scrape off the room name
	currentChar = '\0';
	int j = 0;
	for(int i = 0; i < lastDashIndex; i++) {
		currentChar = lineBuffer[i];
		if((isalpha(currentChar)) || (currentChar == '-')) {
			roomName[j+1] = '\0';
			roomName[j] = currentChar;
			j++;
		} else {
			returnValue = 0;
		}
	}	
	return returnValue;
}

/*
	A room name is valid if the checksum equals the five most common letters in
	the name, arranged in order of how many times they appear, with ties broken
	by alphabetization.
	Returns 1 if room is valid, 0 otherwise.
*/
int validateRoom(char *roomName, char *providedRoomChecksum)
{
	char computedRoomChecksum[] = "\0\0\0\0\0\0";
	int roomNameLength = strlen(roomName);
	int letterFrequency[26];
	int topFive[] = {0,0,0,0,0};
	for (int i = 0; i < 26; i++) {
		letterFrequency[i] = 0;
	}
	for (int i = 0; i < roomNameLength; i++) {
		char currentChar = roomName[i];
		int alphabetIndex = currentChar - 'a';
		letterFrequency[alphabetIndex]++;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 25; j >= 0; j--) {
			if (letterFrequency[j] >= letterFrequency[topFive[i]]) {
				topFive[i] = j;
			}
		}
	letterFrequency[topFive[i]] = -1;
	topFive[i] += 'a';
	computedRoomChecksum[i] = topFive[i];
	}
	if(strcmp(computedRoomChecksum, providedRoomChecksum) == 0) {
		return 1;
	} else {
		return 0;
	}
}

/*
	The room names are encoded with a shift cipher. The shift is the sector ID.
	They're all lower case and any dashes get turned into spaces.
	Any non alpha, non-dash characters are filtered out by this point
*/	
int decipherRoomName(char *roomName, char *decipheredRoomName, long *sectorID)
{
	long cipherShift = *sectorID % 26L;
	int roomNameLength = strlen(roomName); 
	for (int i = 0; i < roomNameLength; i++) {
		decipheredRoomName[i+1] = '\0';
		if(isalpha(roomName[i])) {
			decipheredRoomName[i] = roomName[i] - 'a' + cipherShift;
			decipheredRoomName[i] %= 26;
			decipheredRoomName[i] += 'a';
		} else {
			decipheredRoomName[i] = ' ';
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	char lineBuffer[MAX_LINE_SIZE] = "";
	FILE *inputFile;
	char roomName[MAX_LINE_SIZE] = "";
	char decipheredRoomName[MAX_LINE_SIZE] = "";
	long sectorID = 0;
	char roomChecksum[6] = "";
	int validRooms = 0;
	int invalidRooms = 0;
	inputFile = fopen(argv[1], "r");
	if(!inputFile) {
		printf("Error opening file! %s\n", strerror(errno));
		return 1;
	}
	int newLinesAreFound = 1;
	int linesRead = 0;
	int errorLines = 0;
	while(newLinesAreFound > 0)
	{
		newLinesAreFound = fscanf(inputFile, "%s\n", lineBuffer);
		if (newLinesAreFound > 0) {
			linesRead++;
			if(cleanAndSplitInput(lineBuffer, roomName, &sectorID, roomChecksum)) {
				if(validateRoom(roomName, roomChecksum)) {
					validRooms++;
					decipherRoomName(roomName, decipheredRoomName, &sectorID);
					printf("Line:\t%d\tDeciphered:\t%s\t\tSector:\t%ld\n", linesRead, decipheredRoomName, sectorID);
				} else {
					invalidRooms++;
				}	
			} else {
				printf("Line:\t%d\tnot cleaned OK\n", linesRead);
				errorLines++;
			}
		}
	}
	printf("Complete!\n");
	printf("Processed %d lines and found %d lines with errors.\n", linesRead, errorLines);
	printf("Number of valid rooms: %d\n", validRooms);
	printf("Number of invalid/decoy rooms: %d\n", invalidRooms);
	return 0;
}
