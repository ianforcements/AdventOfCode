#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int displacementVector[] = {0,0}; // Displacement in blocks, North then East
int currentFacing = 0; //current facing in degrees clockwise from North

void  UpdateDisplacement(int travelDistance)
{
	currentFacing %= 360;
	switch(currentFacing) {
		case 0:
			displacementVector[0] += travelDistance;
			break;
		case 90:
			displacementVector[1] += travelDistance;
			break;
		case 180:
			displacementVector[0] -= travelDistance;
			break;
		case 270:
			displacementVector[1] -= travelDistance;
			break;
		default:
			printf("Bad angle error");
	}
	printf("Current position: %d blocks North, %d blocks East.\n", displacementVector[0], displacementVector[1]);
	return;
}
			

int  main()
{
	char input[] = "R3, L2, L2, R4, L1, R2, R3, R4, L2, R4, L2, L5, L1, R5, R2, R2, L1, R4, R1, L5, L3, R4, R3, R1, L1, L5, L4, L2, R5, L3, L4, R3, R1, L3, R1, L3, R3, L4, R2, R5, L190, R2, L3, R47, R4, L3, R78, L1, R3, R190, R4, L3, R4, R2, R5, R3, R4, R3, L1, L4, R3, L4, R1, L4, L5, R3, L3, L4, R1, R2, L4, L3, R3, R3, L2, L5, R1, L4, L1, R5, L5, R1, R5, L4, R2, L2, R1, L5, L4, R4, R4, R3, R2, R3, L1, R4, R5, L2, L5, L4, L1, R4, L4, R4, L4, R1, R5, L1, R1, L5, R5, R1, R1, L3, L1, R4, L1, L4, L4, L3, R1, R4, R1, R1, R2, L5, L2, R4, L1, R3, L5, L2, R5, L4, R5, L5, R3, R4, L3, L3, L2, R2, L5, L5, R3, R4, R3, R4, R3, R1,";

//	char input[] = "R3100, L12345678, L10";

	char travelDistanceString[] = "";
	int travelDistance = 0;
	int iterateTo = strlen(input);

	for(int i = 0; i < iterateTo; i++) {
		switch(input[i]) {
			case 'R':
				currentFacing += 270;
				printf("Turning right!\n");
				break;
			case 'L':
				currentFacing += 90;
				printf("Turning left!\n");
				break;
			case ' ':
				break;
			case ',':
				travelDistance = atoi(travelDistanceString);
				printf("Moving %d forwards!\n", travelDistance);
				UpdateDisplacement(travelDistance);
				strcpy(travelDistanceString,"");
				travelDistance = 0;
				break;
			default:	
				travelDistanceString[strlen(travelDistanceString)+1] = '\0';
				travelDistanceString[strlen(travelDistanceString)] = input[i];
		}
	}
	int absoluteY = abs(displacementVector[0]);
	int absoluteX = abs(displacementVector[1]);
	int displacementInBlocks = absoluteX + absoluteY;
	printf("The target site is %d blocks away.\n", displacementInBlocks);
	return 0;
}
