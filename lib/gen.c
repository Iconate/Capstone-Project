#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_linalg.h>

/*! \brief Input: _. Output: A matrix 
 *
 * Generates a unimodular matrix.
*/
gsl_matrix* genU(){
     int z, j ,display = 1;

     /* Initialize and allocate memory for necessary matrices */
     gsl_matrix* result = gsl_matrix_alloc(SIZE,SIZE);
     gsl_matrix* result2 = gsl_matrix_alloc(SIZE,SIZE);
     gsl_matrix* base = gsl_matrix_alloc(SIZE,SIZE);
     gsl_matrix_set_identity(result);
     gsl_matrix_set_identity(result2);
     gsl_matrix_set_identity(base);
     /* Display progress of generating a unimodular to the user */
     printf("Generating Unimodular\n");
	 printf("|..................................................|\r\033[01;35m|");
	fflush(stdout);
     if(SIZE/50 > 1) display = SIZE/50;
	
     for(z = 0; z<DEPTH; z++){
          if(z%2 == 0){      
               for(j=0; j < SIZE ; j++)
               {
	           if(j%2 == 0){      
                       randomLine(base,j);
                       gsl_linalg_matmult(result,base,result2);    
                       gsl_matrix_memcpy(result,result2);
                       clearLine(base,j);
		       if(j % display == 0){
			       printf("-");
			       fflush(stdout);
		       }
			   }
               }
          }else{
			  printf("| 50%%\r\033[01;32m|");
               for(j=SIZE-1; j >= 0 ; j--)
               {
	           if(j%2 == 1){      
                       randomLine(base,j);
                       gsl_linalg_matmult(result,base,result2);    
                       gsl_matrix_memcpy(result,result2);
                       clearLine(base,j);
       		       if(j % display == 1){
			       printf("=");
			       fflush(stdout);
		       }
		   }
		}
          }
     }
     
     printf("| 100%% DONE\rUnimodular Generated\033[0m\n");
     
     free(result2);
     free(base);
     
     return result;
}

/*! \brief Input: none. Output: A vector
 *
 * Generates a random vector.
*/
gsl_matrix* genR(){
    srand(time(NULL));
    int i;
    gsl_matrix* m = gsl_matrix_alloc(1,SIZE);
    for(i =0 ; i< SIZE; i++)
        gsl_matrix_set(m,0,i,(rand() % RRANGE*2)-RRANGE);
    return m;
}

/*! \brief Input:_  Output: A vector.
 *
 * Generates a random message for testing
 */
gsl_matrix* genM(){
    srand(time(NULL));
    int i;
    gsl_matrix* m = gsl_matrix_alloc(1,SIZE);
    for(i =0 ; i< SIZE; i++)
        gsl_matrix_set(m,0,i,rand()%256);
    return m;
}

/*! \file gen.c 
 * \brief File contains the functions used to generate matrices.
 *
 * Generates the unimodular matrix, private key.
 * \ingroup methods
 */
