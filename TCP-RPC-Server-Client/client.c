// Kourias Triantafyllos-Dimitrios cs141092
// Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Error handling function
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    //Declare Variables
    int sockfd, portno;
    int option, size, i;

    //Struct for socket
    struct sockaddr_in serv_addr;
    struct hostent *server;

    //Argument input validation and correct usage
    if (argc < 3) {
      fprintf(stderr,"Wrong arguments, Usage: %s <hostname> <port>\n", argv[0]);
      exit(0);
    }

    //Socket Connection
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host on: %s\n",argv[1]);
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    printf("Trying to connect...\n");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Connected.\n");

    //Scan & Validating Input Array's Size
    do{
      printf("Give size of array: ");
      scanf("%d", &size);
      if(!((size>0) && (size<9999))){  //Check for invalid input size
        printf("\nSize must be a number betwenn 0-9999\n");
        char dummy;
        scanf("%c", &dummy); // eats a character off the buffer,so can scanf correctly again
      }
    }while(!((size>0) && (size<9999)));

    //Input Array's Elements with given size
    int array[size];
    for(i=0;i<size;i++)
    {
        printf("Array[%d] = ", i+1);
        scanf("%d",&array[i]);
    }

    //Send array & size to server
    send(sockfd, &size, sizeof size, 0);
    send(sockfd, &array, sizeof array, 0);

    //Menu which user can repeatedly select a choice until he types/choose option 4 to quit
    do {
        printf("\nMenu\n");
        printf("1. Average value\n");
        printf("2. Max and Min value\n");
        printf("3. Array multiplied by real number\n");
        printf("4. Quit\n");
        printf("Select an option: ");
        scanf("%d", &option);
        printf("\n");

        //When user selects an option from the menu then this option being sent to the server/rpcclient
        //to know which function to call from the rpcserver.
        //In each option declare the result variable depending the type, then the option number is being sent
        //and receives the requested result and prints it on the terminal.

        if(option == 1)
        {
            float averagevalue;
            send(sockfd, &option, sizeof option, 0);
            //Receive Average Value from Server and print it
            recv(sockfd, &averagevalue, sizeof averagevalue, 0);
            printf("\t Average value of array= %.1f\n",averagevalue);
        }

        else if(option == 2)
        {
            int maxminarr[2];
            send(sockfd, &option, sizeof option, 0);
            //Receives an array with the max min value and prints them
            recv(sockfd, &maxminarr, 2*sizeof(int), 0);
            printf("\tMax Value= %d\n",maxminarr[0]);
            printf("\tMin Value= %d\n",maxminarr[1]);
        }
        else if(option == 3)
        {

            float parray[size];
            float multiplier;
            //Reads the number to multiply the array,sends thath number alongside with the option
            //and receives and prints the array after multiplication
            printf("Give a real number to multiply: ");
            scanf("%f",&multiplier);
            printf("\n");
            send(sockfd, &option, sizeof option, 0);
            send(sockfd, &multiplier, sizeof multiplier, 0);

            recv(sockfd, &parray, sizeof parray, 0);
            printf("\tAfter Receive:Product array after multiplication:\n");
            for(i=0;i<size;i++)
            {
                printf("\tProduct[%d] = %.1f\n", i,parray[i]);
            }
        }
        else if(option == 4)
        {
          //send that the client wants to quit option and closes the connection
          send(sockfd, &option, sizeof option, 0);
        }
        else
        {
            //if user selects an option not on the menu
            printf("Invalid option! Try again (Accepted options 1-4)\n");
        }

     } while (option!=4);

 //Close the socket and exit the program
  close(sockfd);
  printf("Terminated Succesfully\n");
  return 0;
}
