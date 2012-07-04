#include "../lib/matrix_op.c"
#include "../lib/test_op.c"
#include "../lib/priv.c"
#include "../lib/pub.c"
#include "../lib/encrypt.c"
#include "../lib/decrypt.c"
#include "../lib/constants.h"

int check(gsl_matrix *m){
	int i;
	int ret = 0;
	for(i = 0; i < SIZE; i++){
		if(gsl_matrix_get(m,0,i) < 0 && gsl_matrix_get(m,0,i) > 255){
			ret = 1;
			break;
		}
	}
	return ret;
}

gsl_matrix *ctom(char *msg){
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

char  *mtoc(gsl_matrix* mmsg){
	int i;
	
	for(i = 0; i < SIZE; i++){
		if(gsl_matrix_get(mmsg,0,i) ==  (double)('\0'))break;
	}
	
	char *msg = calloc(i,sizeof(char));
	
	printf("%d \n",i);
	
	msg[i--] = '\0';
	
	for(; i >= 0 ; i--){
		msg[i] = (char)(gsl_matrix_get(mmsg,0,i));
	}
	
	printf("%d \n",strlen(msg));
	
	return msg;
}



int main() {
	gsl_matrix *uni = gsl_matrix_alloc(SIZE,SIZE);
	readUni(uni);
	gsl_matrix *uniinv = gsl_matrix_alloc(SIZE,SIZE);
	
	gsl_matrix *priv, *pub, *privinv;
	while(1){
		priv = create_private_key();
		pub = create_public_key(priv,uni);
		privinv = gsl_matrix_alloc(SIZE,SIZE);
		gsl_matrix_memcpy(uniinv,m_invert(uni));
		gsl_matrix_memcpy(privinv,m_invert(priv));
		
		gsl_matrix *message = genM();
		gsl_matrix *encrypt_message;
		gsl_matrix *decrypt_message;
		
		encrypt_message = encryptor(message, pub);
		decrypt_message = decryptor(encrypt_message, priv, uni, privinv, uniinv);
		
		int comparison1 = compare(message,decrypt_message);
		printf("%d \n",comparison1);
		if(comparison1 == 0) break;
		gsl_matrix_free(priv);
		gsl_matrix_free(pub);
		gsl_matrix_free(privinv);
		gsl_matrix_free(message);
		gsl_matrix_free(encrypt_message);
		gsl_matrix_free(decrypt_message);
	}
	
	printf("------------------------\n");	
	
	char *msg = "sdfghjuytrdfvbnjiu765rfvbhjuytredcvbnmkiuytrdcvbnmkiuytrdcvbnmkiuyt6rfgbnmkiu876trfvbnjkiuy6trfvbnmkiuyt5rdfcvbnjuytrfdcvbnjuytrd\n";
	gsl_matrix *message;//genM();
	gsl_matrix *encrypt_message, *decrypt_message;
	
	while(1){
		printf("%s\n",msg);
		
		message = ctom(msg);
		
		encrypt_message = encryptor(message, pub);
		while(1){
			decrypt_message = decryptor(encrypt_message, priv, uni, privinv, uniinv);
			if(check(decrypt_message) == 0)break;
		}
		char *dmsg = mtoc(decrypt_message);
		
		printf("%s\n",dmsg);
		printf("%d \n",strcmp(msg,dmsg));
		
		if(strcmp(msg,dmsg) != 0) break;	
	}
	
	int i;
	
	for( i = 0 ; i < SIZE; i++){
		if(gsl_matrix_get(decrypt_message,0,i) == 0.0)break;
	}
	
	printVector(message);
	printVector(decrypt_message);
	
	printf("%f \n",(double)('\0'));
	printf("%d \n",i);
	
	gsl_matrix_free(message);
	gsl_matrix_free(encrypt_message);
	gsl_matrix_free(decrypt_message);		
	
	return 0;
}


/*! \file testLargeIS.c
 *  \brief Integrated/System Testing
 * 
 * Test the the module integration/entire system and prints results the test.
 * \ingroup test
 */
