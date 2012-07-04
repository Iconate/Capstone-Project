/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <gsl/gsl_matrix.h>
#include "../lib/constants.h"

/*! \brief  Inputs: Port information.  Output: _ 
 * 
 * Initiates the server for communication.
 */
void initiate_server(char *selec,int port) {
     int sockfd, newsockfd, portno, n, i;
     int *c = malloc(sizeof(int));
	
     socklen_t clilen;
	
     /* Make necessary declarations and initilizations */
     gsl_matrix *priv,*pub,*uni, *msg, *output, *privinv, *pubinv, *uniinv;

     privinv = gsl_matrix_alloc(SIZE,SIZE);
     pubinv = gsl_matrix_alloc(SIZE,SIZE);
     uniinv = gsl_matrix_alloc(SIZE,SIZE);
     msg = gsl_matrix_alloc(1,SIZE);
     output = gsl_matrix_alloc(1,SIZE);

     /* Create server address struct and socket file descriptor */
     struct sockaddr_in serv_addr, cli_addr;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
     /* Ensure socket file descriptor can be created on host system */
     if (sockfd < 0) 
        perror("ERROR opening socket");
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     
     portno = port;
     /* Specify necessary record information in the server address structure */
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     /* Bind to the socket file descriptor */
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              perror("ERROR on binding");
     
     /* Start listening for a maximum of 1 connection */
     listen(sockfd,1);

     clilen = sizeof(cli_addr);
     /* Decide whether to generate or use a previously generated unimodular key based 
     on user flag option */
     if(selec[1] =='g'){
         uni = genU();
	 writeUni(uni);
     }else{
	 uni = gsl_matrix_alloc(SIZE,SIZE);
	 readUni(uni);
     }
     gsl_matrix_memcpy(uniinv,m_invert(uni));
     /* Test the goodness of the keys generated*/
     while(1){
	 /* Create private and public keys along with test message */
	 priv = create_private_key();
         pub = create_public_key(priv,uni);
	 gsl_matrix *test = genM();
	     
         /* Find the inverses*/
	 gsl_matrix_memcpy(privinv,m_invert(priv));
	 gsl_matrix_memcpy(pubinv,m_invert(pub));
	     
	 /* Comparing a sample message with the correct decryption/encryption of ggh */
    	 int comparison1 = compare(test,decryptor(encryptor(test,pub),priv,uni,privinv,uniinv));
	 /* Comparing a sample message with the incorrect decryption/encryption of ggh using the public key*/
	 int comparison2 = compare(test,baddecryptor(encryptor(test,pub),pub,pubinv));	
    	 
	 printf("Erroneous Values of decryption with private key: %d\n", comparison1);
	 printf("Erroneous Values of decryption with public key: %d\n", comparison2);
	 printf("===================================================\n");
	/* If a correct key is found, keep it. */
	 if(comparison1 == 0 && comparison2 == SIZE){
		printf("Key Generated \n");
		gsl_matrix_free(test);
		break;
	 }
	 
     }
	    
     printf("Listening on port %d... \n",port);
     /* Now that keys are generated, accepting connections on a new socket file descriptor */
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     
     if (newsockfd < 0) 
          perror("ERROR on accept");
     /* Send the public key to the client whom connects */
     for( i = 0; i < SIZE*SIZE ; i++){
	n = write(newsockfd,(pub->data)+i, sizeof(*(pub->data)));
	if (n < 0) perror("ERROR writing to socket");
     }
     
     printf("Key transmitted on port %d... \n",port);

     while(1) {
	/* clear msg buffer */
	gsl_matrix_set_zero(msg);
	/* check flag is 1 by default */
	*c = 1;
	while(1){
		/* Begin decrypting messages and ensure correct message was sent with 
		* check method */
		n = read(newsockfd,msg->data,SIZE*sizeof(*(msg->data)));   
		if (n < 0) perror("ERROR reading from socket");
		gsl_matrix_memcpy(output,decryptor(msg,priv,uni,privinv,uniinv));
		if(check(output) == 0)break;
		n = write(newsockfd,c,sizeof(int));   
	}
	/* change check flag to 0 to indicate the correct message was received */
	*c = 0;
	n = write(newsockfd,c,sizeof(int));   
	/* Quit if the user types /q */
	if ((mattochar(output))[0] == '/' && (mattochar(output))[1] == 'q')break;
	
	printf("%s\n", mattochar(output));
     }
     close(newsockfd);
     close(sockfd);
}

/*! \file server.c
 *  \brief Contains the functions necessary for the server's communication with the client.
 * \ingroup networking
 */
