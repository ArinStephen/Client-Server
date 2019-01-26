// Arin Abed-Stephen
// Peoplesoft 0869127
// Homework 2
// Client Side

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;


int main(int argc, char *argv[])
{
    //Initalizes the variables
    int sockfd, portno, n;
    char hostname[40];
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    
    char buffer[256];
    string buffStr;
    
    //Asks for hostname and places it into the server variable
    cout << "Please enter a host name: ";
    cin >> hostname;
    server = gethostbyname(hostname);
    
    //Ask for the port number
    cout << "Please enter server port: ";
    cin >> portno;
    //Opening a socket and program exits if there is an error opening it
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cout << "ERROR opening socket";
        exit(0);
    }
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        cout << "ERROR connecting";
        exit(0);
    }
    //This is to ignore the newline caused by the previous cin so that the getline function does not hang
    cin.ignore();
    
    while(true){
        //Asks for major name and stores it into the buffer, if buffer is empty the program exits.
        cout << "Please enter a major: ";
        cin.getline(buffer, sizeof(buffer));
        if(buffer[0] == '\0'){
            cout << "Disconnected\n";
            break;
        }
        else{
            //sends buffer to server
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0){
                cout << "ERROR writing to socket";
                exit(0);
            }
            //reads buffer sent by server
            n = read(sockfd,buffer,255);
            if (n < 0){
                cout << "ERROR reading from socket";
                exit(0);
            }
            printf("%s\n",buffer);
        }
        //resets the buffer to zero
        memset(buffer, 0, sizeof(buffer));
    }
    return 0;
}
