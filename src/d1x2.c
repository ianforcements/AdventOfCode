#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {gridSize = 10000};
int displacementVector[] = {gridSize/2,gridSize/2}; // Displacement in blocks, North then East
int currentFacing = 0; //current facing in degrees clockwise from North

struct {
	unsigned int visited : 1;
} grid[gridSize][gridSize];

int  UpdateDisplacement(int travelDistance)
{
	for (int i = 1; i <= travelDistance; i++) {
		currentFacing %= 360;
		switch(currentFacing) {
			case 0:
				displacementVector[0]++;
				break;
			case 90:
				displacementVector[1]++;
				break;
			case 180:
				displacementVector[0]--;
				break;
			case 270:
				displacementVector[1]--;
				break;
			default:
				printf("Bad angle error");
		}
		printf("Current position: %d blocks North, %d blocks East.\n", displacementVector[0]-gridSize/2, displacementVector[1]-gridSize/2);
		if(MarkGridAtDisplacementVector() != 1){
			printf("Revisited location!");
			return 0;
		}
	}
	return 1;
}

int MarkGridAtDisplacementVector()
{
	//this will return 1 the first time a location has been visited, and 0 the second time.
	//the search stops after the second time
	return ++grid[displacementVector[0]][displacementVector[1]].visited;
}
			

int  main()
{
	char input[] = "R3, L2, L2, R4, L1, R2, R3, R4, L2, R4, L2, L5, L1, R5, R2, R2, L1, R4, R1, L5, L3, R4, R3, R1, L1, L5, L4, L2, R5, L3, L4, R3, R1, L3, R1, L3, R3, L4, R2, R5, L190, R2, L3, R47, R4, L3, R78, L1, R3, R190, R4, L3, R4, R2, R5, R3, R4, R3, L1, L4, R3, L4, R1, L4, L5, R3, L3, L4, R1, R2, L4, L3, R3, R3, L2, L5, R1, L4, L1, R5, L5, R1, R5, L4, R2, L2, R1, L5, L4, R4, R4, R3, R2, R3, L1, R4, R5, L2, L5, L4, L1, R4, L4, R4, L4, R1, R5, L1, R1, L5, R5, R1, R1, L3, L1, R4, L1, L4, L4, L3, R1, R4, R1, R1, R2, L5, L2, R4, L1, R3, L5, L2, R5, L4, R5, L5, R3, R4, L3, L3, L2, R2, L5, L5, R3, R4, R3, R4, R3, R1,";

//	char input[] = "R8, R4, R4, R8,"; //test case
	
	int targetNotReached = MarkGridAtDisplacementVector();	
	char travelDistanceString[] = "";
	int travelDistance = 0;
	int iterateTo = strlen(input);
	for(int i = 0; i < iterateTo; i++) {
		switch(input[i]) {
			case 'R':
				currentFacing += 90;
				printf("Turning right!\n");
				break;
			case 'L':
				currentFacing += 270;
				printf("Turning left!\n");
				break;
			case ' ':
				break;
			case ',':
				travelDistance = atoi(travelDistanceString);
				printf("Moving %d forwards!\n", travelDistance);
				targetNotReached = UpdateDisplacement(travelDistance);
				strcpy(travelDistanceString,"");
				travelDistance = 0;
				break;
			default:	
				travelDistanceString[strlen(travelDistanceString)+1] = '\0';
				travelDistanceString[strlen(travelDistanceString)] = input[i];
		}
		if(targetNotReached != 1) {
			break;
		}
	}
	int absoluteY = abs(displacementVector[0]-gridSize/2);
	int absoluteX = abs(displacementVector[1]-gridSize/2);
	int displacementInBlocks = absoluteX + absoluteY;
	printf("\n\n+++Target reached+++\n\n");
	printf("The target site is %d blocks from the origin point.\n", displacementInBlocks);
	return 0;
}
