#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../lib/constants.h"
#include "../lib/m_operations.c"
#include "../lib/t_operations.c"
#include "../lib/rand_m.c"

/*! \brief Input:debug values as an integer  Output: integer.
 *
 * Tests the mattochar and chartomat functions. 
 * The pangram that it begins with should be the message after converting to a matrix and back.
 */
int testConvert(int dBg){
	char *msgO, *msgC; // Original and Converted Message as character arrarys.
	gsl_matrix *msgM = gsl_matrix_alloc(1, SIZE); //Message as a gsl Matrix
	msgO = malloc(SIZE*(sizeof(char)));
	msgC = malloc(SIZE*(sizeof(char))); 
	
	msgO = "The quick brown fox jumps over the lazy dog";
	msgM = chartomat(msgO); // convert original char array to matrix
	msgC = mattochar(msgM); // convert matrix to a new char array
	
	if (dBg != 0){
		printf("=====================\n");
		int o,c,i;
		//Print sizes
		o =strnlen(msgO, SIZE);
		c =strnlen(msgC, SIZE);
		printf("\tSize of original char array: [%d] ",o);
		printf("\tSize of converted char array: [%d] \n",c);
		
		//Print the result of the original char array
		printf("\tOriginal: ");
		for(i=0; i<o; i++){
			printf("%c", msgO[i]);
		}
		
		//Print the matrix
		printf("\n\tMatrix: ");
		
		if (dBg ==2){
			for(i=0; i<o; i++)
			{
				printf("%0.1f ", gsl_matrix_get(msgM, 0, i));
			}
		}
		else {
			printf("<Matrix printing skipped>");
		}
		
		//Print the result of the new char array
		printf("\n\tConverted: ");
		for(i=0; i<c; i++){
			printf("%c", msgC[i]);
		}
		printf("\n");
		
	}
	
	gsl_matrix_free(msgM);
	printf("* MSG CONVERSION ... ");
	if (strcmp(msgO,msgC)==0){ //Test correctness
		printf("OK\n");
		return 2; //if correct return 2.
	}
	else{
		printf("NG | Error Msg: Original and converted message are not the same.\n");
		return 0; //else return 0.
	}			
}
/*! \brief Input:debug values as an integer  Output: integer.
 *
 * Test the invert function in the context of GGH
 */
int testGInvert(){
	
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
	
	int i,j;
	int equality = 0;
    for(i =0; i<SIZE; i++){
		for(j =0 ; j<SIZE; j++){
			if(gsl_matrix_get(identity,i,j) != gsl_matrix_get(result,i,j)) {
				equality += 1;
			}
		}
	}
		printf("%d\n", equality);
	}
		printf("* MATRIX INVERSE [CONTEXT] ... ");
		if (equality == 0) { //Test correctness
			printf("OK\n");
			return 2;   //if correct return 2.
		}
		else {
			printf("NG | Error Code: %d The identity matrix, and the product of a random matrix and its inverse are not equal.\n", equality);
			return 0; //else return 0.
		}
		
}
				
/*! \brief Input:debug values as an integer  Output: integer.
 *
 * Test the invert function idependent of context. The product of a matrix and its inverse should equal the to identity.
 */
int testMInvert(int dBg){
	gsl_matrix *idMatrix = gsl_matrix_alloc(SIZE,SIZE); // identity
	gsl_matrix *aMatrix = gsl_matrix_alloc(SIZE,SIZE);  // matrix
	gsl_matrix *inMatrix = gsl_matrix_alloc(SIZE,SIZE); // its inverse
	gsl_matrix *product = gsl_matrix_alloc(SIZE,SIZE);  // product of matrix an idenity
	
	aMatrix = createMatrix(); //generate random matrix
	inMatrix = m_invert(aMatrix); // calculate inverse.
	gsl_linalg_matmult(aMatrix,inMatrix,product);//calculate the product
	gsl_matrix_set_identity(idMatrix); // create identity
	roundMatrix(product);
	roundMatrix(idMatrix);
	
	int i,j;
	int equality = 0;
    for(i =0; i<SIZE; i++){
		for(j =0 ; j<SIZE; j++){
			if(gsl_matrix_get(product,i,j) != gsl_matrix_get(idMatrix,i,j)) {
				equality += 1;
			}
		}
	}
	if (dBg == 2){
		printf("=====================\n");
		printMatrix(aMatrix);
		printf("\n\n");
		printMatrix(inMatrix);
		printf("\n\n");
		printMatrix(idMatrix);
		printf("\n\n");
		printMatrix(product);
		printf("\n\n");
	}
	if (dBg != 0){
		printf("\tThere are %d differences between the identity matrix and the product of a random matrix and its inverse\n",equality);
	}
	
	gsl_matrix_free(idMatrix);
	gsl_matrix_free(aMatrix);
	gsl_matrix_free(inMatrix);
	gsl_matrix_free(product);
	
	printf("* MATRIX INVERSE ... ");
	if (equality == 0) { //Test correctness
		printf("OK\n");
		return 2;   //if correct return 2.
	}
	else {
		printf("NG | Error Code: %d The identity matrix, and the product of a random matrix and its inverse are not equal.\n", equality);
		return 0; //else return 0.
	}
}

