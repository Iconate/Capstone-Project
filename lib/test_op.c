#include "constants.h"
#include <time.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


//MATRIX PROPERTIES: determinant, hadamard ratio, compare, decrypt_badcyphertext 

/*! \brief Input: A Matrix. Output: value of the determinant of the matrix - to screen
 *
 * Calculates the determinant. 
 */
double determinant(gsl_matrix* m){
    int sig;
    double det;
    gsl_matrix *temp = gsl_matrix_calloc(SIZE,SIZE);
    gsl_matrix_memcpy(temp,m);
    gsl_permutation *perm = gsl_permutation_alloc(SIZE);
    gsl_linalg_LU_decomp (temp, perm ,&sig);
    gsl_permutation_free(perm);
    det = fabs(gsl_linalg_LU_det (temp,sig));
    gsl_matrix_free(temp);
    return det;
}

/*! \brief Input: A Matrix. Output: value of the hadamard ratio of the matrix - to screen
 *
 * Calculates the Hadamard ratio.
 */
void had(gsl_matrix* m){
    int i;
    double normvalue = 1;
    gsl_vector* mvector = gsl_vector_alloc(SIZE);
	
    for(i=0 ; i< SIZE; i++){
		gsl_vector_set_zero(mvector);
        gsl_matrix_get_row(mvector,m,i);
        normvalue = normvalue * gsl_blas_dnrm2(mvector);
    }
	
    double hadvalue = pow((determinant(m)/normvalue),(1./SIZE));
    printf("hadvalue: %lf\n",hadvalue);
}

//MATRIX PRINTING: printMatrix, printMatrixText

/*! \brief Input: A matrix. Output:elements of the matrix  - to screen
 *
 * Prints the elements of the matrix.
 */
void printMatrix(gsl_matrix* m){
    int i, j;
    for(i=0; i<SIZE; i++){
        for(j=0; j<SIZE; j++)
            printf("%f ;",gsl_matrix_get(m,i,j));
        printf("\n");
    }
}

/*! \brief Input: A vector. Output:elements of the vector  - to screen
 *
 * Prints the elements of the matrix.
 */
void printVector(gsl_matrix* m){
    int i;
    for(i=0; i<SIZE; i++){
		printf("%f; ",gsl_matrix_get(m,0,i));
    }
    printf("\n");
}

/*! \brief Input: A matrix. Output:elements of the matrix  - to screen
 *
 * Prints the elements of the matrix.
 */
gsl_matrix* createMatrix(){
	srand(time(NULL));
	int i;
	gsl_matrix* m = gsl_matrix_alloc(SIZE,SIZE);
	for(i =0 ; i< SIZE*SIZE; i++)
		gsl_matrix_set(m,i % SIZE,(int)i/SIZE,(rand() % RANGE*2)-RANGE);
	return m;
}


/*! \file test_op.c 
 *  \brief Contains shared testing methods.
 *
 * Matrix Properties (determinant, hadamard ratio, compare, decrypt_badcyphertext), 
 * Matrix Printing (printMatrix, printMatrixText)
 *  \ingroup methods test
 */
