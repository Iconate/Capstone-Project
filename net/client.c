#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
/*! \brief  Inputs: Message  Output: To screen. 
 *
 * Handles error mesages that may occur on the client or server side. This function is shared.
 */
void error(const char *msg)/*!< Error message */
{
    perror(msg);
    exit(0);
}

/*! \brief  Inputs: The host and port information.  Output: _ 
 * 
 * Initiates the client for communication.
 */
void initiate_client(char *hostname, /*!< name of the host */ 
					 int port) /*!< port number */ {
						 
    int sockfd, portno, n, i;
    int *c = malloc(sizeof(int));
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /* Allocate space for a matrix buffer */
    gsl_matrix *pub = gsl_matrix_calloc(SIZE,SIZE);
    gsl_matrix *mbuffer;
						 
    char *talkbuffer = malloc(SIZE);
						 
    portno = port;
    /* Create file descriptor for socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    /* Check that the host exists before trying to connect to it */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    /* Make changes to server address structure for connection */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    /* Check if connection succeeds.. */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");

    printf("Connecting to %s on port %d...\n",hostname,port);
    /* After connection, begin receiving the public key from the server to encrypt with*/
    for( i = 0; i < SIZE*SIZE ; i++){
          n = read(sockfd,(pub->data)+i, sizeof(*(pub->data)));
	  if (n < 0) perror("ERROR reading from socket");
    } 
    
    printf("Key Received!\n");
    
    while(1) {
	    printf("SEND(type /q to quit): ");
	    
	    bzero(talkbuffer,SIZE);
	    fgets(talkbuffer,SIZE-1,stdin);
            
	    *c = 1;
	    while(1){
		/* Encrypt what the client types and assign it to mbuffer */
		mbuffer = chartomat(talkbuffer);
		mbuffer = encryptor(mbuffer,pub);
		    
		/* Send mbuffer to the server for decryption */
		n = write(sockfd,mbuffer->data,SIZE*sizeof(*(mbuffer->data)));
		if (n < 0) error("ERROR writing to socket");
		    
		/* This is a check sent from server-side to ensure a proper ASCII message was received */
		n = read(sockfd,c, sizeof(c));
		if(*c == 0) break;
	    }
	    /* Quit if the user types /q */
	    if (talkbuffer[0] == '/' && talkbuffer[1] == 'q') break;
	    /* clear mbuffer */
	    gsl_matrix_set_zero(mbuffer);
    }
    close(sockfd);
}

/*! \file client.c
 *  \brief Contains the functions necessary for the client's communication with the server.
 * \ingroup networking
 */

