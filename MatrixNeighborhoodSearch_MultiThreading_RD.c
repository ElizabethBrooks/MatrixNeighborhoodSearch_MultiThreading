/*
	Name: Elizabeth Brooks
	File: MatrixNeighborhoodSearch_MultiThreading
	Date Modified: May 12, 2016
*/
//Header inclusions
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include <math.h>
//Global class variables
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER; //Allow for mutual exclusion to handle critical sections
FILE *smf; //Store the binary file name globaly for each thread to access
unsigned long MAX_RANDOM_VALUES; //Set the maximum random range to the highest/widest column/row index
//Class methods
long generate_random(long maxVal) { //Method to return a random value in the half-open interval [0, maxVal]; Assuming 0 <= maxVal <= MAX_RANDOM_VALUES
    unsigned long numBins = (unsigned long) maxVal+1; //Divide the distriubtion to be randomly selected from into bins
    unsigned long randomNum = (unsigned long) MAX_RANDOM_VALUES+1; //The maximum value for the random number
    unsigned long binSize = randomNum/numBins; //Divide by bin size
    unsigned long errorCheck = randomNum%numBins; //Used to verify there are no errors in creating the random num
	long resultNum; //The random number to be returned
  	do {
   		resultNum = random(); //Set the random value
  	} while (randomNum - errorCheck <= (unsigned long)resultNum); //End do, while without overflow
  	return resultNum/binSize; //Return the truncated random number
} //End generate_random
void* neighborhood_search(void *numElementsInput) { //Method for performing neighborhood search functions
	int numElementsCurrent; //Number of matrix elements
	memcpy(&numElementsCurrent,(int *)numElementsInput,sizeof(int)); //Copy the number of elements
    int sideLength = sqrt(numElementsCurrent); //The size of the matrix rows and columns
    int sumNeighborhood, newInt = 0; //Sum of neighbor values
    int numNeighbors = 8; //Number of neighbors to check
	pthread_mutex_lock(&count_mutex);//Mutually exclude multiple threads from writing to the binary file at the same time
	int columnPos = generate_random(sideLength-1); //Randomly generate a column position
	int rowPos = generate_random(sideLength-1); //Randomly generate a row position
	//Seach the neighborhood and sum the current neighbors values
	fseek(smf, sizeof(int)*((columnPos-1)*sideLength+(rowPos-1)), SEEK_SET); //Neighbor one
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos)*sideLength+(rowPos-1)), SEEK_SET); //Neighbor two
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos+1)*sideLength+(rowPos-1)), SEEK_SET); //Neighbor three
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos-1)*sideLength+(rowPos)), SEEK_SET); //Neighbor four
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos+1)*sideLength+(rowPos)), SEEK_SET); //Neighbor five
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos-1)*sideLength+(rowPos+1)), SEEK_SET); //Neighbor six
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos)*sideLength+(rowPos+1)), SEEK_SET); //Neighbor seven
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*((columnPos+1)*sideLength+(rowPos+1)), SEEK_SET); //Neighbor eight
	if (ferror(smf)) { //Check if neighbor does not exist
		numNeighbors--; //Decremment numNeighbors
	}else{ //Neighbor does exist
		fread(&newInt, sizeof(int), 1, smf); //Read neighbor position
		sumNeighborhood += newInt; //Add the neighbors value
	} //End if, else
	fseek(smf, sizeof(int)*(columnPos*sideLength+rowPos), SEEK_SET); //Look for the specified random byte and position pointer
	if(sumNeighborhood > (numNeighbors/2)) { //Majority of neighbor values are 1
		newInt = 1; //Set value to be written to 1
		fwrite(&newInt, sizeof(int), 1, smf); //Change the specified byte
		printf("Value updated to 1 (Row: %i, Column: %i)\n", rowPos, columnPos); //Print changes to the screen
    } else if(sumNeighborhood < (numNeighbors/2)) { //Majority of neighbor values are 0
    	newInt = 0; //Set value to be written to 0
    	fwrite(&newInt, sizeof(int), 1, smf); //Change the specified byte
    	printf("Value updated to 0 (Row: %i, Column: %i)\n", rowPos, columnPos); //Print changes to the screen
    } else { //Either they had no neighbors or a majority could not be determined
    	printf("Value not updated (Row: %i, Column: %i)\n", rowPos, columnPos); //Print that no change occured to the screen
    } //End if, else
    pthread_mutex_unlock(&count_mutex);
} //End neighborhood_search
int main(void) { //The main method
	//Initialize variables
	int numElements; //Number of matrix elements
	int numThreads; //Number of threads to be generated
	int sumNeighborhoodMatrix; //Sum of neighborhood values
	int check = 1; //Check variable for file parsing and neighborhood searching
	int tempInt = 0; //Initialize temporary int value
	int threadNum = 0; //Thread array index
	printf("Please enter the number of desired elements for the square matrix: "); //Request user to input number of elements for the matrix
	scanf("%i",&numElements); //Save user input for number of matrix elements
	/*int squareElements = sqrt(numElements);
	if((squareElements%2) != 0) { //Require only squareable values for matrix size
		printf("Invalid square matrix size entered... program exited."); //Print error message to the screen
		return -1; //Exit program with error
	} //End if*/
	MAX_RANDOM_VALUES = numElements-1; //Set the max value to be randomly generated for column and row positions
	printf("Please enter the number of desired threads: "); //Request user to input number of thread processes
	scanf("%i",&numThreads); //Save user input for number of thread processes
	smf = fopen("squareMatrix_file.bin","wb"); //Open binary file to write random matrix values to
    if(smf == NULL) { //Catch errors in opening binary file
    	printf("Error creating file... program exited."); //Print error message to the screen
    	return -1; //Exit program with error
    } //End if
    int i = 0; //Initialize check variable
    for(i=0; i<numElements; i++) { //Write random 0's and 1's for the number of elements entered by user
    	tempInt = rand()%2; //Select a random 0 or 1; rand()%a generates values between 0 to a-1
    	fwrite(&tempInt, sizeof(int), 1, smf); //Write the random integer value to the open binary file
    } //End for
    fclose(smf); //Close the written to binary file
    smf = fopen("squareMatrix_file.bin","rb"); //Open binary file to change matrix values during neighborhood searches
    pthread_t threadPool[numThreads]; //Thread pool structure
    while(check>0) { //Loop through the file and perform neighborhood searches until all values are either 1 or 0
    	fflush(stdout); //Clear output buffer before reading matrix values
    	rewind(smf); //Reset pointer to beginning of binary file
    	for(i=0; i<numElements; i++) { //Loop through the binary file
	    	fread(&tempInt, sizeof(int), 1, smf); //Read each line
	    	sumNeighborhoodMatrix += tempInt; //Sum neighbor values
	    } //End for
    	if(sumNeighborhoodMatrix==0) { //Entire neighborhood is set to 0
    		check = -1; //Set check to FALSE to end program
    		printf("Entire neighborhood is set to 0!\n"); //Print program completion
    	} else if(sumNeighborhoodMatrix==numElements) { //Entire neighborhood is set to 1
    		check = -1; //Set check to FALSE to end program
    		printf("Entire neighborhood is set to 1!\n"); //Print program completion
    	} else { //Keep searching
			fflush(stdout);
			int Error = pthread_create(&threadPool[threadNum],NULL,neighborhood_search,(void *)&numElements); //Create threads for searching neighborhoods
			if (Error) { //Catch errors in creating threads
				printf("Error creating threads... program exited."); //Print error message to the screen
				return -1; //Exit program with error
			} else { //No error
				threadNum++; //Increment the current thread number
			} //End inner if, else
		} //End if, else
	} //End while
	for(i=0; i<numThreads; i++) { //Combine threads and ensure each has completed their operations
		pthread_join(threadPool[i],(void **)0); //Combine threads
	} //End for
	fclose(smf); //Close binary file	
	return 0; //End of program return statement
} //End main
