Name: Elizabeth Brooks
Date Modified: May 12, 2016
File: CS470 Lab3

#MatrixNeighborhoodSearch_MultiThreading
Lab 3: "Given a sequence of numbers x0, x1, …xn ϵ {0,1} in a binary file, where xi representssquare
matrix element in a linear fashion (concatenate the lines of the matrix for example), write in
C/C++ a simulation process as follows: a) start M threads, b) each thread generates a column and
a row randomly, c) considering the element location in the matrix given by the generated column
and row change the value of the matrix element to 0 if its 4/8 neighbors are predominantly 0,
and 1 otherwise." --> http://www.cwu.edu/~szilardv/images/uploadedImgs/Operating_Systems_Lab_3.pdf

##To compile: (Using library linkers)
$ gcc -o test MatrixNeighborhoodSearch_MultiThreading.c -lpthread -lm
$ ./test

Once the program is running, the user must enter a square matrix number of elements and the number of threads desired.
