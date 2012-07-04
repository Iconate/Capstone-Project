#include "constants.h"
#include <gsl/gsl_linalg.h>
#include <math.h>

/*! \brief Input: cyphertext as matrix, private key, unimodular, inverse of private key, inverse of unimodular
 * Output: Decrypted message as a matrix. 
 *
 * Performs decryption on a message. This is what Alice does to Bob's encrypted message.
*/
 gsl_matrix* decryptor(gsl_matrix *c,  /*!< Cyphertext as a vector */
				   gsl_matrix *v, /*!< Private Key*/ 
				   gsl_matrix *u, /*!< Unimodular matrix */
				   gsl_matrix *vinv,/*!< Inverse of private key */
				   gsl_matrix *uinv)  /*!< Inverse of Unimodular matrix */
{

	
	gsl_matrix *decryp = gsl_matrix_alloc(1,SIZE);	
	gsl_matrix *m = gsl_matrix_alloc(1,SIZE); 	/*result is stored in here*/
	
	/*Compute c*V^(-1)*/
	gsl_linalg_matmult(c,vinv,decryp);
	
	/*Round the elements of decryp*/
	roundVector(decryp);
	
	/*Compute decryp*U^(-1) = m*/
	gsl_linalg_matmult(decryp,uinv,m);

	roundVector(m);
	
	gsl_matrix_free(decryp);
	return m;
}

/*! \brief Input: cyphertext as matrices, public key, inverse of public key. Output: Decrypted message as a matrix. 
 *
 * Attempts to decrypt the message only using the public key, used for testing the security of the encryption. 
 * Performs decryption on a message. This is what Eve does to Bob's encrypted message. 
 * This is a measure of the encryption strength, hence a property.
 */
gsl_matrix* baddecryptor( gsl_matrix *c,  /*!< Cyphertext as a vector */
					 gsl_matrix *pub, /*!< Public Key */
					 gsl_matrix *pubinv) /*!< Inverse of Public Key*/ 
{	
	gsl_matrix *decryp = gsl_matrix_alloc(1,SIZE);	
	gsl_linalg_matmult(c,pubinv,decryp);
	roundVector(decryp);
	
	return decryp;	
}

/*! \file decrypt.c
 *  \brief Decrypt a message sent as a matrix.
 *
 * Takes the encrypted message (cyphertext) in the form of a vector, and decodes it 
 * to return the original message0.
 * \ingroup ggh
*/
