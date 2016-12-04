#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tests that a given position is within the boundaries
// Boundary mask is 1 within boundary, and 0 elsewhere
int CheckBoundary(int x, int y, int boundaryMask[7][7])
{
	x += 3;
	y += 3;
	int positionIsOK = boundaryMask[x][y];
	return positionIsOK;
}

// creates new position based on directions, then tests that it's a valid position
// if valid, it updates position
void UpdatePosition(char direction, int *positionVector, int boundaryMask[7][7])
{
	int newX = positionVector[0];
	int newY = positionVector[1];

	switch(direction) {
		case 'U':
			newY++;
			break;
		case 'D':
			newY--;
			break;
		case 'R':
			newX++;
			break;
		case 'L':
			newX--;
			break;
		default:
			printf("Bad instruction!\n");
	}
	if(CheckBoundary(newX, newY, boundaryMask)) {
		positionVector[0] = newX;
		positionVector[1] = newY;
	}
	return;
}

// Records the value of the keypad at current position and prints the current code
void RecordCodeAtPosition(int *positionVector, char *passCode, char keyMap[7][7])
{
	//this translation is required because position vector has origin in centre of keypad, key map has origin at corner
	int currentX = positionVector[0] + 3;
	int currentY = positionVector[1] + 3;
	char currentKey = keyMap[currentX][currentY];
	int passCodeLength = strlen(passCode);
	passCode[passCodeLength+1] = '\0';
	passCode[passCodeLength] = currentKey;
	printf("Current position is %d, %d.\n", positionVector[0], positionVector[1]);
	printf("This corresponds to the key %c.\n", currentKey);
	printf("The code so far is %s.\n", passCode);
	return;
}



int main(int argc, char *argv[], char *envp[])
{
	//init stuff
	int positionVector[] = {-2,0};
	char passCode[99] = "";
	FILE *inputFileBuffer;
	char lineBuffer[999] = "";
	int newLineFound = 0;
	inputFileBuffer = fopen(argv[1], "r");
	//boundary mask is 1 within bounds and 0 otherwise. It has one value of extra padding in each direction
	int boundaryMask[7][7] = {{0,0,0,0,0,0,0}, {0,0,0,1,0,0,0}, {0,0,1,1,1,0,0},{0,1,1,1,1,1,0}, {0,0,1,1,1,0,0}, {0,0,0,1,0,0,0}, {0,0,0,0,0,0,0}};
	//keymap could probably also replace the boundary mask. Exclamation point indicates forbidden location
	char keyMap[7][7] = {{'!','!','!','!','!','!','!'},{'!','!','!','5','!','!','!'}, {'!','!','A','6','2','!','!'}, {'!','D','B','7','3','1','!'}, {'!','!','C','8','4','!','!'}, {'!','!','!','9','!','!','!'}, {'!','!','!','!','!','!','!'}};

	do {
		newLineFound = fscanf(inputFileBuffer, "%s", lineBuffer);
		if(newLineFound == 1) {
			for(int i=0; i<strlen(lineBuffer); i++) {
				UpdatePosition(lineBuffer[i], positionVector, boundaryMask);
			}
			RecordCodeAtPosition(positionVector, passCode, keyMap);
		}
	} while(newLineFound == 1);
	
	return 0;
}

