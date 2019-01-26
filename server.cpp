//Arin Abed-Stephen
//PeopleSoft 0869127
//Homework 2
//Server Side

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

struct dataTable{
    
    string subject;
    string num1;
    string num2;
};

int search(vector<dataTable> &data, char (&buffer)[256]){
    int vNum = -1;
    string word(buffer);
    for(int i = 0; i < data.size(); i++){
        if(word == data[i].subject){
            vNum = i;
            return vNum;
        }
    }
    return vNum;
}

int main(int argc, char *argv[])
{
    //Initalizes variables
    int sockfd, newsockfd, portno;
    char buffer[256];
    char hostname[40];
    string sentence;
    vector<dataTable> data;
    dataTable information;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *hp;
    socklen_t clilen;
    int n;
    int result;
    
    ifstream file;
    string major;
    string number1;
    string number2;
    string filename;
    //Asks for filename. If the file doesn't exist, it will ask again
    cout << "Please enter the filename: ";
    cin >> filename;
    file.open(filename.c_str());
    while(file.fail()){
        file.clear();
        cout << "File doesn't exist. Please enter again: ";
        cin >> filename;
        file.open(filename.c_str());
    }
    //Grabs the first line and stores each part into the corresponding variable within dataTable structure
    getline(file, major, '\t');
    file >> number1 >> number2;
    information.subject = major;
    information.num1 = number1;
    information.num2 = number2;
    //places dataTable node into vector
    data.push_back(information);
    while(file){
        getline(file, major, '\t');
        //this removes the newline and space from the beginning of the next lines within the file to store it properly
        major.erase(remove(major.begin(), major.end(), '\n'), major.end());
        major.erase(major.begin());
        file >> number1 >> number2;
        information.subject = major;
        information.num1 = number1;
        information.num2 = number2;
        data.push_back(information);
    }
    
    //Asks for the port number
    cout << "Please enter Port Number: ";
    cin >> portno;
    //Sets the host name to be localhost by copying the string into the character array
    string hostN = "localhost";
    strcpy(hostname, hostN.c_str());

    hp = gethostbyname(hostname);
    //opens the socket, if there is an error, it quits
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cout << "ERROR opening socket\n";
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = hp -> h_addrtype;
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr.sin_port = htons(portno);
    if (::bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0){
        cout << "ERROR on binding\n";
        return -1;
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if (newsockfd < 0){
        cout << "ERROR on accept";
        exit(1);
    }
    memset(buffer, 0, sizeof(buffer));
    while(true){
        n = read(newsockfd,buffer,255);
        if (n < 0){
            cout << "ERROR reading from socket";
            exit(1);
        }
        //Stores the result of the search function into the result variable
        result = search(data, buffer);
        if(result >= 0){
            //Makes a string of the buffer in order to be able to create a sentence with it
            string strBuff(buffer);
            sentence = "The average early career pay for an " + strBuff + " major is " + data[result].num1 + ".\n" +
                       "The corresponding mid-career pay is " + data[result].num2 + ".";
            //copies the sentence into the buffer
            strcpy(buffer, sentence.c_str());
        }
        else{
            sentence = "That major is not in the table";
            strcpy(buffer, sentence.c_str());
        }
        //writes to buffer and sends it to the client
        n = write(newsockfd,buffer, sentence.length());
        if (n < 0){
            cout << "ERROR writing to socket";
            exit(1);
        }
        //resets the buffer to 0
        memset(buffer, 0, sizeof(buffer));
    }
    return 0;
}

