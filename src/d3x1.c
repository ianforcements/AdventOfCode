#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

int TriangleIsValid( int currentTriangle[3])
{
	// this exercise uses a specific method for evaluating if a triangle is correct
	// according to the exercise, a triangle is correct if any two sides add up
	// to greater than the third side
	// for the purpose of this exercise, this means a triangle is correct if
	// the greatest side is smaller than the sum of the remaining two sides
	int sumOfSides = currentTriangle[0] + currentTriangle[1] + currentTriangle[2];

	// find the largest side
	int largestSide = 0;
	if (currentTriangle[0] > currentTriangle[1]) {
		largestSide = currentTriangle[0];
	} else {
		largestSide = currentTriangle[1];
	}
	if (currentTriangle[2] > largestSide) {
		largestSide = currentTriangle[2];
	}
	if (sumOfSides - largestSide > largestSide) {
		return 1;
	} else{
		return 0;
	}
}

int main (int argc, char *argv[], char *envp[])
{
	int validTriangles = 0;
	int currentTriangle[3] = {0,0,0};
	FILE * inputFileBuffer;
	inputFileBuffer = fopen(argv[1], "r");
	int newDataFound = 0;
	//int a = 0;
	//int b = 0;
	//int c = 0;
	do {
		//clear out the triangle data
		for(int i = 0; i < 3; i++) {
			currentTriangle[i] = 0;
		}
		//read the next line of triangle data
		newDataFound = fscanf(inputFileBuffer, "%d %d %d\n", &currentTriangle[0],&currentTriangle[1],&currentTriangle[2]);
		if(newDataFound == 3) {
			if(TriangleIsValid(currentTriangle)) {
				validTriangles++;
			}
		} else if (newDataFound > 0) {
			printf("Incomplete triangle in input file!\n");
		} else {
			printf("End of file.\n");
		}
	} while (newDataFound == 3);
	printf("There are %d valid triangles.\n", validTriangles); 		
	return 0;
}

