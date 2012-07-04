#include "constants.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

/*! \brief Input: A private key as a matrix. Output: A public key, w as a matrix. 
 *
 * Generates a public key.
*/
gsl_matrix* create_public_key(gsl_matrix* v, gsl_matrix* u) {	
	gsl_matrix *w = gsl_matrix_alloc(SIZE,SIZE);
	gsl_linalg_matmult(u,v,w);
	return w;
}

/*! \file pub.c 
 * \brief  Generates a public key from a given private key.
 *
 * \ingroup ggh
*/
