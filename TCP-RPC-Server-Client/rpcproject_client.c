// Kourias Triantafyllos-Dimitrios cs141092
// Server - RPC Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "rpcproject.h"

// RPC Client functions, called from Main() who works as Server for the Client.
// Each function receive as parameters the rpc-server address to make the connection, the array size
// and the array, then each function send the array & size to the rpc-server to do the do the calculations.
// When gets the value from the rpc-server it returns the float average value of the array to the main().

// RPC Client function for average value
float
average_prog_1(char *host,int size,int *uarr1)
{
	CLIENT *clnt;
	float  *result_1;
	User_array  average_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, AVERAGE_PROG, AVERAGE_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	average_1_arg.userarray.userarray_len=size;
	average_1_arg.userarray_size=size;
	average_1_arg.userarray.userarray_val=(int *) malloc(size*sizeof(int));
	average_1_arg.userarray.userarray_val= uarr1;
	result_1 = average_1(&average_1_arg, clnt);
	if (result_1 == (float *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else{
		return *result_1;
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


// RPC Client function for max-min of the array
int *
maxmin_prog_1(char *host,int size,int *uarr2)
{
	int *maxminarr;
	CLIENT *clnt;
	max_min  *result_1;
	User_array  maxmin_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, MAXMIN_PROG, MAXMIN_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	maxmin_1_arg.userarray.userarray_len=size;
	maxmin_1_arg.userarray_size=size;
	maxmin_1_arg.userarray.userarray_val=(int *) malloc(size*sizeof(int));
	maxmin_1_arg.userarray.userarray_val= uarr2;
	maxminarr = (int *) malloc(2*sizeof(int));
	result_1 = maxmin_1(&maxmin_1_arg, clnt);

	if (result_1 == (max_min *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else{
		maxminarr[0] = result_1->max;

		maxminarr[1] = result_1->min;
		return maxminarr;
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


// RPC Client function to multipy the array with a number
float *
product_prog_1(char *host,int size,int *uarr3,float multnumber)
{
	float *finalarr;
	CLIENT *clnt;
	product_array  *result_1;
	multiplication  product_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PRODUCT_PROG, PRODUCT_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	product_1_arg.userarray.userarray_len=size;
	product_1_arg.userarray_size=size;
	product_1_arg.userarray.userarray_val=(int *) malloc(size*sizeof(int));
	product_1_arg.multiplier=multnumber;
	product_1_arg.userarray.userarray_val= uarr3;
	result_1 = product_1(&product_1_arg, clnt);
	finalarr = (float *) malloc(size*sizeof(float));
	finalarr = result_1->prodarr.prodarr_val;

	if (result_1 == (product_array *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else
  {
        /*for(int i=0; i<size; i++)
        {
            printf("RPC_CLIENT->X3[%d]:%.1f\n",i,x3[i]);
        }*/
		return finalarr;
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}


// Main Function which serves as a server
int
main (int argc, char *argv[])
{
	// Declare variables
	int sockfd, newsockfd, portno, clilen; //, n;
	int option,size,i,sum=0,max,min;
	float mulnumber,averageresult;
	char *host;
	int *arrmaxmin;
	float *arrproduct;
	int flag;

	//Socket Connection
	struct sockaddr_in serv_addr, cli_addr;
	if (argc < 3) {
		 fprintf(stderr,"Wrong arguments, Usage: %s <hostname> <port>\n", argv[0]);
		 exit(0);}
	host = argv[1];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd,5);
	printf("Waiting for a client connection...\n");

  //Waits for a client to connect as long as the server runs, can accept many clients
	for (;;)  {
		 clilen = sizeof(cli_addr);
		 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		 if (newsockfd < 0)
			  error("ERROR on accept");
				if (fork() == 0)
				{
					close (sockfd);
					printf("Client Connected.\n");

					//when a client connects receives the array and its size and declare it
          recv(newsockfd,&size, sizeof size, 0);
          int usersarray[size];
          recv(newsockfd, &usersarray, sizeof usersarray, 0);

					// Waits for the client to make a choice from the menu and receives the choice
					do {
						recv(newsockfd, &option, sizeof option, 0);

						// According to the client choice, calls the correspondig rpc function and gets the returned value and then sends it to client
            if(option == 1)
            {
            	averageresult = average_prog_1 (host, size, usersarray);
							printf("RPC Server returned average value: %.1f\n", averageresult);

              send(newsockfd, &averageresult, sizeof averageresult, 0);
            }
						else if(option == 2)
            {
            	arrmaxmin = maxmin_prog_1(host, size, usersarray);
              printf("RPC Server returned values Max: %d, Min: %d\n", arrmaxmin[0],arrmaxmin[1]);
              send(newsockfd, arrmaxmin, 2*sizeof(int), 0);
            }
            else if(option == 3)
            {

            	recv(newsockfd, &mulnumber, sizeof mulnumber, 0);
              arrproduct = product_prog_1(host, size, usersarray, mulnumber);
              printf("RPC Server returned Product array after multiplication:\n");
              /*for(int i=0; i<size; i++)
              {
              	printf("\tProduct[%d] = %.1f\n",i,arrproduct[i]);
              }*/

              send(newsockfd, arrproduct, sizeof usersarray, 0);
            }

					} while (option!=4);
					//When the client choose to quit,the connection closes and waits for another client to connect
          printf("Client terminated the session\n");
          printf("\n");
					printf("Waiting for a client connection...\n");
					exit (0) ;

				}
				close(newsockfd);
			}
exit (0);
}
