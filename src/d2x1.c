#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int CheckBoundary(int x, int y)
{
	int positionIsOK = 0;
	if((x < 2) && (x > -2) && (y < 2) && (y > -2)) {
		positionIsOK = 1;
	}
	return positionIsOK;
}

void UpdatePosition(char direction, int *positionVector)
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
	if(CheckBoundary(newX, newY)) {
		positionVector[0] = newX;
		positionVector[1] = newY;
	}
	return;
}

void RecordCodeAtPosition(int *positionVector, char *passCode)
{
	char keyRange[] = "123456789";
	int keyIndex = positionVector[0]-(positionVector[1]*3)+4;
	char currentKey = keyRange[keyIndex];
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
	int positionVector[] = {0,0};
	char passCode[99] = "";
	FILE *inputFileBuffer;
	char lineBuffer[999] = "";
	int newLineFound = 0;
	inputFileBuffer = fopen(argv[1], "r");

	do {
		newLineFound = fscanf(inputFileBuffer, "%s", lineBuffer);
		if(newLineFound == 1) {
			for(int i=0; i<strlen(lineBuffer); i++) {
				UpdatePosition(lineBuffer[i], positionVector);
			}
			RecordCodeAtPosition(positionVector, passCode);
		}
	} while(newLineFound == 1);
	
	return 0;
}

