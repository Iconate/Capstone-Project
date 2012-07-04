
#include "../lib/matrix_op.c"
#include "../lib/test_op.c"
#include "../lib/priv.c"
#include "../lib/pub.c"
#include "../lib/constants.h"
#include <math.h>

int compare_matrix(gsl_matrix *m1, gsl_matrix *m2) {
	int i,j;
	int errors = 0;
        roundMatrix(m2);
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			if((double)gsl_matrix_get(m1,i,j) != fabs(gsl_matrix_get(m2,i,j))){ errors++; printf("g");  }
		}
	}
	return errors;
}	


int main() {
	
        gsl_matrix *U = gsl_matrix_calloc(SIZE,SIZE);
        readUni(U);
	while(1){
	gsl_matrix *priv = create_private_key();
	gsl_matrix *pub = create_public_key(priv,U);
        gsl_matrix *inverse = m_invert(pub);
        gsl_matrix *result = gsl_matrix_calloc(SIZE,SIZE);
        gsl_matrix *identity = gsl_matrix_calloc(SIZE,SIZE);
        gsl_matrix_set_identity(identity);
        gsl_linalg_matmult(pub,inverse,result);
	printf("%d\n", compare_matrix(identity,result));
	}
        return 0;
}