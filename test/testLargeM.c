#include "../lib/constants.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <string.h>
#include <string.h>
#include <math.h>
/*#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h> */
#include <stdio.h>
#include "../lib/constants.h"
#include "../lib/m_operations.c"
#include "../lib/encrypt.c"
#include "../lib/decrypt.c"
#include "../lib/t_operations.c"
#include "../lib/pub.c"
#include "../lib/priv.c"
#include "../net/client.c"
#include "../net/server.c"


/*! \brief Input:_  Output: Message.
 *
 * Test a test on the Networking module. Whatever is typed on client end should be
 * displayed on the server end. There is no encryption involved. Data sent is not GGH secure.
 */
int testNetwork(char *flag,int port,char* hostname ){
			if(strcmp(flag, "l") == 0) {
				printf("Listening on port %d...\n",port);
				initiate_server("y", port);
			}
			else if(strcmp(flag, "c") == 0) {
				initiate_client(hostname, port);
			}
			else {
				fprintf(stderr, "Incorrect arguments, type ggh -h for correct usage\n");
			}
			exit(1);
		return 0;
}

/*! \brief Input: flag for multiple runs  Output: Message.
 *
 * This test creates a public and private key. The functions were tested for correctness in testSmall.c
 * Here we look at the integration of these functions to create the keys to have the properties
 * required of the GGH algortihm.
 */
int testGGH(int mruns){
	printf("* Executing %i GGH run(s)\n",mruns);

	gsl_matrix *uniMatrix = gsl_matrix_alloc(SIZE,SIZE);
	gsl_matrix *privKey = gsl_matrix_alloc(SIZE,SIZE);
	gsl_matrix *pubKey = gsl_matrix_alloc(SIZE,SIZE);
	gsl_matrix *message = gsl_matrix_alloc(SIZE,SIZE);
	gsl_matrix *Cmessage = gsl_matrix_alloc(SIZE,SIZE);
	gsl_matrix *Dmessage = gsl_matrix_alloc(SIZE,SIZE);
	
	int i, match, mismatch =0;
	if (mruns == 1){
		// Choose a good basis (Private key) [V]
		printf("Choose a good basis (Private key) [V]\n");
		privKey = create_private_key();
		
		// Create the unimodular matrix [U]
		printf("Create the unimodular matrix [U]\n");
		uniMatrix = genU();
		
		// Compute a bad basis (Public Key)
		printf("Compute a bad basis (Public Key)\n");
		pubKey = create_public_key(privKey,uniMatrix);
		
		// Generate the message
		printf("Generate the message\n");
		message = genM();
		
		// Encrypt
		printf("Compute cypher text.\n");
		Cmessage = encryptor(message,pubKey);
		
		// Decrypt
		printf("Decrypt\n");
		Dmessage = decryptor(Cmessage,privKey,uniMatrix);
		
		// Compare Messages
		printf("Compare Messages\n");
		
		if(compare(message,Dmessage)==0){
			printf("The messages match.\n");
		}
		else{
			printf("Message and decryped message do not match.\n");
		}
		
	}
	else{
		uniMatrix = genU();
		printf("Reusing the generated unimodular matrix [U] on 100 GGH runs.\n");
		for(i =0;i < (mruns+1); i++){
			// Choose a good basis (Private key) [V]
			privKey = create_private_key();
			
			// Compute a bad basis (Public Key)
			pubKey = create_public_key(privKey,uniMatrix);
			
			// Generate the message
			message = genM();
			
			// Encrypt
			Cmessage = encryptor(message,pubKey);
			
			// Decrypt
			Dmessage = decryptor(Cmessage,privKey,uniMatrix);
			
			// Compare Messages		
			if(compare(message,Dmessage)==0){
				match++;
			}
			else{
				mismatch++;
			}
		}
	}
	printf("  Number of matches: %i",match);
	return 1;
}

int main(int argc, char **argv) {
	if (argc == 1) { 
		printf("Run testLargeM -h for test options.\n\n");
	}
	else{
			if(strcmp(argv[1], "-nl") == 0) {
				printf("_Simulating a Network test_\n");
				testNetwork("l",atoi(argv[2]),"null");
			}
			if(strcmp(argv[1], "-nc") == 0) {
				printf("_Simulating a Network test_\n");
				testNetwork("c",atoi(argv[2]),argv[3]);
			}
			else if(strcmp(argv[1], "-g") == 0) {
				printf("_Testing GGH Module_\n");
				testGGH(1);
			}
			else if(strcmp(argv[1], "-gm") == 0) {
				printf("_Testing GGH Module_\n");
				testGGH(atoi(argv[2]));
			}
			else if (strcmp(argv[1], "-h") == 0) {
				printf("This module tests the ggh and network implementations. This is black box testing.\n");
				printf("_GGH TESTING_\nAim: Create a private and prublic key. Check properties of keys for correctness.\n");
				printf("To test the GGH implementation once in detail: './testLargeM -g'\n");
				printf("To test the GGH implementation multiple times: './testLargeM -gm 100'\n\n");
				printf("_NETWORK TESTING_\nAim: Create an unencrypted network simulation.\n");
				printf("To test network port listening:    './testLargeM -nl <port>'\n");
				printf("To test network server connection: './testLargeM -nc <port> <hostname>\n\n'");
			}
			else {
				printf("Run testLargeM -h for test options.\n");
			}
	}
	return 1;
}


/*! \file testLargeM.c
 *  \brief Module Testing
 * 
 * Tests the Networking and GGH modules seperately.
 * \ingroup test
 */