/*! \brief Input: debug values as an integer  Output: integer.
 *
 * Tests the unimodular generation and it's properties. The determinant of
 * a unimodular, its inverse and the product of itself and other unimodulars shoudl be 1.
 */
int testUni(int dBg) {
	gsl_matrix *idMatrix = gsl_matrix_alloc(SIZE,SIZE); // identity
	gsl_matrix *uMatrix = gsl_matrix_alloc(SIZE,SIZE);  // unimodular 1
	gsl_matrix *yMatrix = gsl_matrix_alloc(SIZE,SIZE);  // unimodular 2
	gsl_matrix *uiMatrix = gsl_matrix_alloc(SIZE,SIZE); // unimodular inverse 1
	gsl_matrix *yiMatrix = gsl_matrix_alloc(SIZE,SIZE); // unimodular inverse 2
	
	gsl_matrix *tempProduct = gsl_matrix_alloc(SIZE,SIZE); // temp product store
	float val1,val2,val3,val4,val5,val6; //temp value stores
	int tests = 0;
	
	gsl_matrix_set_identity(idMatrix); // create identity
	uMatrix = genU(); //generate unimodular 1
	yMatrix = genU(); //generate unimodular 2
	uiMatrix = m_invert(uMatrix); //generate unimodular inverse 1
	yiMatrix = m_invert(yMatrix); //generate unimodular inverse 2
	
	
	//determinant of the unimodular matrices and identity should  be 1;
	val1 = determinant(uMatrix);
	val2 = determinant(yMatrix);
	val3 = determinant(idMatrix);
	if (dBg != 0){
		printf("=====================\nNote: This test partially relies on the accuracy of the TEST MATRIX INVERSE result.\n");
		printf("uMatrix, %f\n",val1);
		printf("yMatrix, %f\n",val2);
		printf("idMatrix, %f\n\n",val3);
		printf("uMatrix rounded, %f\n",nearbyint(val1));
		printf("yMatrix rounded, %f\n",nearbyint(val2));
		printf("idMatrix rounded, %f\n\t-------------\n",nearbyint(val3));
		
	}
	
	if (( nearbyint(val1) +  nearbyint(val2) + nearbyint(val3)) == 3.0) {
		tests += 1; //test +1
		if (dBg != 0){
			printf("The determinant of the unimodular matrices and identity are all 1.\n");
		}
	}
	
	
	// the inverses' determinants should be 1. The inverse is a unimodular.
	uiMatrix = m_invert(uMatrix);
	yiMatrix = m_invert(yMatrix);
	val1 = determinant(uiMatrix);
	val2 = determinant(yiMatrix);
	
	if (dBg == 1){
		printf("uiMatrix, %f\n",val1);
		printf("yiMatrix, %f\n",val2);
		printf("uiMatrix rounded, %f\n",nearbyint(val1));
		printf("yiMatrix rounded, %f\n\t------------\n",nearbyint(val2));
	}
	
	if (( nearbyint(val1) +  nearbyint(val2)) == 2.0) {
		tests += 1; //+1
		if (dBg != 0){
			printf("The determinant of the inverses of the unimodular matrices are all 1.\n"); 
		}
	}
	
	//The product of any two unimodular matrices is a unimodular matrix. the identity is also a unimodular
	//product of all with identity
	gsl_linalg_matmult(uMatrix,idMatrix,tempProduct);
	val1 = determinant(tempProduct);
	
	gsl_linalg_matmult(yMatrix,idMatrix,tempProduct);
	val2 = determinant(tempProduct);
	
	gsl_linalg_matmult(uiMatrix,idMatrix,tempProduct);
	val3 = determinant(tempProduct);
	
	gsl_linalg_matmult(yiMatrix,idMatrix,tempProduct);
	val4 = determinant(tempProduct);
	
	//inverses of each other
	gsl_linalg_matmult(uMatrix,uiMatrix,tempProduct);
	val5 = determinant(tempProduct);
	
	gsl_linalg_matmult(yMatrix,yiMatrix,tempProduct);
	val6 = determinant(tempProduct);
	
	if (dBg == 1){
		printf("u, id, %f\n",val1);
		printf("y id, %f\n",val2);
		printf("ui, id, %f\n",val3);
		printf("yi,id %f\n",val4);
		printf("u, ui, %f\n",val5);
		printf("y yi, %f\n",val6);
	}
	if (( nearbyint(val1) +  nearbyint(val2) +  nearbyint(val3)+ nearbyint(val4) +  nearbyint(val5) +  nearbyint(val6)) == 6.0) {
		tests += 1; //+1
		if (dBg != 0){
			printf("The determinant of the product of any two of the unimodular matrices is 1.\n"); 
		}
	}
	
	
	gsl_matrix_free(idMatrix);
	gsl_matrix_free(uMatrix);
	gsl_matrix_free(yMatrix);
	gsl_matrix_free(uiMatrix);
	gsl_matrix_free(yiMatrix);
	gsl_matrix_free(tempProduct);
	
	printf("* UNIMODULAR GENERATION AND PROPERTIES... ");
	if (tests == 3) { //Test correctness
		printf("OK");
		printf(" || [set debug for important note]\n");
		return 2;   //if correct return 2.
	}
	else {
		printf("NG | Error Code: %i",tests);
		printf(" || [set debug for important note]\n");
		return 0; //else return 0.
	}
	
}

