#include "constants.h"
#include "gen.c"
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

/*! \brief Input: none. Output: A private key, v as a matrix. 
 * 
 * Generates a private key.
*/
gsl_matrix* create_private_key() {
    srand(time(NULL));
    int i,j,r;
    gsl_matrix* m = gsl_matrix_calloc(SIZE,SIZE);
    for(i =0 ; i< SIZE; i++){
        for(j =0 ; j< SIZE; j++){
	     r = (int)((rand() % RANGE*2)-RANGE);
             gsl_matrix_set(m,i,j,r);
	}
    }
    return m;
}

/*! \file priv.c 
 * \brief Generates a private key. 
 *
 * \ingroup ggh
*/
