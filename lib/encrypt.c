#include "constants.h"
#include <gsl/gsl_linalg.h>

/*! \brief  Inputs: message, public key, random peturbator as matrices. Output: Encrypted message matrix.
 *
 * Performs encryption on a message. This is what Bob does to his message.
*/
gsl_matrix* encryptor(gsl_matrix* M, /*!< Message as a vector */
					  gsl_matrix* W) /*!< Public key */
{
    gsl_matrix *mW = gsl_matrix_alloc(1,SIZE);
    gsl_matrix *E = gsl_matrix_alloc(1,SIZE);
    gsl_matrix *R = gsl_matrix_alloc(1,SIZE);
	
    R = genR();
	

/* Calculate (m * W). Matrix * Matrix Multiplication */
    gsl_linalg_matmult(M,W,mW);
	
// Calculate e = (m * W) + r
   gsl_matrix_add(mW,R);
   gsl_matrix_memcpy(E,mW);
	
// Release memory Note: What else can be safely freed from memory?
    gsl_matrix_free(mW);
    gsl_matrix_free(R);
// Return encrypted message, E.
    return E;
}

/*! \file encrypt.c
 *  \brief Encrypt a message sent as a matrix.
 * Takes the messages in the form of a vector and encrpyts it using the public key
 * and random perturbator.
 * \ingroup ggh
*/