int main(int argc, char **argv) {
	printf("\n");	
	int dBg,counter,skip,i =0;
	counter = 0; //function accuracy
	dBg = 0; //debug mode level
	if (argc == 1) { 
		printf("Run ./testLargeS -h for test options.\n\n");
	}
	else{
		for (i = 1; i < argc; i++) {
			if (strcmp(argv[i],"-f") ==0) {
				dBg = 1; //set flag on
			}
			if (strcmp(argv[i],"-g") ==0) {
				dBg = 2; //set flag on
			}
			if (strcmp(argv[i],"-s") ==0) {
				//skip = 2; //set flag on
			}
			if (strcmp(argv[i],"-h") ==0) {
				printf("Use \n\t ./testSmall -f \nfor a shallow debug mode and \n\t ./testSmall -f\n for a deeper debug mode");
			}
		}
	}

	//Perform Tests
	counter += testConvert(dBg); // counter +2
		counter += testMInvert(dBg); // +2
	counter+= testGInvert(); //+2
		counter += testUni(dBg); // +2
	
	//Results
	printf("TEST IN SMALL ... ");	
	if (counter == (6-skip)) { 
		printf("OK\n");
	}
	else{
		printf("NG | Error Code: %i\n",counter);
	}
	return 0;
}


/*! \file testSmall.c
 *  \brief Unit testing
 *
 * Test the functions seperately and prints results of each test.
 * Functions that can be tested in each file. Bracketed functions were tested in this file.
 * - lib
 * 	-# decrypt: decryptor
 *  -# encrypt: encryptor
 *  -# m_operations: [chartomat, mattochar, inverse,] randomline, clearline, setResultMatrix, setresultVector 
 *  -# priv: create_private_key
 *  -# pub: create_public_key
 *  -# rand_m: genU(), genR(), genM()
 *  -# t_operations: determinant, had, compare, decrypt_badcyphertext, printMatrix, printMatrixText
 * - net
 *  -# client: error, initiate_client
 *  -# server: initiate_server
 *
 * \ingroup test
 */