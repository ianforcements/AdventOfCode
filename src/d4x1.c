/*******************************************************************************
	Advent of Code
	Day 4, Exercise 1
	by Ian Hutchinson
	ian.hutchinson@gmail.com

	This code will:
	Given an input of garbled-but-formatted room data containing decoy
	entries, use a provided method to remove the decoy data.
	Split the data into its components.
	Provide the sum of sector IDs for the valid rooms.
*******************************************************************************/

#include <stdio.h> //printf
#include <string.h> // string manipulation
#include <stdlib.h> // used for strtol
#include <errno.h> // error handling used when opening files
#include <ctype.h> // for isalpha and isdigit

#define MAX_LINE_SIZE 99


/*
	data is passed through here line by line
	data has three components, name, sector ID and room checksum.
  	only the length of the checksum is known (5 characters)
  	we do know that the lines are formatted like so:
  	room-name-sectorID[roomchecksum]
	we can use the position of the brackets and final dash to delimit the substrings
*/
int cleanAndSplitInput(char lineBuffer[], char *roomName, long *sectorID, char *roomChecksum)
{
	// variables for data validation
	int returnValue = 1;
	int closeBracketIndex = -1;
	int openBracketIndex = -1;
	int lastDashIndex = -1;
	int illegalCharacterFound = 0;
	
	char sectorIDSubstring[MAX_LINE_SIZE];

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
	//do some tests to confirm data is ok
	if(illegalCharacterFound != 0) {printf("Input error: Illegal character!\n"); returnValue = 0;}
	if(closeBracketIndex != length - 1) {printf("Input error: Close bracket in bad position!\n");returnValue = 0;}
	if((closeBracketIndex - openBracketIndex) != 6) {printf("Input error: Brackets incorrectly separated!\n");returnValue = 0;}
	if((lastDashIndex == -1) || (lastDashIndex > openBracketIndex)) {printf("Input error: Final dash in bad position!\n");returnValue = 0;}
	
	//scrape off the checksum
	char currentChar = '\0';
	for(int i = 0; i < 5; i++) {
		currentChar = lineBuffer[i+openBracketIndex+1];
		if(isalpha(currentChar)) {
			roomChecksum[i+1] = '\0';
			roomChecksum[i] = currentChar;
		} else {
			printf("Input error: Checksum contains bad character!\n");
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
			printf("Input error: Sector ID contains bad character! %c\n", currentChar);
			returnValue = 0;
		}
	}
	*sectorID = strtol(sectorIDSubstring, NULL, 10);
	
	//scrape off the room name
	currentChar = '\0';
	int j = 0;
	for(int i = 0; i < lastDashIndex; i++) {
		currentChar = lineBuffer[i];
		if(isalpha(currentChar)) {
			roomName[j+1] = '\0';
			roomName[j] = currentChar;
			j++;
		} else if (currentChar == '-') {
			continue;
		}
		else {
			printf("Input error: Room name contains bad character!\n");
			returnValue = 0;
		}
	}	
	return returnValue;
}

/*
	A room name is valid if the checksum equals the five most common letters in
	the name, arranged in order of how many times they appear, with ties broken
	by alphabetization.
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
	

int main(int argc, char *argv[])
{
	char lineBuffer[MAX_LINE_SIZE] = "";
	FILE *inputFile;
	char roomName[MAX_LINE_SIZE] = "";
	long sectorID = 0;
	char roomChecksum[6] = "";
	long roomSectorIDSum = 0;
	int validRooms = 0;
	int invalidRooms = 0;
	int errorLines = 0;

	inputFile = fopen(argv[1], "r");
	if(!inputFile) {
		printf("Error opening file! %s\n", strerror(errno));
		return 1;
	}
	int newLinesAreFound = 1;
	int linesRead = 0;
	while(newLinesAreFound > 0)
	{
		newLinesAreFound = fscanf(inputFile, "%s\n", lineBuffer);
		if (newLinesAreFound > 0) {
			linesRead++;
			if(cleanAndSplitInput(lineBuffer, roomName, &sectorID, roomChecksum)) {
				if(validateRoom(roomName, roomChecksum)) {
					validRooms++;
					roomSectorIDSum += sectorID;
					
				} else {
					invalidRooms++;
				}	
			} else {
				errorLines++;
			}
		}
	}
	printf("Complete!\n");
	printf("Processed %d lines and found %d lines with errors.\n", validRooms + invalidRooms + errorLines, errorLines);
	printf("Number of valid rooms: %d\n", validRooms);
	printf("Number of invalid/decoy rooms: %d\n", invalidRooms);
	printf("Sum of Sector IDs of valid rooms: %ld\n", roomSectorIDSum);

	return 0;
}
