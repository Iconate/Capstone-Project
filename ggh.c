#include "lib/matrix_op.c"
#include "lib/test_op.c"
#include "lib/priv.c"
#include "lib/pub.c"
#include "lib/encrypt.c"
#include "lib/decrypt.c"
#include "net/server.c"
#include "net/client.c"
#include <stdio.h>
#include <string.h>

/*
ggh.c creates the main executable for both server and client
*/
int main(int argc, char **argv) {
	int debugF,i,port;
	char *selec = "";

	/* If we don't have the right amount of arguments, fail */
	if (argc > 5 || argc == 1) { 
		fprintf(stderr, "Not enough, or too many arguments!\n");
	}
	/* Otherwise let's parse the arguments and act appropriately  */
	else {				
		for (i = 1; i < argc; i++) {
			/*This will activate full debug mode for the code, specific outputs will be activated when this flag is on*/
			if (strncmp(argv[i],"-f",2)==0) {
				printf("FULL DEBUG MODE ON\n");
				debugF = 1;
			}
			/*This activates the server code and forces it to generate a new unimodular */
			else if(strcmp(argv[i], "-l") == 0 && strcmp(argv[i+1], "-g") == 0) {
				selec = argv[++i];
				port = atoi(argv[++i]);
				initiate_server(selec, port);
			}
			/*This activates the server code and forces to read the unimodular from a file*/
			else if(strcmp(argv[i], "-l") == 0 ) {
				port = atoi(argv[++i]);
				initiate_server(selec, port);
			}
			/*This activates the client code which will attempt to connect to the specified hostname */
			else if(strcmp(argv[i], "-c") == 0) {
				// next argument should be the hostname
				char *hostname = argv[++i];
				// next argument should be the port
				port = atoi(argv[++i]);
				initiate_client(hostname, port);
			}
			/* This will display help information. */
			else if (strcmp(argv[i], "-h") == 0) {
				printf("ggh -l -g port  -- invokes listening mode on port p making a new unimodular Ex: ggh -l -g 7000\n");
				printf("ggh -l port -- invokes listening mode using a previously generated unimodular Ex: ggh -l 7000\n");
				printf("ggh -c hostname port-- connects ggh to a server running ggh in listening mode.\n\tEx: ggh -c 192.168.0.1 7000\n");
			}
			/* If the incorrect arguments were inputed the user has to know */
			else {
				fprintf(stderr, "Incorrect arguments, type ggh -h for correct usage\n");
			}
		}
	}

	return 0;
}


/*!\file ggh.c 
 * \brief The main program. Required function are called into this file. 
 
 * \defgroup ggh GGH 
 * Directly related to the GGH implementation.
 * \defgroup methods Methods & Global Variables 
 * Indirectly related to the Networking and GGH implementation. 
 * This includes global variables and secondary functions. 
 * \defgroup networking Networking 
 * Directly related to the creation of the network.
 * \defgroup test Testing
 * Verfication and validation of the software.

 
 * \mainpage Project Summary
 * \authors Martin Cammisa, Nathan Kruger, Christo Mitov, Jevonne Peters
 * \details This project focused on the implementation and analysis of The 
 * Goldreich–Goldwasser–Halevi (GGH) lattice-based cryptosystem, accommodating for flaws
 * and vulnerabilities that have been discovered so far.
 * Our project aims to create the application of GGH encryption as a real world product. 
 * GGH is a public-key encryption system relies on lattice based cryptography. We will take an 
 * in depth look at the algorithm by analysing each of the components that make it both secure, 
 * and insecure. A prototype algorithm was created to test such scenarios and each of its 
 * variables will be tested for anomalies. These tests will encompass ﬂaws proposed through 
 * various scientiﬁc papers such as Nyugen's attack, and those that we may discover in our own 
 * research. The project willlook at speciﬁc cases where the algorithm causes a problem with 
 * encryption or decryption, and will attempt to mitigate these in the ﬁnal product.
 *
 * The successful completion of the project will produce a method of encrypting and 
 * transmitting a message to another machine without a third party being able to retrieve the 
 * public information and decipher the message using GGH. The uses of the product would be 
 * similar to those of GnuPG, a crypto engine that can be used directly from command prompt. 
 *
 * For Wiki and Code: <a href="http://code.google.com/p/capstone4zp6/">Capstone4ZP6 GoogleCode Hosting</a>
 *
 * Program Information: # <a href="http://www.cas.mcmaster.ca/cas/">McMaster Computer Science<a>
 * Course Information : # <a href="http://registrar.mcmaster.ca/CALENDAR/PDF-Archive/McMaster_UG_Calendar-2010-2011web.pdf">Comp Sci 4ZP6 Capstone Project</a> 
*/
