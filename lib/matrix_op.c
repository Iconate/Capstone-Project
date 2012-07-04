#include "constants.h"
#include <time.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "strnlen.h"
#include "strnlen.c"

// MATRIX MANIPULATION: check, chartomat, mattochar,inverse, randomLine, clearLine, compare, writeUni, readUni 
gsl_matrix *genM();

/*! \brief Input: Matrix Output: Int
 *
 * Checks that the matrix input has proper ascii values
*/
int check(gsl_matrix *m){
	int i;
	int ret = 0;
	double temp;
	for(i = 0; i < SIZE; i++){
		temp = (double)gsl_matrix_get(m,0,i);
		if( temp < 0.0 || temp > 255.0){
			ret = 1;
			break;
		}
	}
	return ret;
}

/*! \brief Input: Characters Output: Matrix
 *
 * Converts an array of characters to a matrix.
*/
gsl_matrix* chartomat(char *msg){
	gsl_matrix* mmsg = genM();
	int i;
	double temp;
	
	for(i = 0; i < strnlen(msg,SIZE-1); i++){
		temp =  (double)(msg[i]);
		gsl_matrix_set(mmsg,0,i,temp);
	}
	
	gsl_matrix_set(mmsg,0,i,(double)('\0'));
	
	return mmsg;
}

/*! \brief Input: Matrix. Output: Characters.
 *
 * Converts a matrix to an array of characters.
*/
char* mattochar(gsl_matrix* mmsg){
	int i;
	
	for(i = 0; i < SIZE; i++)
		if(gsl_matrix_get(mmsg,0,i) ==  (double)('\0'))break;
		
	char *msg = malloc(i*sizeof(char));
	
	msg[i--] = '\0';
	
	for(; i >= 0 ; i--)
		msg[i] = (char)(gsl_matrix_get(mmsg,0,i));
	
	return msg;
}

/*! \brief Input: A matrix. Output: The inverse of the given matrix. 
 *
 * Returns the inverse of a matrix.
*/
gsl_matrix* m_invert(gsl_matrix* m) {
    int sig;
    gsl_matrix *temp = gsl_matrix_calloc(SIZE, SIZE);
    gsl_matrix_memcpy(temp,m);

    gsl_matrix *inverse = gsl_matrix_alloc(SIZE, SIZE);
    gsl_permutation *perm = gsl_permutation_alloc(SIZE);

    gsl_linalg_LU_decomp(temp, perm , &sig);
    gsl_linalg_LU_invert(temp, perm, inverse);
    
    gsl_permutation_free(perm);	
    gsl_matrix_free(temp);
    return inverse;
}

/*! \brief Input: A matrix and the index of a row. Output: _
 *
 * Receives a matrix and a row number and populates that row with a random 
 * selection of 1,0,-1 dependent on SCALE value, used to populate genU matrix
*/
void randomLine(gsl_matrix *m, int j){
     int i;
     double randVal;
     for(i =0 ; i< SIZE; i++){
         randVal = (double)rand()/RAND_MAX;
         if(randVal < SCALE){
             gsl_matrix_set(m,j,i,-1);
         }else if(randVal > (1 - SCALE)){
             gsl_matrix_set(m,j,i,1);
         }else{
             gsl_matrix_set(m,j,i,0);
         }
     }
     gsl_matrix_set(m,j,j,1);
}


/*! \brief Input: A matrix and the index of a row. Output: _
 *
 * Receives a matrix and a row number and changes the row to become all
 * zeros except at the value received, used to increase performance of
 * genU
*/
void clearLine(gsl_matrix *m, int j){
     int i;
     for(i =0 ; i< SIZE; i++){
         gsl_matrix_set(m,j,i,0);
     }
     gsl_matrix_set(m,j,j,1);
}

/*! \brief Input: A matrix Output: _
 *
 * Round the values of the matrix. 
*/
void roundMatrix(gsl_matrix* m){
	int i,j;
	for (i=0; i<SIZE; i++){
		for (j=0; j<SIZE; j++){
        		gsl_matrix_set(m,i,j,round(gsl_matrix_get(m,i,j)));
	        }
	}
}


/*! \brief Input: A matrix Output: _
 *
 * Round the values of the vector. 
*/
void roundVector(gsl_matrix* m){
	int i;
	for (i=0; i<SIZE; i++){
		gsl_matrix_set(m,0,i,round(gsl_matrix_get(m,0,i)));
	}
}

/*! \brief Input: Two Matrices. Output: Integer representing number of item differences.
 *
 * Compares the values of two matrices and returns the number of item differences.
 */
int compare(gsl_matrix *mo, gsl_matrix *mf){
    int i, errors = 0;
    roundVector(mf);
    roundVector(mo);
    for(i =0 ; i< SIZE; i++)
        if(gsl_matrix_get(mo,0,i) != gsl_matrix_get(mf,0,i)) errors += 1;
    return errors;
}


/*! \brief Input: gsl_matrix Output: _
 *
 * Write matrix sent to function into the file named unikey 
 */
void writeUni(gsl_matrix *m){
	FILE * f = fopen ("unikey", "wb");
	gsl_matrix_fwrite (f, m);
	fclose (f);
}

/*! \brief Input: gsl_matrix Output: _
 *
 * Write into the matrix sent the information in file unikey
 */
void readUni(gsl_matrix *m){
	FILE * f = fopen ("unikey", "rb");
	gsl_matrix_fread (f, m);
	fclose (f);
}

/*! \file matrix_op.c 
 *  \brief Contains shared matrix operations that manipulate matrices. 
 *
 * Matrix Manipulations (chartomat, mattochar, inverse, randomline, clearline, setResultMatrix, setresultVector), 
 *  \ingroup methods ggh
*/
